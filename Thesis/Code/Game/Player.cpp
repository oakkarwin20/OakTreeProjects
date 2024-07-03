#include "Game/App.hpp"
#include "Game/Player.hpp"
#include "Game/Quadruped.hpp"
#include "Game/PhysicsController.hpp"
#include "Game/AnimationController.hpp"
#include "Game/PhysicsController.hpp"
#include "Game/MoveStateController.hpp"
#include "Game/GameMode_Animations.hpp"
#include "Game/Map_Animations.hpp"


//----------------------------------------------------------------------------------------------------------------------
Player::Player( GameMode_Animations* gameModeAnimations, Vec3 const& spawnPos )
	: m_gameModeAnimations( gameModeAnimations )
	, m_position( spawnPos )
{
	m_physicsController		= new PhysicsController	 ( this );
	m_animController		= new AnimationController( this, m_physicsController, nullptr );
	m_moveStateController	= new MoveStateController( this, m_animController, m_physicsController );
	m_animController->m_moveStateController = m_moveStateController;		// Have to set the moveStateController after initializing the animController since it didn't exist at the time
}


//----------------------------------------------------------------------------------------------------------------------
Player::~Player()
{
}


//----------------------------------------------------------------------------------------------------------------------
void Player::Startup()
{
}


//----------------------------------------------------------------------------------------------------------------------
void Player::Update( float deltaSeconds )
{	
	m_physicsController	 ->Update( deltaSeconds );
	m_moveStateController->Update( deltaSeconds );
	m_animController	 ->Update( deltaSeconds );
}


//----------------------------------------------------------------------------------------------------------------------
void Player::Render() const
{
	m_physicsController->Render();
	m_animController->Render();

	// Debug draw raycast
	std::vector<Vertex_PCU> raycastVerts;

	// Raycast arrow, player to floor
	AddVertsForArrow3D( raycastVerts, m_raycastPlayerToFloor.m_rayStartPosition, m_raycastPlayerToFloor.m_impactPos, 0.6f, Rgba8::MAGENTA );
	if ( m_raycastPlayerToFloor.m_didImpact )
	{
		// Impact normal
		Vec3 endPos = m_raycastPlayerToFloor.m_impactPos + ( m_raycastPlayerToFloor.m_impactNormal * 5.0f );
		AddVertsForArrow3D ( raycastVerts, m_raycastPlayerToFloor.m_impactPos, endPos, 0.8f, Rgba8::BLUE );
		AddVertsForSphere3D( raycastVerts, m_raycastPlayerToFloor.m_impactPos, 0.9f );
	}

	// Raycast arrow, player to fwd
	Vec3 rayFwdEnd = m_raycastPlayerToFwd.m_rayStartPosition + ( m_raycastPlayerToFwd.m_rayFwdNormal * m_physicsController->m_vaultTriggerDist );
	AddVertsForArrow3D( raycastVerts, m_raycastPlayerToFwd.m_rayStartPosition, rayFwdEnd, 0.6f, Rgba8::MAGENTA );
	if ( (m_raycastPlayerToFwd.m_didImpact) && (m_raycastPlayerToFwd.m_impactDist <= m_physicsController->m_vaultTriggerDist) )
	{
		// Impact normal
		Vec3 endPos = m_raycastPlayerToFwd.m_impactPos + ( m_raycastPlayerToFwd.m_impactNormal * 5.0f );
		AddVertsForArrow3D ( raycastVerts, m_raycastPlayerToFwd.m_impactPos, endPos, 0.8f, Rgba8::BLUE );
		AddVertsForSphere3D( raycastVerts, m_raycastPlayerToFwd.m_impactPos, 0.9f );
	}

	// Draw call
	g_theRenderer->SetBlendMode( BlendMode::ALPHA );
	g_theRenderer->SetModelConstants();
	g_theRenderer->BindTexture( nullptr );
	g_theRenderer->BindShader( nullptr );
	g_theRenderer->DrawVertexArray( static_cast<int>( raycastVerts.size() ), raycastVerts.data() );

}


//----------------------------------------------------------------------------------------------------------------------
void Player::Shutdown()
{
}


//----------------------------------------------------------------------------------------------------------------------
void Player::UpdateAnimations( float deltaSeconds )
{
	m_animController->Update( deltaSeconds );
}


//----------------------------------------------------------------------------------------------------------------------
std::string Player::GetStateAsString()
{
	MoveState& curMoveState = m_moveStateController->m_curMoveState;
	if ( curMoveState == MoveState::MOVESTATE_LOCOMOTION )
	{
		return "IDLE";
	}
	else if ( curMoveState == MoveState::MOVESTATE_JUMP )
	{
		return "Jump";
	}
	return "INVALID";
}


//----------------------------------------------------------------------------------------------------------------------
void Player::RaycastPlayerToFloor()
{
	Vec3			   const& physicsPos = m_physicsController->m_position;
	std::vector<AABB3> const& floorList	 = m_gameModeAnimations->m_map->m_floorList;
	RaycastResult3D nearestHitResult_PlayerToFloor;
	RaycastResult3D nearestHitResult_PlayerToFwd;
	
	Vec3 const&	playerFwd	= m_physicsController->m_orientation.GetForwardDir_XFwd_YLeft_ZUp();
	for ( int i = 0; i < floorList.size(); i++ )
	{
		AABB3 const&	curABB3		 = floorList[i];
		// Player to floor raycast
		RaycastResult3D curHitResult = RaycastVsAABB3D( physicsPos, -Vec3::Z_UP, m_rayMaxLength, curABB3 );
		if ( curHitResult.m_impactDist < nearestHitResult_PlayerToFloor.m_impactDist )
		{
			nearestHitResult_PlayerToFloor = curHitResult;
		}

		// Player to fwd raycast
		curHitResult			= RaycastVsAABB3D( physicsPos, playerFwd, m_rayMaxLength, curABB3 );
		if ( curHitResult.m_impactDist < nearestHitResult_PlayerToFwd.m_impactDist )
		{
			nearestHitResult_PlayerToFwd = curHitResult;
		}
	}
	m_raycastPlayerToFloor	= nearestHitResult_PlayerToFloor;
	m_raycastPlayerToFwd	= nearestHitResult_PlayerToFwd;
	if ( !m_raycastPlayerToFwd.m_didImpact )
	{
		m_raycastPlayerToFwd.m_rayFwdNormal		= playerFwd;
		m_raycastPlayerToFwd.m_rayStartPosition = physicsPos;
		m_raycastPlayerToFwd.m_rayMaxLength		= m_rayMaxLength;
	}
}