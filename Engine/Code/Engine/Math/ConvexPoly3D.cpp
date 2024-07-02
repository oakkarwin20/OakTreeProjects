#include "Engine/Math/ConvexPoly3D.hpp"
#include "Engine/Math/Vec3.hpp"
#include "Engine/Core/EngineCommon.hpp"


//----------------------------------------------------------------------------------------------------------------------
ConvexPoly3D::ConvexPoly3D( std::vector<Vec3> ccwOrderedPoints )
{
	m_ccwOrderedPoints = ccwOrderedPoints;
}


//----------------------------------------------------------------------------------------------------------------------
ConvexPoly3D::~ConvexPoly3D()
{
}


//----------------------------------------------------------------------------------------------------------------------
bool ConvexPoly3D::IsPointInside( Vec3 const& point )
{
	UNUSED( point );
	return false;
}


//----------------------------------------------------------------------------------------------------------------------
Vec3 ConvexPoly3D::GetNearestPoint( Vec3 const& pointToCheck )
{
	UNUSED( pointToCheck );
	return Vec3();
}
