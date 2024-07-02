#pragma once

#include "Game/GameModeBase.hpp"
#include "Game/GameCommon.hpp"
#include "Game/App.hpp"

#include "Engine/Animations/AnimationTimeline.hpp"
#include "Engine/Animations/AnimationClip.hpp"
#include "Engine/Animations/AnimationPose.hpp"
#include "Engine/Input/InputSystem.hpp"
#include "Engine/Renderer/Camera.hpp"
#include "Engine/Math/MathUtils.hpp"
#include "Engine/Math/CubicBezierCurve3D.hpp"
#include "Engine/Core/Stopwatch.hpp"


//----------------------------------------------------------------------------------------------------------------------
class Texture;
class AnimationClip;
class Map_Animations;
class Player;


//----------------------------------------------------------------------------------------------------------------------
class GameMode_Animations : public GameModeBase
{
public:
	GameMode_Animations();
	virtual ~GameMode_Animations();
	virtual void Startup();
	virtual void Update( float deltaSeconds );
	virtual void Render() const;
	virtual void Shutdown();

	// Camera and Render Functions
	void UpdateDebugKeys();
	void UpdateGameMode3DCamera( float deltaSeconds );
	void RenderWorldObjects()	const;
	void RenderUIObjects()		const;

	// Debug Functions
	void AddVertsForCompass( std::vector<Vertex_PCU>&compassVerts, Vec3 startPosition, float axisLength, float axisThickness ) const;
	void UpdateTargetInput( float deltaSeconds );

	// Character update
	void UpdateInput_CameraAndPlayer( float deltaSeconds );
	void UpdatePhysics( float deltaSeconds );
//	void UpdateAnimations( float deltaSeconds );
	void UpdateAnimTimeline();

public:
	// Core Variables
	float m_currentTime					= 0.0f;
	float m_walkLerpSpeed				= 4.0f;
	float m_sprintLerpSpeed				= m_walkLerpSpeed * 4.0f;
	float m_goalWalkLerpSpeed			= m_walkLerpSpeed;
	float m_currentWalkLerpSpeed		= m_goalWalkLerpSpeed;

	// Camera Variables
	Camera	m_gameMode3DWorldCamera;
	Camera	m_gameMode3DUICamera;
	// Camera movement Variables
	float	m_defaultSpeed	= 30.0f;
	float	m_currentSpeed	= 30.0f;
	float	m_fasterSpeed	= m_defaultSpeed * 4.0f;
	float	m_elevateSpeed  = 6.0f;
	float	m_turnRate		= 90.0f;
	// Camera lerp variables (Juice)
	float	m_fovDefault	= 60.0f;
	float	m_fovCurrent	= m_fovDefault;
	float	m_fovMax		= m_fovDefault * 1.5f;

	// Debug variables
	float	m_defaultDist_CameraToPlayer = 100.0f;
	float	m_curDist_CameraToPlayer	 = m_defaultDist_CameraToPlayer;
	float	m_maxDist_CameraToPlayer	 = m_defaultDist_CameraToPlayer * 1.2f;

	// Lighting Variables
	Vec3	m_sunDirection				= Vec3::ZERO;
	float   m_sunIntensity				= 0.85f;
	float   m_ambientIntensity			= 0.35f;
	float	m_specularIntensity			= 0.75f;
	float	m_specularPower				= 18.0f;

	// Animation variables
	float m_elapsedAnimationTime = 1;
	float m_animTimeTurnLeftMS	 = 0.0f;
	// Animation Timeline
	AABB2 m_animTimelineBounds		 = AABB2( Vec2( SCREEN_SIZE_X * 0.01f, SCREEN_SIZE_Y * 0.01f ), Vec2(SCREEN_SIZE_X * 0.99f, SCREEN_SIZE_Y * 0.2f) );
	AnimationTimeline m_animTimeline = AnimationTimeline( m_animTimelineBounds );

	// Gameplay pointers
	Map_Animations* m_map	 = nullptr;
	Player*			m_player = nullptr;

	// Inverse Kinematics
	Vec3 m_ikTargetPos = Vec3::ZERO;
};
