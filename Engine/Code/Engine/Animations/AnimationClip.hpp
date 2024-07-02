 #pragma once

#include "Engine/Animations/Transform.hpp"
#include "Engine/Animations/AnimationPose.hpp"
#include "Engine/Math/Quaternion.hpp"
#include "Engine/Math/Vec3.hpp"
#include "Engine/Math/Mat44.hpp"

#include <vector>


// A JointKeyframe is a joint's TRS at a specified time
//----------------------------------------------------------------------------------------------------------------------
struct Vec3Keyframe
{
	float m_keyframeTime = 0.0f;		// in miliSeconds
	Vec3  m_data;
};


//----------------------------------------------------------------------------------------------------------------------
struct QuaternionKeyframe
{
	float		m_keyframeTime = 0.0f;
	Quaternion  m_data;
};


//----------------------------------------------------------------------------------------------------------------------
// A jointAnimationCurve has a list of a SINGLE joint's TRS vaues over the entire animation clip
struct JointAnimationCurve
{
	std::vector<Vec3Keyframe>		m_positionKeyframeList; 
	std::vector<QuaternionKeyframe> m_rotationKeyframeList; 
	std::vector<Vec3Keyframe>		   m_scaleKeyframeList;
};


//----------------------------------------------------------------------------------------------------------------------
class AnimationClip
{
public:
	AnimationClip();
	AnimationPose SampleAnimPoseAtTime( float elapsedAnimationTime, AnimationPose const& animationBindPose, bool isAnimationLooping = false, bool removeRootMotion = false );
	void ComputeStartEndTime();
	bool IsFinished();
	void ResetLocalTime();
	void SetName( std::string const& clipName );
	std::string GetName() const;

public:
	// List of all joints with its own TRS curves
	std::vector<JointAnimationCurve> m_jointCurveList;
	float m_startTime			= 0.0f;
	float m_endTime				= 0.0f;
	float m_localTime			= 0.0f;

private:
	bool  m_isFinishedPlaying	= false;
	float m_sampleTimeLastFrame = FLT_MAX;
	std::string m_clipName		= "UN-NAMED";
};