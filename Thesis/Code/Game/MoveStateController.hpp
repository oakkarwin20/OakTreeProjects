#pragma once

#include "Engine/Animations/AnimationPose.hpp"

//----------------------------------------------------------------------------------------------------------------------
class Player;
class AnimationController;
class PhysicsController;
class AnimationClip;


//----------------------------------------------------------------------------------------------------------------------
enum MoveState
{
	MOVESTATE_LOCOMOTION,			// Base movement including idle/walk/run
	MOVESTATE_JUMP,
	MOVESTATE_FALL,	
	MOVESTATE_FALLING_TO_ROLL,
	MOVESTATE_FALLING_TO_LAND,
	MOVESTATE_FRONT_FLIP_DEFAULT,		// Flip 1
	MOVESTATE_FRONT_FLIP_TWIST,			// Flip 2
	MOVESTATE_FRONT_FLIP_ROLL,			// Flip 3
	MOVESTATE_VAULT,
	MOVESTATE_CROUCHED,
};



//----------------------------------------------------------------------------------------------------------------------
struct AnimationTransition
{
	// Cross fade variables
	float			m_maxCrossFadeTime_MS	= 0.0f;			// time in miliseconds
	float			m_curCrossFadeTime_MS	= 0.0f;			// time in miliseconds
	bool			m_useRootMotion			= false;
	AnimationPose*	m_animBindPose			= nullptr;
	AnimationPose*	m_animPosePrev			= nullptr;
	AnimationClip*	m_animClipCur			= nullptr;
	AnimationPose*	m_blendIntoPose			= nullptr;
};


//----------------------------------------------------------------------------------------------------------------------
// struct BlendNode
// {
// 	BlendNode*		m_childNodeA	= nullptr;
// 	BlendNode*		m_childNodeB	= nullptr;
// 	AnimationClip*  m_animClip		= nullptr;
// 	AnimationPose*  m_bindPose		= nullptr;
// 	AnimationPose*  m_resultPose	= nullptr;
// 	float		    m_blendFactor	= 0.0f;
// 	bool			m_loopAnim		= false;
// };


//----------------------------------------------------------------------------------------------------------------------
// Note: MoveStateControllerController is for GameModeAnimations ONLY
//----------------------------------------------------------------------------------------------------------------------
class MoveStateController
{
public:
	MoveStateController( Player* player, AnimationController* animController, PhysicsController* physicsController );
	~MoveStateController();

	void Update( float deltaSeconds );
	void SetState( MoveState newState );
	void UpdateMoveStates( MoveState newState );

	// Util functions
	void UpdateParkourFlipCounterAndState();
	void ResetParkourFlipData();

public:
	// References to owner
	Player*					m_player			= nullptr;
	AnimationController*	m_animController	= nullptr;
	PhysicsController*		m_physicsController	= nullptr;

	// Core variables
	bool					m_isCrossfade			= false;
	bool					m_crossfade_PoseToPose	= false;		// if false, blend poseToClip
	MoveState				m_curMoveState			= MOVESTATE_LOCOMOTION;
	MoveState				m_prevMoveState			= MOVESTATE_LOCOMOTION;
	AnimationTransition		m_animTransition;

	// BlendPose
	AnimationPose m_blendPose;

	// Parkour flip counter
	int  m_parkourFlipCounter	= 0;
	int  m_parkourFlipsMax		= 3;		// Keep track of total number of flips we can do
	bool m_parkourFlipRequested = false;
	bool m_parkourFlipFinished	= true;
};