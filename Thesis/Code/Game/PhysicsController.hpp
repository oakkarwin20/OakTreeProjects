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
class Player;


//----------------------------------------------------------------------------------------------------------------------
// Note: PhysicsController is for GameModeAnimations ONLY
//----------------------------------------------------------------------------------------------------------------------
class PhysicsController
{
public:
	PhysicsController( Player* player );
	~PhysicsController();

	void Startup();
	void Shutdown();
	void Update( float deltaSeconds );
	void Render() const;

public:
	// Core variables
	Player*		m_player				= nullptr;
	Vec3		m_position				= Vec3::ZERO;
	Vec3		m_velocity				= Vec3::ZERO;
	EulerAngles	m_orientation			= EulerAngles();
	Vec3		m_moveDirection			= Vec3::ZERO;
	float		m_playerPhysicsRadius	= 10.0f;
	bool		m_isPhysicsEnabled		= true;

	// Player movement Variables
	float	m_defaultMoveSpeed		= 25.0f;
	float	m_curSpeedScalar		= 25.0f;			// Move speed to apply this frame
	float	m_fasterSpeed			= m_defaultMoveSpeed * 4.0f;
	float	m_maxSpeedXY			= 100.0f;		
	float	m_maxSpeedZ				= 100.0f;			// m_maxGravity
	float	m_jumpForce				= 3'000.0f;			// 5000.0f is a good amount?
	float	m_turnRate				= 360.0f;
	// Springing
	bool	m_isSprinting			= false;
	float   m_rightTriggerLastFrame = 0.0f;
	// Jump variables
	bool	m_canTriggerJump	= true;
	bool	m_isJumping			= false;
	// Vault variables
	bool	m_canTriggerVault	= true;
	bool	m_isVaulting		= false;
	float   m_vaultTriggerDist  = 50.0f;
	// Crouch variables
	bool	m_canTriggerCrouch	= true;
	bool	m_isCrouching		= false;



	// Juice lerping variable
	bool m_startCameraLerpCloser = false;
};
