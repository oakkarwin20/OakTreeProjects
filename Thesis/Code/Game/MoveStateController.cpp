#include "MoveStateController.hpp"
#include "AnimationController.hpp"
#include "PhysicsController.hpp"
#include "Player.hpp"

#include "Engine/Core/ErrorWarningAssert.hpp"
#include "Engine/Core/EngineCommon.hpp"

#include <stack>


//----------------------------------------------------------------------------------------------------------------------
MoveStateController::MoveStateController( Player* player, AnimationController* animController, PhysicsController* physicsController )
	: m_player( player )
	, m_animController( animController )
	, m_physicsController( physicsController )
{
}


//----------------------------------------------------------------------------------------------------------------------
MoveStateController::~MoveStateController()
{
}


//----------------------------------------------------------------------------------------------------------------------
void MoveStateController::Update( float deltaSeconds )
{
	UNUSED( deltaSeconds );

	// Attempt to understand blendtrees
// 	 m_rootBlendNode				= new BlendNode();
// 	 BlendNode* childA				= new BlendNode();
// 	 BlendNode* childB				= new BlendNode();
// 	 m_rootBlendNode->m_childNodeA	= childA;
// 	 m_rootBlendNode->m_childNodeB	= childB;
// 	*m_rootBlendNode->m_childNodeB->m_animClip = m_animController->m_animClip_Jump;
// 
// 	BlendNode* subChildA						 = new BlendNode();
// 	BlendNode* subChildB						 = new BlendNode();
// 	 m_rootBlendNode->m_childNodeA->m_childNodeA = subChildA;
// 	 m_rootBlendNode->m_childNodeA->m_childNodeB = subChildB;
// 	*m_rootBlendNode->m_childNodeA->m_childNodeA->m_animClip = m_animController->m_animClip_Walk;
// 	*m_rootBlendNode->m_childNodeA->m_childNodeB->m_animClip = m_animController->m_animClip_Run;
// 
// 	std::stack<BlendNode*> blendNodeList;
// 	blendNodeList.push( m_rootBlendNode );
// 	// Create stack with all nodes
// 	for ( int i = 0; i < blendNodeList.size(); i++ )
// 	{
// 		BlendNode* curNode = blendNodeList.top();
// 		if ( curNode == nullptr )
// 		{
// 			continue;
// 		}
// 
// 		if ( curNode->m_childNodeA )
// 		{
// 			blendNodeList.push( curNode->m_childNodeB );
// 			blendNodeList.push( curNode->m_childNodeA );
// 		}
// 	}
// 
// 	// Loop through stack and blend at each step
// 	AnimationPose animPoseToRender;
// 	AnimationPose animPoseBlendedResult;
// 	int blendNodeListSize = blendNodeList.size();
// 	for ( int i = 0; i < blendNodeListSize; i++ )
// 	{
// 		if ( blendNodeListSize == 0 )
// 		{
// 			break;
// 		}
// 		BlendNode* curNodeA = blendNodeList.top();
// 		blendNodeList.pop();
// 		BlendNode* curNodeB = blendNodeList.top();
// 		blendNodeList.pop();
// 
// 		if ( curNodeA->m_childNodeA )
// 		{
// 			// Blend between childA & childB
// 			// New approach (multiple animations sampled based on states)
// 			AnimationPose animPoseA	= curNodeA->m_animClip->SampleAnimPoseAtTime( curNodeA->m_animClip->m_localTime, *curNodeA->m_bindPose, curNodeA->m_loopAnim );
// 			AnimationPose animPoseB	= curNodeB->m_animClip->SampleAnimPoseAtTime( curNodeB->m_animClip->m_localTime, *curNodeB->m_bindPose, curNodeB->m_loopAnim );
// 
// 			curNodeA				= blendNodeList.top();
// 			animPoseBlendedResult	= AnimationPose::GetBlendFromTwoPoses( animPoseA, animPoseB, curNodeA->m_blendFactor );
// 			*curNodeA->m_resultPose	= animPoseBlendedResult;
// 		}
// 		else    // this node has a child so nodeA must be the result. Blend nodeA's resultPose with nodeB
// 		{
// 			AnimationPose animPoseA = *curNodeA->m_resultPose;
// 			AnimationPose animPoseB = curNodeB->m_animClip->SampleAnimPoseAtTime( curNodeB->m_animClip->m_localTime, *curNodeB->m_bindPose, curNodeB->m_loopAnim );
// 			animPoseBlendedResult	= AnimationPose::GetBlendFromTwoPoses( animPoseA, animPoseB, curNodeA->m_blendFactor );
// 		}
// 	}
// 	animPoseToRender = animPoseBlendedResult;
}


//----------------------------------------------------------------------------------------------------------------------
void MoveStateController::SetState( MoveState newState )
{
	if ( newState == MoveState::MOVESTATE_LOCOMOTION )
	{
		// Update anim Controller
 		m_animController->m_animClip_Idle.ResetLocalTime();
 		m_animController->m_animClip_Walk.ResetLocalTime();
 		m_animController->m_animClip_Run. ResetLocalTime();

		// Update crossfade
		m_animTransition						= AnimationTransition();
		m_isCrossfade							= true;
		m_crossfade_PoseToPose					= true;
		m_animTransition.m_maxCrossFadeTime_MS	= 200.0f;
		m_animTransition.m_curCrossFadeTime_MS	= 0.0f;
		m_animTransition.m_animPosePrev			= &m_animController->m_animPoseLastFrame;
		m_animTransition.m_animClipCur			= &m_animController->m_animClip_Idle;
		m_animTransition.m_animBindPose			= &m_animController->m_animBindPose_Idle;
		
		// Update state
		UpdateMoveStates( newState );

		// Reset jump data
		m_physicsController->m_canTriggerJump = true;
	}
	else if ( newState == MoveState::MOVESTATE_JUMP )
	{
		// Update anim Controller
		m_animController->m_animClip_Jump.ResetLocalTime();

		// Update crossfade
		m_animTransition						= AnimationTransition();
		m_isCrossfade							= true;
		m_crossfade_PoseToPose					= false;
		m_animTransition.m_maxCrossFadeTime_MS	= 200.0f;
		m_animTransition.m_curCrossFadeTime_MS	= 0.0f;
		m_animTransition.m_animPosePrev			= &m_animController->m_animPoseLastFrame;
		m_animTransition.m_animClipCur			= &m_animController->m_animClip_Jump;
		m_animTransition.m_animBindPose			= &m_animController->m_animBindPose_Jump;

		// Update state
		UpdateMoveStates( newState );

		// Update debug info
		m_animController->m_animClipCurPlayed = &m_animController->m_animClip_Jump;
	}
	else if ( newState == MoveState::MOVESTATE_FALL )
	{
		// Update anim Controller
		m_animController->m_animClip_FallingIdle.ResetLocalTime();

		// Update crossfade
		m_animTransition						= AnimationTransition();
		m_isCrossfade							= true;
		m_crossfade_PoseToPose					= false;
		m_animTransition.m_maxCrossFadeTime_MS	= 200.0f;
		m_animTransition.m_curCrossFadeTime_MS	= 0.0f;
		m_animTransition.m_animPosePrev			= &m_animController->m_animPoseLastFrame;
		m_animTransition.m_animClipCur			= &m_animController->m_animClip_FallingIdle;
		m_animTransition.m_animBindPose			= &m_animController->m_animBindPose_FallingIdle;

		// Update state
		UpdateMoveStates( newState );

		// Update physics controller
		m_physicsController->m_canTriggerJump	= false;
		m_physicsController->m_isJumping		= false;

		// Update debug info
		m_animController->m_animClipCurPlayed = &m_animController->m_animClip_FallingIdle;
	}
	else if ( newState == MoveState::MOVESTATE_FALLING_TO_LAND )
	{
		// Update anim Controller
		m_animController->m_animClip_FallingToLand.ResetLocalTime();

		// Update crossfade
		m_animTransition								= AnimationTransition();
		m_isCrossfade									= true;
		m_crossfade_PoseToPose							= false;
		m_animTransition.m_maxCrossFadeTime_MS			= 200.0f;
		m_animTransition.m_curCrossFadeTime_MS			= 0.0f;
		m_animTransition.m_animPosePrev					= &m_animController->m_animPoseLastFrame;
		m_animTransition.m_animClipCur					= &m_animController->m_animClip_FallingToLand;
		m_animTransition.m_animBindPose					= &m_animController->m_animBindPose_FallingToLand;
//		m_animTransition.m_useRootMotion				= false;
//		m_animController->m_rootJointDispLastFrame_LS	= Vec3::ZERO;

		// Update state
		UpdateMoveStates( newState );

		// Update debug info
		m_animController->m_animClipCurPlayed = &m_animController->m_animClip_FallingToLand;
	}
	else if ( newState == MoveState::MOVESTATE_FALLING_TO_ROLL )
	{
		// Update anim Controller
		m_animController->m_animClip_FallingToRoll.ResetLocalTime();

		// Update crossfade
		m_animTransition								= AnimationTransition();
		m_isCrossfade									= true;
		m_crossfade_PoseToPose							= false;
		m_animTransition.m_maxCrossFadeTime_MS			= 200.0f;
		m_animTransition.m_curCrossFadeTime_MS			= 0.0f;
		m_animTransition.m_animPosePrev					= &m_animController->m_animPoseLastFrame;
		m_animTransition.m_animClipCur					= &m_animController->m_animClip_FallingToRoll;
		m_animTransition.m_animBindPose					= &m_animController->m_animBindPose_FallingToRoll;
		m_animTransition.m_useRootMotion				= true;
		m_animController->m_rootJointDispLastFrame_LS	= Vec3::ZERO;

		// Update state
		UpdateMoveStates( newState );

		// Update debug info
		m_animController->m_animClipCurPlayed = &m_animController->m_animClip_FallingToRoll;
	}
	else if ( newState == MoveState::MOVESTATE_VAULT )
	{
		// Update anim Controller
		m_animController->m_animClip_Vault.ResetLocalTime();
//		m_animController->m_animClip_Vault.m_localTime = 260.0f;

		// Update crossfade
		m_animTransition								= AnimationTransition();
		m_isCrossfade									= true;
		m_crossfade_PoseToPose							= false;
		m_animTransition.m_maxCrossFadeTime_MS			= 100.0f;
		m_animTransition.m_curCrossFadeTime_MS			= 0.0f;
		m_animTransition.m_animPosePrev					= &m_animController->m_animPoseLastFrame;
		m_animTransition.m_animClipCur					= &m_animController->m_animClip_Vault;
		m_animTransition.m_animBindPose					= &m_animController->m_animBindPose_Vault;
		m_animTransition.m_useRootMotion				= true;
		m_animController->m_rootJointDispLastFrame_LS	= Vec3::ZERO;

		UpdateMoveStates( newState );

		// Update physics controller
		m_physicsController->m_canTriggerVault	= false;
		m_physicsController->m_isVaulting		= false;
		m_physicsController->m_isPhysicsEnabled	= false;

		// Update debug info
		m_animController->m_animClipCurPlayed = &m_animController->m_animClip_Vault;
	}
	else if ( newState == MoveState::MOVESTATE_CROUCHED )
	{
		// Update anim Controller
		m_animController->m_animClip_CrouchedIdle.ResetLocalTime();

		// Update crossfade
		m_animTransition								= AnimationTransition();
		m_isCrossfade									= true;
		m_crossfade_PoseToPose							= true;
		m_animTransition.m_maxCrossFadeTime_MS			= 400.0f;
		m_animTransition.m_curCrossFadeTime_MS			= 0.0f;
		m_animTransition.m_animPosePrev					= &m_animController->m_animPoseLastFrame;
		m_animTransition.m_animClipCur					= &m_animController->m_animClip_CrouchedIdle;
		m_animTransition.m_animBindPose					= &m_animController->m_animBindPose_CrouchedIdle;
		m_animTransition.m_useRootMotion				= false;
		m_animController->m_rootJointDispLastFrame_LS	= Vec3::ZERO;

		UpdateMoveStates( newState );

		// Update physics controller
		m_physicsController->m_canTriggerVault	= false;
		m_physicsController->m_isVaulting		= false;

		// Update debug info
		m_animController->m_animClipCurPlayed = &m_animController->m_animClip_CrouchedIdle;
	}
}


//----------------------------------------------------------------------------------------------------------------------
void MoveStateController::UpdateMoveStates( MoveState newState )
{
	m_prevMoveState = m_curMoveState;
	m_curMoveState  = newState;
}


//----------------------------------------------------------------------------------------------------------------------
void MoveStateController::UpdateParkourFlipCounterAndState()
{
	// Update parkour flip counter
	if ( m_parkourFlipCounter == m_parkourFlipsMax ) 
	{
		// Reset counter
		m_parkourFlipCounter = 1;
	}
	else
	{
		// Increment counter
		m_parkourFlipCounter++;
	}

	// Update move state based on parkour flip counter
	m_prevMoveState = m_curMoveState;
	switch ( m_parkourFlipCounter )
	{
	case 1: 
		m_curMoveState  = MOVESTATE_FRONT_FLIP_DEFAULT;  
		break;
	case 2: 
		m_curMoveState  = MOVESTATE_FRONT_FLIP_TWIST;  
		break;
	case 3: 
		m_curMoveState  = MOVESTATE_FRONT_FLIP_ROLL;  
		break;

	default:
		ERROR_RECOVERABLE( "INVALID parkour flip counter chosen" );
		break;
	}
	m_parkourFlipFinished  = false;
	m_parkourFlipRequested = false;
}


//----------------------------------------------------------------------------------------------------------------------
void MoveStateController::ResetParkourFlipData()
{
	m_parkourFlipFinished  = true;
	m_parkourFlipRequested = false;
	m_parkourFlipCounter   = 0;
}
