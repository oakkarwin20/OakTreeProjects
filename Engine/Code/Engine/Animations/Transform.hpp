 #pragma once

#include "Engine/Math/Quaternion.hpp"
#include "Engine/Math/Vec3.hpp"
#include "Engine/Math/Mat44.hpp"


//----------------------------------------------------------------------------------------------------------------------
// Transform the noun, Transform is an object
struct Transform
{
public:
	Transform();
	Transform( Vec3 const& position, Quaternion const& rotation, Vec3 const& scale );
	static Transform ApplyChildToParentTransform( Transform const& child, Transform const& parent );

public:
	// All data is stored in local space, relative to parent
	Vec3		m_position = Vec3::ZERO;
	Quaternion	m_rotation = Quaternion();
	Vec3		m_scale		= Vec3( 1.0f, 1.0f, 1.0f );
};