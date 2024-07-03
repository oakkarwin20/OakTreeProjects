 #pragma once

#include <string>
#include <vector>


//----------------------------------------------------------------------------------------------------------------------
class AnimationPose;
class AnimationClip;


//----------------------------------------------------------------------------------------------------------------------
class FbxFileImporter
{
public:
	static void LoadAnimationPose( std::string const& fileName, AnimationPose& newAnimationPose, std::string const& poseName  );
	static void LoadCurveForAllJointsAtElapsedTime( std::string const& fileName, AnimationPose& newAnimationPose, AnimationPose const& bindAnimationPose, float elapsedAnimationTime );
	static void LoadAnimationClip( std::string const& fileName, AnimationClip& animationClip, AnimationPose const& bindAnimationPose, std::string const& clipName );		// One clip is one animation layer?
};