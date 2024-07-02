#include "Game/App.hpp"
#include "Game/GameMode_QuaternionTest.hpp"
#include "Game/Quadruped.hpp"
#include "Game/Map_Animations.hpp"

#include "Engine/Animations/FbxFileImporter.hpp"
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

#include <stack>

//----------------------------------------------------------------------------------------------------------------------
GameMode_QuaternionTest::GameMode_QuaternionTest()
{
	// Stack test
// 	std::stack <int> stackList;
// 	int num1 = 10;
// 	int num2 = 20;
// 	int num3 = 30;
// 	stackList.push( num1 );
// 	stackList.push( num2 );
// 	stackList.push( num3 );
// 	stackList.pop();					// Pop() removes the top/last element 
// 	int topValue = stackList.top();		// Top() returns a reference to the top/last element 
// 
// 
// 	// Vector test
// 	Vec3 test = Vec3( 0.7f, 0.7f, 0.0f );
// 	Vec3 test2 = test.GetNormalized();
// 
// 	int* pointer1 = new int;
// 	*pointer1	  = 8;
// 	pointer1++;
// 	pointer1--;
// 	int  number1  = 5;
// 	int& number2  = number1;
// 	int* address1 = &number1;
// 	int* address2 = &number2;
// 	float rangeMappedResult = RangeMapClamped( 13.0f, 10.0f, 15.0f, 200.0f, 250.0f );
// 	float lerpedValue		= Interpolate( 10.0f, 25.0f, 0.2f );
// 	int x = 0;
// 	UNUSED( address1 );
// 	UNUSED( address2 );
// 	UNUSED( rangeMappedResult );
// 	UNUSED( lerpedValue );
// 
// 	// Test code
// 	// Definition for singly - linked list.
// 	struct ListNode
// 	{
// 		int val;
// 		ListNode* next;
// 		
// 		ListNode() 
// 			: val( 0 )
// 			, next( nullptr ) {}
// 		
// 		ListNode( int x ) 
// 			: val( x )
// 			, next( nullptr ) {}
// 		
// 		ListNode( int x, ListNode* next ) 
// 			: val( x )
// 			, next( next ) {}
// 	};
// 
// 	// L1
// 	ListNode* nodeA1 = new ListNode( 2 );
// 	ListNode* nodeB1 = new ListNode( 4 );
// 	ListNode* nodeC1 = new ListNode( 9 );
// 	nodeA1->next = nodeB1;
// 	nodeB1->next = nodeC1;
// 	// L2
// 	ListNode* nodeA2 = new ListNode( 5 );
// 	ListNode* nodeB2 = new ListNode( 6 );
// 	ListNode* nodeC2 = new ListNode( 4 );
// 	ListNode* nodeD2 = new ListNode( 9 );
// 	nodeA2->next = nodeB2;
// 	nodeB2->next = nodeC2;
// 	nodeC2->next = nodeD2;
// 
// 
// 	ListNode* l1 = nodeA1;
// 	ListNode* l2 = nodeA2;
// 
// 	{
// 		ListNode* node1 = l1;
// 		ListNode* node2 = l2;
// 
// 		std::vector<ListNode*> nodeList;     // In reverse order
// 		ListNode* answerListNode   = nullptr;
// 		bool shouldCreateExtraNode = false;
// 		// Add 2 numbers
// 		while ( node1 || node2 )
// 		{
// 			// Compute sum of values
// 			int value1 = 0;
// 			if ( node1 )
// 			{
// 				value1 = node1->val;
//                 node1  = node1->next;
// 			}
//             else 
//             {
//                 node1 = nullptr;
//             }
// 			int value2 = 0;
// 			if ( node2 )
// 			{
// 				value2 = node2->val;
//                 node2 = node2->next;
// 			}
//             else
//             {
//                 node2 = nullptr;
//             }
// 			int sum = value1 + value2;
//             if ( sum >= 10 )
//             {
//                 if ( node2 )
//                 {
//                     node2->val += 1;
//                 }
//                 else if ( node1 )
//                 {
//                     node1->val += 1;
//                 }
//                 else 
//                 {
//                     // Create new node
//                     shouldCreateExtraNode = true;
//                 }
//                 sum = sum % 10;
//             }
// 
// 			// Get child nodes
// 			ListNode* newNode = new ListNode( sum );
// 			nodeList.push_back( newNode );
// 		}
//         if ( shouldCreateExtraNode )
//         {
//             ListNode* newNode = new ListNode( 1 );
// 			nodeList.push_back( newNode );
//         }
// 
// 
// 		// Return in reverse order
// /*
// 		int nodeListSize = nodeList.size() - 1;
// 		for ( int i = nodeListSize; i > 0; i-- )
// 		{
// 			ListNode* curListNode  = nodeList[ i ];
// 			ListNode* nextListNode = nodeList[ i - 1 ];
// 
// 			curListNode->next = nextListNode;
// 		}
// 		answerListNode = nodeList[ nodeListSize ];
// */
// 
//         int nodeListSize = int( nodeList.size() );
// 		for ( int i = 1; i < nodeListSize; i++ )
// 		{
// 			ListNode* curListNode  = nodeList[ i - 1];
// 			ListNode* nextListNode = nodeList[ i ];
// 			curListNode->next      = nextListNode;
// 		}
//         answerListNode = nodeList[0];
// 		x = 1;
// 		UNUSED(x);
// 	}
}

//----------------------------------------------------------------------------------------------------------------------
GameMode_QuaternionTest::~GameMode_QuaternionTest()
{
}

//----------------------------------------------------------------------------------------------------------------------
void GameMode_QuaternionTest::Startup()
{
	g_debugFreeFly_F1			= true;
	g_debugRenderRaycast_F2		= true;
	g_debugBasis_F3				= true;
	g_debugAngles_F5			= true;
	g_debugToggleConstraints_F8 = true;
	g_debugFollowTarget_F11		= false;

	InitializeCreatures();
	m_ikChain_FABRIK->m_isSingleStep_Debug	= false;
	m_ikChain_FABRIK->m_numIterations		= 100;


	//----------------------------------------------------------------------------------------------------------------------
	// Initialize Player cam pos and orientation
	//----------------------------------------------------------------------------------------------------------------------
// 	m_gameMode3DWorldCamera.m_position						= Vec3( 40.0f, 50.0f, 10.0f ); 
// 	m_gameMode3DWorldCamera.m_orientation.m_yawDegrees		= -90.0f;
// 	m_gameMode3DWorldCamera.m_orientation.m_pitchDegrees	=  15.0f;
	m_gameMode3DWorldCamera.m_position						= Vec3( 0.0f, 120.0f, 20.0f );
	m_gameMode3DWorldCamera.m_orientation					= EulerAngles( -90.0f, 5.0f, 0.0f );

	// Init lighting
	EulerAngles sunOrientation = EulerAngles( 0.0f, 65.0f, 0.0f );
	m_sunDirection = sunOrientation.GetForwardDir_XFwd_YLeft_ZUp();

	 m_map = new Map_Animations();
}


//----------------------------------------------------------------------------------------------------------------------
void GameMode_QuaternionTest::Update( float deltaSeconds )
{	
	// Update core systems
	UpdatePauseQuitAndSlowMo();
	UpdateDebugKeys();
	UpdateCameraAndCreatureInput( deltaSeconds );
	
	// Update Creature
	UpdateTargetInput( deltaSeconds );
	UpdateCreature( deltaSeconds );
	// Clamp chainInfoIndex
	int numSolutions = int( m_ikChain_FABRIK->m_solutionsThisFrameList.size() - 1 );
	m_chainInfoIndex = GetClamped( m_chainInfoIndex, 0, numSolutions );
	if ( m_chainInfoIndex < 0 )
	{
		m_chainInfoIndex = 0;
	}


	//----------------------------------------------------------------------------------------------------------------------
	// Quat test
	//----------------------------------------------------------------------------------------------------------------------
//	Stringf( "(Q origin) quatX: %0.2f, quatY: %0.2f, quatZ: %0.2f, quatW: %0.2f\n", m_quat.x, m_quat.y, m_quat.z, m_quat.w).c_str();

	// Matrix->Quat test
	Vec3  start				= Vec3( 0.0f, 0.0f, 50.0f );
	Vec3  quadFwd			= m_quat * Vec3::X_FWD;
	Vec3  quadLeft			= m_quat * Vec3::Y_LEFT;
	Vec3  quadUp			= m_quat * Vec3::Z_UP;
	Mat44 matTest			= Mat44( quadFwd, quadLeft, quadUp, start );
	Quaternion quatFromMat	= Quaternion::GetAsQuaternion( matTest );
	m_quatTest_mToQ			= quatFromMat;
//	DebuggerPrintf( Stringf( "(Mat->Quat), quatX: %0.2f, quatY: %0.2f, quatZ: %0.2f, quatW: %0.2f\n\n", m_quatTest_mToQ.x, m_quatTest_mToQ.y, m_quatTest_mToQ.z, m_quatTest_mToQ.w ).c_str() );

	// Update Camera
	UpdateGameMode3DCamera();
}


//----------------------------------------------------------------------------------------------------------------------
void GameMode_QuaternionTest::Render() const
{
	RenderWorldObjects();
	RenderUIObjects();
}


//----------------------------------------------------------------------------------------------------------------------
void GameMode_QuaternionTest::Shutdown()
{
}


//----------------------------------------------------------------------------------------------------------------------
void GameMode_QuaternionTest::UpdateDebugKeys()
{
	// Toggle common debug bools
	if ( g_theInput->WasKeyJustPressed( KEYCODE_F1 ) )
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
		SetChainConstraints();
	}
	if ( g_theInput->WasKeyJustPressed( KEYCODE_F11 ) )
	{
		g_debugFollowTarget_F11 = !g_debugFollowTarget_F11;
	}
	if ( g_theInput->WasKeyJustPressed( KEYCODE_BACKSPACE ) )
	{
//		m_ikChain_FABRIK->m_renderEulerAnglesGoal_notCur != m_ikChain_FABRIK->m_renderEulerAnglesGoal_notCur;
 		m_ikChain_FABRIK->m_lerpEulerToGoal = !m_ikChain_FABRIK->m_lerpEulerToGoal;
	}
	if ( g_theInput->WasKeyJustPressed( KEYCODE_HOME ) )
	{
		m_ikChain_FABRIK->SetChainPreference_MatrixToEuler( FWD_LEFT_TEMPORAL );
	}
	if ( g_theInput->WasKeyJustPressed( KEYCODE_END ) )
	{
		m_ikChain_FABRIK->SetChainPreference_MatrixToEuler( YAW_PREFERRED );
	}
	if ( g_theInput->WasKeyJustPressed( KEYCODE_INSERT ) )
	{
		m_ikChain_FABRIK->SetChainPreference_MatrixToEuler( PITCH_PREFERRED );
	}
	if ( g_theInput->WasKeyJustPressed( KEYCODE_DELETE ) )
	{
		m_ikChain_FABRIK->m_attemptDeadlockSolutions = !m_ikChain_FABRIK->m_attemptDeadlockSolutions;
	}


	// Control camera dist from player
	if ( g_theInput->IsKeyDown( '9' ) )
	{
		m_distCamAwayFromPlayer += 10.0f;
	}
	if ( g_theInput->IsKeyDown( '0' ) )
	{
		m_distCamAwayFromPlayer -= 10.0f;
	}

	// Preset positions for target
	// X
	if ( g_theInput->WasKeyJustPressed( '1' ) )
	{
		m_ikChain_FABRIK->m_target.m_currentPos_WS = Vec3( 30.0f, 0.0f, 0.0f );
	}
	// Y
	if ( g_theInput->WasKeyJustPressed( '2' ) )
	{
		m_ikChain_FABRIK->m_target.m_currentPos_WS = Vec3( 0.0f, 30.0f, 0.0f );
	}
	// Z
	if ( g_theInput->WasKeyJustPressed( '3' ) )
	{
		m_ikChain_FABRIK->m_target.m_currentPos_WS = Vec3( 0.0f, 0.0f, 30.0f );
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

	//----------------------------------------------------------------------------------------------------------------------
	// Increase / decrease num solverIterations
	//----------------------------------------------------------------------------------------------------------------------
	if ( g_theInput->WasKeyJustPressed( '9' ) )
	{
		// Decrease
		m_ikChain_FABRIK->m_numIterations--;
		m_ikChain_FABRIK->m_numIterations = GetClamped( m_ikChain_FABRIK->m_numIterations, 0, INT_MAX );
	}
	if ( g_theInput->WasKeyJustPressed( '0' ) )
	{
		// Decrease
		m_ikChain_FABRIK->m_numIterations++;
	}
	if ( g_theInput->WasKeyJustPressed( '5' ) )
	{
		// Decrease by 10
		m_ikChain_FABRIK->m_numIterations -= 100;
		m_ikChain_FABRIK->m_numIterations = GetClamped( m_ikChain_FABRIK->m_numIterations, 0, INT_MAX );
	}
	if ( g_theInput->WasKeyJustPressed( '6' ) )
	{
		// Decrease by 10
		m_ikChain_FABRIK->m_numIterations += 100;
	}
	if ( g_theInput->WasKeyJustPressed( '7' ) )
	{
		// Decrease by 10
		m_ikChain_FABRIK->m_numIterations -= 10;
		m_ikChain_FABRIK->m_numIterations  = GetClamped( m_ikChain_FABRIK->m_numIterations, 0, INT_MAX );
	}
	if ( g_theInput->WasKeyJustPressed( '8' ) )
	{
		// Decrease by 10
		m_ikChain_FABRIK->m_numIterations += 10;
	}


	//----------------------------------------------------------------------------------------------------------------------
	// Cycle chainInfoIndex
	//----------------------------------------------------------------------------------------------------------------------
	if ( g_theInput->IsKeyDown( KEYCODE_SHIFT ) )
	{
		if ( g_theInput->IsKeyDown( KEYCODE_SEMICOLON ) )
		{
			m_chainInfoIndex--;
			m_chainInfoIndex = GetClamped( m_chainInfoIndex, 0, INT_MAX );
		}
		if ( g_theInput->IsKeyDown( KEYCODE_SINGLEQUOTE ) )
		{
			m_chainInfoIndex++;
			int numSolutions = int( m_ikChain_FABRIK->m_solutionsThisFrameList.size() - 1 );
			m_chainInfoIndex = GetClamped( m_chainInfoIndex, 0, numSolutions );
			if ( m_chainInfoIndex < 0 )
			{
				m_chainInfoIndex = 0;
			}
		}
	}
	else
	{
		if ( g_theInput->WasKeyJustPressed( KEYCODE_SEMICOLON ) )
		{
			m_chainInfoIndex--;
		}
		if ( g_theInput->WasKeyJustPressed( KEYCODE_SINGLEQUOTE ) )
		{
			m_chainInfoIndex++;
		}
	}
}


//----------------------------------------------------------------------------------------------------------------------
void GameMode_QuaternionTest::UpdateGameMode3DCamera()
{
	//----------------------------------------------------------------------------------------------------------------------
	// Camera follows mouse
	//----------------------------------------------------------------------------------------------------------------------
	// Implement displacement.x to yaw and displacement.y to pitch
	//----------------------------------------------------------------------------------------------------------------------

	// Update WORLD camera as perspective
	Vec2 cursorClientDelta								  = g_theInput->GetCursorClientDelta();
	float mouseSpeed									  = 0.05f;
	float yaw											  = cursorClientDelta.x * mouseSpeed;
	float pitch											  = cursorClientDelta.y * mouseSpeed;
	m_gameMode3DWorldCamera.m_orientation.m_yawDegrees	 -= yaw;
	m_gameMode3DWorldCamera.m_orientation.m_pitchDegrees += pitch;
	
	m_gameMode3DWorldCamera.m_orientation.m_pitchDegrees = GetClamped( m_gameMode3DWorldCamera.m_orientation.m_pitchDegrees, -85.0f, 85.0f );

	if ( g_debugFreeFly_F1 )
	{
		// Set cameraPos roam "freely"
		m_gameMode3DWorldCamera.SetPerspectiveView( 2.0f, 60.0f, 0.1f, 10000.0f );
		m_gameMode3DWorldCamera.SetRenderBasis( Vec3(0.0f, 0.0f, 1.0f), Vec3(-1.0f, 0.0f, 0.0f), Vec3(0.0f, 1.0f, 0.0f) );
	}

	// Update UI camera
	m_gameMode3DUICamera.SetOrthoView( Vec2( 0.0f, 0.0f ), Vec2( WORLD_SIZE_X, WORLD_SIZE_Y ) );		
}


//----------------------------------------------------------------------------------------------------------------------
void GameMode_QuaternionTest::RenderWorldObjects() const
{
	//----------------------------------------------------------------------------------------------------------------------
	// Begin World Camera
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
	Vec3 sunDir = m_sunDirection.GetNormalized();
	g_theRenderer->SetLightingConstants( sunDir, m_sunIntensity, m_ambientIntensity, m_gameMode3DWorldCamera.m_position, 0, 1, m_specularIntensity, m_specularPower );


	//----------------------------------------------------------------------------------------------------------------------
	// Render Map
//	m_map->Render( verts );

	//----------------------------------------------------------------------------------------------------------------------
	// Render world compass
	AddVertsForCompass( compassVerts, Vec3( 0.0, 0.0f, -1.0f ), 100.0f, 0.1f );

	//----------------------------------------------------------------------------------------------------------------------
	// "ProtoMode" title
	Vec3 textOrigin = Vec3( 200.0f, 150.0f,  0.0f );
	Vec3 iBasis		= Vec3(	  0.0f,  -1.0f,  0.0f );
	Vec3 jBasis		= Vec3(	  0.0f,   0.0f,  1.0f );
	g_theApp->m_textFont->AddVertsForText3D( textVerts, textOrigin, iBasis, jBasis, 25.0f, "Quaternion test!", Rgba8::GREEN );

	//----------------------------------------------------------------------------------------------------------------------
	// Render skybox
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
	// Quaternion visualizer
	//----------------------------------------------------------------------------------------------------------------------	
	// Quat->Matrix, take world XYZ vectors and multiply by quat
	Vec3  xEast		= Vec3::X_FWD;
	Vec3  yNorth	= Vec3::Y_LEFT;
	Vec3  zSky		= Vec3::Z_UP;
	Vec3  quatFwd	= m_quat * xEast;
	Vec3  quatLeft	= m_quat * yNorth;
	Vec3  quatUp	= m_quat * zSky;
	float scalar	= 10.0f;
	AddVertsForArrow3D( verts, Vec3(), (quatFwd	 * scalar), 1.0f, Rgba8::RED   ); 
	AddVertsForArrow3D( verts, Vec3(), (quatLeft * scalar), 1.0f, Rgba8::GREEN ); 
	AddVertsForArrow3D( verts, Vec3(), (quatUp	 * scalar), 1.0f, Rgba8::BLUE  ); 
//	DebuggerPrintf( Stringf( "(Q origin) quatX: %0.2f, quatY: %0.2f, quatZ: %0.2f, quatW: %0.2f\n", m_quat.x, m_quat.y, m_quat.z, m_quat.w).c_str() );

	// Test for multiplying by inverse quat
// 	xEast			= m_quat * xEast  * -1.0f;
// 	yNorth			= m_quat * yNorth * -1.0f;
// 	zSky			= m_quat * zSky	  * -1.0f;
// 	quadFwd			= xEast;
// 	quadLeft		= yNorth;
// 	quadUp			= zSky;
// 	Vec3 start		= Vec3(0.0f, 50.0f, 0.0f);
// 	AddVertsForArrow3D( verts, start, start + quadFwd	* scalar, 1.0f, Rgba8::RED	 ); 
// 	AddVertsForArrow3D( verts, start, start + quadLeft	* scalar, 1.0f, Rgba8::GREEN ); 
// 	AddVertsForArrow3D( verts, start, start + quadUp	* scalar, 1.0f, Rgba8::BLUE  ); 

	//----------------------------------------------------------------------------------------------------------------------
	// Matrix->Quat test
	//----------------------------------------------------------------------------------------------------------------------
	Vec3 start				= Vec3( 0.0f, 0.0f, 50.0f );
	Mat44 matTest			= Mat44( quatFwd, quatLeft, quatUp, start );
	Quaternion quatFromMat	= Quaternion::GetAsQuaternion( matTest );
	quatFwd					= quatFromMat * xEast;
	quatLeft				= quatFromMat * yNorth;
	quatUp					= quatFromMat * zSky;
	AddVertsForArrow3D( verts, start, (start + quatFwd	* scalar), 1.0f, Rgba8::RED   );
	AddVertsForArrow3D( verts, start, (start + quatLeft	* scalar), 1.0f, Rgba8::GREEN ); 
	AddVertsForArrow3D( verts, start, (start + quatUp	* scalar), 1.0f, Rgba8::BLUE  ); 
//	DebuggerPrintf( Stringf( "(Mat->Quat), quatX: %0.2f, quatY: %0.2f, quatZ: %0.2f, quatW: %0.2f\n\n", quatFromMat.x, quatFromMat.y, quatFromMat.z, quatFromMat.w).c_str() );


	//----------------------------------------------------------------------------------------------------------------------
	// End world camera
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
	
	// Test animation objects
	g_theRenderer->SetBlendMode( BlendMode::ALPHA );
	g_theRenderer->SetModelConstants();
	g_theRenderer->BindTexture( nullptr );
	g_theRenderer->BindShader( nullptr );
	g_theRenderer->DrawVertexArray( static_cast<int>( m_animVerts.size() ), m_animVerts.data() );

	// Transformed ikChain model origin
	g_theRenderer->SetBlendMode( BlendMode::ALPHA );
	Mat44 ikChainMat = m_ikChain_FABRIK->m_firstJoint->GetIkChainMatrix_ModelToWorld();
	g_theRenderer->SetModelConstants( ikChainMat );
	g_theRenderer->BindTexture( nullptr );
	g_theRenderer->BindShader( nullptr );
	g_theRenderer->DrawVertexArray( static_cast<int>( verts_ikChainBody.size() ), verts_ikChainBody.data() );

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
	g_theRenderer->DrawVertexArray( static_cast<int>( textVerts.size() ), textVerts.data() );
	// Reset binded texture
	g_theRenderer->BindTexture( nullptr );
}


//----------------------------------------------------------------------------------------------------------------------
void GameMode_QuaternionTest::RenderUIObjects() const
{
	//----------------------------------------------------------------------------------------------------------------------
	// Begin UI Camera
	g_theRenderer->BeginCamera( m_gameMode3DUICamera );

	//----------------------------------------------------------------------------------------------------------------------
	// Get or Create font
	std::vector<Vertex_PCU> textVerts;
	Rgba8 textColor_darkRed		= Rgba8::GREEN;
	Rgba8 textColor_darkGreen	= Rgba8::DARK_GREEN;
 	// Initialize and set UI variables
	float cellHeight		= 2.0f;
 	AABB2 textbox1			= AABB2( Vec2( 0.0f, 0.0f ), Vec2( WORLD_SIZE_X, WORLD_SIZE_Y - 1.0f ) );	
 	Vec2 timeAlignment		= Vec2( 1.0f, 1.0f  );
 	std::string gameInfo	= Stringf( "F1 (Toggle camera/player) Player || Mode (F6/F7 for prev/next) || Quaternion Test (3D)" );
 	if ( g_debugFreeFly_F1 )
 	{
 		gameInfo = Stringf( "F1 (Toggle camera/player) Camera || Mode (F6/F7 for prev/next) || Quaternion Test (3D)" );
 	}
 	g_theApp->m_textFont->AddVertsForTextInBox2D( textVerts, textbox1, cellHeight, gameInfo, textColor_darkRed, 0.75f, Vec2( 0.0f, 1.0f ), TextDrawMode::SHRINK_TO_FIT );
 
 	// Core Values
 	float fps							= 1.0f / g_theApp->m_gameClock.GetDeltaSeconds();
 	float scale							= g_theApp->m_gameClock.GetTimeScale();
 	std::string cameraPosText			= Stringf( "Cam position:            %0.2f, %0.2f, %0.2f",	 m_gameMode3DWorldCamera.m_position.x,					m_gameMode3DWorldCamera.m_position.y,						m_gameMode3DWorldCamera.m_position.z );
 	std::string cameraOrientationText	= Stringf( "Cam Orientation (YPR):   %0.2f, %0.2f, %0.2f",	 m_gameMode3DWorldCamera.m_orientation.m_yawDegrees,		m_gameMode3DWorldCamera.m_orientation.m_pitchDegrees,		m_gameMode3DWorldCamera.m_orientation.m_rollDegrees );
 	std::string timeText				= Stringf( "Time: %0.2f. FPS: %0.2f, Scale %0.2f.", g_theApp->m_gameClock.GetTotalSeconds(), fps, scale );
 	std::string quatValuesText			= Stringf( "quatValues: X:%0.2f, Y:%0.2f, Z:%0.2f, W:%0.2f", m_quat.x, m_quat.y, m_quat.z, m_quat.w ); 
	std::string quatOriginText			= Stringf( "(Q origin),  quatX: %0.2f, quatY: %0.2f, quatZ: %0.2f, quatW: %0.2f\n",   m_quat.x, m_quat.y, m_quat.z, m_quat.w).c_str();
	std::string matToQuatText			= Stringf( "(Mat->Quat), quatX: %0.2f, quatY: %0.2f, quatZ: %0.2f, quatW: %0.2f\n\n", m_quatTest_mToQ.x, m_quatTest_mToQ.y, m_quatTest_mToQ.z, m_quatTest_mToQ.w ).c_str();


 	float textAlignmentY	= 1.0f;
 	float textHeight		= 0.03f;
 	g_theApp->m_textFont->AddVertsForTextInBox2D( textVerts, textbox1, cellHeight,				   timeText, textColor_darkRed, 0.75f, Vec2( 1.0f, textAlignmentY				), TextDrawMode::SHRINK_TO_FIT );
 	g_theApp->m_textFont->AddVertsForTextInBox2D( textVerts, textbox1, cellHeight, 			  cameraPosText, textColor_darkRed, 0.75f, Vec2( 0.0f, textAlignmentY -= textHeight ), TextDrawMode::SHRINK_TO_FIT );
 	g_theApp->m_textFont->AddVertsForTextInBox2D( textVerts, textbox1, cellHeight,    cameraOrientationText, textColor_darkRed, 0.75f, Vec2( 0.0f, textAlignmentY -= textHeight ), TextDrawMode::SHRINK_TO_FIT );
 	g_theApp->m_textFont->AddVertsForTextInBox2D( textVerts, textbox1, cellHeight,			 quatValuesText, textColor_darkRed, 0.75f, Vec2( 0.0f, textAlignmentY -= textHeight ), TextDrawMode::SHRINK_TO_FIT );
	g_theApp->m_textFont->AddVertsForTextInBox2D( textVerts, textbox1, cellHeight,			 quatOriginText, textColor_darkRed, 0.75f, Vec2( 0.0f, textAlignmentY -= textHeight ), TextDrawMode::SHRINK_TO_FIT );
	g_theApp->m_textFont->AddVertsForTextInBox2D( textVerts, textbox1, cellHeight,			  matToQuatText, textColor_darkRed, 0.75f, Vec2( 0.0f, textAlignmentY -= textHeight ), TextDrawMode::SHRINK_TO_FIT );
 
  
 	textAlignmentY = 0.97f;
 	if ( g_debugText_F4 )
 	{
 		//----------------------------------------------------------------------------------------------------------------------
 		// Bend More angles
 		//----------------------------------------------------------------------------------------------------------------------
 		// Debt angle
 		g_theApp->m_textFont->AddVertsForTextInBox2D(	textVerts, textbox1, cellHeight, 
 			Stringf( "Debt Angle: %0.2f", m_ikChain_FABRIK->m_debtAngle ), textColor_darkRed, 0.75f, Vec2( 0.0f, textAlignmentY -= textHeight ) );
 		// Joint 0 (root)
 		g_theApp->m_textFont->AddVertsForTextInBox2D(	textVerts, textbox1, cellHeight, 
 			Stringf( "Joint0 bendMore_pitch: %0.2f", m_ikChain_FABRIK->m_jointList[ 0 ]->m_bendMore_pitch ), textColor_darkRed, 0.75f, Vec2( 0.0f, textAlignmentY -= textHeight ) );
 		// Joint 1
 		if ( m_ikChain_FABRIK->m_jointList.size() >= 2 )
 		{
 			g_theApp->m_textFont->AddVertsForTextInBox2D( textVerts, textbox1, cellHeight,
 				Stringf( "Joint1 bendMore_pitch: %0.2f", m_ikChain_FABRIK->m_jointList[ 1 ]->m_bendMore_pitch ),  textColor_darkRed, 0.75f, Vec2( 0.0f, textAlignmentY -= textHeight ) );
		}
		// Joint 2
		if ( m_ikChain_FABRIK->m_jointList.size() >= 3 )
		{
			g_theApp->m_textFont->AddVertsForTextInBox2D( textVerts, textbox1, cellHeight,
				Stringf( "Joint2 bendMore_pitch: %0.2f", m_ikChain_FABRIK->m_jointList[ 2 ]->m_bendMore_pitch ),  textColor_darkRed, 0.75f, Vec2( 0.0f, textAlignmentY -= textHeight ) );
		}
		// Joint 3
		if ( m_ikChain_FABRIK->m_jointList.size() >= 4 )
		{
			g_theApp->m_textFont->AddVertsForTextInBox2D( textVerts, textbox1, cellHeight,
				Stringf( "Joint3 bendMore_pitch: %0.2f", m_ikChain_FABRIK->m_jointList[ 3 ]->m_bendMore_pitch ),  textColor_darkRed, 0.75f, Vec2( 0.0f, textAlignmentY -= textHeight ) );
		}
		// Joint 4
		if ( m_ikChain_FABRIK->m_jointList.size() >= 5 )
		{
			g_theApp->m_textFont->AddVertsForTextInBox2D( textVerts, textbox1, cellHeight,
				Stringf( "Joint4 bendMore_pitch: %0.2f", m_ikChain_FABRIK->m_jointList[ 4 ]->m_bendMore_pitch ),  textColor_darkRed, 0.75f, Vec2( 0.0f, textAlignmentY -= textHeight ) );
		}
		// Joint 5
		if ( m_ikChain_FABRIK->m_jointList.size() >= 6 )
		{
			g_theApp->m_textFont->AddVertsForTextInBox2D( textVerts, textbox1, cellHeight,
				Stringf( "Joint5 bendMore_pitch: %0.2f", m_ikChain_FABRIK->m_jointList[ 5 ]->m_bendMore_pitch ),  textColor_darkRed, 0.75f, Vec2( 0.0f, textAlignmentY -= textHeight ) );
		}
		// Joint 6
		if ( m_ikChain_FABRIK->m_jointList.size() >= 7 )
		{
			g_theApp->m_textFont->AddVertsForTextInBox2D( textVerts, textbox1, cellHeight,
				Stringf( "Joint6 bendMore_pitch: %0.2f", m_ikChain_FABRIK->m_jointList[ 6 ]->m_bendMore_pitch ),  textColor_darkRed, 0.75f, Vec2( 0.0f, textAlignmentY -= textHeight ) );
		}
		// Joint 7
		if ( m_ikChain_FABRIK->m_jointList.size() >= 8 )
		{
			g_theApp->m_textFont->AddVertsForTextInBox2D( textVerts, textbox1, cellHeight,
				Stringf( "Joint7 bendMore_pitch: %0.2f", m_ikChain_FABRIK->m_jointList[ 7 ]->m_bendMore_pitch ),  textColor_darkRed, 0.75f, Vec2( 0.0f, textAlignmentY -= textHeight ) );
		}
		// Debug deadlock info
		for ( int i = 0; i < m_ikChain_FABRIK->m_deadlockDebugInfoList.size(); i++ )
		{
			std::string& curString = m_ikChain_FABRIK->m_deadlockDebugInfoList[i];
			g_theApp->m_textFont->AddVertsForTextInBox2D( textVerts, textbox1, cellHeight, curString, textColor_darkRed, 0.75f, Vec2( 0.0f, textAlignmentY -= textHeight * 2.0f ) );
 		}

 		//----------------------------------------------------------------------------------------------------------------------
 		// IK chain WS pos
 		Vec3 const& chainPos_WS = m_ikChain_FABRIK->m_position_WS;
 		g_theApp->m_textFont->AddVertsForTextInBox2D(	textVerts, textbox1, cellHeight, 
 			Stringf( "IK_Chain pos_WS: %0.2f, %0.2f, %0.2f", chainPos_WS.x, chainPos_WS.y, chainPos_WS.z ),
			textColor_darkGreen, 0.75f, Vec2( 1.0f, textAlignmentY -= textHeight ), TextDrawMode::SHRINK_TO_FIT );
		//----------------------------------------------------------------------------------------------------------------------
		// IK chain WS euler
		EulerAngles curEuler_WS = m_ikChain_FABRIK->m_eulerAngles_WS;
		g_theApp->m_textFont->AddVertsForTextInBox2D(	textVerts, textbox1, cellHeight, 
			Stringf( "IK_Chain YPR_WS: %0.2f, %0.2f, %0.2f", curEuler_WS.m_yawDegrees, curEuler_WS.m_pitchDegrees, curEuler_WS.m_rollDegrees ),
			textColor_darkGreen, 0.75f, Vec2( 1.0f, textAlignmentY -= textHeight ) );
		//----------------------------------------------------------------------------------------------------------------------
		// Joint 0 (root)
		EulerAngles curEuler_LS = m_ikChain_FABRIK->m_firstJoint->m_eulerAnglesGoal_LS;
		g_theApp->m_textFont->AddVertsForTextInBox2D(	textVerts, textbox1, cellHeight, 
			Stringf( "Joint0_YPR: %0.2f, %0.2f, %0.2f", curEuler_LS.m_yawDegrees, curEuler_LS.m_pitchDegrees, curEuler_LS.m_rollDegrees ),	
			textColor_darkGreen, 0.75f, Vec2( 1.0f, textAlignmentY -= textHeight ) );
		//----------------------------------------------------------------------------------------------------------------------
		// Joint 1
		if ( m_ikChain_FABRIK->m_jointList.size() >= 2 )
		{
			curEuler_LS = m_ikChain_FABRIK->m_jointList[1]->m_eulerAnglesGoal_LS;
			g_theApp->m_textFont->AddVertsForTextInBox2D(	textVerts, textbox1, cellHeight, 
				Stringf( "Joint1_YPR: %0.2f, %0.2f, %0.2f", curEuler_LS.m_yawDegrees, curEuler_LS.m_pitchDegrees, curEuler_LS.m_rollDegrees ),
				textColor_darkGreen, 0.75f, Vec2( 1.0f, textAlignmentY -= textHeight ) );
		}
		//----------------------------------------------------------------------------------------------------------------------
		// Joint 2
		if ( m_ikChain_FABRIK->m_jointList.size() >= 3 )
		{
			curEuler_LS = m_ikChain_FABRIK->m_jointList[ 2 ]->m_eulerAnglesGoal_LS;
			g_theApp->m_textFont->AddVertsForTextInBox2D( textVerts, textbox1, cellHeight,
				Stringf( "Joint2_YPR: %0.2f, %0.2f, %0.2f", curEuler_LS.m_yawDegrees, curEuler_LS.m_pitchDegrees, curEuler_LS.m_rollDegrees ),
				textColor_darkGreen, 0.75f, Vec2( 1.0f, textAlignmentY -= textHeight ) );
		}
		//----------------------------------------------------------------------------------------------------------------------
		// Joint 3
		if ( m_ikChain_FABRIK->m_jointList.size() >= 4 )
		{
			curEuler_LS = m_ikChain_FABRIK->m_jointList[ 3 ]->m_eulerAnglesGoal_LS;
			g_theApp->m_textFont->AddVertsForTextInBox2D( textVerts, textbox1, cellHeight,
				Stringf( "Joint3_YPR: %0.2f, %0.2f, %0.2f", curEuler_LS.m_yawDegrees, curEuler_LS.m_pitchDegrees, curEuler_LS.m_rollDegrees ),
				textColor_darkGreen, 0.75f, Vec2( 1.0f, textAlignmentY -= textHeight ) );
		}
		//----------------------------------------------------------------------------------------------------------------------
		// Joint 4
		if ( m_ikChain_FABRIK->m_jointList.size() >= 5 )
		{
			curEuler_LS = m_ikChain_FABRIK->m_jointList[ 4 ]->m_eulerAnglesGoal_LS;
			g_theApp->m_textFont->AddVertsForTextInBox2D( textVerts, textbox1, cellHeight,
				Stringf( "Joint4_YPR: %0.2f, %0.2f, %0.2f", curEuler_LS.m_yawDegrees, curEuler_LS.m_pitchDegrees, curEuler_LS.m_rollDegrees ),
				textColor_darkGreen, 0.75f, Vec2( 1.0f, textAlignmentY -= textHeight ) );
		}
		//----------------------------------------------------------------------------------------------------------------------
		// Joint 5
		if ( m_ikChain_FABRIK->m_jointList.size() >= 6 )
		{
			curEuler_LS = m_ikChain_FABRIK->m_jointList[ 5 ]->m_eulerAnglesGoal_LS;
			g_theApp->m_textFont->AddVertsForTextInBox2D( textVerts, textbox1, cellHeight,
				Stringf( "Joint5_YPR: %0.2f, %0.2f, %0.2f", curEuler_LS.m_yawDegrees, curEuler_LS.m_pitchDegrees, curEuler_LS.m_rollDegrees ),
				textColor_darkGreen, 0.75f, Vec2( 1.0f, textAlignmentY -= textHeight ) );
		}
		//----------------------------------------------------------------------------------------------------------------------
		// Joint 6
		if ( m_ikChain_FABRIK->m_jointList.size() >= 7 )
		{
			curEuler_LS = m_ikChain_FABRIK->m_jointList[ 6 ]->m_eulerAnglesGoal_LS;
			g_theApp->m_textFont->AddVertsForTextInBox2D( textVerts, textbox1, cellHeight,
				Stringf( "Joint6_YPR: %0.2f, %0.2f, %0.2f", curEuler_LS.m_yawDegrees, curEuler_LS.m_pitchDegrees, curEuler_LS.m_rollDegrees ),
				textColor_darkGreen, 0.75f, Vec2( 1.0f, textAlignmentY -= textHeight ) );
 		}
 	}	
	
	//----------------------------------------------------------------------------------------------------------------------
	// End UI Camera
	g_theRenderer->EndCamera( m_gameMode3DUICamera );

	//----------------------------------------------------------------------------------------------------------------------
	// Draw for UI camera text
	g_theRenderer->SetBlendMode( BlendMode::ALPHA );
	g_theRenderer->SetModelConstants();
	g_theRenderer->BindTexture( &g_theApp->m_textFont->GetTexture() );
	g_theRenderer->BindShader( nullptr );
	g_theRenderer->DrawVertexArray( static_cast<int>( textVerts.size() ), textVerts.data() );
	g_theRenderer->BindTexture( nullptr );
}


//----------------------------------------------------------------------------------------------------------------------
void GameMode_QuaternionTest::AddVertsForCompass( std::vector<Vertex_PCU>&compassVerts, Vec3 startPosition, float axisLength, float axisThickness ) const
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
void GameMode_QuaternionTest::UpdateTargetInput( float deltaSeconds )
{
	UNUSED( deltaSeconds );

	bool  isShiftDown = g_debugBasis_F3;
	float stepAmount  = m_defaultSpeed;
	if ( g_theInput->IsKeyDown( KEYCODE_SHIFT ) )
	{
		stepAmount *= 2.0f;
	}

	float increment = 0.1f;
	// (+X)
	if ( g_theInput->WasKeyJustPressed( '1' ) || (g_theInput->IsKeyDown( '1' ) && isShiftDown) )
	{
		m_quat.x += increment;
	}
	// (-X)
	if ( g_theInput->WasKeyJustPressed( '2' ) || (g_theInput->IsKeyDown( '2' ) && isShiftDown) )
	{
		m_quat.x -= increment;
	}
	// (+Y)
	if ( g_theInput->WasKeyJustPressed( '3' ) || (g_theInput->IsKeyDown( '3' ) && isShiftDown) )
	{
		m_quat.y += increment;
	}
	// (-Y)
	if ( g_theInput->WasKeyJustPressed( '4' ) || (g_theInput->IsKeyDown( '4' ) && isShiftDown) )
	{
		m_quat.y -= increment;
	}
	// (+Z)
	if ( g_theInput->WasKeyJustPressed( '5' ) || (g_theInput->IsKeyDown( '5' ) && isShiftDown) )
	{
		m_quat.z += increment;
	}
	// (-Z)
	if ( g_theInput->WasKeyJustPressed( '6' ) || (g_theInput->IsKeyDown( '6' ) && isShiftDown) )
	{
		m_quat.z -= increment;
	}
	// (+W)
	if ( g_theInput->WasKeyJustPressed( '7' ) || (g_theInput->IsKeyDown( '7' ) && isShiftDown) )
	{
		m_quat.w += increment;
	}
	// (-W)
	if ( g_theInput->WasKeyJustPressed( '8' ) || (g_theInput->IsKeyDown( '8' ) && isShiftDown) )
	{
		m_quat.w -= increment;
	}
	// (0 to normalize)
	if ( g_theInput->WasKeyJustPressed( '0' ) )
	{
	}
	// R to reset
	if ( g_theInput->WasKeyJustPressed( 'R' ) )
	{
		m_quat = Quaternion( 0.0f, 0.0f, 0.0f, 1.0f );
	}
	m_quat.Normalize();
}


//----------------------------------------------------------------------------------------------------------------------
void GameMode_QuaternionTest::UpdateCreature( float deltaSeconds )
{
	// Debug single step
// 	if ( g_theInput->WasKeyJustPressed( KEYCODE_RIGHTBRACKET ) )
// 	{
// 		m_ikChain_FABRIK->m_isSingleStep_Debug = !m_ikChain_FABRIK->m_isSingleStep_Debug;
// 	}
// 	if ( m_ikChain_FABRIK->m_isSingleStep_Debug )
// 	{
// 		if ( g_theInput->WasKeyJustPressed( KEYCODE_LEFTBRACKET ) )
// 		{
// 			UpdatePoleVector();
// 			m_creature->Update();
// 		}
// 	}
// 	else
// 	{
// 		UpdatePoleVector();
// 		m_creature->Update();
// 	}

	//----------------------------------------------------------------------------------------------------------------------
	// Only solve 1 frame when key is pressed
	//----------------------------------------------------------------------------------------------------------------------
	if ( g_theInput->WasKeyJustPressed( KEYCODE_RIGHTBRACKET ) )
	{
		m_ikChain_FABRIK->m_isSingleStep_Debug = !m_ikChain_FABRIK->m_isSingleStep_Debug;
		if ( !m_ikChain_FABRIK->m_isSingleStep_Debug )
		{
			m_chainInfoIndex = 0;
		}
	}
	if ( m_ikChain_FABRIK->m_isSingleStep_Debug )
	{
		if ( g_theInput->WasKeyJustPressed( KEYCODE_LEFTBRACKET ) )
		{
			UpdatePoleVector();
// 			if ( m_ikChain_FABRIK->m_deadlockSolverType == DEADLOCK_SOLVER_RESET_AND_POSE_ROOT_TO_Z )
// 			{
// 				m_ikChain_FABRIK->DeadlockSolver_ResetAndPoseRootZUp();
// 				m_ikChain_FABRIK->SaveDebugPose_curIter();
// 			}
			m_creature->Update( deltaSeconds );
		}
	}
	else
	{
		UpdatePoleVector();	
// 		if ( m_ikChain_FABRIK->m_deadlockSolverType == DEADLOCK_SOLVER_RESET_AND_POSE_ROOT_TO_Z )
// 		{
// 			m_ikChain_FABRIK->DeadlockSolver_ResetAndPoseRootZUp();
// 			m_ikChain_FABRIK->SaveDebugPose_curIter();
// 		}		
		m_creature->Update( deltaSeconds );
	}
}


//----------------------------------------------------------------------------------------------------------------------
void GameMode_QuaternionTest::RenderCreature( std::vector<Vertex_PCU>& verts, 
										 std::vector<Vertex_PCU>& verts_textured, 
										 std::vector<Vertex_PCU>& textVerts, 
										 std::vector<Vertex_PCU>& verts_BackFace,
										 std::vector<Vertex_PCU>& verts_chainBody ) const
{
	// ikChain body MS
	AABB3 bounds = AABB3( -1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f );
	AddVertsForAABB3D( verts_chainBody, bounds, Rgba8::BROWN );
	// ikChain
	if ( g_debugToggleMesh_Backspace )
	{
		m_creature->RenderMesh( g_theRenderer, nullptr, g_litShader_PCUTBN, Rgba8::WHITE );
	}
	else
	{
		m_creature->Render( verts_textured, Rgba8::WHITE, Rgba8::WHITE );
	}
	m_ikChain_FABRIK->RenderTarget_CurrentPos( verts, 0.5f );
	// Target
	Vec3 targetPos	= m_ikChain_FABRIK->m_target.m_currentPos_WS;
	float width		= 0.75f;
	bounds			= AABB3( targetPos - Vec3(width, width, width), targetPos + Vec3(width, width, width) );
	AddVertsForAABB3D( verts, bounds, Rgba8::MAGENTA );
	m_ikChain_FABRIK->RenderTarget_IJK( verts, 4.0f );

	m_ikChain_FABRIK->RenderTarget_CurrentPos( verts, 0.5f, Rgba8::CYAN );


	float textHeight = 0.75f;
	if ( g_debugRenderRaycast_F2 )
	{
		//----------------------------------------------------------------------------------------------------------------------
		// Debug rendering chain solutions per iteration
		//----------------------------------------------------------------------------------------------------------------------	
		unsigned char brightness = 180;
		if ( m_ikChain_FABRIK->m_solutionsThisFrameList.size() > 0 && m_ikChain_FABRIK->m_isSingleStep_Debug )
		{
			ChainInfo& curChainInfo  = m_ikChain_FABRIK->m_solutionsThisFrameList[ m_chainInfoIndex ];
			for ( int jointInfoIndex = 0; jointInfoIndex < curChainInfo.m_jointInfoList.size(); jointInfoIndex++ )
			{
				JointInfo& curJointInfo = curChainInfo.m_jointInfoList[jointInfoIndex];
				curJointInfo.m_jointPos_MS;
				Vec3 fwd, left, up;
				curJointInfo.m_eulerAngles_MS.GetAsVectors_XFwd_YLeft_ZUp( fwd, left, up );
				// Render jointPos, fwd, left, up
				Vec3 const& jointPos = curJointInfo.m_jointPos_WS;
				AddVertsForSphere3D( verts, jointPos, 1.0f, 16.0f, 16.0f );		// JointPos
				float   arrowLength = 10.0f;
				Vec3	fwdEnd		= jointPos + ( curJointInfo.m_fwdDir_WS  * arrowLength );  
				arrowLength			= 5.0f;
				Vec3	leftEnd		= jointPos + ( curJointInfo.m_leftDir_WS * arrowLength );
				Vec3	upEnd   	= jointPos + ( curJointInfo.m_upDir_WS	 * arrowLength );
				AddVertsForArrow3D( verts, jointPos, fwdEnd,  0.5f, Rgba8(255, brightness, brightness, 200) );		// Fwd
				AddVertsForArrow3D( verts, jointPos, leftEnd, 0.5f, Rgba8(brightness, 255, brightness, 200) );		// Left
				AddVertsForArrow3D( verts, jointPos, upEnd,   0.5f, Rgba8(brightness, brightness, 255, 200) );		// Up
			}
		}
	}
	if ( g_debugBasis_F3 )
	{
		m_ikChain_FABRIK->DebugDrawJoints_IJK( verts, 0.3f, 4.0f );
	}
	if ( g_debugText_F4 )
	{
		Vec3 camLeft	= m_gameMode3DWorldCamera.m_orientation.GetAsMatrix_XFwd_YLeft_ZUp().GetJBasis3D();
		Vec3 camUp		= m_gameMode3DWorldCamera.m_orientation.GetAsMatrix_XFwd_YLeft_ZUp().GetKBasis3D();
		m_ikChain_FABRIK->DebugTextJointPos_LocalSpace( textVerts, textHeight, 4.0f, camLeft, camUp, g_theApp->m_textFont, Rgba8::WHITE, false );
		m_ikChain_FABRIK->DebugTextJointPos_WorldSpace( textVerts, textHeight, 6.0f, camLeft, camUp, g_theApp->m_textFont, Rgba8::RED );
	}
	if ( g_debugAngles_F5 )
	{
		m_ikChain_FABRIK->DebugDrawConstraints_YPR( verts_BackFace, 3.0f );
	}

	// Debug draw noise position
	AddVertsForSphere3D( verts, m_debugNoisePos, 1.0f, 4.0f, 4.0f, Rgba8::MAGENTA );

	//----------------------------------------------------------------------------------------------------------------------
	// Debug rendering rotation axis
	//----------------------------------------------------------------------------------------------------------------------
	Vec3 end = m_ikChain_FABRIK->m_firstJoint->m_jointPos_LS + m_ikChain_FABRIK->m_debugVector_RotationAxis_MS * 2.0f;
	AddVertsForArrow3D( verts, m_ikChain_FABRIK->m_firstJoint->m_jointPos_LS, end, 0.2f, Rgba8::MAGENTA );
}


//----------------------------------------------------------------------------------------------------------------------
void GameMode_QuaternionTest::InitializeCreatures()
{
	//----------------------------------------------------------------------------------------------------------------------
	// Creature
	//----------------------------------------------------------------------------------------------------------------------
	// Create creature root and arm
	m_creature							= new CreatureBase( Vec3( 0.0f, 0.0f, 0.0f ) );
	Vec3 leftHip						= Vec3( 0.0f, 0.0f, 0.0f );
	m_ikChain_FABRIK					= m_creature->CreateChildChain( "m_ikChain", leftHip, nullptr, true );
	// Init logic for regular FABRIK
	m_ikChain_FABRIK->AddNewJointToChain( Vec3(			0.0f, 0.0f, 0.0f ), EulerAngles() );		// 1
 	m_ikChain_FABRIK->AddNewJointToChain( Vec3( m_boneLength, 0.0f, 0.0f ), EulerAngles() );		// 2
 	m_ikChain_FABRIK->AddNewJointToChain( Vec3( m_boneLength, 0.0f, 0.0f ), EulerAngles() );		// 3
 	m_ikChain_FABRIK->AddNewJointToChain( Vec3( m_boneLength, 0.0f, 0.0f ), EulerAngles() );		// 4
 	m_ikChain_FABRIK->AddNewJointToChain( Vec3( m_boneLength, 0.0f, 0.0f ), EulerAngles() );		// 5
 	m_ikChain_FABRIK->AddNewJointToChain( Vec3( m_boneLength, 0.0f, 0.0f ), EulerAngles() );		// 6
 	m_ikChain_FABRIK->AddNewJointToChain( Vec3( m_boneLength, 0.0f, 0.0f ), EulerAngles() );		// 7
 	m_ikChain_FABRIK->AddNewJointToChain( Vec3( m_boneLength, 0.0f, 0.0f ), EulerAngles() );		// 8
//  	m_ikChain_FABRIK->AddNewJointToChain( Vec3( 10.0f, 0.0f, 0.0f ), EulerAngles() );		// 8
//  	m_ikChain_FABRIK->AddNewJointToChain( Vec3( 10.0f, 0.0f, 0.0f ), EulerAngles() );		// 8
//  	m_ikChain_FABRIK->AddNewJointToChain( Vec3( 10.0f, 0.0f, 0.0f ), EulerAngles() );		// 8
//  	m_ikChain_FABRIK->AddNewJointToChain( Vec3( 10.0f, 0.0f, 0.0f ), EulerAngles() );		// 8
//  	m_ikChain_FABRIK->AddNewJointToChain( Vec3( 10.0f, 0.0f, 0.0f ), EulerAngles() );		// 8
//  	m_ikChain_FABRIK->AddNewJointToChain( Vec3( 10.0f, 0.0f, 0.0f ), EulerAngles() );		// 8
//  	m_ikChain_FABRIK->AddNewJointToChain( Vec3( 10.0f, 0.0f, 0.0f ), EulerAngles() );		// 8
//  	m_ikChain_FABRIK->AddNewJointToChain( Vec3( 10.0f, 0.0f, 0.0f ), EulerAngles() );		// 8
//  	m_ikChain_FABRIK->AddNewJointToChain( Vec3( 10.0f, 0.0f, 0.0f ), EulerAngles() );		// 8
//  	m_ikChain_FABRIK->AddNewJointToChain( Vec3( 10.0f, 0.0f, 0.0f ), EulerAngles() );		// 8
//  	m_ikChain_FABRIK->AddNewJointToChain( Vec3( 10.0f, 0.0f, 0.0f ), EulerAngles() );		// 8
//  	m_ikChain_FABRIK->AddNewJointToChain( Vec3( 10.0f, 0.0f, 0.0f ), EulerAngles() );		// 8
//  	m_ikChain_FABRIK->AddNewJointToChain( Vec3( 10.0f, 0.0f, 0.0f ), EulerAngles() );		// 8

	// Spider leg with articulate joints
//	m_ikChain_FABRIK->AddNewJointToChain( Vec3( 0.0f, 0.0f, 0.0f ), EulerAngles() );		// 0 (Root)
//	m_ikChain_FABRIK->AddNewJointToChain( Vec3( 5.0f, 0.0f, 0.0f ), EulerAngles() );		// 1  
//	m_ikChain_FABRIK->AddNewJointToChain( Vec3( 5.0f, 0.0f, 0.0f ), EulerAngles() );		// 2
//	m_ikChain_FABRIK->AddNewJointToChain( Vec3( 5.0f, 0.0f, 0.0f ), EulerAngles() );		// 3
//	m_ikChain_FABRIK->AddNewJointToChain( Vec3( 5.0f, 0.0f, 0.0f ), EulerAngles() );		// 4 (End effector)

	// Set once to remember starting position, rootPos_MS will change in forwards pass, but need to remember prevPos for the backwards pass
	m_ikChain_FABRIK->m_chainRootPosBeforeFwdFABRIK_MS = Vec3(  0.0f, 0.0f, 0.0f );		
	m_ikChain_FABRIK->UpdateJointsPositions_MS();				// Only needs to be called once on startup? 
	m_ikChain_FABRIK->m_target.m_currentPos_WS	= Vec3( 40.0f, 0.0f, 0.0f );

	// Set solverTypes
	m_ikChain_FABRIK->SetSolverType( CHAIN_SOLVER_FABRIK );
	// Set matrix to euler preference 
	m_ikChain_FABRIK->SetChainPreference_MatrixToEuler( FWD_TEMPORAL );
	// Deadlock solver type
	m_ikChain_FABRIK->m_deadlockSolverType = DEADLOCK_SOLVER_DEBT_ANGLE;
//	m_ikChain_FABRIK->m_deadlockSolverType = DEADLOCK_SOLVER_RESET_AND_POSE_ROOT_TO_Z;
	// Disable deadlock solutions
	m_ikChain_FABRIK->m_attemptDeadlockSolutions = true;
	m_ikChain_FABRIK->m_lerpEulerToGoal			 = true;
	// Set lerping turn rate
//	m_ikChain_FABRIK->m_turnRate = 22500.0f; 

 	// Actual parameters
	SetChainConstraints();

	float startRadius = 1.0f;
	float endRadius   = 0.0f;
	m_creature->InitJointMeshes_Ellipsoid( m_ikChain_FABRIK, Vec3::ZERO, Vec3( m_boneLength, 0.0f, 0.0f ), startRadius, endRadius, Vec2(1.0f, 1.0f), Vec2(1.0f, 1.0f) );
}


//----------------------------------------------------------------------------------------------------------------------
void GameMode_QuaternionTest::SetChainConstraints()
{	
// 	m_ikChain_FABRIK->m_jointList[0]->SetConstraints_YPR( FloatRange( -135.0f, 135.0f ), FloatRange( -135.0f, 135.0f ), FloatRange( -0.0f, 0.0f ) );
// 	m_ikChain_FABRIK->m_jointList[1]->SetConstraints_YPR( FloatRange(   -0.0f,   0.0f ), FloatRange(  -15.0f, 165.0f ), FloatRange( -0.0f, 0.0f ) );
// 	m_ikChain_FABRIK->m_jointList[2]->SetConstraints_YPR( FloatRange(  -15.0f, -15.0f ), FloatRange(  -45.0f,  85.0f ), FloatRange( -0.0f, 0.0f ) );
// 	return;

	//----------------------------------------------------------------------------------------------------------------------
	if ( !g_debugToggleConstraints_F8 )
	{
   		for ( int i = 0; i < m_ikChain_FABRIK->m_jointList.size(); i++ )
   		{
   			IK_Joint3D* currentSegment = m_ikChain_FABRIK->m_jointList[ i ];
// 			currentSegment->SetConstraints_YPR( FloatRange( -75.0f, 75.0f ), FloatRange( -75.0f, 75.0f ), FloatRange( -0.0f, 0.0f ) );
  			currentSegment->SetConstraints_YPR( FloatRange( -360.0f, 360.0f ), FloatRange( -360.0f, 360.0f ), FloatRange( -0.0f, 0.0f ) );
   		}
	}
	else
	{
		for ( int i = 0; i < m_ikChain_FABRIK->m_jointList.size(); i++ )
		{
			IK_Joint3D* currentSegment = m_ikChain_FABRIK->m_jointList[ i ];

			// 75 degree one constraint
			currentSegment->SetConstraints_YPR( FloatRange( -75.0f, 75.0f ), FloatRange( -75.0f, 75.0f ), FloatRange( -0.0f, 0.0f ) );
			// Alternate yaw and pitch constraints
//    			int remainder = i % 2;
//    			if ( remainder == 0 )
//    			{
//    				// Yaw only if even
//    				currentSegment->SetConstraints_YPR( FloatRange( -75.0f, 75.0f ), FloatRange( -0.0f, 0.0f ), FloatRange( -0.0f, 0.0f ) );
//    			}
//    			else
//    			{
//    				// Pitch only if odd
//    				currentSegment->SetConstraints_YPR( FloatRange( -0.0f, 0.0f ), FloatRange( -75.0f, 75.0f ), FloatRange( -0.0f, 0.0f ) );
//    			}
		}

		// Spider leg test 
// 		m_ikChain_FABRIK->m_jointList[0]->SetConstraints_YPR( FloatRange(-45.0f, 135.0f), FloatRange(-55.0f, -45.0f), FloatRange(-0.0f, -0.0f) );
// 		m_ikChain_FABRIK->m_jointList[1]->SetConstraints_YPR( FloatRange( -0.0f,   0.0f), FloatRange(  0.0f, 135.0f), FloatRange(-0.0f, -0.0f) );
// 		m_ikChain_FABRIK->m_jointList[2]->SetConstraints_YPR( FloatRange( -0.0f,   0.0f), FloatRange(  0.0f, 135.0f), FloatRange(-0.0f, -0.0f) );
// 		m_ikChain_FABRIK->m_jointList[3]->SetConstraints_YPR( FloatRange( -0.0f,   0.0f), FloatRange(  0.0f,  75.0f), FloatRange(-0.0f, -0.0f) );
// 		m_ikChain_FABRIK->m_jointList[4]->SetConstraints_YPR( FloatRange( -0.0f,   0.0f), FloatRange(  0.0f,   0.0f), FloatRange(-0.0f, -0.0f) );
	}
}


//----------------------------------------------------------------------------------------------------------------------
void GameMode_QuaternionTest::UpdateCameraAndCreatureInput( float deltaSeconds )
{
	Vec3 iBasis, jBasis, kBasis;
	m_gameMode3DWorldCamera.m_orientation.GetAsVectors_XFwd_YLeft_ZUp( iBasis, jBasis, kBasis );
	iBasis.z = 0.0f;
	jBasis.z = 0.0f;
	iBasis	 = iBasis.GetNormalized(); 
	jBasis	 = jBasis.GetNormalized(); 
	kBasis   = kBasis.GetNormalized();

	//----------------------------------------------------------------------------------------------------------------------
	// All directions are local
	//----------------------------------------------------------------------------------------------------------------------
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
		
		//----------------------------------------------------------------------------------------------------------------------
		// Model controls (movement)
		//----------------------------------------------------------------------------------------------------------------------
		// Forward
		if ( g_theInput->IsKeyDown( 'W' ) )
		{
			m_creature->m_modelRoot->m_jointPos_MS += ( Vec3::X_FWD * m_currentSpeed ) * deltaSeconds;
		}
		// Left
		if ( g_theInput->IsKeyDown( 'A' ) )
		{
			m_creature->m_modelRoot->m_jointPos_MS += ( Vec3::Y_LEFT * m_currentSpeed ) * deltaSeconds;
		}
		// Backwards
		if ( g_theInput->IsKeyDown( 'S' ) )
		{
			m_creature->m_modelRoot->m_jointPos_MS -= ( Vec3::X_FWD * m_currentSpeed ) * deltaSeconds;
		}
		// Right
		if ( g_theInput->IsKeyDown( 'D' ) )
		{
			m_creature->m_modelRoot->m_jointPos_MS -= ( Vec3::Y_LEFT * m_currentSpeed ) * deltaSeconds;
		}
		// Sky (+Z)
		if ( g_theInput->IsKeyDown( 'E' ) )
		{
			m_creature->m_modelRoot->m_jointPos_MS += ( Vec3::Z_UP * m_currentSpeed ) * deltaSeconds;
		}
		// Ground (-Z)
		if ( g_theInput->IsKeyDown( 'Q' ) )
		{
			m_creature->m_modelRoot->m_jointPos_MS -= ( Vec3::Z_UP * m_currentSpeed ) * deltaSeconds;
		}
		//----------------------------------------------------------------------------------------------------------------------
		// Model controls (euler)
		//----------------------------------------------------------------------------------------------------------------------
		// +Yaw
		if ( g_theInput->IsKeyDown( 'V' ) )
		{
			m_creature->m_modelRoot->m_eulerAnglesGoal_LS.m_yawDegrees += m_currentSpeed * deltaSeconds;
		}
		// -Yaw
		if ( g_theInput->IsKeyDown( 'B' ) )
		{
			m_creature->m_modelRoot->m_eulerAnglesGoal_LS.m_yawDegrees -= m_currentSpeed * deltaSeconds;
		}
		// +Pitch
		if ( g_theInput->IsKeyDown( 'N' ) )
		{
			m_creature->m_modelRoot->m_eulerAnglesGoal_LS.m_pitchDegrees += m_currentSpeed * deltaSeconds;
		}
		// -Pitch
		if ( g_theInput->IsKeyDown( 'M' ) )
		{
			m_creature->m_modelRoot->m_eulerAnglesGoal_LS.m_pitchDegrees -= m_currentSpeed * deltaSeconds;
		}
		// +Roll
		if ( g_theInput->IsKeyDown( KEYCODE_COMMA ) )
		{
			m_creature->m_modelRoot->m_eulerAnglesGoal_LS.m_rollDegrees += m_currentSpeed * deltaSeconds;
		}
		// -Roll
		if ( g_theInput->IsKeyDown( KEYCODE_PERIOD ) )
		{
			m_creature->m_modelRoot->m_eulerAnglesGoal_LS.m_rollDegrees -= m_currentSpeed * deltaSeconds;
		}
	}
}


//----------------------------------------------------------------------------------------------------------------------
void GameMode_QuaternionTest::UpdatePoleVector()
{
	// Update pole vector every frame
// 	Vec3 upNess										= m_leftFoot->m_firstJoint->m_jointPos_LS + m_leftFoot->m_firstJoint->m_upDir  * 10.0f;
// 	Vec3 fwdNess									= m_leftFoot->m_firstJoint->m_jointPos_LS + m_leftFoot->m_firstJoint->m_fwdDir *  5.0f;
// 	m_leftFoot->m_firstJoint->m_poleVector_LS			= fwdNess + upNess;
// 
// 	upNess											= m_leftFoot->m_firstJoint->m_child->m_jointPos_LS + m_leftFoot->m_firstJoint->m_child->m_upDir  * 10.0f;
// 	fwdNess											= m_leftFoot->m_firstJoint->m_child->m_jointPos_LS + m_leftFoot->m_firstJoint->m_child->m_fwdDir *  5.0f;
// 	m_leftFoot->m_firstJoint->m_child->m_poleVector_LS	= fwdNess + upNess;
}