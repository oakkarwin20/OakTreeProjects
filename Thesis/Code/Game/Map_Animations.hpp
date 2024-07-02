#pragma once

#include "Game/GameModeBase.hpp"
#include "Game/GameCommon.hpp"
#include "Game/App.hpp"

class GameMode_Animations;


//----------------------------------------------------------------------------------------------------------------------
class Map_Animations
{
public:
	Map_Animations();
	~Map_Animations();
	void Startup();
	void Shutdown();
	void Update( float deltaSeconds );
	void Render() const;

public:
	GameMode_Animations* m_gameModeAnimations = nullptr;

	AABB3				m_floorBounds  = AABB3();
	AABB3				m_boxBounds_NE = AABB3();
	AABB3				m_boxBounds_NW = AABB3();
	AABB3				m_boxBounds_SE = AABB3();
	AABB3				m_boxBounds_SW = AABB3();
	AABB3				m_boxBounds_Animations = AABB3();
	std::vector<AABB3>	m_floorList;
	VertexBuffer*		m_vbo   = nullptr;
};
