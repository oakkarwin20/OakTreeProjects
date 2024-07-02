#include "Engine/Animations/AnimationPose.hpp"
#include "Engine/Math/MathUtils.hpp"
#include "Engine/Core/VertexUtils.hpp"

#include "Engine/Core/ErrorWarningAssert.hpp"


//----------------------------------------------------------------------------------------------------------------------
Transform AnimationPose::LocalToModel_Transform( int jointIndex ) const
{
	AnimationJoint const&	curJoint			= m_jointList[jointIndex];
	int						parentIndex			= curJoint.m_parentJointIndex;
	Transform				modelSpaceTransform = curJoint.m_localToParentTransform;
	while ( parentIndex >= 0 )
	{
		Transform const& localToParentTransformOfThisJoint	= modelSpaceTransform;
		Transform const& parentJointTransform				= m_jointList[parentIndex].m_localToParentTransform;
		modelSpaceTransform									= Transform::ApplyChildToParentTransform(localToParentTransformOfThisJoint, parentJointTransform);
		parentIndex											= m_jointList[parentIndex].m_parentJointIndex;
	}
	return modelSpaceTransform;
}


//----------------------------------------------------------------------------------------------------------------------
// Also can rename parameters as:
// poseA = startPose
// poseB = endPose
//----------------------------------------------------------------------------------------------------------------------
AnimationPose AnimationPose::GetBlendFromTwoPoses( AnimationPose const& poseA, AnimationPose const& poseB, float blendFactorParametric )
{
	AnimationPose blendedPose = poseA;
	// loop through all joints from poseA and poseB, blend position, rotation, scale per joint
	for ( int jointIndex = 0; jointIndex < poseA.m_jointList.size(); jointIndex++ )
	{
		AnimationJoint const& curJointA = poseA.m_jointList[jointIndex];
		AnimationJoint const& curJointB = poseB.m_jointList[jointIndex];

		Transform const& jointTransformA = curJointA.m_localToParentTransform;
		Transform const& jointTransformB = curJointB.m_localToParentTransform;
		Transform& jointTransformBlended = blendedPose.m_jointList[jointIndex].m_localToParentTransform;

		jointTransformBlended.m_position = Interpolate	 ( jointTransformA.m_position,  jointTransformB.m_position, blendFactorParametric );		// Position
		jointTransformBlended.m_rotation = NormalizedLerp( jointTransformA.m_rotation,  jointTransformB.m_rotation, blendFactorParametric );		// Rotation
		jointTransformBlended.m_scale	 = Interpolate	 ( jointTransformA.m_scale,		jointTransformB.m_scale,	blendFactorParametric );		// Scale
	}

	return blendedPose;
}


//----------------------------------------------------------------------------------------------------------------------
void AnimationPose::DebugPrintJointHierarchy()
{
	for ( int jointIndex = 0; jointIndex < m_jointList.size(); jointIndex++ )
	{
		AnimationJoint const& curJoint = m_jointList[jointIndex];
		// Debug print jointInfo
		DebuggerPrintf( "jointIndex: %d, jointName: %s\n", jointIndex, curJoint.m_jointName.c_str() );
	}
}


//----------------------------------------------------------------------------------------------------------------------
void AnimationPose::AddVertsForDebugJointsAndBones( std::vector<Vertex_PCU>& verts ) const
{
	// AddVerts to debug draw joints & bones
	for ( int jointIndex = 0; jointIndex < m_jointList.size(); jointIndex++ )
	{
		AnimationJoint const& joint			  = m_jointList[jointIndex];
		Transform const& curJointTransform_WS = LocalToModel_Transform(jointIndex);
		if ( joint.m_parentJointIndex >= 0 )
		{
			Transform const& parentJointTransform_WS = LocalToModel_Transform( joint.m_parentJointIndex );
			AddVertsForCone3D	( verts, parentJointTransform_WS.m_position, curJointTransform_WS.m_position, 0.5f );
			AddVertsForSphere3D ( verts,    curJointTransform_WS.m_position, 0.2f, 4.0f, 4.0f, Rgba8::DARK_CYAN );
//			DebugDrawJointBasis ( verts, curJointTransform_WS, 2.0f, 0.1f );
		}
	}
}