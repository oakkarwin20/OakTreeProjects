#include "PhysicsController.hpp"
#include "MoveStateController.hpp"
#include "GameMode_Animations.hpp"
#include "Player.hpp"


//----------------------------------------------------------------------------------------------------------------------
PhysicsController::PhysicsController( Player* player )
	: m_player( player )
{
	m_position    = m_player->m_position;
	m_position.z += m_playerPhysicsRadius;
}


//----------------------------------------------------------------------------------------------------------------------
PhysicsController::~PhysicsController()
{
}


//----------------------------------------------------------------------------------------------------------------------
void PhysicsController::Startup()
{
}


//----------------------------------------------------------------------------------------------------------------------
void PhysicsController::Update( float deltaSeconds )
{
	bool wasRotatedThisFrame		= false;
	Vec3 forceAccumlatedThisFrame	= Vec3::ZERO;

	//----------------------------------------------------------------------------------------------------------------------
	// Handle input, player controls for movement
	//----------------------------------------------------------------------------------------------------------------------
	XboxController xboxController = g_theInput->GetController( 0 );
	// Get camera relative directions
 	Vec3 iBasis, jBasis, kBasis;
 	EulerAngles& worldCameraEuler = m_player->m_gameModeAnimations->m_gameMode3DWorldCamera.m_orientation;
	worldCameraEuler.GetAsVectors_XFwd_YLeft_ZUp( iBasis, jBasis, kBasis );
 	iBasis.z = 0.0f;
 	jBasis.z = 0.0f;
 	iBasis.Normalize();
 	jBasis.Normalize(); 
 	kBasis.Normalize();
 	// Movement speed modifiers
	float rightTrigger = xboxController.GetRightTrigger();		// Range [0,1];
 	if ( g_theInput->WasKeyJustPressed( KEYCODE_SHIFT ) || (rightTrigger > 0.1f) )
 	{
 		m_curSpeedScalar = m_fasterSpeed;
		m_isSprinting = true;
 	}	
 	if ( g_theInput->WasKeyJustReleased( KEYCODE_SHIFT ) || (m_rightTriggerLastFrame > rightTrigger) )
 	{
 		m_curSpeedScalar		= m_defaultMoveSpeed;
		m_isSprinting			= false;
		m_startCameraLerpCloser = true;
 	}
	m_rightTriggerLastFrame = rightTrigger;

	// Joystick movement
	AnalogJoystick leftJoystick = xboxController.GetLeftJoyStick();
	Vec2  joystickDir			= leftJoystick.GetPosition().GetNormalized();
	float magnitude				= joystickDir.GetLength();
	if ( !CompareIfFloatsAreEqual( magnitude, 0.0f, 0.01f ) )
	{
		Vec3 joystickMoveDir  = Vec3::ZERO;
		joystickMoveDir		 += jBasis * -joystickDir.x;		// yaw
		joystickMoveDir		 += iBasis *  joystickDir.y;		// pitch
		m_moveDirection		 += joystickMoveDir;
		wasRotatedThisFrame	  = true;
	}


	// Crouch logic
	if ( g_theInput->WasKeyJustPressed( 'C' ) )
	{
		if ( m_isCrouching )
		{
			// Exit crouch
			// Toggle crouch state
			m_canTriggerCrouch  = true;
			m_isCrouching		= false;
			m_player->m_moveStateController->SetState( MoveState::MOVESTATE_LOCOMOTION );
		}
		else
		{
			// Enter crouch
			if ( m_canTriggerCrouch )
			{
				// Toggle crouch state
				m_canTriggerCrouch  = false;
				m_isCrouching		= true;
				m_player->m_moveStateController->SetState( MoveState::MOVESTATE_CROUCHED );
			}
		}

	}

 	// Forward
 	if ( g_theInput->IsKeyDown( 'W' ) )
 	{
 		m_moveDirection		+= iBasis;
 		wasRotatedThisFrame  = true;
 	}
 	// Left
 	if ( g_theInput->IsKeyDown( 'A' ) )
 	{
 		m_moveDirection		+= jBasis;
 		wasRotatedThisFrame  = true;
 	}
 	// Backwards
 	if ( g_theInput->IsKeyDown( 'S' ) )
 	{
 		m_moveDirection		-= iBasis;
 		wasRotatedThisFrame  = true;
 	}
 	// Right
 	if ( g_theInput->IsKeyDown( 'D' ) )
 	{
 		m_moveDirection		-= jBasis;
 		wasRotatedThisFrame  = true;
 	}
 	// Jump, Sky (+Z)
 	if ( g_theInput->WasKeyJustPressed( KEYCODE_SPACE_BAR ) || xboxController.WasButtonJustPressed(BUTTON_A) )
 	{
   		if ( m_canTriggerJump )
  		{
 			// Toggle jump state
  			m_canTriggerJump = false;
  			m_isJumping		 = true;
			m_player->m_moveStateController->SetState( MoveState::MOVESTATE_JUMP );

 			// Add jump force
 			forceAccumlatedThisFrame = Vec3::Z_UP * m_jumpForce;
 			// Reset zVelocity to avoid having to apply enough jumpForce to overcome zVelocity
 			m_velocity.z = 0.0f;
 		}
 	}
	// Parkour flips
	if ( g_theInput->WasKeyJustPressed('F') || xboxController.WasButtonJustPressed(BUTTON_B) )
	{
		MoveStateController* moveStateController = m_player->m_moveStateController;
		if ( moveStateController->m_parkourFlipFinished )
		{
			moveStateController->UpdateParkourFlipCounterAndState();
		}
		else
		{
			moveStateController->m_parkourFlipRequested = true;
		}
	}
	// Vault
	if ( g_theInput->WasKeyJustPressed( 'V' ) )
	{
		RaycastResult3D const& rayResult = m_player->m_raycastPlayerToFwd;
		if ( rayResult.m_didImpact && rayResult.m_impactDist <= m_vaultTriggerDist )
		{
			if ( m_canTriggerVault )
			{
				m_canTriggerVault	= false;
				m_isVaulting		= true;
				m_player->m_moveStateController->SetState( MOVESTATE_VAULT );
			}
		}
	}

 	m_moveDirection.Normalize();
 	Vec3 moveDir = Vec3::ZERO;
 	if ( wasRotatedThisFrame )
 	{
 		moveDir = m_moveDirection;
 	}
 	forceAccumlatedThisFrame += moveDir * m_curSpeedScalar;


	//----------------------------------------------------------------------------------------------------------------------
	// Turn player towards m_moveDirection ONLY IF rotated this frame, 
	// we want to avoid turning world east every frame unless the player presses a button
	//----------------------------------------------------------------------------------------------------------------------
	Vec3 playerFwd			= m_orientation.GetForwardDir_XFwd_YLeft_ZUp();
	Vec3 goalDir			= playerFwd;
	if ( wasRotatedThisFrame )
	{
		goalDir = m_moveDirection;
	}
	float turnRate			= m_turnRate * deltaSeconds;
	Vec3  rotatedPlayerFwd	= Interpolate( playerFwd, goalDir, turnRate );
	rotatedPlayerFwd.Normalize();
	float yawCur			= m_orientation.m_yawDegrees;
	float pitchCur			= m_orientation.m_pitchDegrees;
	float rollCur			= m_orientation.m_rollDegrees;
	float yawGoal			= Atan2Degrees( goalDir.y, goalDir.x );
	float pitchGoal			= pitchCur;
	float rollGoal			= rollCur;
	yawCur					= GetTurnedTowardDegrees( yawCur,   yawGoal,   turnRate );
	pitchCur				= GetTurnedTowardDegrees( pitchCur, pitchGoal, turnRate );
	rollCur					= GetTurnedTowardDegrees( rollCur,  rollGoal,  turnRate );
	m_orientation			= EulerAngles( yawCur, pitchCur, rollCur );


	//----------------------------------------------------------------------------------------------------------------------
	// Apply player physics
	//----------------------------------------------------------------------------------------------------------------------
	if ( m_isPhysicsEnabled )
	{
		// Apply gravity
		float scalar = 8.0f;
		forceAccumlatedThisFrame += ( Vec3(0.0f, 0.0f, -1.0f) * g_GRAVITY * scalar );

		// Apply forces accumulated this frame to velocity
		Vec3 deltaForce  = forceAccumlatedThisFrame * deltaSeconds; 
		m_velocity		 = m_velocity + deltaForce;
		// Decelerate along the ground
		m_velocity.x	*= 0.98f;
		m_velocity.y	*= 0.98f;

		// Clamp xyVelocity separately from zVelocity to allow a better feeling of jumping/falling
		Vec2 velocityXY = Vec2( m_velocity.x, m_velocity.y );
		velocityXY		= velocityXY.GetClamped( m_maxSpeedXY );
		m_velocity.x	= velocityXY.x;
		m_velocity.y	= velocityXY.y;
		m_velocity.z	= GetClamped( m_velocity.z, -m_maxSpeedZ, m_maxSpeedZ );

		// Update position after all forces have been accumulated
		Vec3 deltaPosition	= ( m_velocity * deltaSeconds );
		m_position			= m_position + deltaPosition;

		// Ensure physics sphere bottom starts at player feet
		m_player->m_position    = m_position;
		m_player->m_position.z -= m_playerPhysicsRadius;
	}
}


//----------------------------------------------------------------------------------------------------------------------
void PhysicsController::Render() const
{
	std::vector<Vertex_PCU> verts;
	// Player sphere
	//	AddVertsForSphere3D( verts, m_position, m_playerVisualRadius, 16.0f );		
	if ( g_debugRenderRaycast_F2 )
	{
		float stacksAndSlices = 4.0f;
		AddVertsForUVSphereZWireframe3D( verts, m_position, m_playerPhysicsRadius, stacksAndSlices, stacksAndSlices, 0.05f );
		AddVertsForUVSphereZWireframe3D( verts, m_player->m_position, 1.0f, stacksAndSlices, stacksAndSlices, 0.05f, Rgba8::PINK );
	}
	// Player velocity
	Vec3  dir			= m_velocity.GetNormalized();
	float magnitude		= m_velocity.GetLength();
	float visualScalar  = 1.0f;
	float thickness		= 0.2f;
	Vec3  velocityEnd	= m_position + dir * magnitude * visualScalar;
	AddVertsForArrow3D ( verts, m_position, velocityEnd, thickness, Rgba8::CYAN );
	// Player move dir
	velocityEnd			= m_position + m_moveDirection * magnitude * visualScalar;
	AddVertsForArrow3D ( verts, m_position, velocityEnd, thickness, Rgba8::DARK_YELLOW );
	// Player orientation
	Mat44 playerMat		= m_orientation.GetAsMatrix_XFwd_YLeft_ZUp();
	float length		= 10.0f;
	thickness			= 0.4f;
	Vec3  iBasis		= m_position + playerMat.GetIBasis3D() * length;
	Vec3  jBasis		= m_position + playerMat.GetJBasis3D() * length;
	Vec3  kBasis		= m_position + playerMat.GetKBasis3D() * length;
	AddVertsForArrow3D( verts, m_position, iBasis, thickness, Rgba8::RED   );
	AddVertsForArrow3D( verts, m_position, jBasis, thickness, Rgba8::GREEN );
	AddVertsForArrow3D( verts, m_position, kBasis, thickness, Rgba8::BLUE  );
	// Draw call for physics sphere and debug draws
	g_theRenderer->BindTexture( nullptr );
	g_theRenderer->BindShader ( nullptr );
	g_theRenderer->DrawVertexArray( int( verts.size() ), verts.data() );
}