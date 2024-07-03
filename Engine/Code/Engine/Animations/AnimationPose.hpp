 #pragma once

#include "Engine/Math/Mat44.hpp"
#include "Engine/Animations/Transform.hpp"

#include <string>
#include <vector>


//----------------------------------------------------------------------------------------------------------------------
struct Vertex_PCU;


//----------------------------------------------------------------------------------------------------------------------
struct AnimationJoint
{
	Transform	m_localToParentTransform;		// Transform the noun not the verb. An object with TRS data.
	int			m_parentJointIndex;
	std::string m_jointName;
};


//----------------------------------------------------------------------------------------------------------------------
class AnimationPose
{
public:
	Transform LocalToModel_Transform( int jointID ) const;
	static AnimationPose GetBlendFromTwoPoses( AnimationPose const& poseA, AnimationPose const& poseB, float blendFactorParametric );		// Parametric means [0,1]
	void DebugPrintJointHierarchy();
	void AddVertsForDebugJointsAndBones( std::vector<Vertex_PCU>& verts ) const;
	void DebugDrawJointBasis( std::vector<Vertex_PCU>& animVerts, int jointID, float length, float thickness ) const;

public:
	std::vector<AnimationJoint> m_jointList;
	std::string m_poseName = "INVALID POSE";
};