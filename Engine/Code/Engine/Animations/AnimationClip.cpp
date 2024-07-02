#include "Engine/Animations/AnimationClip.hpp"
#include "Engine/Math/MathUtils.hpp"


//----------------------------------------------------------------------------------------------------------------------
AnimationClip::AnimationClip()
{
}


//----------------------------------------------------------------------------------------------------------------------
AnimationPose AnimationClip::SampleAnimPoseAtTime( float sampleTime, AnimationPose const& animationBindPose, bool isAnimationLooping, bool removeRootMotion )
{
	if ( isAnimationLooping && (sampleTime > m_endTime) )
	{
        float animDuration  = m_endTime - m_startTime;
        float newSampleTime = fmodf( sampleTime, animDuration );
		sampleTime          = newSampleTime;
	}
    else if ( !isAnimationLooping )
    {
        if ( sampleTime >= m_endTime )
        {
            float animDuration    = m_endTime - m_startTime;
            float newSampleTime   = fmodf( sampleTime, animDuration );
            sampleTime            = newSampleTime;
            if ( sampleTime <= m_sampleTimeLastFrame )
            {
                m_isFinishedPlaying = true;
                sampleTime          = m_endTime;
            }
            else
            {
                m_isFinishedPlaying = false;
            }
        }
        else
        {
            m_isFinishedPlaying = false;
        }
        m_sampleTimeLastFrame = sampleTime;
    }
    m_localTime = sampleTime;

	AnimationPose animationPoseAtTime;
	// Specify any time and get an animated pose AT THAT TIME
 	for ( int jointIndex = 0; jointIndex < m_jointCurveList.size(); jointIndex++ )
 	{
 		JointAnimationCurve const& curJointAnimCurve = m_jointCurveList[jointIndex];
 		AnimationJoint curAnimJoint = animationBindPose.m_jointList[jointIndex];
 
 		// Position
  		bool considerTranslation = true;
  		if ( removeRootMotion && jointIndex == 0 )
  		{
  			considerTranslation = false;
  		}
  		if ( considerTranslation )
  		{
 		    std::vector<Vec3Keyframe> const& posKeyframeList = curJointAnimCurve.m_positionKeyframeList;
 		    if ( posKeyframeList.size() > 0 )
 		    {
 			    // 1. Get the keyframe index where the keyframe time is greater than the sample time, save curKeyframeIndex & prevKeyframeIndex
 			    int curKeyframeIndex  = 0;
 			    int prevKeyframeIndex = 0;
 			    for ( int posKeyframeIndex = 0; posKeyframeIndex < posKeyframeList.size(); posKeyframeIndex++ )
 			    {
 				    Vec3Keyframe const& posKeyframe = posKeyframeList[posKeyframeIndex];
 				    if ( posKeyframe.m_keyframeTime >= sampleTime )
 				    {
 					    curKeyframeIndex = posKeyframeIndex;
 					    break;
 				    }
 			    }
 			    if ( curKeyframeIndex > 0 )
 			    {
 				    prevKeyframeIndex = curKeyframeIndex - 1;
 			    }
 			    // 2. Blend between curKeyframe and prevKeyframe to compute sampled joint transform
 			    float curKeyframeTime							 = posKeyframeList[curKeyframeIndex ].m_keyframeTime;
 			    float prevKeyframeTime							 = posKeyframeList[prevKeyframeIndex].m_keyframeTime;
 			    Vec3  curKeyframePos							 = posKeyframeList[curKeyframeIndex ].m_data;
 			    Vec3  prevKeyframePos							 = posKeyframeList[prevKeyframeIndex].m_data;
 			    float fractionWithinRange						 = GetFractionWithinRange( sampleTime, prevKeyframeTime, curKeyframeTime );
 			    Vec3  lerpedPos									 = Interpolate( prevKeyframePos, curKeyframePos, fractionWithinRange );
 			    curAnimJoint.m_localToParentTransform.m_position = lerpedPos;
 		    }
        }

		// Position (old attempt)
// 		int numPosKeyframes = int( curJointAnimCurve.m_positionKeyframeList.size() );
// 		for ( int posKeyframeIndex = 0; posKeyframeIndex < numPosKeyframes; posKeyframeIndex++ )
// 		{
// 			// Update Cur joint's position at elapsed time
// 			Vec3Keyframe const& curJointPosKeyframe = curJointAnimCurve.m_positionKeyframeList[posKeyframeIndex];
// 			if ( sampleTime > curJointPosKeyframe.m_keyframeTime )
// 			{
// 				Vec3Keyframe const& lastKeyframe = curJointPosKeyframe;
// 				if ( posKeyframeIndex == ( numPosKeyframes - 1 ) )
// 				{
// 					// If cur index is the last pose in the curve, no need to blend
// 					curAnimJoint.m_localToParentTransform.m_position = curJointPosKeyframe.m_data;
// 				}
// 				else
// 				{
// 					// If there's more poses after this frame, we need to blend between this frame's pose and next frame's pose
// 					Vec3Keyframe const& nextKeyframe				 = curJointAnimCurve.m_positionKeyframeList[posKeyframeIndex + 1];
// 					float				fractionWithinRange			 = GetFractionWithinRange( sampleTime, lastKeyframe.m_keyframeTime, nextKeyframe.m_keyframeTime );
// 					Vec3				lerpPos						 = Interpolate( curJointPosKeyframe.m_data, nextKeyframe.m_data, fractionWithinRange );
// 					curAnimJoint.m_localToParentTransform.m_position = lerpPos;
// 				}
// 			}
// 		}


		// Rotation
 		std::vector<QuaternionKeyframe> const& rotKeyframeList = curJointAnimCurve.m_rotationKeyframeList;
 		if ( rotKeyframeList.size() > 0 )
 		{
			// 1. Find the index where the keyframe time is greater than the sample time, save curKeyframeIndex & prevKeyframeIndex
 			int curKeyframeIndex  = 0;
 			int prevKeyframeIndex = 0;
 			for ( int rotKeyframeIndex = 0; rotKeyframeIndex < rotKeyframeList.size(); rotKeyframeIndex++ )
 			{
 				QuaternionKeyframe const& rotKeyframe = rotKeyframeList[rotKeyframeIndex];
 				if ( rotKeyframe.m_keyframeTime >= sampleTime )
 				{
 					curKeyframeIndex = rotKeyframeIndex;
 					break;
 				}
 			}
 			if ( curKeyframeIndex > 0 )
 			{
 				prevKeyframeIndex = curKeyframeIndex - 1;
 			}
 			// 2. Blend between 2 keyframes to compute the sampled joint transform
 			float curKeyframeTime								= rotKeyframeList[curKeyframeIndex ].m_keyframeTime;
 			float prevKeyframeTime								= rotKeyframeList[prevKeyframeIndex].m_keyframeTime;
 			Quaternion curKeyframeRotation						= rotKeyframeList[curKeyframeIndex ].m_data;
 			Quaternion prevKeyframeRotation						= rotKeyframeList[prevKeyframeIndex].m_data;
 			Quaternion lerpedRotation							= NormalizedLerp( sampleTime, prevKeyframeTime, curKeyframeTime, prevKeyframeRotation, curKeyframeRotation );
 			curAnimJoint.m_localToParentTransform.m_rotation	= lerpedRotation;
 		}


		// Scale
        std::vector<Vec3Keyframe> const& scaleKeyframeList = curJointAnimCurve.m_scaleKeyframeList;
        if ( scaleKeyframeList.size() > 0 )
        {
            // 1. Get curKeyframeTime greater than sample time, save curKeyframeTime & prevKeyframeTime
            int curKeyframeIndex  = 0;
            int prevKeyframeIndex = 0;
            for ( int scaleKeyframeIndex = 0; scaleKeyframeIndex < scaleKeyframeList.size(); scaleKeyframeIndex++ )
            {
                Vec3Keyframe const& scaleKeyframe = scaleKeyframeList[ scaleKeyframeIndex ];
                float scaleKeyframeTime = scaleKeyframe.m_keyframeTime;
                if ( scaleKeyframeTime >= sampleTime )
                {
                    curKeyframeIndex = scaleKeyframeIndex;
                    break;
                }
            }
            if ( curKeyframeIndex > 0 )
            {
                prevKeyframeIndex = curKeyframeIndex - 1;
            }
            // 2. Blend between curKeyframeScale and prevKeyframeScale
            float  curKeyframeTime                          = scaleKeyframeList[ curKeyframeIndex  ].m_keyframeTime;
            float prevKeyframeTime                          = scaleKeyframeList[ prevKeyframeIndex ].m_keyframeTime;
            Vec3   curKeyframeScale                         = scaleKeyframeList[ curKeyframeIndex  ].m_data;
            Vec3  prevKeyframeScale                         = scaleKeyframeList[ prevKeyframeIndex ].m_data;
            float fractionWithinRange                       = GetFractionWithinRange( sampleTime, prevKeyframeTime, curKeyframeTime );
            Vec3  lerpedScale                               = Interpolate( prevKeyframeScale, curKeyframeScale, fractionWithinRange );
            curAnimJoint.m_localToParentTransform.m_scale   = lerpedScale;
        }

		// Scale (old)
// 		int numScaleKeyframes = int( curJointAnimCurve.m_scaleKeyframeList.size() );
// 		for ( int scaleKeyframeIndex = 0; scaleKeyframeIndex < numScaleKeyframes; scaleKeyframeIndex++ )
// 		{
// 			// Update Cur joint's scale at elapsed time
// 			Vec3Keyframe const& curJointScaleKeyframe = curJointAnimCurve.m_scaleKeyframeList[ scaleKeyframeIndex ];
// 			if ( sampleTime > curJointScaleKeyframe.m_keyframeTime )
// 			{
// 				Vec3Keyframe const& lastKeyframe = curJointScaleKeyframe;
// 				if ( scaleKeyframeIndex == ( numScaleKeyframes - 1 ) )
// 				{
// 					// If cur index is the last pose in the curve, no need to blend
// 					curAnimJoint.m_localToParentTransform.m_scale = curJointScaleKeyframe.m_data;
// 				}
// 				else
// 				{
// 					// If there's more poses after this frame, we need to blend between this frame's pose and next frame's pose
// 					Vec3Keyframe const& nextKeyframe			  = curJointAnimCurve.m_scaleKeyframeList[ scaleKeyframeIndex + 1];
// 					float				fractionWithinRange		  = GetFractionWithinRange( sampleTime, lastKeyframe.m_keyframeTime, nextKeyframe.m_keyframeTime );
// 					Vec3				lerpScale				  = Interpolate( curJointScaleKeyframe.m_data, nextKeyframe.m_data, fractionWithinRange );
// 					curAnimJoint.m_localToParentTransform.m_scale = lerpScale;
// 				}
// 			}
// 		}

		animationPoseAtTime.m_jointList.push_back( curAnimJoint );
	}
	return animationPoseAtTime;
}


//----------------------------------------------------------------------------------------------------------------------
void AnimationClip::ComputeStartEndTime()
{
	m_startTime = 0.0f;
	m_endTime	= 0.0f;

	// Loop through all curves and get the longest end time
	for ( int i = 0; i < m_jointCurveList.size(); i++ )
	{
		JointAnimationCurve const& curJointAnimCurve = m_jointCurveList[i];
		int maxIndex	 = int( curJointAnimCurve.m_positionKeyframeList.size() ) - 1;
		if ( maxIndex > 0 )
		{
			float posKeyTime = curJointAnimCurve.m_positionKeyframeList[ maxIndex ].m_keyframeTime;
			if ( posKeyTime > m_endTime )
			{
				m_endTime = posKeyTime;
			}
		}
		maxIndex		 = int( curJointAnimCurve.m_rotationKeyframeList.size() ) - 1;
		if ( maxIndex > 0 )
		{
			float rotKeyTime = curJointAnimCurve.m_rotationKeyframeList[ maxIndex ].m_keyframeTime;
			if ( rotKeyTime > m_endTime )
			{
				m_endTime = rotKeyTime;
			}
		}
		maxIndex		   = int( curJointAnimCurve.m_scaleKeyframeList.size() ) - 1;
		if ( maxIndex > 0 )
		{
			float scaleKeyTime = curJointAnimCurve.m_scaleKeyframeList[ maxIndex ].m_keyframeTime;
			if ( scaleKeyTime > m_endTime )
			{
				m_endTime = scaleKeyTime;
			}
		}
	}
}


//----------------------------------------------------------------------------------------------------------------------
bool AnimationClip::IsFinished()
{
    return m_isFinishedPlaying;
}


//----------------------------------------------------------------------------------------------------------------------
void AnimationClip::ResetLocalTime()
{
    m_localTime = 0.0f;
}


//----------------------------------------------------------------------------------------------------------------------
void AnimationClip::SetName( std::string const& clipName )
{
    m_clipName = clipName;
}


//----------------------------------------------------------------------------------------------------------------------
std::string AnimationClip::GetName() const
{
    return m_clipName;
}
