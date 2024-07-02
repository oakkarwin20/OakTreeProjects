#pragma once

#include "Game/GameModeBase.hpp"
#include "Game/GameCommon.hpp"
#include "Game/App.hpp"

#include "Engine/Animations/AnimationClip.hpp"
#include "Engine/Animations/AnimationPose.hpp"


//----------------------------------------------------------------------------------------------------------------------
class Player;
class PhysicsController;
class MoveStateController;


//----------------------------------------------------------------------------------------------------------------------
// Note: AnimationController is for GameModeAnimations ONLY
//----------------------------------------------------------------------------------------------------------------------
class AnimationController
{
public:
	AnimationController( Player* player, PhysicsController* physicsController, MoveStateController* moveStateController );
	~AnimationController();

	void Startup();
	void Shutdown();
	void Update( float deltaSeconds );
	void Render() const;

	void DebugDrawJointBasis( std::vector<Vertex_PCU>& animVerts, Transform const& curJointTransform, float length, float thickness );
	void AddVertsForDebugJointsAndBones( AnimationPose const& animPoseToRender );

	// State machine nodes
	void MovePlayerRootMotionX ( AnimationPose& animPoseToRender );		// MovePlayerPhysicsPosViaRootMotion
	void MovePlayerRootMotionXZ( AnimationPose& animPoseToRender );		// MovePlayerPhysicsPosViaRootMotion
	AnimationPose GenerateLocomotionPose();
	AnimationPose SamplePose_Fall( float deltaSeconds );
	AnimationPose SamplePose_FallToRoll( float deltaSeconds );		// Should rename animation to "land and roll"
	AnimationPose SamplePose_FrontFlipDefault( float deltaSeconds );
	AnimationPose SamplePose_FrontFlipTwist	 ( float deltaSeconds );
	AnimationPose SamplePose_FrontFlipRoll	 ( float deltaSeconds );
	AnimationPose SamplePoseFromPlayingClip( std::string const& poseName, AnimationClip& animClipToSample, AnimationPose const& bindPose, float timeScalarMS, float deltaSeconds , bool isAnimLooping, bool removeRootMotion );

public:
	// Core variables
	std::vector<Vertex_PCU> m_animVerts;
	Player*				 m_player				= nullptr;
	PhysicsController*	 m_physicsController	= nullptr;
	MoveStateController* m_moveStateController	= nullptr;
	float				 m_totalTimeMS			= 0.0f;
	float				 m_timeScalarMS			= 1000.0f;


	// Animation variables
	AnimationPose m_animBindPose_Run;
	AnimationClip m_animClip_Run;

	AnimationPose m_animBindPose_Walk;
	AnimationClip m_animClip_Walk;

	AnimationPose m_animBindPose_Idle;
	AnimationClip m_animClip_Idle;

	AnimationPose m_animBindPose_Jump;
	AnimationClip m_animClip_Jump;

	AnimationPose m_animBindPose_TurnLeft;
	AnimationClip m_animClip_TurnLeft;

	AnimationPose m_animBindPose_TurnRight;
	AnimationClip m_animClip_TurnRight;

	AnimationPose m_animBindPose_TwistDance;
	AnimationClip m_animClip_TwistDance;

	AnimationPose m_animBindPose_FallingIdle;
	AnimationClip m_animClip_FallingIdle;

	AnimationPose m_animBindPose_FallingToLanding;
	AnimationClip m_animClip_FallingToLanding;

	AnimationPose m_animBindPose_FallingToRoll;
	AnimationClip m_animClip_FallingToRoll;

	AnimationPose m_animBindPose_FallingToLand;
	AnimationClip m_animClip_FallingToLand;

	AnimationPose m_animBindPose_FlyingLeftKick;
	AnimationClip m_animClip_FlyingLeftKick;

	AnimationPose m_animBindPose_HighLeftKick;
	AnimationClip m_animClip_HighLeftKick;

	AnimationPose m_animBindPose_RightCrescentKick;
	AnimationClip m_animClip_RightCrescentKick;

	AnimationPose m_animBindPose_LeftJabPunch;
	AnimationClip m_animClip_LeftJabPunch;

	AnimationPose m_animBindPose_RightJabPunch;
	AnimationClip m_animClip_RightJabPunch;

	AnimationPose m_animBindPose_RightHook;
	AnimationClip m_animClip_RightHook;

	AnimationPose m_animBindPose_RightCrossPunch;
	AnimationClip m_animClip_RightCrossPunch;

	AnimationPose m_animBindPose_FrontFlip;
	AnimationClip m_animClip_FrontFlip;

	AnimationPose m_animBindPose_FrontFlipTwist;
	AnimationClip m_animClip_FrontFlipTwist;

	AnimationPose m_animBindPose_FrontFlipRoll;
	AnimationClip m_animClip_FrontFlipRoll;

	AnimationPose m_animBindPose_Vault;
	AnimationClip m_animClip_Vault;

	AnimationPose m_animBindPose_CrouchedIdle;
	AnimationClip m_animClip_CrouchedIdle;

	AnimationPose m_animBindPose_CrouchedWalkForward;
	AnimationClip m_animClip_CrouchedWalkForward;


	// Used for debugging and setting move states
	AnimationClip* m_animClipCurPlayed = nullptr;
	AnimationPose  m_animPoseLastFrame;
	std::string	   m_debugPoseName	= "UN-NAMED pose";
	float		   m_animTime		= 0.0f;

	// Root motion
	Vec3 m_rootJointDispLastFrame_LS = Vec3::ZERO;
};
