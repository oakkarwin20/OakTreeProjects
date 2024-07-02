#include "Game/App.hpp"
#include "Game/GameMode_Animations.hpp"
#include "Game/Map_Animations.hpp"
#include "Game/Player.hpp"
#include "Game/PhysicsController.hpp"
#include "Game/AnimationController.hpp"
#include "Game/MoveStateController.hpp"

#include "Engine/Window/Window.hpp"
#include "Engine/Animations/AnimationTimeline.hpp"
#include "Engine/ThirdParty/Squirrel/Noise/SmoothNoise.hpp"
#include "Engine/Core/Time.hpp"
#include "Engine/Core/StringUtils.hpp"
#include "Engine/SkeletalSystem/IK_Chain3D.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"
#include "Engine/Renderer/DebugRenderSystem.hpp"
#include "Engine/Math/MathUtils.hpp"
#include "Engine/Core/EngineCommon.hpp"
#include "Engine/Input/InputSystem.hpp"
#include "Engine/Audio/AudioSystem.hpp"
#include "Engine/Math/RandomNumberGenerator.hpp"
#include "Engine/SkeletalSystem/CreatureBase.hpp"

//----------------------------------------------------------------------------------------------------------------------
GameMode_Animations::GameMode_Animations()
{
}

//----------------------------------------------------------------------------------------------------------------------
GameMode_Animations::~GameMode_Animations()
{
}

//----------------------------------------------------------------------------------------------------------------------
void GameMode_Animations::Startup()
{
	g_debugFreeFly_F1			= true;
	g_debugRenderRaycast_F2		= true;
	g_debugBasis_F3				= true;
	g_debugText_F4				= true;
	g_debugAngles_F5			= true;
	g_debugToggleConstraints_F8 = true;
	g_debugFollowTarget_F11		= false;

	// Initialize Player cam pos and orientation
	m_gameMode3DWorldCamera.m_position	  = Vec3( 0.0f, 120.0f, 20.0f );
	m_gameMode3DWorldCamera.m_orientation = EulerAngles( -90.0f, 5.0f, 0.0f );

	// Init lighting
	EulerAngles sunOrientation = EulerAngles( 0.0f, 65.0f, 0.0f );
	m_sunDirection = sunOrientation.GetForwardDir_XFwd_YLeft_ZUp();


// 	Vec3 leftOffset = Vec3(0.0f, 0.0f, 0.0f);
// 	float time = 0.0f;
// 	for ( int i = 0; i < 6; i++ )
// 	{
// 		AnimationPose newAnimPose;
// //		FbxFileImporter::LoadCurveForAllJointsAtElapsedTime( "Data/Animations/Run.fbx", newAnimPose, m_bindAnimationPose, time );
// 		newAnimPose = m_animationClip.SampleAnimPoseAtTime( time, m_bindAnimationPose );
// 		for ( int jointId = 0; jointId < newAnimPose.m_jointList.size(); jointId++ )
// 		{
// 			AnimationJoint const& joint = newAnimPose.m_jointList[jointId];
// 			Transform const& curJointTransform = newAnimPose.LocalToModel_Transform(jointId);
// 			if ( joint.m_parentJointIndex >= 0 )
// 			{
// 				Transform const& parentJointTransform = newAnimPose.LocalToModel_Transform( joint.m_parentJointIndex );
// 				AddVertsForCone3D( m_animVerts, parentJointTransform.m_position + leftOffset, curJointTransform.m_position + leftOffset, 0.5f );
// 			}
// 		}
// 		leftOffset += Vec3(0.0f, 40.0f, 0.0f);
// 		time += 100.0f;
// 	}

	 m_map	  = new Map_Animations();
	 m_player = new Player( this, Vec3(0.0f, 0.0f, 1.0f) );
	 m_player->Update( 0.016f );

	 // Init animation timeline
	 m_animTimeline.UpdateAnimClipAndBindPose( &m_player->m_animController->m_animClip_Vault, m_player->m_animController->m_animBindPose_Vault );
	 m_animTimeline.UpdateSampledPose();
}


//----------------------------------------------------------------------------------------------------------------------
void GameMode_Animations::Update( float deltaSeconds )
{	
	// Update core systems
	UpdatePauseQuitAndSlowMo();
	UpdateDebugKeys();
	UpdateTargetInput( deltaSeconds );
	UpdateInput_CameraAndPlayer( deltaSeconds );
	UpdatePhysics( deltaSeconds );
	m_player->RaycastPlayerToFloor();
	UpdateAnimTimeline();
	UpdateGameMode3DCamera( deltaSeconds );
}


//----------------------------------------------------------------------------------------------------------------------
void GameMode_Animations::Render() const
{
	RenderWorldObjects();
	RenderUIObjects();
}


//----------------------------------------------------------------------------------------------------------------------
void GameMode_Animations::Shutdown()
{
}


//----------------------------------------------------------------------------------------------------------------------
void GameMode_Animations::UpdateDebugKeys()
{
	// Toggle common debug bools
	XboxController xboxController = g_theInput->GetController( 0 );
	if ( g_theInput->WasKeyJustPressed( KEYCODE_F1 ) || xboxController.WasButtonJustPressed(LEFT_SHOULDER) )
	{
		g_debugFreeFly_F1 = !g_debugFreeFly_F1;
	}
	if ( g_theInput->WasKeyJustPressed( KEYCODE_F2 ) )
	{
		g_debugRenderRaycast_F2 = !g_debugRenderRaycast_F2;
	}
	if ( g_theInput->WasKeyJustPressed( KEYCODE_F3 ) )
	{
		g_debugBasis_F3 = !g_debugBasis_F3;
	}
	if ( g_theInput->WasKeyJustPressed( KEYCODE_F4 ) )
	{
		g_debugText_F4 = !g_debugText_F4;
	}
	if ( g_theInput->WasKeyJustPressed( KEYCODE_F5 ) )
	{
		g_debugAngles_F5 = !g_debugAngles_F5;
	}
	if ( g_theInput->WasKeyJustPressed( KEYCODE_F8 ) )
	{
		g_debugToggleConstraints_F8 = !g_debugToggleConstraints_F8;
	}
	if ( g_theInput->WasKeyJustPressed( KEYCODE_F11 ) )
	{
		g_debugFollowTarget_F11 = !g_debugFollowTarget_F11;
	}
	if ( g_theInput->WasKeyJustPressed( KEYCODE_BACKSPACE ) )
	{
	}
	if ( g_theInput->WasKeyJustPressed( KEYCODE_HOME ) )
	{
	}
	if ( g_theInput->WasKeyJustPressed( KEYCODE_END ) )
	{
	}
	if ( g_theInput->WasKeyJustPressed( KEYCODE_INSERT ) )
	{
	}
	if ( g_theInput->WasKeyJustPressed( KEYCODE_DELETE ) )
	{
		g_ToggleAnimTimeline_Delete = !g_ToggleAnimTimeline_Delete;
		bool isCursorHidden			= (g_ToggleAnimTimeline_Delete == true) ? false : true;
		g_isCursorHidden			= isCursorHidden;

		// Update caret info
		m_animTimeline.ToggleVisibility();
	}

	// Control camera dist from player
	if ( g_theInput->IsKeyDown( '9' ) )
	{
		m_defaultDist_CameraToPlayer += 10.0f;
	}
	if ( g_theInput->IsKeyDown( '0' ) )
	{
		m_defaultDist_CameraToPlayer -= 10.0f;
	}

	if ( g_theInput->WasKeyJustPressed( 'R' ) )
	{
		m_player->m_physicsController->m_position = Vec3(0.0f, 0.0f, 10.0f);
	}


	// Preset camera positions
	if ( g_theInput->WasKeyJustPressed( 'X' ) )
	{
		// Look down X-axis (YZ plane)
		m_gameMode3DWorldCamera.m_position	  = Vec3( 120.0f, 0.0f, 20.0f );
		m_gameMode3DWorldCamera.m_orientation = EulerAngles( 180.0f, 5.0f, 0.0f );
	}
	if ( g_theInput->WasKeyJustPressed( 'Y' ) )
	{
		// Look down Y-axis (XZ plane)
		m_gameMode3DWorldCamera.m_position	  = Vec3( 0.0f, 120.0f, 20.0f );
		m_gameMode3DWorldCamera.m_orientation = EulerAngles( -90.0f, 5.0f, 0.0f );
	}
	if ( g_theInput->WasKeyJustPressed( 'Z' ) )
	{
		// Look down Z-axis (XY plane)
		m_gameMode3DWorldCamera.m_position	  = Vec3( 2.0f, 0.0f, 120.0f );
		m_gameMode3DWorldCamera.m_orientation = EulerAngles( 0.0f, 89.9f, 0.0f );
	}
}


//----------------------------------------------------------------------------------------------------------------------
void GameMode_Animations::UpdateGameMode3DCamera( float deltaSeconds )
{
	//----------------------------------------------------------------------------------------------------------------------
	// Camera follows mouse OR controller
	//----------------------------------------------------------------------------------------------------------------------
	// Update WORLD camera from mouse movement
	Vec2 cursorClientDelta								  = g_theInput->GetCursorClientDelta();
	float mouseSpeed									  = 0.05f;
	float yaw											  = cursorClientDelta.x * mouseSpeed;
	float pitch											  = cursorClientDelta.y * mouseSpeed;

	// Update WORLD camera from controller right joystick
	XboxController xboxController = g_theInput->GetController( 0 );
	AnalogJoystick rightJoystick  = xboxController.GetRightJoyStick();
	yaw							 += rightJoystick.GetPosition().x;
	pitch						 += rightJoystick.GetPosition().y;

	// Apply rotations to camera based on mouse OR controller input
	m_gameMode3DWorldCamera.m_orientation.m_yawDegrees	 -= yaw;
	m_gameMode3DWorldCamera.m_orientation.m_pitchDegrees += pitch;
	m_gameMode3DWorldCamera.m_orientation.m_pitchDegrees = GetClamped( m_gameMode3DWorldCamera.m_orientation.m_pitchDegrees, -85.0f, 85.0f );

	if ( g_debugFreeFly_F1 )
	{
		// Set cameraPos roam "freely"
		m_gameMode3DWorldCamera.SetPerspectiveView( 2.0f, 60.0f, 0.1f, 10000.0f );
		m_gameMode3DWorldCamera.SetRenderBasis( Vec3(0.0f, 0.0f, 1.0f), Vec3(-1.0f, 0.0f, 0.0f), Vec3(0.0f, 1.0f, 0.0f) );
	}
	else
	{
		// Set camera to follow player
		Vec3 cameraFwd	= m_gameMode3DWorldCamera.m_orientation.GetForwardDir_XFwd_YLeft_ZUp();
		if ( m_player->m_physicsController->m_isSprinting )
		{
			float lerpDelta	= deltaSeconds * 0.5f;
			// Lerp distance, camera away from player
			m_curDist_CameraToPlayer = Interpolate( m_curDist_CameraToPlayer, m_maxDist_CameraToPlayer, lerpDelta );
			// Lerp FOV
			m_fovCurrent = Interpolate( m_fovCurrent, m_fovMax, lerpDelta );
			m_gameMode3DWorldCamera.SetPerspectiveView( 2.0f, m_fovCurrent, 0.1f, 10000.0f );
		}
		else if ( m_player->m_physicsController->m_startCameraLerpCloser )
		{
			float lerpDelta	= deltaSeconds * 2.0f;
			// Lerp camera closer to player (nearer)
			m_curDist_CameraToPlayer = Interpolate( m_curDist_CameraToPlayer, m_defaultDist_CameraToPlayer, lerpDelta );
			if ( CompareIfFloatsAreEqual( m_curDist_CameraToPlayer, m_defaultDist_CameraToPlayer, 0.1f ) )
			{
				m_player->m_physicsController->m_startCameraLerpCloser = false;
			}
			// Lerp FOV
			m_fovCurrent = Interpolate( m_fovCurrent, m_fovDefault, lerpDelta );
			m_gameMode3DWorldCamera.SetPerspectiveView( 2.0f, m_fovCurrent, 0.1f, 10000.0f );
		}
		Vec3 cameraPosAttachedToPlayer	    = m_player->m_position - ( cameraFwd * m_curDist_CameraToPlayer );
		float extraOffset					= 2.0f;
		Vec3 zOffset						= Vec3(0.0f, 0.0f, m_player->m_physicsController->m_playerPhysicsRadius + extraOffset );
		m_gameMode3DWorldCamera.m_position  = cameraPosAttachedToPlayer + zOffset;
	}

	// Update UI camera
	m_gameMode3DUICamera.SetOrthoView( Vec2( 0.0f, 0.0f ), Vec2( SCREEN_SIZE_X, SCREEN_SIZE_Y ) );		
}


//----------------------------------------------------------------------------------------------------------------------
void GameMode_Animations::RenderWorldObjects() const
{
	//----------------------------------------------------------------------------------------------------------------------
	// Begin World Camera
	//----------------------------------------------------------------------------------------------------------------------
	g_theRenderer->BeginCamera( m_gameMode3DWorldCamera );

	// Initialize common render variables
	std::vector<Vertex_PCU> textVerts;
	std::vector<Vertex_PCU> verts;
	std::vector<Vertex_PCU> compassVerts;
	std::vector<Vertex_PCU> verts_BackFace;
	std::vector<Vertex_PCU> verts_testTexture;
	std::vector<Vertex_PCU> verts_ikChainBody;

	//----------------------------------------------------------------------------------------------------------------------
	// Lighting
	//----------------------------------------------------------------------------------------------------------------------
	Vec3 sunDir = m_sunDirection.GetNormalized();
	g_theRenderer->SetLightingConstants( sunDir, m_sunIntensity, m_ambientIntensity, m_gameMode3DWorldCamera.m_position, 0, 1, m_specularIntensity, m_specularPower );


	//----------------------------------------------------------------------------------------------------------------------
	// Render Map
	m_map->Render();

	//----------------------------------------------------------------------------------------------------------------------
	// Render world compass
	//----------------------------------------------------------------------------------------------------------------------
//	AddVertsForCompass( compassVerts, Vec3( 0.0, 0.0f, -1.0f ), 100.0f, 0.1f );

	//----------------------------------------------------------------------------------------------------------------------
	// "ProtoMode" title
	//----------------------------------------------------------------------------------------------------------------------
	Vec3 textOrigin = Vec3( 200.0f, 150.0f,  0.0f );
	Vec3 iBasis		= Vec3(	  0.0f,  -1.0f,  0.0f );
	Vec3 jBasis		= Vec3(	  0.0f,   0.0f,  1.0f );
	g_theApp->m_textFont->AddVertsForText3D( textVerts, textOrigin, iBasis, jBasis, 25.0f, "Animations!", Rgba8::GREEN );

	//----------------------------------------------------------------------------------------------------------------------
	// Render skybox
	//----------------------------------------------------------------------------------------------------------------------
	std::vector<Vertex_PCU> verts_skybox;
	AddVertsForSphere3D( verts_skybox, Vec3(0.0f, 0.0f, 100.0f), 1000.0f, 32.0f, 32.0f );
	//	AddVertsForQuad3D( verts_skybox, Vec3(1000.0f, -1000.0f, -1000.0f), Vec3(-1000.0f, -1000.0f, -1000.0f), Vec3(-1000.0f, -1000.0f, 1000.0f), Vec3(1000.0f, -1000.0f, 1000.0f), Rgba8::WHITE, AABB2::ZERO_TO_ONE );
	g_theRenderer->SetBlendMode( BlendMode::ALPHA );
	g_theRenderer->SetRasterizerMode( RasterizerMode::SOLID_CULL_NONE );
	g_theRenderer->SetModelConstants();
	g_theRenderer->BindTexture( g_theApp->m_texture_Skybox );
	g_theRenderer->BindShader( nullptr );
	g_theRenderer->DrawVertexArray( static_cast<int>( verts_skybox.size() ), verts_skybox.data() );
	// Reset bindings
	g_theRenderer->SetRasterizerMode( RasterizerMode::SOLID_CULL_BACK );

	//----------------------------------------------------------------------------------------------------------------------
	// Render Floor
	//----------------------------------------------------------------------------------------------------------------------
//	AddVertsForAABB3D( verts, AABB3( -100.0f, -100.0f, -2.0f, 100.0f, 100.0f, -1.0f ), Rgba8::DARK_YELLOW );

	//----------------------------------------------------------------------------------------------------------------------
	// Render Creature
	//----------------------------------------------------------------------------------------------------------------------
//	RenderCreature( verts, verts_testTexture, textVerts, verts_BackFace, verts_ikChainBody );


	//----------------------------------------------------------------------------------------------------------------------
	// Animation test render
	//----------------------------------------------------------------------------------------------------------------------
// 	std::vector<Vertex_PCU> m_animVerts;
// 	for ( int jointId = 0; jointId < m_newAnimationPose.m_jointList.size(); jointId++ )
// 	{
// 		AnimationJoint const& joint = m_newAnimationPose.m_jointList[jointId];
// 		Transform const& curJointTransform = m_newAnimationPose.LocalToModel_Transform(jointId);
// 		if ( joint.m_parentJointIndex >= 0 )
// 		{
// 			Transform const& parentJointTransform = m_newAnimationPose.LocalToModel_Transform( joint.m_parentJointIndex );
// 			AddVertsForCone3D( m_animVerts, parentJointTransform.m_position, curJointTransform.m_position, 2.0f );
// 		}
// 	}

	//----------------------------------------------------------------------------------------------------------------------
	// Animation Timeline tool
	//----------------------------------------------------------------------------------------------------------------------
	if ( m_animTimeline.IsVisible() )
	{
		m_animTimeline.RenderAnimPose( verts );
	}
	//----------------------------------------------------------------------------------------------------------------------
	// Player
	//----------------------------------------------------------------------------------------------------------------------
	else
	{
		m_player->Render();
	}

	//----------------------------------------------------------------------------------------------------------------------
	// Render ikTarget
	//----------------------------------------------------------------------------------------------------------------------
	AddVertsForSphere3D( verts, m_ikTargetPos, 2.0f, 4.0f, 4.0f, Rgba8::MAGENTA );



	//----------------------------------------------------------------------------------------------------------------------
	// End world camera
	//----------------------------------------------------------------------------------------------------------------------
	g_theRenderer->EndCamera( m_gameMode3DWorldCamera );

	//----------------------------------------------------------------------------------------------------------------------
	// Draw calls 
	//----------------------------------------------------------------------------------------------------------------------
	// World objects
	g_theRenderer->SetBlendMode( BlendMode::ALPHA );
	g_theRenderer->SetModelConstants();
	g_theRenderer->BindTexture( nullptr );
	g_theRenderer->BindShader( nullptr );
	g_theRenderer->DrawVertexArray( static_cast<int>( verts.size() ), verts.data() );	

	// Textured objects
	g_theRenderer->SetBlendMode( BlendMode::ALPHA );
	g_theRenderer->SetModelConstants( Mat44(), Rgba8::MAGENTA );
	g_theRenderer->BindTexture( g_theApp->m_texture_TestOpenGL );
	g_theRenderer->BindShader( nullptr );
	g_theRenderer->DrawVertexArray( static_cast<int>( verts_testTexture.size() ), verts_testTexture.data() );

	// Backface objects
	g_theRenderer->SetBlendMode( BlendMode::ALPHA );
	g_theRenderer->SetModelConstants();
	g_theRenderer->BindTexture( nullptr );
	g_theRenderer->BindShader( nullptr );
	g_theRenderer->SetRasterizerMode( RasterizerMode::SOLID_CULL_NONE );
	g_theRenderer->DrawVertexArray( static_cast<int>( verts_BackFace.size() ), verts_BackFace.data() );
	g_theRenderer->SetRasterizerMode( RasterizerMode::SOLID_CULL_BACK );
	
	// Compass 
	g_theRenderer->SetBlendMode( BlendMode::ALPHA );
	g_theRenderer->SetModelConstants();
	g_theRenderer->BindTexture( nullptr );
	g_theRenderer->BindShader( nullptr );
	g_theRenderer->DrawVertexArray( static_cast<int>( compassVerts.size() ), compassVerts.data() );

	// Text objects
	g_theRenderer->SetBlendMode( BlendMode::ALPHA );
	g_theRenderer->SetModelConstants();
	g_theRenderer->BindTexture( &g_theApp->m_textFont->GetTexture() );
	g_theRenderer->BindShader( nullptr );
	g_theRenderer->SetRasterizerMode( RasterizerMode::SOLID_CULL_NONE );
	g_theRenderer->DrawVertexArray( static_cast<int>( textVerts.size() ), textVerts.data() );
	// Reset binded texture
	g_theRenderer->BindTexture( nullptr );
	g_theRenderer->SetRasterizerMode( RasterizerMode::SOLID_CULL_BACK );
}


//----------------------------------------------------------------------------------------------------------------------
void GameMode_Animations::RenderUIObjects() const
{
	//----------------------------------------------------------------------------------------------------------------------
	// Begin UI Camera
	g_theRenderer->BeginCamera( m_gameMode3DUICamera );


	//----------------------------------------------------------------------------------------------------------------------
	// Get or Create font
	//----------------------------------------------------------------------------------------------------------------------
	std::vector<Vertex_PCU> textVerts;  
 	if ( g_debugText_F4 )
 	{
		Rgba8 textColor				= Rgba8::YELLOW;
		Rgba8 textColor_darkGreen	= Rgba8::DARK_GREEN;
 		// Initialize and set UI variables
		float cellHeight		= 2.0f;
 		Vec2 timeAlignment		= Vec2( 1.0f, 1.0f  );
 		std::string gameInfo	= Stringf( "F1 (Toggle camera/player) Player || Mode (F6/F7 for prev/next) || Animations (3D)" );
 		if ( g_debugFreeFly_F1 )
 		{
 			gameInfo = Stringf( "F1 (Toggle camera/player) Camera || Mode (F6/F7 for prev/next) || Animations (3D)" );
 		}
		g_theApp->m_textFont->AddVertsForTextInBox2D( textVerts, g_UiBounds, cellHeight, gameInfo, textColor, 0.75f, Vec2( 0.0f, 1.0f ), TextDrawMode::SHRINK_TO_FIT );
 		// Core Values
 		float fps								= 1.0f / g_theApp->m_gameClock.GetDeltaSeconds();
 		float scale								= g_theApp->m_gameClock.GetTimeScale();
 		std::string cameraPosText				= Stringf( "Cam position:            %0.2f, %0.2f, %0.2f",	m_gameMode3DWorldCamera.m_position.x,				m_gameMode3DWorldCamera.m_position.y,				  m_gameMode3DWorldCamera.m_position.z );
 		std::string cameraOrientationText		= Stringf( "Cam Orientation (YPR):   %0.2f, %0.2f, %0.2f",	m_gameMode3DWorldCamera.m_orientation.m_yawDegrees,	m_gameMode3DWorldCamera.m_orientation.m_pitchDegrees, m_gameMode3DWorldCamera.m_orientation.m_rollDegrees );
 		std::string timeText					= Stringf( "Time: %0.2f. FPS: %0.2f, Scale %0.2f.",			g_theApp->m_gameClock.GetTotalSeconds(), fps, scale );
		std::string playerMovementStateString	= Stringf( "PlayerMoveState:%s",							m_player->GetStateAsString().c_str() );
 		std::string playerPositionString		= Stringf( "PlayerPosition X:%0.2f, Y:%0.2f, Z:%0.2f.",		m_player->m_position.x, m_player->m_position.y, m_player->m_position.z );
 		std::string playerVelocityString		= Stringf( "PlayerVelocity X:%0.2f, Y:%0.2f, Z:%0.2f.",		m_player->m_physicsController->m_velocity.x, m_player->m_physicsController->m_velocity.y, m_player->m_physicsController->m_velocity.z );
 		std::string playerVelocityXYZMagString	= Stringf( "PlayerVelocityXYZ Magnitude:%0.2f",				m_player->m_physicsController->m_velocity.GetLength() );
		Vec2 playerVelocity_V2					= Vec2( m_player->m_physicsController->m_velocity.x, m_player->m_physicsController->m_velocity.y );		// Get only the velocityXY to ignore gravity
 		std::string playerVelocityXYMagString	= Stringf( "PlayerVelocityXY  Magnitude:%0.2f",				playerVelocity_V2.GetLength() );
 		std::string AnimPlayedLclTimeMsString	= "NoAnimPlayed";
		if ( m_player->m_animController->m_animClipCurPlayed )
		{
			AnimPlayedLclTimeMsString			= Stringf( "AnimPlayedLclTimeMS: %0.2f",					m_player->m_animController->m_animClipCurPlayed->m_localTime );
		}
 		std::string animPoseString				= Stringf( "AnimPose: %s",									m_player->m_animController->m_debugPoseName.c_str() );
 		std::string parkourFlipCounterString	= Stringf( "ParkourFlipCounter: %d",						m_player->m_moveStateController->m_parkourFlipCounter );

 		float textAlignmentY	= 1.0f;
 		float textHeight		= 0.03f;
 		g_theApp->m_textFont->AddVertsForTextInBox2D( textVerts, g_UiBounds, cellHeight, timeText,						textColor, 0.75f, Vec2( 1.0f, textAlignmentY ),				  TextDrawMode::SHRINK_TO_FIT );
 		g_theApp->m_textFont->AddVertsForTextInBox2D( textVerts, g_UiBounds, cellHeight, playerMovementStateString,		textColor, 0.75f, Vec2( 0.0f, textAlignmentY -= textHeight ), TextDrawMode::SHRINK_TO_FIT );
 		g_theApp->m_textFont->AddVertsForTextInBox2D( textVerts, g_UiBounds, cellHeight, playerPositionString,			textColor, 0.75f, Vec2( 0.0f, textAlignmentY -= textHeight ), TextDrawMode::SHRINK_TO_FIT );
 		g_theApp->m_textFont->AddVertsForTextInBox2D( textVerts, g_UiBounds, cellHeight, playerVelocityString,			textColor, 0.75f, Vec2( 0.0f, textAlignmentY -= textHeight ), TextDrawMode::SHRINK_TO_FIT );
 		g_theApp->m_textFont->AddVertsForTextInBox2D( textVerts, g_UiBounds, cellHeight, playerVelocityXYZMagString,	textColor, 0.75f, Vec2( 0.0f, textAlignmentY -= textHeight ), TextDrawMode::SHRINK_TO_FIT );
 		g_theApp->m_textFont->AddVertsForTextInBox2D( textVerts, g_UiBounds, cellHeight, playerVelocityXYMagString,		textColor, 0.75f, Vec2( 0.0f, textAlignmentY -= textHeight ), TextDrawMode::SHRINK_TO_FIT );
 		g_theApp->m_textFont->AddVertsForTextInBox2D( textVerts, g_UiBounds, cellHeight, AnimPlayedLclTimeMsString,		textColor, 0.75f, Vec2( 0.0f, textAlignmentY -= textHeight ), TextDrawMode::SHRINK_TO_FIT );
 		g_theApp->m_textFont->AddVertsForTextInBox2D( textVerts, g_UiBounds, cellHeight, animPoseString,				textColor, 0.75f, Vec2( 0.0f, textAlignmentY -= textHeight ), TextDrawMode::SHRINK_TO_FIT );
 		g_theApp->m_textFont->AddVertsForTextInBox2D( textVerts, g_UiBounds, cellHeight, parkourFlipCounterString,		textColor, 0.75f, Vec2( 0.0f, textAlignmentY -= textHeight ), TextDrawMode::SHRINK_TO_FIT );
 	}	


	//----------------------------------------------------------------------------------------------------------------------
	// Debug draw AnimTimeline
	//----------------------------------------------------------------------------------------------------------------------
	std::vector<Vertex_PCU> verts;
	verts.reserve( 350'000 );
	if ( g_ToggleAnimTimeline_Delete )
	{
		m_animTimeline.RenderUI( g_theApp->m_textFont, textVerts, verts, Rgba8::DARK_GRAY, 2.0f, Rgba8::MAGENTA, 1.0f );
	}
	
	//----------------------------------------------------------------------------------------------------------------------
	// End UI Camera
	g_theRenderer->EndCamera( m_gameMode3DUICamera );

	// Draw for Borders
	g_theRenderer->SetBlendMode( BlendMode::ALPHA );
	g_theRenderer->SetModelConstants();
	g_theRenderer->BindTexture( nullptr );
	g_theRenderer->BindShader ( nullptr );
	g_theRenderer->DrawVertexArray( static_cast<int>( verts.size() ), verts.data() );
	g_theRenderer->BindTexture( nullptr );

	//----------------------------------------------------------------------------------------------------------------------
	// Draw for UI camera text
	g_theRenderer->SetBlendMode( BlendMode::ALPHA );
	g_theRenderer->SetModelConstants();
	g_theRenderer->BindTexture( &g_theApp->m_textFont->GetTexture() );
	g_theRenderer->BindShader ( nullptr );
	g_theRenderer->DrawVertexArray( static_cast<int>( textVerts.size() ), textVerts.data() );
	g_theRenderer->BindTexture( nullptr );
}


//----------------------------------------------------------------------------------------------------------------------
void GameMode_Animations::AddVertsForCompass( std::vector<Vertex_PCU>&compassVerts, Vec3 startPosition, float axisLength, float axisThickness ) const
{
	// Render stationary world compass
	// X
	Vec3 endPosX = startPosition + Vec3( axisLength, 0.0f, 0.0f );
	AddVertsForArrow3D( compassVerts, startPosition, endPosX, axisThickness, Rgba8::RED );
	// Y
	Vec3 endPosY = startPosition + Vec3( 0.0f, axisLength, 0.0f );
	AddVertsForArrow3D( compassVerts, startPosition, endPosY, axisThickness, Rgba8::GREEN );
	// Z
	Vec3 endPosZ = startPosition + Vec3( 0.0f, 0.0f, axisLength );
	AddVertsForArrow3D( compassVerts, startPosition, endPosZ, axisThickness, Rgba8::BLUE );
}


//----------------------------------------------------------------------------------------------------------------------
void GameMode_Animations::UpdateTargetInput( float deltaSeconds )
{
	float stepAmount = m_defaultSpeed;
	if ( g_theInput->IsKeyDown( KEYCODE_SHIFT ) )
	{
		stepAmount *= 2.0f;
	}

	// East (+X)
	if ( g_theInput->IsKeyDown( 'I' ) )
	{
		m_ikTargetPos += ( Vec3::X_FWD * m_currentSpeed ) * deltaSeconds;
	}
	// West (-X)
	if ( g_theInput->IsKeyDown( 'K' ) )
	{
		m_ikTargetPos -= ( Vec3::X_FWD * m_currentSpeed ) * deltaSeconds;
	}
	// North (+Y)
	if ( g_theInput->IsKeyDown( 'J' ) )
	{
		m_ikTargetPos += ( Vec3::Y_LEFT * m_currentSpeed ) * deltaSeconds;
	}
	// South (-Y)
	if ( g_theInput->IsKeyDown( 'L' ) )
	{
		m_ikTargetPos -= ( Vec3::Y_LEFT * m_currentSpeed ) * deltaSeconds;
	}
	// Sky (+Z)
	if ( g_theInput->IsKeyDown( 'O' ) )
	{
		m_ikTargetPos += ( Vec3::Z_UP * m_currentSpeed ) * deltaSeconds;
	}
	// Ground (-Z)
	if ( g_theInput->IsKeyDown( 'U' ) )
	{
		m_ikTargetPos -= ( Vec3::Z_UP * m_currentSpeed ) * deltaSeconds;
	}
}


//----------------------------------------------------------------------------------------------------------------------
void GameMode_Animations::UpdateInput_CameraAndPlayer( float deltaSeconds )
{
	Vec3 iBasis, jBasis, kBasis;
	m_gameMode3DWorldCamera.m_orientation.GetAsVectors_XFwd_YLeft_ZUp( iBasis, jBasis, kBasis );
	iBasis.z = 0.0f;
	jBasis.z = 0.0f;
	iBasis.Normalize();
	jBasis.Normalize(); 
	kBasis.Normalize();

	// Movement speed modifiers
	if ( g_theInput->IsKeyDown( KEYCODE_SHIFT ) )
	{
		m_currentSpeed		= m_fasterSpeed;
		m_goalWalkLerpSpeed	= m_sprintLerpSpeed;
	}
	if ( g_theInput->WasKeyJustReleased( KEYCODE_SHIFT ) )
	{
		m_currentSpeed		= m_defaultSpeed;
		m_goalWalkLerpSpeed	= m_walkLerpSpeed;
	}

	// Camera controls
	if ( g_debugFreeFly_F1 )
	{
		// Joystick movement
		XboxController xboxController	= g_theInput->GetController( 0 );
		AnalogJoystick leftJoystick		= xboxController.GetLeftJoyStick();
		Vec2  joystickDir				= leftJoystick.GetPosition().GetNormalized();
		float magnitude					= joystickDir.GetLength();
		if ( !CompareIfFloatsAreEqual( magnitude, 0.0f, 0.01f ) )
		{
			Vec3 joystickMoveDir				 = Vec3::ZERO;
			joystickMoveDir						+= jBasis * -joystickDir.x;		// yaw
			joystickMoveDir						+= iBasis *  joystickDir.y;		// pitch
			m_gameMode3DWorldCamera.m_position	+= ( joystickMoveDir * m_currentSpeed ) * deltaSeconds;
		}

		// Forward
		if ( g_theInput->IsKeyDown( 'W' ) )
		{
			m_gameMode3DWorldCamera.m_position += ( iBasis * m_currentSpeed ) * deltaSeconds;
		}
		// Left
		if ( g_theInput->IsKeyDown( 'A' ) )
		{
			m_gameMode3DWorldCamera.m_position += ( jBasis * m_currentSpeed ) * deltaSeconds;
		}
		// Backwards
		if ( g_theInput->IsKeyDown( 'S' ) )
		{
			m_gameMode3DWorldCamera.m_position -= ( iBasis * m_currentSpeed ) * deltaSeconds;
		}
		// Right
		if ( g_theInput->IsKeyDown( 'D' ) )
		{
			m_gameMode3DWorldCamera.m_position -= ( jBasis * m_currentSpeed ) * deltaSeconds;
		}
		// Sky (+Z)
		if ( g_theInput->IsKeyDown( 'E' ) )
		{
			m_gameMode3DWorldCamera.m_position += ( Vec3( 0.0f, 0.0f, 1.0f ) * m_currentSpeed )  * deltaSeconds;
		}
		// Ground (-Z)
		if ( g_theInput->IsKeyDown( 'Q' ) )
		{
			m_gameMode3DWorldCamera.m_position -= ( Vec3( 0.0f, 0.0f, 1.0f ) * m_currentSpeed )  * deltaSeconds;
		}
	}
	else   
	{
		m_player->Update( deltaSeconds );
	}
}


//----------------------------------------------------------------------------------------------------------------------
void GameMode_Animations::UpdatePhysics( float deltaSeconds )
{
	UNUSED( deltaSeconds );

	// *New version to disable physics while vaulting*
	//----------------------------------------------------------------------------------------------------------------------
	// Physics collisions (Player vs map)
	//----------------------------------------------------------------------------------------------------------------------
	PhysicsController& physicsController = *m_player->m_physicsController;
	Vec3& physicsPos					 = physicsController.m_position;
	float playerRadius					 = physicsController.m_playerPhysicsRadius;
	if ( physicsController.m_isPhysicsEnabled )
	{
		for ( int i = 0; i < m_map->m_floorList.size(); i++ )
		{
			// Check if playerSphere is inside floorAbb
			AABB3& curFloor				  = m_map->m_floorList[i];
			Vec3  nearestPointPlayerToAbb = curFloor.GetNearestPoint( physicsPos );
			Vec3  dispNpToPlayerCenter	  = physicsPos - nearestPointPlayerToAbb;
			float distAbbToPlayer		  = dispNpToPlayerCenter.GetLength();
			if ( distAbbToPlayer < playerRadius )
			{
				// Push out
				Vec3 dirNpToPlayerCenter  = dispNpToPlayerCenter.GetNormalized();
				Vec3 correctedPhysicsPos  = nearestPointPlayerToAbb + ( dirNpToPlayerCenter * playerRadius );
				physicsPos				  = correctedPhysicsPos;
			}
		}
	}

	// Update playerPos based on correctPhysicsPos
	m_player->m_position	= physicsPos;
	m_player->m_position.z -= playerRadius;

	// Old version
// 	//----------------------------------------------------------------------------------------------------------------------
// 	// Physics collisions (Player vs map)
// 	//----------------------------------------------------------------------------------------------------------------------
// 	PhysicsController& physicsController = *m_player->m_physicsController;
// 	for ( int i = 0; i < m_map->m_floorList.size(); i++ )
// 	{
// 		AABB3& curFloor = m_map->m_floorList[i];
// 		// Check if playerSphere is inside floorAbb
// 		Vec3& physicsPos			  = physicsController.m_position;
// 		float playerRadius			  = physicsController.m_playerPhysicsRadius;
// 		Vec3  nearestPointPlayerToAbb = curFloor.GetNearestPoint( physicsPos );
// 		Vec3  dispNpToPlayerCenter	  = physicsPos - nearestPointPlayerToAbb;
// 		float distAbbToPlayer		  = dispNpToPlayerCenter.GetLength();
// 		if ( distAbbToPlayer < playerRadius )
// 		{
// 			// Push out
// 			Vec3 dirNpToPlayerCenter  = dispNpToPlayerCenter.GetNormalized();
// 			Vec3 correctedPhysicsPos  = nearestPointPlayerToAbb + ( dirNpToPlayerCenter * playerRadius );
// 			physicsPos				  = correctedPhysicsPos;
// 			m_player->m_position	  = physicsPos;
// 			m_player->m_position.z	 -= playerRadius;
// 		}
// 	}
}


//----------------------------------------------------------------------------------------------------------------------
void GameMode_Animations::UpdateAnimTimeline()
{
	if ( g_theInput->WasKeyJustPressed( '1' ) )
	{
		m_animTimeline.UpdateAnimClipAndBindPose( &m_player->m_animController->m_animClip_FlyingLeftKick, m_player->m_animController->m_animBindPose_FlyingLeftKick );
	}
	if ( g_theInput->WasKeyJustPressed( '2' ) )
	{
		m_animTimeline.UpdateAnimClipAndBindPose( &m_player->m_animController->m_animClip_HighLeftKick, m_player->m_animController->m_animBindPose_HighLeftKick );
	}

	if ( g_theInput->IsKeyDown( KEYCODE_LEFT_MOUSE ) )
	{
		if ( m_animTimeline.IsVisible() )
		{
			Vec2 cursorPosNormalized = g_theWindow->GetNormalizedCursorPos();
			m_animTimeline.UpdateCaretPos( cursorPosNormalized, SCREEN_SIZE_X );
			m_animTimeline.UpdateSampledPose( 8 );
		}
	}
	else if ( g_theInput->WasKeyJustReleased( KEYCODE_LEFT_MOUSE ) )
	{
		m_animTimeline.DeselectCaret();
	}
}


//----------------------------------------------------------------------------------------------------------------------
// void GameMode_Animations::UpdateAnimations( float deltaSeconds )
// {
	//----------------------------------------------------------------------------------------------------------------------
	// Anim sample time test
	//----------------------------------------------------------------------------------------------------------------------
// 	if ( g_theInput->IsKeyDown( KEYCODE_LEFTARROW  ) )
// 	{
// 		m_elapsedAnimationTime--;
// 	}
// 	if ( g_theInput->IsKeyDown( KEYCODE_RIGHTARROW ) )
// 	{
// 		m_elapsedAnimationTime++;
// 	}
// 
// 	//----------------------------------------------------------------------------------------------------------------------
// 	// Anim blending/sampling test
// 	//----------------------------------------------------------------------------------------------------------------------
// 	m_animVerts.clear();
// 	m_totalTimeMS += (deltaSeconds * 1000.0f );
// 
// 	// New approach (multiple animations sampled based on states)
// 	AnimationPose animPoseToRender;
// 	AnimationPose animPoseIdle;
// 	AnimationPose animPoseWalk;
// 	AnimationPose animPoseRun;
// 	animPoseIdle = m_animClip_Idle.SampleAnimPoseAtTime( m_totalTimeMS, m_animBindPose_Idle, true );
// 	animPoseWalk = m_animClip_Walk.SampleAnimPoseAtTime( m_totalTimeMS, m_animBindPose_Walk, true );
// 	animPoseRun	 = m_animClip_Run. SampleAnimPoseAtTime( m_totalTimeMS, m_animBindPose_Run,  true );
// 
// 	// Adjustable velocity parameters for handling transitions between idle/walk/run
// 	float idleToWalkMin =  4.0f;
// 	float idleToWalkMax = 25.0f;
// 	float walkToRunMax  = 50.0f;
// 
// 	//----------------------------------------------------------------------------------------------------------------------
// 	// Base anim (idle/walk/run)
// 	//----------------------------------------------------------------------------------------------------------------------
// 	// Use velocityXY instead of velocityXYZ to ignore gravity
// 	AnimationPose locomotionPose;
// 	Vec2 playerVelocity_V2		= Vec2( m_player->m_velocity.x, m_player->m_velocity.y );
// 	float velocityMagnitudeXY	= playerVelocity_V2.GetLength();
// 	if ( velocityMagnitudeXY > idleToWalkMin && velocityMagnitudeXY < idleToWalkMax )
// 	{
// 		// Idle to walk
// 		float blendFactor			= RangeMapClamped( velocityMagnitudeXY, idleToWalkMin, idleToWalkMax, 0.0f, 1.0f );
// 		locomotionPose				= AnimationPose::GetBlendFromTwoPoses( animPoseIdle, animPoseWalk, blendFactor );
// 		locomotionPose.m_poseName	= "idleToWalk";
// 	}
// 	else if ( velocityMagnitudeXY >= idleToWalkMax )
// 	{
// 		// Walk to run
// 		float blendFactor			= RangeMapClamped( velocityMagnitudeXY, idleToWalkMax, walkToRunMax, 0.0f, 1.0f );
// 		locomotionPose				= AnimationPose::GetBlendFromTwoPoses( animPoseWalk, animPoseRun, blendFactor );
// 		locomotionPose.m_poseName	= "walkToRun";
// 	}
// 	else
// 	{
// 		// Idle
// 		locomotionPose				= animPoseIdle;
// 		locomotionPose.m_poseName	= "Idle";
// //		locomotionPose= m_animClip_TwistDance.SampleAnimPoseAtTime( m_totalTimeMS, m_animBindPose_TwistDance,  true );	// Dance hack
// 	}
// 	animPoseToRender = locomotionPose;
// 
// 	if ( m_player->m_moveState == MOVE_STATE_JUMP )
// 	{
// 		// New attempt with crossfading in/out of jumps		
// 		// basePose is the current walk pose 
// 		// Blend between basePose and jumpPose over a duration
// 		// Sample basePose at total time
// 		// Sample jumpPose at jumpTime
// 		// Blend between both poses
// 		
// 		// Sample poseToRender by incrementing jump time [0, jumpAnimEndTime]
// 		m_animClip_Jump.m_localTime	+= ( deltaSeconds * 1000.0f );
// 		float animTimeJumpMS		= m_animClip_Jump.m_localTime;
// 		float jumpAnimDuration		= m_animClip_Jump.m_endTime;
// 		float minCrossfadeTimeMS	= jumpAnimDuration * 0.1f;	// 100.0f;		// 1/5th of jumpTotalTime, 2,100 MS
// 		float maxCrossfadeTimeMS	= jumpAnimDuration * 0.9f;	
// 		AnimationPose jumpPose		= m_animClip_Jump.SampleAnimPoseAtTime( animTimeJumpMS, m_animBindPose_Jump, false, true );
// 
// 		// Crossfade from current basePose (idle/walk/run) into jump
// 		if ( animTimeJumpMS < minCrossfadeTimeMS )
// 		{
// 			float blendFactor = RangeMapClamped( animTimeJumpMS, 0.0f, minCrossfadeTimeMS, 0.0f, 1.0f );
// 			animPoseToRender  = AnimationPose::GetBlendFromTwoPoses( m_animPoseBlendIntoJump, jumpPose, blendFactor );
// 		}
// 		else if ( animTimeJumpMS > maxCrossfadeTimeMS )
// 		{
// 			float blendFactor	= RangeMapClamped( animTimeJumpMS, maxCrossfadeTimeMS, jumpAnimDuration, 0.0f, 1.0f );
// 			animPoseToRender	= AnimationPose::GetBlendFromTwoPoses( jumpPose, locomotionPose, blendFactor );
// 		}
// 		else
// 		{
// 			animPoseToRender = jumpPose;
// 		}
// 
// 		// Reset anim state, later the jump animation should trigger an event when jump is finished,
// 		// the state machine should listen and modify the state accordingly back to idle
// 		if ( m_animClip_Jump.IsFinished() )
// 		{
// 			m_player->m_moveState = MOVE_STATE_LOCOMOTION;
// 			m_player->m_canJump		  = true;
// 			m_player->m_isJumping	  = false;
// 		}
// 
// 
// 		// Old version w/o crossfade
// //		// Sample poseToRender by incrementing jump time [0, jumpAnimEndTime]
// //		animTimeJumpMS += ( deltaSeconds * 1000.0f );
// //		animPoseToRender  = m_animClip_Jump.SampleAnimPoseAtTime( animTimeJumpMS, m_animBindPose_Jump, false );
// //
// //		// Reset anim state, later the jump animation should trigger an event when jump is finished,
// //		// the state machine should listen and modify the state accordingly back to idle
// //		if ( m_animClip_Jump.m_isFinishedPlaying )
// //		{
// //			m_player->m_movementState = MOVEMENT_STATE_IDLE;
// //			m_player->m_canJump		  = true;
// //			m_player->m_isJumping	  = false;
// //		}
// 	}
// 	m_debugPoseName			= animPoseToRender.m_poseName;
// 	m_animPoseLastFrame		= animPoseToRender;
// 
// 
// 	if ( m_player->m_moveState == MOVE_STATE_TURN )
// 	{
// 		// Blend basePose (idle/walk/run) with turnAnim
//  		m_animTimeTurnLeftMS += ( deltaSeconds * 1000.0f );
// 		animPoseToRender	  = m_animClip_TurnLeft.SampleAnimPoseAtTime( m_animTimeTurnLeftMS, m_animBindPose_TurnLeft, false, false ); 
//  		if ( m_animClip_TurnLeft.IsFinished() )
//  		{
//  			m_player->m_moveState				= MOVE_STATE_LOCOMOTION;
//  			m_player->m_canTurn						= true;
//  			m_player->m_orientation.m_yawDegrees   += 90.0f;
// 
// // 			float blendFactor = RangeMapClamped( m_animTimeTurnLeftMS, maxCrossfadeTimeMS, jumpAnimDuration, 0.0f, 1.0f );
// // 			animPoseToRender  = AnimationPose::GetBlendFromTwoPoses( animPoseToRender, animPoseIdle, blendFactor );
// 
// 			// walking
// 			// turning
// 			// cross fading, turning -> walking
// 		}
// 	}

	// (OLD approach)
	// 	if ( m_player->m_movementState == MOVEMENT_STATE_TURN )
// 	{
// 		// Blend basePose (idle/walk/run) with turnAnim
//  		m_animTimeTurnLeftMS		   += ( deltaSeconds * 1000.0f );
//  		AnimationPose animPoseTurnLeft	= m_animClip_TurnLeft.SampleAnimPoseAtTime( m_animTimeTurnLeftMS, m_animBindPose_TurnLeft, false );
//  		animPoseToRender				= animPoseTurnLeft; 
// 
// 		float blendFactor = RangeMapClamped( m_player->m_yawDegreesToGoalDir, 0.0f, m_player->m_initialYawDegreesToGoalDir, 0.0f, 1.0f );
// 		animPoseToRender = AnimationPose::GetBlendFromTwoPoses( animPoseTurnLeft, animPoseToRender, blendFactor );
// 
//  
//  		if ( m_animClip_TurnLeft.m_isFinishedPlaying )
//  		{
//  			m_player->m_movementState = MOVEMENT_STATE_IDLE;
//  			m_player->m_canTurn = true;
// 
// 		// Blend basePose (idle/walk/run) with turnAnim
// // 		m_animTimeTurnLeftMS		   += ( deltaSeconds * 1000.0f );
// // 		AnimationPose animPoseTurnLeft	= m_animClip_TurnLeft.SampleAnimPoseAtTime( m_animTimeTurnLeftMS, m_animBindPose_TurnLeft, false );
// // 		float initialYawDegreesToGoal	= m_player->m_initialYawDegreesToGoalDir;
// // 		float curYawDegreesToGoal		= m_player->m_yawDegreesToGoalDir;
// // 		float blendFactor				= RangeMapClamped( curYawDegreesToGoal, initialYawDegreesToGoal, 0.0f, 0.0f, 1.0f );
// // 		animPoseToRender				= AnimationPose::GetBlendFromTwoPoses( animPoseTurnLeft, animPoseToRender, blendFactor );
// // 
// // 		if ( m_animClip_TurnLeft.m_isFinishedPlaying )
// // 		{
// // 			m_player->m_movementState = MOVEMENT_STATE_IDLE;
// // 			m_player->m_canTurn = true;
// 		}
// 	}

	// AddVerts to debug draw joints & bones
// 	for ( int jointIndex = 0; jointIndex < animPoseToRender.m_jointList.size(); jointIndex++ )
// 	{
// 		AnimationJoint const& joint			= animPoseToRender.m_jointList[jointIndex];
// 		Transform const& curJointTransform  = animPoseToRender.LocalToModel_Transform(jointIndex);
// 		if ( joint.m_parentJointIndex >= 0 )
// 		{
// 			Transform const& parentJointTransform = animPoseToRender.LocalToModel_Transform( joint.m_parentJointIndex );
// 			AddVertsForCone3D( m_animVerts, parentJointTransform.m_position, curJointTransform.m_position, 0.5f );
// 		}
// 	}

	// Old approach (only one animation sampled)
	// 	m_animVerts.clear();
	// //	m_totalTimeMS += (deltaSeconds * 1000.0f);
	// 	float velocityMagnitude			 = m_player->m_velocity.GetLength();
	// 	float animSpeedScalarParametric  = RangeMapClamped( velocityMagnitude, 0.05f, 0.15f, 0.0f, 1.0f );
	// 	m_totalTimeMS					+= (deltaSeconds * 1000.0f * animSpeedScalarParametric);
	// 	AnimationPose newAnimPose;
	// 	if ( m_totalTimeMS > m_animationClip_Run.m_endTime )
	// 	{
	// 		m_totalTimeMS = 0.0f;
	// 	}
	// 	newAnimPose = m_animationClip_Run.SampleAnimPoseAtTime( m_totalTimeMS, m_bindAnimationPose_Run, true );
	// 	for ( int jointId = 0; jointId < newAnimPose.m_jointList.size(); jointId++ )
	// 	{
	// 		AnimationJoint const& joint = newAnimPose.m_jointList[jointId];
	// 		Transform const& curJointTransform = newAnimPose.LocalToModel_Transform(jointId);
	// 		if ( joint.m_parentJointIndex >= 0 )
	// 		{
	// 			Transform const& parentJointTransform = newAnimPose.LocalToModel_Transform( joint.m_parentJointIndex );
	// 			AddVertsForCone3D( m_animVerts, parentJointTransform.m_position, curJointTransform.m_position, 0.5f );
	// 		}
	// 	}
// }
