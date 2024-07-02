#include "Game/GameMode_Animations.hpp"
#include "Game/Map_Animations.hpp"
#include "Engine/Core/EngineCommon.hpp"

//----------------------------------------------------------------------------------------------------------------------
Map_Animations::Map_Animations()
{
	m_floorBounds			= AABB3( Vec3( -1000.0f, -1000.0f, -10.0f ), Vec3( 1000.0f, 1000.0f,  0.0f ) );
	m_boxBounds_NE			= AABB3( Vec3(	  0.0f,   250.0f,    0.0f ), Vec3(	50.0f,   300.0f, 20.0f ) );
	m_boxBounds_NW			= AABB3( Vec3(	-50.0f,	  250.0f,    0.0f ), Vec3(	 0.0f,   300.0f, 15.0f ) );
	m_boxBounds_SE			= AABB3( Vec3(	  0.0f,	  200.0f,    0.0f ), Vec3(	50.0f,   250.0f,  5.0f ) );
	m_boxBounds_SW			= AABB3( Vec3(	-50.0f,	  200.0f,    0.0f ), Vec3(	 0.0f,   250.0f, 10.0f ) );
	m_boxBounds_Animations	= AABB3( Vec3(	200.1f,	 -120.0f,    0.0f ), Vec3( 220.0f,	 150.0f, 25.0f ) );
	m_floorList.emplace_back( m_floorBounds  );
	m_floorList.emplace_back( m_boxBounds_NE );
	m_floorList.emplace_back( m_boxBounds_NW );
	m_floorList.emplace_back( m_boxBounds_SE );
	m_floorList.emplace_back( m_boxBounds_SW );
	m_floorList.emplace_back( m_boxBounds_Animations );
	// #ToDo: addVerts once on startup using vertexBuffer instead of pushing verts every frame
}


//----------------------------------------------------------------------------------------------------------------------
Map_Animations::~Map_Animations()
{
}


//----------------------------------------------------------------------------------------------------------------------
void Map_Animations::Startup()
{
}


//----------------------------------------------------------------------------------------------------------------------
void Map_Animations::Shutdown()
{
}


//----------------------------------------------------------------------------------------------------------------------
void Map_Animations::Update( float deltaSeconds )
{
	UNUSED( deltaSeconds );
}


//----------------------------------------------------------------------------------------------------------------------
void Map_Animations::Render() const
{
	std::vector<Vertex_PCU> verts; 
	AddVertsForAABB3D( verts, m_floorBounds,			Rgba8::DARKER_GREEN );
	AddVertsForAABB3D( verts, m_boxBounds_NE,			Rgba8::GRAY );
	AddVertsForAABB3D( verts, m_boxBounds_NW,			Rgba8::GRAY );
	AddVertsForAABB3D( verts, m_boxBounds_SE,			Rgba8::GRAY );
	AddVertsForAABB3D( verts, m_boxBounds_SW,			Rgba8::GRAY );
	AddVertsForAABB3D( verts, m_boxBounds_Animations,	Rgba8::GRAY );
	g_theRenderer->BindTexture( g_theApp->m_texture_MoonSurface );
	g_theRenderer->DrawVertexArray( int( verts.size() ), verts.data() );
	g_theRenderer->BindTexture(nullptr);
}
