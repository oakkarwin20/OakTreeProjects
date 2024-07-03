#include "AnimationController.hpp"
#include "PhysicsController.hpp"
#include "MoveStateController.hpp"
#include "GameMode_Animations.hpp"
#include "Player.hpp"

#include "Engine/Core//ErrorWarningAssert.hpp"
#include "Engine/Animations/FbxFileImporter.hpp"
#include "Engine/SkeletalSystem/IK_Chain3D.hpp"
#include "Engine/SkeletalSystem/CreatureBase.hpp"


//----------------------------------------------------------------------------------------------------------------------
AnimationController::AnimationController( Player* player, PhysicsController* physicsController, MoveStateController* moveStateController )
	: m_player( player )
	, m_physicsController( physicsController )
	, m_moveStateController( moveStateController )
{
	//----------------------------------------------------------------------------------------------------------------------
	// Animation test
//	FbxFileImporter::LoadAnimationPose( "Data/Animations/MayaCubeMesh2.fbx",			m_bindAnimationPose					);
//	FbxFileImporter::LoadAnimationPose( "Data/Animations/MayaCubeMesh3Deformed.fbx",	m_bindAnimationPose					);
//	FbxFileImporter::LoadAnimationPose( "Data/Animations/MayaCubeMesh2Deformed.fbx",	m_bindAnimationPose					);
//	FbxFileImporter::LoadAnimationPose( "Data/Animations/TPose.fbx",					m_bindAnimationPose					);
//	FbxFileImporter::LoadAnimationPose( "Data/Animations/Fast Run.fbx",					m_bindAnimationPose					);
//	FbxFileImporter::LoadAnimationPose( "Data/Animations/Run.fbx",						m_bindAnimationPose					);
	FbxFileImporter::LoadAnimationPose( "Data/Animations/Xbot/Idle.fbx",				m_animBindPose_Idle					, "Idle"				);
	FbxFileImporter::LoadAnimationPose( "Data/Animations/Xbot/Run.fbx",					m_animBindPose_Run					, "Run"					);
	FbxFileImporter::LoadAnimationPose( "Data/Animations/Xbot/Walk.fbx",				m_animBindPose_Walk					, "Walk"				);
	FbxFileImporter::LoadAnimationPose( "Data/Animations/Xbot/JumpRunning.fbx",			m_animBindPose_Jump					, "Jump"				);
//	FbxFileImporter::LoadAnimationPose( "Data/Animations/Xbot/Jump.fbx",				m_animBindPose_Jump					, "Jump"				);
	FbxFileImporter::LoadAnimationPose( "Data/Animations/Xbot/TurnLeft.fbx",			m_animBindPose_TurnLeft				, "TurnLeft	"			);
	FbxFileImporter::LoadAnimationPose( "Data/Animations/Xbot/TurnRight.fbx",			m_animBindPose_TurnRight			, "TurnRight"			);
	FbxFileImporter::LoadAnimationPose( "Data/Animations/Xbot/TwistDance.fbx",			m_animBindPose_TwistDance			, "TwistDance"			);
	FbxFileImporter::LoadAnimationPose( "Data/Animations/Xbot/FallingIdle.fbx",			m_animBindPose_FallingIdle			, "FallingIdle"			);
	FbxFileImporter::LoadAnimationPose( "Data/Animations/Xbot/FallingToLanding.fbx",	m_animBindPose_FallingToLanding		, "FallingToLanding"	);
	FbxFileImporter::LoadAnimationPose( "Data/Animations/Xbot/FallingToRoll.fbx",		m_animBindPose_FallingToRoll		, "FallingToRoll"		);
	FbxFileImporter::LoadAnimationPose( "Data/Animations/Xbot/FallingToLand.fbx",		m_animBindPose_FallingToLand		, "FallingToLand"		);
	FbxFileImporter::LoadAnimationPose( "Data/Animations/Xbot/FlyingLeftKick.fbx",		m_animBindPose_FlyingLeftKick		, "FlyingLeftKick"		);
	FbxFileImporter::LoadAnimationPose( "Data/Animations/Xbot/HighLeftKick.fbx",		m_animBindPose_HighLeftKick			, "HighLeftKick"		);
	FbxFileImporter::LoadAnimationPose( "Data/Animations/Xbot/RightCrescentKick.fbx",	m_animBindPose_RightCrescentKick	, "RightCrescentKick"	);
	FbxFileImporter::LoadAnimationPose( "Data/Animations/Xbot/LeftJabPunch.fbx",		m_animBindPose_LeftJabPunch			, "LeftJabPunch"		);
	FbxFileImporter::LoadAnimationPose( "Data/Animations/Xbot/RightJabPunch.fbx",		m_animBindPose_RightJabPunch		, "RightJabPunch"		);
	FbxFileImporter::LoadAnimationPose( "Data/Animations/Xbot/RightHook.fbx",			m_animBindPose_RightHook			, "RightHook"			);
	FbxFileImporter::LoadAnimationPose( "Data/Animations/Xbot/RightCrossPunch.fbx",		m_animBindPose_RightCrossPunch		, "RightCrossPunch"		);
	FbxFileImporter::LoadAnimationPose( "Data/Animations/Xbot/FrontFlip.fbx",			m_animBindPose_FrontFlip			, "FrontFlip"			);
	FbxFileImporter::LoadAnimationPose( "Data/Animations/Xbot/FrontFlipTwist.fbx",		m_animBindPose_FrontFlipTwist		, "FrontFlipTwist"		);
	FbxFileImporter::LoadAnimationPose( "Data/Animations/Xbot/FrontFlipRoll.fbx",		m_animBindPose_FrontFlipRoll		, "FrontFlipRoll"		);
	FbxFileImporter::LoadAnimationPose( "Data/Animations/Xbot/Vault.fbx",				m_animBindPose_Vault				, "Vault"				);
	FbxFileImporter::LoadAnimationPose( "Data/Animations/Xbot/CrouchedIdle.fbx",		m_animBindPose_CrouchedIdle			, "CrouchedIdle"		);
	FbxFileImporter::LoadAnimationPose( "Data/Animations/Xbot/CrouchedWalkForward.fbx",	m_animBindPose_CrouchedWalkForward	, "CrouchedWalkForward"	);

	// 0 keyframe
//	FbxFileImporter::LoadCurveForAllJointsAtElapsedTime( "Data/Animations/Run.fbx", m_newAnimationPose, m_bindAnimationPose, m_elapsedAnimationTime );		// Used for testing
//	FbxFileImporter::LoadAnimationClip( "Data/Animations/Run.fbx",						m_animationClip,				m_bindAnimationPose				);
//	FbxFileImporter::LoadAnimationClip( "Data/Animations/Fast Run.fbx",					m_animationClip,				m_bindAnimationPose				);
	FbxFileImporter::LoadAnimationClip( "Data/Animations/Xbot/Idle.fbx",				m_animClip_Idle,				m_animBindPose_Idle					, "Idle"				);
	FbxFileImporter::LoadAnimationClip( "Data/Animations/Xbot/Run.fbx",					m_animClip_Run,					m_animBindPose_Run					, "Run"					);
	FbxFileImporter::LoadAnimationClip( "Data/Animations/Xbot/Walk.fbx",				m_animClip_Walk,				m_animBindPose_Walk					, "Walk"				);
	FbxFileImporter::LoadAnimationClip( "Data/Animations/Xbot/JumpRunning.fbx",			m_animClip_Jump,				m_animBindPose_Jump					, "Jump"				);
//	FbxFileImporter::LoadAnimationClip( "Data/Animations/Xbot/Jump.fbx",				m_animClip_Jump,				m_animBindPose_Jump					, "Jump"				);
	FbxFileImporter::LoadAnimationClip( "Data/Animations/Xbot/TurnLeft.fbx",			m_animClip_TurnLeft ,			m_animBindPose_TurnLeft				, "TurnLeft	"			);
	FbxFileImporter::LoadAnimationClip( "Data/Animations/Xbot/TurnRight.fbx",			m_animClip_TurnRight,			m_animBindPose_TurnRight			, "TurnRight"			);
	FbxFileImporter::LoadAnimationClip( "Data/Animations/Xbot/TwistDance.fbx",			m_animClip_TwistDance,			m_animBindPose_TwistDance			, "TwistDance"			);
	FbxFileImporter::LoadAnimationClip( "Data/Animations/Xbot/FallingIdle.fbx",			m_animClip_FallingIdle,			m_animBindPose_FallingIdle			, "FallingIdle"			);
	FbxFileImporter::LoadAnimationClip( "Data/Animations/Xbot/FallingToLanding.fbx",	m_animClip_FallingToLanding,	m_animBindPose_FallingToLanding		, "FallingToLanding"	);
	FbxFileImporter::LoadAnimationClip( "Data/Animations/Xbot/FallingToRoll.fbx",		m_animClip_FallingToRoll,		m_animBindPose_FallingToRoll		, "FallingToRoll"		);
	FbxFileImporter::LoadAnimationClip( "Data/Animations/Xbot/FallingToLand.fbx",		m_animClip_FallingToLand,		m_animBindPose_FallingToLand		, "FallingToLand"		);
	FbxFileImporter::LoadAnimationClip( "Data/Animations/Xbot/FlyingLeftKick.fbx",		m_animClip_FlyingLeftKick,		m_animBindPose_FlyingLeftKick		, "FlyingLeftKick"		);
	FbxFileImporter::LoadAnimationClip( "Data/Animations/Xbot/HighLeftKick.fbx",		m_animClip_HighLeftKick,		m_animBindPose_HighLeftKick			, "HighLeftKick"		);
	FbxFileImporter::LoadAnimationClip( "Data/Animations/Xbot/RightCrescentKick.fbx",	m_animClip_RightCrescentKick,	m_animBindPose_RightCrescentKick	, "RightCrescentKick"	);
	FbxFileImporter::LoadAnimationClip( "Data/Animations/Xbot/LeftJabPunch.fbx",		m_animClip_LeftJabPunch,		m_animBindPose_LeftJabPunch			, "LeftJabPunch"		);
	FbxFileImporter::LoadAnimationClip( "Data/Animations/Xbot/RightJabPunch.fbx",		m_animClip_RightJabPunch,		m_animBindPose_RightJabPunch		, "RightJabPunch"		);
	FbxFileImporter::LoadAnimationClip( "Data/Animations/Xbot/RightHook.fbx",			m_animClip_RightHook,			m_animBindPose_RightHook			, "RightHook"			);
	FbxFileImporter::LoadAnimationClip( "Data/Animations/Xbot/RightCrossPunch.fbx",		m_animClip_RightCrossPunch,		m_animBindPose_RightCrossPunch		, "RightCrossPunch"		);
	FbxFileImporter::LoadAnimationClip( "Data/Animations/Xbot/FrontFlip.fbx",			m_animClip_FrontFlip,			m_animBindPose_FrontFlip			, "FrontFlip"			);
	FbxFileImporter::LoadAnimationClip( "Data/Animations/Xbot/FrontFlipTwist.fbx",		m_animClip_FrontFlipTwist,		m_animBindPose_FrontFlipTwist		, "FrontFlipTwist"		);
	FbxFileImporter::LoadAnimationClip( "Data/Animations/Xbot/FrontFlipRoll.fbx",		m_animClip_FrontFlipRoll,		m_animBindPose_FrontFlipRoll		, "FrontFlipRoll"		);
	FbxFileImporter::LoadAnimationClip( "Data/Animations/Xbot/Vault.fbx",				m_animClip_Vault,				m_animBindPose_Vault				, "Vault"				);
	FbxFileImporter::LoadAnimationClip( "Data/Animations/Xbot/CrouchedIdle.fbx",		m_animClip_CrouchedIdle,		m_animBindPose_CrouchedIdle			, "CrouchedIdle"		);
	FbxFileImporter::LoadAnimationClip( "Data/Animations/Xbot/CrouchedWalkForward.fbx",	m_animClip_CrouchedWalkForward,	m_animBindPose_CrouchedWalkForward	, "CrouchedWalkForward"	);
	
	// Store animations as a list instead of a separate variables
	// Store clips
	m_animClipList.push_back( m_animClip_Run					);
	m_animClipList.push_back( m_animClip_Idle					);
	m_animClipList.push_back( m_animClip_Walk					);
	m_animClipList.push_back( m_animClip_Jump					);
	m_animClipList.push_back( m_animClip_Jump					);
	m_animClipList.push_back( m_animClip_TurnLeft 				);
	m_animClipList.push_back( m_animClip_TurnRight				);
	m_animClipList.push_back( m_animClip_TwistDance				);
	m_animClipList.push_back( m_animClip_FallingIdle			);
	m_animClipList.push_back( m_animClip_FallingToLanding		);
	m_animClipList.push_back( m_animClip_FallingToRoll			);
	m_animClipList.push_back( m_animClip_FallingToLand			);
	m_animClipList.push_back( m_animClip_FlyingLeftKick			);
	m_animClipList.push_back( m_animClip_HighLeftKick			);
	m_animClipList.push_back( m_animClip_RightCrescentKick		);
	m_animClipList.push_back( m_animClip_LeftJabPunch			);
	m_animClipList.push_back( m_animClip_RightJabPunch			);
	m_animClipList.push_back( m_animClip_RightHook				);
	m_animClipList.push_back( m_animClip_RightCrossPunch		);
	m_animClipList.push_back( m_animClip_FrontFlip				);
	m_animClipList.push_back( m_animClip_FrontFlipTwist			);
	m_animClipList.push_back( m_animClip_FrontFlipRoll			);
	m_animClipList.push_back( m_animClip_Vault					);
	m_animClipList.push_back( m_animClip_CrouchedIdle			);
	m_animClipList.push_back( m_animClip_CrouchedWalkForward	);
	// Store poses
	m_animPoseList.push_back(  m_animBindPose_Run					);
	m_animPoseList.push_back(  m_animBindPose_Idle					);
	m_animPoseList.push_back(  m_animBindPose_Walk					);
	m_animPoseList.push_back(  m_animBindPose_Jump					);
	m_animPoseList.push_back(  m_animBindPose_Jump					);
	m_animPoseList.push_back(  m_animBindPose_TurnLeft				);
	m_animPoseList.push_back(  m_animBindPose_TurnRight				);
	m_animPoseList.push_back(  m_animBindPose_TwistDance			);
	m_animPoseList.push_back(  m_animBindPose_FallingIdle			);
	m_animPoseList.push_back(  m_animBindPose_FallingToLanding		);
	m_animPoseList.push_back(  m_animBindPose_FallingToRoll			);
	m_animPoseList.push_back(  m_animBindPose_FallingToLand			);
	m_animPoseList.push_back(  m_animBindPose_FlyingLeftKick		);
	m_animPoseList.push_back(  m_animBindPose_HighLeftKick			);
	m_animPoseList.push_back(  m_animBindPose_RightCrescentKick		);
	m_animPoseList.push_back(  m_animBindPose_LeftJabPunch			);
	m_animPoseList.push_back(  m_animBindPose_RightJabPunch			);
	m_animPoseList.push_back(  m_animBindPose_RightHook				);
	m_animPoseList.push_back(  m_animBindPose_RightCrossPunch		);
	m_animPoseList.push_back(  m_animBindPose_FrontFlip				);
	m_animPoseList.push_back(  m_animBindPose_FrontFlipTwist		);
	m_animPoseList.push_back(  m_animBindPose_FrontFlipRoll			);
	m_animPoseList.push_back(  m_animBindPose_Vault					);
	m_animPoseList.push_back(  m_animBindPose_CrouchedIdle			);
	m_animPoseList.push_back(  m_animBindPose_CrouchedWalkForward	);
}


//----------------------------------------------------------------------------------------------------------------------
AnimationController::~AnimationController()
{
}


//----------------------------------------------------------------------------------------------------------------------
void AnimationController::Update( float deltaSeconds )
{
	//----------------------------------------------------------------------------------------------------------------------
	// Anim blending/sampling test
	//----------------------------------------------------------------------------------------------------------------------
	m_animVerts.clear();
	m_totalTimeMS += ( deltaSeconds * m_timeScalarMS );
	AnimationPose animPoseToRender;

	// Blend into walk
	m_moveStateController->m_blendPose						= GenerateLocomotionPose();
	m_moveStateController->m_animTransition.m_blendIntoPose = &m_moveStateController->m_blendPose;

	if ( m_moveStateController->m_isCrossfade )
	{
		DebuggerPrintf("crossfading\n");		// Blend between prevMoveState and curMoveState for some time (crossFadeDuration)
		AnimationTransition& animTransition		= m_moveStateController->m_animTransition;
		AnimationClip&		 curTransitionClip	= *animTransition.m_animClipCur;
		float				 blendFactor		= 0.0f;
		float				 crossFadeTimeCur	= animTransition.m_curCrossFadeTime_MS;
		AnimationPose		 blendIntoPose;
		if ( crossFadeTimeCur >= animTransition.m_maxCrossFadeTime_MS )
		{
			m_moveStateController->m_isCrossfade = false;
		}
		else
		{
			// Increment curAnimClip local time
			animTransition.m_curCrossFadeTime_MS += deltaSeconds * m_timeScalarMS;
		}

		if ( m_moveStateController->m_crossfade_PoseToPose )
		{
			// Use the blendIntoPose already provided by the transition struct
			blendIntoPose = *animTransition.m_blendIntoPose;
			blendFactor   = RangeMapClamped( crossFadeTimeCur, 0.0f, animTransition.m_maxCrossFadeTime_MS, 0.0f, 1.0f );
			m_totalTimeMS = crossFadeTimeCur;
		}
		else   // Blend pose to clip
		{
			// Sample blendIntoPose in this during crossfade from the transitionClip
			// Use blendIntoPose for cross-fading out of the prevPose
			blendIntoPose	= curTransitionClip.SampleAnimPoseAtTime( curTransitionClip.m_localTime, *animTransition.m_animBindPose );
			blendFactor		= RangeMapClamped( curTransitionClip.m_localTime, 0.0f, animTransition.m_maxCrossFadeTime_MS, 0.0f, 1.0f );
		}

		// Cross fade (blend between old and new poses)
		curTransitionClip.m_localTime	= crossFadeTimeCur;
		animPoseToRender				= AnimationPose::GetBlendFromTwoPoses( *animTransition.m_animPosePrev, blendIntoPose, blendFactor );
		animPoseToRender.m_poseName		= "Crossfade";
		
		if ( animTransition.m_useRootMotion )
		{
			MovePlayerRootMotionX( animPoseToRender );
		}
	}
	else	// Blend regularly
	{
		animPoseToRender = GenerateLocomotionPose();

		// Old approach to blending jump with hard coded crossfading
 		if ( m_moveStateController->m_curMoveState == MoveState::MOVESTATE_JUMP )
 		{
 			// New attempt with crossfading in/out of jumps		
 			// basePose is the current walk pose 
 			// Blend between basePose and jumpPose over a duration
 			// Sample basePose at total time
 			// Sample jumpPose at jumpTime
 			// Blend between both poses
 
 			// Sample poseToRender by incrementing jump time [0, jumpAnimEndTime]
 			m_animClip_Jump.m_localTime	+= ( deltaSeconds * m_timeScalarMS );
 			float animTimeJumpMS		 = m_animClip_Jump.m_localTime;
			animPoseToRender			 = m_animClip_Jump.SampleAnimPoseAtTime( animTimeJumpMS, m_animBindPose_Jump, false, false ); 
			animPoseToRender.m_poseName  = "Jump";
			m_animClipCurPlayed			 = &m_animClip_Jump;

 
 			// Reset anim state, later the jump animation should trigger an event when jump is finished,
 			// the state machine should listen and modify the state accordingly back to idle
//			if ( m_animClip_Jump.IsFinished() )
			if ( m_animClip_Jump.m_localTime > (m_animClip_Jump.m_endTime * 0.7f) )
 			{
				// Update physics controller
 				m_moveStateController->SetState( MoveState::MOVESTATE_FALL );	
 			}
 		}
		else if ( m_moveStateController->m_curMoveState == MoveState::MOVESTATE_FALL )
		{
			float landBufferHeight = 1.0f;
			animPoseToRender = SamplePose_Fall( deltaSeconds );
			// Transition into a roll animation if the player has been falling for longer than 'airTimeTillRoll'
			float airTimeTillRoll = 700.0f;
			if ( m_animClip_FallingIdle.m_localTime >= airTimeTillRoll )
			{
				m_moveStateController->SetState( MoveState::MOVESTATE_FALLING_TO_ROLL );	
			}
			else if ( m_player->m_raycastPlayerToFloor.m_impactDist <= m_physicsController->m_playerPhysicsRadius + landBufferHeight )
			{
//				m_moveStateController->SetState( MoveState::MOVESTATE_FALLING_TO_LAND );	
				m_moveStateController->SetState( MoveState::MOVESTATE_LOCOMOTION );	
			}
		}
		else if ( m_moveStateController->m_curMoveState == MoveState::MOVESTATE_FALLING_TO_LAND )
		{
			// Sample poseToRender by incrementing jump time [0, jumpAnimEndTime]
			m_animClip_FallingToLand.m_localTime	+= ( deltaSeconds * m_timeScalarMS );
			float animTimeFallToLandMS				= m_animClip_FallingToLand.m_localTime;
			animPoseToRender						= m_animClip_FallingToLand.SampleAnimPoseAtTime( animTimeFallToLandMS, m_animBindPose_FallingToLand, false, false ); 
			animPoseToRender.m_poseName				= "FallToLand";
			m_animClipCurPlayed						= &m_animClip_FallingToLand;

			if ( m_animClip_FallingToLand.m_localTime > (m_animClip_FallingToLand.m_endTime * 0.95f) )
			{
				// Update physics controller
				m_moveStateController->SetState( MoveState::MOVESTATE_LOCOMOTION );	
			}
		}
		else if ( m_moveStateController->m_curMoveState == MoveState::MOVESTATE_FALLING_TO_ROLL )
		{
			animPoseToRender = SamplePose_FallToRoll( deltaSeconds );

			// Root motion attempt
			AnimationJoint& rootJoint						 = animPoseToRender.m_jointList[0];
 			Vec3  totalAnimDisplacement						 = rootJoint.m_localToParentTransform.m_position;
 			Vec3  localOffsetThisFrame						 = totalAnimDisplacement - m_rootJointDispLastFrame_LS;
			float travelDistX								 = fabsf( localOffsetThisFrame.x );
			Vec3  fwdDir									 = m_player->m_physicsController->m_orientation.GetForwardDir_XFwd_YLeft_ZUp();
			Vec3  fwdDisp									 = ( fwdDir * travelDistX );
 			m_player->m_physicsController->m_position		+= fwdDisp;
			rootJoint.m_localToParentTransform.m_position.x	 = 0.0f;
 			m_rootJointDispLastFrame_LS						 = totalAnimDisplacement;
//			if ( m_animClip_FallingToRoll.IsFinished() )
			if ( m_animClip_FallingToRoll.m_localTime >= (m_animClip_FallingToRoll.m_endTime * 0.7f) )
			{
				m_moveStateController->SetState( MoveState::MOVESTATE_LOCOMOTION );
				m_rootJointDispLastFrame_LS = Vec3::ZERO;
			}
		}
		else if ( m_moveStateController->m_curMoveState == MoveState::MOVESTATE_FRONT_FLIP_DEFAULT )
		{
			AnimationPose sampledPose = SamplePoseFromPlayingClip( "Front Flip Default", m_animClip_FrontFlip, m_animBindPose_FrontFlip, m_timeScalarMS, deltaSeconds, false, false );
			animPoseToRender = sampledPose;
			// Root motion
			MovePlayerRootMotionX( animPoseToRender );	
			if ( m_animClip_FrontFlip.IsFinished() )
			{
				m_animClip_FrontFlip.ResetLocalTime();
				m_moveStateController->m_parkourFlipFinished = true;
				m_rootJointDispLastFrame_LS				 = Vec3::ZERO;
				if ( m_moveStateController->m_parkourFlipRequested )
				{
					m_moveStateController->UpdateParkourFlipCounterAndState();
					m_moveStateController->SetState( MoveState::MOVESTATE_FRONT_FLIP_TWIST );
				}
				else
				{
					m_moveStateController->SetState( MoveState::MOVESTATE_LOCOMOTION );
					m_moveStateController->ResetParkourFlipData();
				}
			}
		}
		else if ( m_moveStateController->m_curMoveState == MoveState::MOVESTATE_FRONT_FLIP_TWIST )
		{
			AnimationPose sampledPose = SamplePoseFromPlayingClip( "Front Flip Twist", m_animClip_FrontFlipTwist, m_animBindPose_FrontFlipTwist, m_timeScalarMS, deltaSeconds, false, false );
			animPoseToRender = sampledPose;
			// Root motion
			MovePlayerRootMotionX( animPoseToRender );	
			if ( m_animClip_FrontFlipTwist.IsFinished() )
			{
				m_animClip_FrontFlipTwist.ResetLocalTime();
				m_moveStateController->m_parkourFlipFinished = true;
				m_rootJointDispLastFrame_LS				 = Vec3::ZERO;
				if ( m_moveStateController->m_parkourFlipRequested )
				{
					m_moveStateController->UpdateParkourFlipCounterAndState();
					m_moveStateController->SetState( MoveState::MOVESTATE_FRONT_FLIP_ROLL );
				}
				else
				{
					m_moveStateController->SetState( MoveState::MOVESTATE_LOCOMOTION );
					m_moveStateController->ResetParkourFlipData();
				}
			}
		}
		else if ( m_moveStateController->m_curMoveState == MoveState::MOVESTATE_FRONT_FLIP_ROLL )
		{
			AnimationPose sampledPose = SamplePoseFromPlayingClip( "Front Flip Roll", m_animClip_FrontFlipRoll, m_animBindPose_FrontFlipRoll, m_timeScalarMS, deltaSeconds, false, false );
			animPoseToRender = sampledPose;
			// Root motion
			MovePlayerRootMotionX( animPoseToRender );	
			if ( m_animClip_FrontFlipRoll.IsFinished() )
			{
				m_animClip_FrontFlipRoll.ResetLocalTime();
				m_rootJointDispLastFrame_LS = Vec3::ZERO;
				if ( m_moveStateController->m_parkourFlipRequested )
				{
					m_moveStateController->UpdateParkourFlipCounterAndState();
					m_moveStateController->SetState( MoveState::MOVESTATE_FRONT_FLIP_ROLL );
				}
				else
				{
					m_moveStateController->SetState( MoveState::MOVESTATE_LOCOMOTION );
					m_moveStateController->ResetParkourFlipData();
				}
			}
		}
		else if ( m_moveStateController->m_curMoveState == MoveState::MOVESTATE_VAULT )
		{
			// Update local clock
			AnimationPose sampledPose = SamplePoseFromPlayingClip( "Vault", m_animClip_Vault, m_animBindPose_Vault, m_timeScalarMS, deltaSeconds, false, false );
			animPoseToRender = sampledPose;

			// Root motion
//			MovePlayerRootMotionX( animPoseToRender );
			MovePlayerRootMotionXZ( animPoseToRender );

			if ( m_animClip_Vault.IsFinished() )
			{
				m_animClip_Vault.ResetLocalTime();
				m_rootJointDispLastFrame_LS			= Vec3::ZERO;
				m_moveStateController->SetState( MoveState::MOVESTATE_LOCOMOTION );
				m_physicsController->m_canTriggerVault  = true; 
				m_physicsController->m_isVaulting		= false;
				m_physicsController->m_isPhysicsEnabled = true;
			}
		}
	}
	m_debugPoseName		= animPoseToRender.m_poseName;
	m_animPoseLastFrame	= animPoseToRender;

	//----------------------------------------------------------------------------------------------------------------------
	// IK attempt 1
	// 1. Set up ikChain with correct joints
	// 2. Update ikChain to reach target
	// 3. Update joint data using ikSolver
	//----------------------------------------------------------------------------------------------------------------------
//  	CreatureBase playerFeetCreature			= CreatureBase();
//  	IK_Chain3D* leftFoot_ikChain			= playerFeetCreature.CreateChildChain( "leftFoot" );
// //	Transform const& legTransform_WS		= animPoseToRender.LocalToModel_Transform( 1 );
// 	Transform const& legTransform_WS		= animPoseToRender.LocalToModel_Transform( 62 );
// 	leftFoot_ikChain->m_eulerAngles_WS		= EulerAngles::GetAsEuler_XFwd_YLeft_ZUp( legTransform_WS.m_rotation );
// 	leftFoot_ikChain->m_position_WS			= legTransform_WS.m_position;
// 	std::vector<float> rollValues;
//  	// Loop specifically over the indexes with left feet joints
// // 	for ( int jointIndex = 1; jointIndex <= 5; jointIndex++ )
// // 	for ( int jointIndex = 1; jointIndex <= 3; jointIndex++ )
// // 	for ( int jointIndex = 1; jointIndex <= 2; jointIndex++ )
//  	for ( int jointIndex = 62; jointIndex <= 63; jointIndex++ )
//  	{
//  		AnimationJoint& curJointTransform	= animPoseToRender.m_jointList[jointIndex];
//  		Vec3&			curJointPos_LS		= curJointTransform.m_localToParentTransform.m_position;
//  		Quaternion&		curJointQuat		= curJointTransform.m_localToParentTransform.m_rotation;
//  		EulerAngles		curJointEulerAngles = EulerAngles::GetAsEuler_XFwd_YLeft_ZUp( curJointQuat );
// 		// Test to swizzle the basis cuz my IK was xFwdBone assumed but the animators made it zFwdBones
// //  	Vec3 xEast				 = Vec3::X_FWD;
// //  	Vec3 yNorth				 = Vec3::Y_LEFT;
// //  	Vec3 zSky				 = Vec3::Z_UP;
// //  	xEast					 = curJointQuat * xEast;
// //  	yNorth					 = curJointQuat * yNorth;
// //  	zSky					 = curJointQuat * zSky;
// // 		EulerAngles curJointEulerAngles = EulerAngles::GetAsEuler_XFwd_YLeft_ZUp( zSky, yNorth );
// // 		EulerAngles curJointEulerAngles = EulerAngles::GetAsEuler_XFwd_YLeft_ZUp( xEast, yNorth );
// //		curJointEulerAngles.m_pitchDegrees += 90.0f;
// 
//  		leftFoot_ikChain->AddNewJointToChain( curJointPos_LS, curJointEulerAngles );
// 		rollValues.push_back( curJointEulerAngles.m_rollDegrees );
//  	}
//  	leftFoot_ikChain->m_solverType				= CHAIN_SOLVER_FABRIK;
//  	leftFoot_ikChain->UpdateJointsPositions_MS();				// Only needs to be called once on startup? 
//  	leftFoot_ikChain->SetChainPreference_MatrixToEuler( FWD_TEMPORAL );
//  	leftFoot_ikChain->m_turnRate				= 22500.0f;
//  	leftFoot_ikChain->m_target.m_currentPos_WS	= m_player->m_gameModeAnimations->m_ikTargetPos;
//   	playerFeetCreature.Update( deltaSeconds );
//   	// Loop through all joints in IK chain and convert rotation from EulerAngles to Quats
// // 	int animJointIndex = 1;
//    	int animJointIndex = 62;
//    	for ( int i = 0; i < leftFoot_ikChain->m_jointList.size(); i++ )
//    	{
//    		// Update joint transform data
//    		IK_Joint3D* curJoint		= leftFoot_ikChain->m_jointList[i];
// //		curJoint->m_eulerAnglesGoal_LS.m_rollDegrees = rollValues[i];
//    		Mat44 curJointMat_LS		= curJoint->m_eulerAnglesGoal_LS.GetAsMatrix_XFwd_YLeft_ZUp();
// 		if ( i == 0 )
// 		{
// 	 		curJointMat_LS.AppendYRotation( 90.0f );
// //			curJointMat_LS.AppendXRotation( -176.0f );
// 		}
// // 		curJointMat_LS.AppendYRotation( 90.0f );
//    		Quaternion curJointQuat_LS	= Quaternion::GetAsQuaternion( curJointMat_LS );
//    		// Get reference to animJoint
//    		AnimationJoint& curJointTransform = animPoseToRender.m_jointList[ animJointIndex ];
//    		curJointTransform.m_localToParentTransform.m_rotation = curJointQuat_LS;
//    		animJointIndex++;
//    	}


	animPoseToRender.AddVertsForDebugJointsAndBones( m_animVerts );		// New approach, added util render function to pose class
//	AddVertsForDebugJointsAndBones( animPoseToRender );					// Old approach to rendering

	// How to debug draw bones/joints
	// How to convert between spaces using TRS

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
	// Old approach before controllers were implemented
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
// 	Vec2 playerVelocity_V2		= Vec2( m_physicsController->m_velocity.x, m_physicsController->m_velocity.y );
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
// 	if ( m_player->m_moveStateController == MOVE_STATE_JUMP )
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
// 	}
// 	m_debugPoseName			= animPoseToRender.m_poseName;
// 	m_animPoseLastFrame		= animPoseToRender;
// 
// 
// 	if ( m_player->m_moveState == MOVE_STATE_TURN )
// 	{
// 		// Blend basePose (idle/walk/run) with turnAnim
// 		m_animTimeTurnLeftMS += ( deltaSeconds * 1000.0f );
// 		animPoseToRender	  = m_animClip_TurnLeft.SampleAnimPoseAtTime( m_animTimeTurnLeftMS, m_animBindPose_TurnLeft, false, false ); 
// 		if ( m_animClip_TurnLeft.IsFinished() )
// 		{
// 			m_player->m_moveState				= MOVE_STATE_LOCOMOTION;
// 			m_player->m_canTurn						= true;
// 			m_player->m_orientation.m_yawDegrees   += 90.0f;
// 
// 			// 			float blendFactor = RangeMapClamped( m_animTimeTurnLeftMS, maxCrossfadeTimeMS, jumpAnimDuration, 0.0f, 1.0f );
// 			// 			animPoseToRender  = AnimationPose::GetBlendFromTwoPoses( animPoseToRender, animPoseIdle, blendFactor );
// 
// 			// walking
// 			// turning
// 			// cross fading, turning -> walking
// 		}
// 	}
// 
// 	//----------------------------------------------------------------------------------------------------------------------
// 	// AddVerts to debug draw joints & bones
// 	//----------------------------------------------------------------------------------------------------------------------
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
}


//----------------------------------------------------------------------------------------------------------------------
void AnimationController::Render() const
{
	// Draw call for animation Joints/Bones
	g_theRenderer->SetBlendMode( BlendMode::ALPHA );
	Mat44 playerMat		 = m_physicsController->m_orientation.GetAsMatrix_XFwd_YLeft_ZUp();
	Vec3 animationPos	 = m_player->m_position;
	playerMat.SetTranslation3D( animationPos );
	g_theRenderer->SetModelConstants( playerMat );
	g_theRenderer->BindTexture( nullptr );
	g_theRenderer->BindShader( nullptr );
	g_theRenderer->DrawVertexArray( static_cast<int>( m_animVerts.size() ), m_animVerts.data() );
}


//----------------------------------------------------------------------------------------------------------------------
void AnimationController::DebugDrawJointBasis( std::vector<Vertex_PCU>& animVerts, Transform const& curJointTransform, float length, float thickness )
{
	Vec3	   const& curJointPos_WS	= curJointTransform.m_position;
	Quaternion const& curJointQuat_WS   = curJointTransform.m_rotation;
	Mat44			  curJointMatrix_WS = Quaternion::GetAsMatrix( curJointPos_WS, curJointQuat_WS );
	Vec3		quadFwd					= curJointMatrix_WS.GetIBasis3D();
	Vec3		quadLeft				= curJointMatrix_WS.GetJBasis3D();
	Vec3		quadUp					= curJointMatrix_WS.GetKBasis3D();
	float		scalar					= length;
	Vec3 const& start					= curJointPos_WS;
	Vec3		 fwdEnd					= start + (quadFwd	 * scalar);
	Vec3		leftEnd					= start + (quadLeft * scalar );
	Vec3		  upEnd					= start + (quadUp   * scalar );
	AddVertsForArrow3D( animVerts, start,  fwdEnd, thickness, Rgba8::RED	); 
	AddVertsForArrow3D( animVerts, start, leftEnd, thickness, Rgba8::GREEN  ); 
	AddVertsForArrow3D( animVerts, start,   upEnd, thickness, Rgba8::BLUE   ); 	
}


//----------------------------------------------------------------------------------------------------------------------
void AnimationController::AddVertsForDebugJointsAndBones( AnimationPose const& animPoseToRender )
{
	//----------------------------------------------------------------------------------------------------------------------
	// AddVerts to debug draw joints & bones
	//----------------------------------------------------------------------------------------------------------------------
	for ( int jointIndex = 0; jointIndex < animPoseToRender.m_jointList.size(); jointIndex++ )
	{
		AnimationJoint const& joint			  = animPoseToRender.m_jointList[jointIndex];
		Transform const& curJointTransform_WS = animPoseToRender.LocalToModel_Transform(jointIndex);
		if ( joint.m_parentJointIndex >= 0 )
		{
			Transform const& parentJointTransform_WS = animPoseToRender.LocalToModel_Transform( joint.m_parentJointIndex );
			AddVertsForCone3D	( m_animVerts, parentJointTransform_WS.m_position, curJointTransform_WS.m_position, 0.5f );
			AddVertsForSphere3D ( m_animVerts,    curJointTransform_WS.m_position, 0.2f, 4.0f, 4.0f, Rgba8::DARK_CYAN );
//			DebugDrawJointBasis ( m_animVerts, curJointTransform_WS, 2.0f, 0.1f );
		}
	}
}


//----------------------------------------------------------------------------------------------------------------------
void AnimationController::MovePlayerRootMotionX( AnimationPose& animPoseToRender )
{
	AnimationJoint& rootJoint						 = animPoseToRender.m_jointList[0];
	Vec3  totalAnimDisplacement						 = rootJoint.m_localToParentTransform.m_position;
	Vec3  localOffsetThisFrame						 = totalAnimDisplacement - m_rootJointDispLastFrame_LS;
	float travelDistX								 = fabsf( localOffsetThisFrame.x );
	Vec3  fwdDir									 = m_player->m_physicsController->m_orientation.GetForwardDir_XFwd_YLeft_ZUp();
	Vec3  fwdDisp									 = ( fwdDir * travelDistX );
	m_player->m_physicsController->m_position		+= fwdDisp;
	rootJoint.m_localToParentTransform.m_position.x	 = 0.0f;
	m_rootJointDispLastFrame_LS					 = totalAnimDisplacement;
}


//----------------------------------------------------------------------------------------------------------------------
void AnimationController::MovePlayerRootMotionXZ( AnimationPose& animPoseToRender )
{
	AnimationJoint& rootJoint						 = animPoseToRender.m_jointList[0];
	Vec3  rootJointDispThisFrame_LS					 = rootJoint.m_localToParentTransform.m_position;
	Vec3  localOffsetThisFrame						 = rootJointDispThisFrame_LS - m_rootJointDispLastFrame_LS;
	float travelDistX								 = fabsf( localOffsetThisFrame.x );
//	float travelDistZ								 = fabsf( localOffsetThisFrame.z );
	float travelDistZ								 = localOffsetThisFrame.z;
	Vec3  fwdDir;
	Vec3  leftDir;
	Vec3  upDir;
	m_player->m_physicsController->m_orientation.GetAsVectors_XFwd_YLeft_ZUp( fwdDir, leftDir, upDir );
	Vec3  fwdDisp									 = ( fwdDir * travelDistX );
	Vec3  upDisp									 = ( upDir  * travelDistZ );
	m_player->m_physicsController->m_position		+= fwdDisp + upDisp;
	rootJoint.m_localToParentTransform.m_position.x	 = 0.0f;
	m_rootJointDispLastFrame_LS						 = rootJointDispThisFrame_LS;
}


//----------------------------------------------------------------------------------------------------------------------
AnimationPose AnimationController::GenerateLocomotionPose()
{
	// New locomotion logic (Idle/Walk/Run) + (crouch/crouchedWalk)
	DebuggerPrintf("Regular\n");

	AnimationPose locomotionPose;

	// Adjustable velocity parameters for handling transitions between idle/walk/run
	float idleToWalkMin =  4.0f;
	float idleToWalkMax = 25.0f;
	float walkToRunMax  = 50.0f;
	// Use velocityXY instead of velocityXYZ to ignore gravity
	Vec2 playerVelocity_V2		= Vec2( m_physicsController->m_velocity.x, m_physicsController->m_velocity.y );
	float velocityMagnitudeXY	= playerVelocity_V2.GetLength();

	if ( m_moveStateController->m_curMoveState == MoveState::MOVESTATE_LOCOMOTION )
	{
		DebuggerPrintf("Idle/Walk/Run\n");
		// New approach (multiple animations sampled based on states)
		AnimationPose animPoseIdle	= m_animClip_Idle.SampleAnimPoseAtTime( m_totalTimeMS, m_animBindPose_Idle, true );
		AnimationPose animPoseWalk	= m_animClip_Walk.SampleAnimPoseAtTime( m_totalTimeMS, m_animBindPose_Walk, true );
		AnimationPose animPoseRun	= m_animClip_Run. SampleAnimPoseAtTime( m_totalTimeMS, m_animBindPose_Run,  true );

		// Get locomotion pose (idle/walk/run)
		if ( velocityMagnitudeXY > idleToWalkMin && velocityMagnitudeXY < idleToWalkMax )
		{
			// Idle to walk
			float blendFactor			= RangeMapClamped( velocityMagnitudeXY, idleToWalkMin, idleToWalkMax, 0.0f, 1.0f );
			locomotionPose				= AnimationPose::GetBlendFromTwoPoses( animPoseIdle, animPoseWalk, blendFactor );
			locomotionPose.m_poseName	= "idleToWalk";

			// Update debug info
			m_animClipCurPlayed = &m_animClip_Walk;
		}
		else if ( velocityMagnitudeXY >= idleToWalkMax )
		{
			// Walk to run
			float blendFactor			= RangeMapClamped( velocityMagnitudeXY, idleToWalkMax, walkToRunMax, 0.0f, 1.0f );
			locomotionPose				= AnimationPose::GetBlendFromTwoPoses( animPoseWalk, animPoseRun, blendFactor );
			locomotionPose.m_poseName	= "walkToRun";

			// Update debug info
			m_animClipCurPlayed = &m_animClip_Run;
		}
		else
		{
			// Idle
			locomotionPose				= animPoseIdle;
			locomotionPose.m_poseName	= "Idle";

			// Update debug info
			m_animClipCurPlayed = &m_animClip_Idle;
		}
	}
	else if ( m_moveStateController->m_curMoveState == MoveState::MOVESTATE_CROUCHED )
	{
		DebuggerPrintf( "crouch/crouchedWalk\n" );
		AnimationPose animPoseIdle = m_animClip_CrouchedIdle	   .SampleAnimPoseAtTime( m_totalTimeMS, m_animBindPose_CrouchedIdle, true );
		AnimationPose animPoseWalk = m_animClip_CrouchedWalkForward.SampleAnimPoseAtTime( m_totalTimeMS, m_animBindPose_CrouchedWalkForward, true );

		// Determine blend factor based on velocity
		// Sample crouched pose using blend factor
		float blendFactor			= RangeMapClamped( velocityMagnitudeXY, idleToWalkMin, idleToWalkMax, 0.0f, 1.0f );
		locomotionPose				= AnimationPose::GetBlendFromTwoPoses( animPoseIdle, animPoseWalk, blendFactor );
		locomotionPose.m_poseName	= "crouch";
		DebuggerPrintf( "BlendFactor: %0.2f", blendFactor );

		// Update debug info
		m_animClipCurPlayed = &m_animClip_CrouchedIdle;
	}
	return locomotionPose;

	// PRESERVED OLD IDLE/WALK/RUN LOCOMOTION LOGIC
// 	DebuggerPrintf("Regular\n");
// 	// New approach (multiple animations sampled based on states)
// 	AnimationPose animPoseIdle	= m_animClip_Idle.SampleAnimPoseAtTime( m_totalTimeMS, m_animBindPose_Idle, true );
// 	AnimationPose animPoseWalk	= m_animClip_Walk.SampleAnimPoseAtTime( m_totalTimeMS, m_animBindPose_Walk, true );
// 	AnimationPose animPoseRun	= m_animClip_Run. SampleAnimPoseAtTime( m_totalTimeMS, m_animBindPose_Run,  true );
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
// 	Vec2 playerVelocity_V2		= Vec2( m_physicsController->m_velocity.x, m_physicsController->m_velocity.y );
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
// //		locomotionPose= m_animClip_TwistDance.SampleAnimPoseAtTime( m_totalTimeMS, m_animBindPose_TwistDance, true );	// Dance hack
// 	}
// 	return locomotionPose;
}


//----------------------------------------------------------------------------------------------------------------------
AnimationPose AnimationController::SamplePose_Fall( float deltaSeconds )
{
	DebuggerPrintf("Falling\n"); 
	float timeScalar					= 1000.0f;
	m_animClip_FallingIdle.m_localTime += ( deltaSeconds * timeScalar );
	float animTimeFallMS				= m_animClip_FallingIdle.m_localTime;
	AnimationPose animPoseFall			= m_animClip_FallingIdle.SampleAnimPoseAtTime( animTimeFallMS, m_animBindPose_FallingIdle, true );

	// Blend from prevPose into fallPose
	//----------------------------------------------------------------------------------------------------------------------
 	AnimationPose fallPose;
// 	float playerZ		= m_player->m_position.z; 
// 	float prevToFallMin = 100.0f; 
// 	float prevToFallMax = 1.0f;
// 	float blendFactor	= RangeMapClamped( playerZ, prevToFallMin, prevToFallMax, 0.0f, 1.0f );
// 	fallPose			= AnimationPose::GetBlendFromTwoPoses( animPoseFall, prevPose, blendFactor );
// 	fallPose.m_poseName	= "Falling";

	// Hack test (no blending)
	fallPose			= animPoseFall;
	fallPose.m_poseName	= "Falling";
	return fallPose;
}


//----------------------------------------------------------------------------------------------------------------------
AnimationPose AnimationController::SamplePose_FallToRoll( float deltaSeconds )
{
	DebuggerPrintf("Rolling\n"); 
	float timeScalar					 = 1000.0f;
	m_animClip_FallingToRoll.m_localTime += ( deltaSeconds * timeScalar );
	float animTimeFallMS				 = m_animClip_FallingToRoll.m_localTime;
	AnimationPose rollPose				 = m_animClip_FallingToRoll.SampleAnimPoseAtTime( animTimeFallMS, m_animBindPose_FallingToRoll, false, false );
	rollPose.m_poseName					 = "Rolling";

	std::vector<Vec3Keyframe>& posKeyFrames = m_animClip_FallingToRoll.m_jointCurveList[0].m_positionKeyframeList;
	for ( int i = 0; i < posKeyFrames.size(); i++ )
	{
		Vec3Keyframe& curPos = posKeyFrames[i];
		Vec3 rootJointPos	 = curPos.m_data;
		DebuggerPrintf( "JointPos X:%0.2f, Y:%0.2f, Z:%0.2f\n", rootJointPos.x, rootJointPos.y, rootJointPos.z );
	}
	return rollPose;
}


//----------------------------------------------------------------------------------------------------------------------
AnimationPose AnimationController::SamplePose_FrontFlipDefault( float deltaSeconds )
{
	DebuggerPrintf("Front Flip Default\n"); 
	float timeScalar					 = 1000.0f;
	m_animClip_FrontFlip.m_localTime	+= ( deltaSeconds * timeScalar );
	float animTimeMS					 = m_animClip_FrontFlip.m_localTime;
	AnimationPose animPose				 = m_animClip_FrontFlip.SampleAnimPoseAtTime( animTimeMS, m_animBindPose_FrontFlip, false, false );
	animPose.m_poseName					 = "Front Flip Default";

	std::vector<Vec3Keyframe>& posKeyFrames = m_animClip_FrontFlip.m_jointCurveList[0].m_positionKeyframeList;
	for ( int i = 0; i < posKeyFrames.size(); i++ )
	{
		Vec3Keyframe& curPos = posKeyFrames[i];
		Vec3 rootJointPos	 = curPos.m_data;
		DebuggerPrintf( "JointPos X:%0.2f, Y:%0.2f, Z:%0.2f\n", rootJointPos.x, rootJointPos.y, rootJointPos.z );
	}
	return animPose;
}


//----------------------------------------------------------------------------------------------------------------------
AnimationPose AnimationController::SamplePose_FrontFlipTwist( float deltaSeconds )
{
	AnimationPose sampledPose = SamplePoseFromPlayingClip( "Front Flip Twist", m_animClip_FrontFlipTwist, m_animBindPose_FrontFlipTwist, m_timeScalarMS, deltaSeconds, false, false );
	return sampledPose;

// 	DebuggerPrintf("Front Flip Twist\n"); 
// 	float timeScalar						= 1000.0f;
// 	m_animClip_FrontFlipTwist.m_localTime	+= ( deltaSeconds * timeScalar );
// 	float animTimeMS						= m_animClip_FrontFlipTwist.m_localTime;
// 	AnimationPose animPose					= m_animClip_FrontFlipTwist.SampleAnimPoseAtTime( animTimeMS, m_animBindPose_FrontFlipTwist, false, false );
// 	animPose.m_poseName						= "Front Flip Twist";
// 
// 	std::vector<Vec3Keyframe>& posKeyFrames = m_animClip_FrontFlipTwist.m_jointCurveList[0].m_positionKeyframeList;
// 	for ( int i = 0; i < posKeyFrames.size(); i++ )
// 	{
// 		Vec3Keyframe& curPos = posKeyFrames[i];
// 		Vec3 rootJointPos	 = curPos.m_data;
// 		DebuggerPrintf( "JointPos X:%0.2f, Y:%0.2f, Z:%0.2f\n", rootJointPos.x, rootJointPos.y, rootJointPos.z );
// 	}
// 	return animPose;
}


//----------------------------------------------------------------------------------------------------------------------
AnimationPose AnimationController::SamplePose_FrontFlipRoll( float deltaSeconds )
{
	AnimationPose sampledPose = SamplePoseFromPlayingClip( "Front Flip Roll", m_animClip_FrontFlipRoll, m_animBindPose_FrontFlipRoll, m_timeScalarMS, deltaSeconds, false, false );
	return sampledPose;
}


//----------------------------------------------------------------------------------------------------------------------
// NOTE: poseName is GIVEN to the returned pose, poseName is NOT used to find an existing animClip or animPose
// Increment the animation clip local time
// Sample a pose from the clip
AnimationPose AnimationController::SamplePoseFromPlayingClip( std::string const& poseName, AnimationClip& animClipToSample, AnimationPose const& bindPose, float timeScalarMS, float deltaSeconds, bool isAnimLooping, bool removeRootMotion )
{
	animClipToSample.m_localTime	+= ( deltaSeconds * timeScalarMS );
	float animTimeMS				= animClipToSample.m_localTime;
	AnimationPose animPose			= animClipToSample.SampleAnimPoseAtTime( animTimeMS, bindPose, isAnimLooping, removeRootMotion );
	animPose.m_poseName				= poseName;
	return animPose;
}