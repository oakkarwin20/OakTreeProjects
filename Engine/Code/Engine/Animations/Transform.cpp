#include "Engine/Animations/Transform.hpp"


//----------------------------------------------------------------------------------------------------------------------
Transform::Transform()
{
}


//----------------------------------------------------------------------------------------------------------------------
Transform::Transform( Vec3 const& position, Quaternion const& rotation, Vec3 const& scale )
	: m_position( position ), m_rotation( rotation ), m_scale( scale )
{
}


//----------------------------------------------------------------------------------------------------------------------
// PS = Parent space, relative to grandparent
// CS = Child space, relative to parent
// GS = Grandparent space, relative to great-grandParent
// Transform as a verb
Transform Transform::ApplyChildToParentTransform( Transform const& child_CS, Transform const& parent_PS )
{
	Transform transform_GS;

	// Combine scale
	transform_GS.m_scale	= parent_PS.m_scale * child_CS.m_scale;
	// Combine rotation
	transform_GS.m_rotation = parent_PS.m_rotation * child_CS.m_rotation;		// Append local offset
	// Combine position
	Vec3 childPos_PS		= parent_PS.m_rotation * ( parent_PS.m_scale * child_CS.m_position );	// Get child position into parent space
	transform_GS.m_position = parent_PS.m_position + childPos_PS;	

	return transform_GS;
}