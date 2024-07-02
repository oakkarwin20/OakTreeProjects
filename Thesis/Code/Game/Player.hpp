#pragma once

#include "Game/GameModeBase.hpp"
#include "Game/GameCommon.hpp"
#include "Game/App.hpp"

#include "Engine/Animations/AnimationClip.hpp"
#include "Engine/Animations/AnimationPose.hpp"
#include "Engine/Input/InputSystem.hpp"
#include "Engine/Renderer/Camera.hpp"
#include "Engine/Math/MathUtils.hpp"
#include "Engine/Math/CubicBezierCurve3D.hpp"
#include "Engine/Core/Stopwatch.hpp"


//----------------------------------------------------------------------------------------------------------------------
class GameMode_Animations;
class AnimationController;
class PhysicsController;
class MoveStateController;


//----------------------------------------------------------------------------------------------------------------------
// Note: Player is for GameModeAnimations ONLY
//----------------------------------------------------------------------------------------------------------------------
class Player
{
public:
	Player( GameMode_Animations* gameModeAnimations, Vec3 const& spawnPos );
	~Player();
	void Startup();
	void Update( float deltaSeconds );
	void Render() const;
	void Shutdown();

	void UpdateAnimations( float deltaSeconds );

	// Movement state Utils
	std::string GetStateAsString();

	// Raycast functions
	void RaycastPlayerToFloor();

public:
	// Core variables
	GameMode_Animations*	m_gameModeAnimations  = nullptr;
	AnimationController*	m_animController	  = nullptr;
	PhysicsController*		m_physicsController	  = nullptr;
	MoveStateController*	m_moveStateController = nullptr;
	Vec3					m_position			  = Vec3::ZERO;

	// Raycast variables
	RaycastResult3D m_raycastPlayerToFloor;
	RaycastResult3D m_raycastPlayerToFwd;
	float m_rayMaxLength = 100.0f;
};
