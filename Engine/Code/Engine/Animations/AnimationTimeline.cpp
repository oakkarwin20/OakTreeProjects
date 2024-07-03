#include "Engine/Animations/AnimationTimeline.hpp"
#include "Engine/Animations/AnimationClip.hpp"
#include "Engine/Core/VertexUtils.hpp"
#include "Engine/Core/Vertex_PCU.hpp"
#include "Engine/Core/StringUtils.hpp"
#include "Engine/Math/AABB2.hpp"
#include "Engine/Renderer/Renderer.hpp"
#include "Engine/Math/MathUtils.hpp"
#include "Engine/Renderer/BitmapFont.hpp"


//----------------------------------------------------------------------------------------------------------------------
AnimationTimeline::AnimationTimeline( AABB2 const& bounds )
	: m_bounds( bounds )
{
	m_caretPos = Vec2( 0.0f, m_bounds.m_maxs.y );
	
// 	Vec2 newMins;
// 	float offset				= 10.0f;
// 	TransformPosition2D( newMins, Vec2::X_FWD, Vec2::Y_LEFT, Vec2(m_bounds.m_mins.x, m_bounds.m_maxs.y + offset) );
// 	Vec2 newMaxs				= Vec2( newMins.x + m_infoBoundsMaxX, newMins.y + m_infoBoundsMaxY );
// 	m_infoBounds_animClipName	= AABB2( newMins, newMaxs );

	UpdateInfoBounds();
}


//----------------------------------------------------------------------------------------------------------------------
AnimationTimeline::~AnimationTimeline()
{
}


//----------------------------------------------------------------------------------------------------------------------
void AnimationTimeline::RenderAnimPose( std::vector<Vertex_PCU>& verts ) const
{
	if ( m_isVisible )
	{
		m_animPoseToRender.AddVertsForDebugJointsAndBones( verts );
	}
}


//----------------------------------------------------------------------------------------------------------------------
void AnimationTimeline::RenderUI( BitmapFont* bitmapFont, std::vector<Vertex_PCU>& textVerts, std::vector<Vertex_PCU>& verts, Rgba8 const& boundsColor, float caretThickness, Rgba8 const& caretColor ) const
{
	if ( m_isVisible )
	{
		AddVertsForAABB2D( verts, m_bounds, boundsColor );
		if ( m_isCaretSelected )
		{
			Rgba8 selectedCaretColor = caretColor * 0.7f;
			AddVertsForLineSegment2D( verts, m_caretPos, Vec2( m_caretPos.x, 0.0f ), caretThickness, selectedCaretColor );
		}
		else
		{
			AddVertsForLineSegment2D( verts, m_caretPos, Vec2( m_caretPos.x, 0.0f ), caretThickness, caretColor );
		}

		// Loop through all jointCurves in animClip
		// Get keyframes at time
		// rangemap keyFrameTime to bounds.X
		// AddVertsForDisc at rangeMappedPos
// 		float animClipStartTime = m_animClip->m_startTime;
// 		float animClipEndTime	= m_animClip->m_endTime;
// 		float boundsMinX		= m_bounds.m_mins.x;
// 		float boundsMaxX		= m_bounds.m_maxs.x;
// 		float boundsMaxY		= m_bounds.m_maxs.y;
// 		for ( int i = 0; i < m_animClip->m_jointCurveList.size(); i++ )
// 		{
// 			JointAnimationCurve	const& curJointCurve	 = m_animClip->m_jointCurveList[i]; 
// 
// 			// Get debugPosition to render for translation keyframes on the timeline, rangemapped within the timeline bounds
// 			std::vector<Vec3Keyframe> const& translationKeyframeList = curJointCurve.m_positionKeyframeList;
// 			for ( int posKeyIndex = 0; posKeyIndex < translationKeyframeList.size(); posKeyIndex++ )
// 			{
// 				Vec3Keyframe const& curTranslationKey	= translationKeyframeList[posKeyIndex];
// 				float				translationKeyPosX	= RangeMapClamped( curTranslationKey.m_keyframeTime, animClipStartTime, animClipEndTime, boundsMinX, boundsMaxX );
// 				Vec2				translationKeyPos	= Vec2( translationKeyPosX, boundsMaxY * m_translationHeightScalar );			// Render at high 0.5% of the bounds
// 				AddVertsForDisc2D( verts, translationKeyPos, keyframeRadius, translationKeyframeColor );
// 			}
// 			// Get debugPosition to render for rotation keyframes on the timeline, rangemapped within the timeline bounds
// 			std::vector<QuaternionKeyframe> const& rotKeyframeList = curJointCurve.m_rotationKeyframeList;
// 			for ( int rotKeyIndex = 0; rotKeyIndex < rotKeyframeList.size(); rotKeyIndex++ )
// 			{
// 				QuaternionKeyframe const& curRotKey		 = rotKeyframeList[rotKeyIndex];
// 				float					  rotKeyPosX	 = RangeMapClamped( curRotKey.m_keyframeTime, animClipStartTime, animClipEndTime, boundsMinX, boundsMaxX );
// 				Vec2					  rotKeyDiscPos	 = Vec2( rotKeyPosX, boundsMaxY * m_rotationHeightScalar );			// Render at high 0.5% of the bounds
// 				AddVertsForDisc2D( verts, rotKeyDiscPos, keyframeRadius, rotationKeyframeColor );
// 			}
// 			// Get debugPosition to render for scale keyframes on the timeline, rangemapped within the timeline bounds
// 			std::vector<Vec3Keyframe> const& scaleKeyframeList = curJointCurve.m_scaleKeyframeList;
// 			for ( int scaleKeyIndex = 0; scaleKeyIndex < scaleKeyframeList.size(); scaleKeyIndex++ )
// 			{
// 				Vec3Keyframe const& curScaleKey		= scaleKeyframeList[scaleKeyIndex];
// 				float				scaleKeyPosX	= RangeMapClamped( curScaleKey.m_keyframeTime, animClipStartTime, animClipEndTime, boundsMinX, boundsMaxX );
// 				Vec2				scaleKeyPos		= Vec2( scaleKeyPosX, boundsMaxY * m_scaleHeightScalar );			// Render at high 0.3% of the bounds
// 				AddVertsForDisc2D( verts, scaleKeyPos, keyframeRadius, scaleKeyframeColor );
// 			}
// 		} 
		
		for ( int i = 0; i < m_keyframeVerts.size(); i++ )
		{
			verts.push_back( m_keyframeVerts[i] );
		}

		//----------------------------------------------------------------------------------------------------------------------
		// Render extra info
		// animClip name and hoveredKeyframe time
		//----------------------------------------------------------------------------------------------------------------------
		// AnimClipName background
		AddVertsForAABB2D( verts, m_infoBounds_animClipName, Rgba8::LIGHTBLUE );
		// CaretTime background
		AddVertsForAABB2D( verts, m_infoBounds_caretTime, Rgba8::DARK_BLUE );		
		// Translation info background
		AddVertsForAABB2D( verts, m_infoBounds_translation, Rgba8::DARK_BLUE );		
		// Rotation info background
		AddVertsForAABB2D( verts, m_infoBounds_rotation, Rgba8::DARK_BLUE );		
		// Scale info background
		AddVertsForAABB2D( verts, m_infoBounds_scale, Rgba8::DARK_BLUE );		

		// Animation Clip Name
		std::string animClipName		= "ClipName: " + m_animClipToPlay->GetName();
		Vec2		alignment			= Vec2( 0.0f, 0.0f );
		bitmapFont->AddVertsForTextInBox2D( textVerts, m_infoBounds_animClipName, m_textHeight, animClipName, Rgba8::WHITE, 1.0f, alignment );
		// Joint Name
		AddVertsForAABB2D( verts, m_infoBounds_jointName, Rgba8::DARK_BLUE );		
		std::string jointNameString		= Stringf( "Joint Name: %s", m_animPoseToRender.m_jointList[ m_jointID ].m_jointName.c_str() );
		bitmapFont->AddVertsForTextInBox2D( textVerts, m_infoBounds_jointName, m_textHeight, jointNameString, Rgba8::WHITE, 1.0f, alignment );	
		// CaretTime info
		std::string caretTimeString		= Stringf( "Anim local time: %0.2f", m_caretTime );
		bitmapFont->AddVertsForTextInBox2D( textVerts, m_infoBounds_caretTime, m_textHeight, caretTimeString, Rgba8::WHITE, 1.0f, alignment );	
		// Translation info
		Transform	jointTransform		= m_animPoseToRender.LocalToModel_Transform( m_jointID );
		Vec3 const& jointPos			= jointTransform.m_position;
		std::string translationString	= Stringf( "Joint Translation: X:%0.2f, Y:%0.2f, Z:%0.2f", jointPos.x, jointPos.y, jointPos.z );
		bitmapFont->AddVertsForTextInBox2D( textVerts, m_infoBounds_translation, m_textHeight, translationString, Rgba8::WHITE, 1.0f, alignment );	
		// Rotation info
		Quaternion const& jointRot		= jointTransform.m_rotation;
		std::string rotationString		= Stringf( "Joint Rotation: X:%0.2f, Y:%0.2f, Z:%0.2f, W:%0.2f", jointRot.x, jointRot.y, jointRot.z, jointRot.w );
		bitmapFont->AddVertsForTextInBox2D( textVerts, m_infoBounds_rotation, m_textHeight, rotationString, Rgba8::WHITE, 1.0f, alignment );	
		// Scale info
		Vec3 const& jointScale			= jointTransform.m_scale;
		std::string scaleString			= Stringf( "Joint Scale: X:%0.2f, Y:%0.2f, Z:%0.2f", jointScale.x, jointScale.y, jointScale.z );
		bitmapFont->AddVertsForTextInBox2D( textVerts, m_infoBounds_scale, m_textHeight, scaleString, Rgba8::WHITE, 1.0f, alignment );	
	}
}


//----------------------------------------------------------------------------------------------------------------------
void AnimationTimeline::UpdateCaretPos( Vec2 const& cursorPosNormalized, float SCREEN_SIZE_X )
{
	// New approach to fix incorrect caret offset
	float cusorPosXScreenSpace	= cursorPosNormalized.x * SCREEN_SIZE_X;
 	Vec2  cursorPosUiSpace		= Vec2( cusorPosXScreenSpace, m_bounds.m_maxs.y );
 	m_caretPos					= cursorPosUiSpace;
 	m_caretPos.x				= GetClamped( m_caretPos.x, m_bounds.m_mins.x, m_bounds.m_maxs.x );
 	m_caretTime					= m_caretPos.x;
 
 	// Hack to get anim local  time
 	float fractionWithinRange = GetFractionWithinRange( m_caretTime, m_bounds.m_mins.x, m_bounds.m_maxs.x );
 	float range = m_animClipToPlay->m_endTime - m_animClipToPlay->m_startTime;
 	m_caretTime = m_animClipToPlay->m_startTime + ( range * fractionWithinRange );


	// Old approach (wrong, incorrect offset with caretPosX)
// 	Vec2 cursorPosUiSpace	  = Vec2( cursorPosNormalized.x * m_bounds.m_maxs.x, m_bounds.m_maxs.y );
// 	m_caretPos				  = cursorPosUiSpace;
// 	m_caretPos.x			  = GetClamped( m_caretPos.x, m_bounds.m_mins.x, m_bounds.m_maxs.x );
// 	m_caretTime				  = m_caretPos.x;
// 
// 	// Hack to get anim local  time
// 	float fractionWithinRange = GetFractionWithinRange( m_caretTime, m_bounds.m_mins.x, m_bounds.m_maxs.x );
// 	float range = m_animClip->m_endTime - m_animClip->m_startTime;
// 	m_caretTime = m_animClip->m_startTime + ( range * fractionWithinRange );

	SelectCaret();
}


//----------------------------------------------------------------------------------------------------------------------
void AnimationTimeline::SetAnimClipAndBindPose( AnimationClip* newAnimClip, AnimationPose& newBindPose )
{
	m_animClipToPlay	= newAnimClip;
	m_bindPoseToRender	= newBindPose;
}


//----------------------------------------------------------------------------------------------------------------------
void AnimationTimeline::UpdateSampledPose( int numSlices )
{
	float sampleTime	= RangeMapClamped( m_caretPos.x, m_bounds.m_mins.x, m_bounds.m_maxs.x, m_animClipToPlay->m_startTime, m_animClipToPlay->m_endTime );
	m_animPoseToRender	= m_animClipToPlay->SampleAnimPoseAtTime( sampleTime, m_bindPoseToRender );
	UpdateKeyframePositions( numSlices );
}


//----------------------------------------------------------------------------------------------------------------------
void AnimationTimeline::UpdateKeyframePositions( int numSlices, float keyframeRadius )
{
	m_keyframeVerts.clear();
//	m_keyframeVerts.reserve();		// #Todo: Reserve based on size of animation

	Rgba8 const& translationKeyframeColor	= Rgba8::RED;
	Rgba8 const& rotationKeyframeColor		= Rgba8::GREEN; 
	Rgba8 const& scaleKeyframeColor			= Rgba8::BLUE;

	// Loop through all jointCurves in animClip
	// Get keyframes at time
	// rangemap keyFrameTime to bounds.X
	// AddVertsForDisc at rangeMappedPos
	float animClipStartTime = m_animClipToPlay->m_startTime;
	float animClipEndTime	= m_animClipToPlay->m_endTime;
	float boundsMinX		= m_bounds.m_mins.x;
	float boundsMaxX		= m_bounds.m_maxs.x;
	float boundsMaxY		= m_bounds.m_maxs.y;
	for ( int i = 0; i < m_animClipToPlay->m_jointCurveList.size(); i++ )
	{
		JointAnimationCurve	const& curJointCurve	 = m_animClipToPlay->m_jointCurveList[i]; 

		// Get debugPosition to render for translation keyframes on the timeline, rangemapped within the timeline bounds
		std::vector<Vec3Keyframe> const& translationKeyframeList = curJointCurve.m_positionKeyframeList;
		for ( int posKeyIndex = 0; posKeyIndex < translationKeyframeList.size(); posKeyIndex++ )
		{
			Vec3Keyframe const& curTranslationKey	= translationKeyframeList[posKeyIndex];
			float				translationKeyPosX	= RangeMapClamped( curTranslationKey.m_keyframeTime, animClipStartTime, animClipEndTime, boundsMinX, boundsMaxX );
			Vec2				translationKeyPos	= Vec2( translationKeyPosX, boundsMaxY * m_translationHeightScalar );			// Render at high 0.5% of the bounds
			AddVertsForDisc2D( m_keyframeVerts, translationKeyPos, keyframeRadius, translationKeyframeColor, numSlices );
		}
		// Get debugPosition to render for rotation keyframes on the timeline, rangemapped within the timeline bounds
		std::vector<QuaternionKeyframe> const& rotKeyframeList = curJointCurve.m_rotationKeyframeList;
		for ( int rotKeyIndex = 0; rotKeyIndex < rotKeyframeList.size(); rotKeyIndex++ )
		{
			QuaternionKeyframe const& curRotKey		 = rotKeyframeList[rotKeyIndex];
			float					  rotKeyPosX	 = RangeMapClamped( curRotKey.m_keyframeTime, animClipStartTime, animClipEndTime, boundsMinX, boundsMaxX );
			Vec2					  rotKeyDiscPos	 = Vec2( rotKeyPosX, boundsMaxY * m_rotationHeightScalar );			// Render at high 0.5% of the bounds
			AddVertsForDisc2D( m_keyframeVerts, rotKeyDiscPos, keyframeRadius, rotationKeyframeColor, numSlices );
		}
		// Get debugPosition to render for scale keyframes on the timeline, rangemapped within the timeline bounds
		std::vector<Vec3Keyframe> const& scaleKeyframeList = curJointCurve.m_scaleKeyframeList;
		for ( int scaleKeyIndex = 0; scaleKeyIndex < scaleKeyframeList.size(); scaleKeyIndex++ )
		{
			Vec3Keyframe const& curScaleKey		= scaleKeyframeList[scaleKeyIndex];
			float				scaleKeyPosX	= RangeMapClamped( curScaleKey.m_keyframeTime, animClipStartTime, animClipEndTime, boundsMinX, boundsMaxX );
			Vec2				scaleKeyPos		= Vec2( scaleKeyPosX, boundsMaxY * m_scaleHeightScalar );			// Render at high 0.3% of the bounds
			AddVertsForDisc2D( m_keyframeVerts, scaleKeyPos, keyframeRadius, scaleKeyframeColor, numSlices );
		}
	} 
}


//----------------------------------------------------------------------------------------------------------------------
void AnimationTimeline::UpdateInfoBounds()
{
	//----------------------------------------------------------------------------------------------------------------------
	// New version, keep bounds static
	Vec2 newMins;
	float offset				= 10.0f;		// #ToDo: Make 'offset' a parameter instead of a hard coded number
	TransformPosition2D( newMins, Vec2::X_FWD, Vec2::Y_LEFT, Vec2(m_bounds.m_mins.x, m_bounds.m_maxs.y + offset) );
	Vec2 newMaxs				= Vec2 ( m_infoBoundsMaxX, newMins.y + m_infoBoundsMaxY );
	m_infoBounds_animClipName	= AABB2( newMins, newMaxs );

	// Transform position for (caret) local time info Bounds 
	newMins						= Vec2 ( newMins.x, newMins.y - m_textHeight );
	newMaxs						= Vec2 ( m_infoBoundsMaxX, newMins.y + m_infoBoundsMaxY );
	m_infoBounds_caretTime		= AABB2( newMins, newMaxs );

	// Transform position for joint name info bounds
	newMins						= Vec2 ( newMins.x, newMins.y - m_textHeight);
	newMaxs						= Vec2 ( m_infoBoundsMaxX, newMins.y + m_infoBoundsMaxY );
	m_infoBounds_jointName		= AABB2( newMins, newMaxs );

	// Transform position for joint translation info bounds
	newMins						= Vec2 ( newMins.x, newMins.y - m_textHeight );
	newMaxs						= Vec2 ( m_infoBoundsMaxX, newMins.y + m_infoBoundsMaxY );
	m_infoBounds_translation	= AABB2( newMins, newMaxs );

	// Transform position for joint translation info bounds
	newMins						= Vec2 ( newMins.x, newMins.y - m_textHeight );
	newMaxs						= Vec2 ( m_infoBoundsMaxX, newMins.y + m_infoBoundsMaxY );
	m_infoBounds_rotation		= AABB2( newMins, newMaxs );

	// Transform position for joint translation info bounds
	newMins						= Vec2 ( newMins.x, newMins.y - m_textHeight );
	newMaxs						= Vec2 ( m_infoBoundsMaxX, newMins.y + m_infoBoundsMaxY );
	m_infoBounds_scale			= AABB2( newMins, newMaxs );


	//----------------------------------------------------------------------------------------------------------------------
	// Old version, update bounds relative to caretPos
	//----------------------------------------------------------------------------------------------------------------------
	// Update CareInfoBounds
// 	Vec2 newMins;
// 	TransformPosition2D( newMins, Vec2::X_FWD, Vec2::Y_LEFT, m_caretPos );
// 	Vec2 newMaxs			= Vec2( newMins.x + m_infoBoundsMaxX, newMins.y + m_infoBoundsMaxY );
// 	m_infoBounds_caretTime	= AABB2( newMins, newMaxs );
// 
// 	// Transform position for joint translation info bounds
// 	newMins						= Vec2( newMins.x, newMins.y + m_textHeight );
// 	newMaxs						= Vec2( newMins.x + m_infoBoundsMaxX, newMins.y + m_infoBoundsMaxY );
// 	m_infoBounds_translation	= AABB2( newMins, newMaxs );
// 
// 	// Transform position for joint name info bounds
// 	newMins					= Vec2( newMins.x, newMaxs.y );
// 	newMaxs					= Vec2( newMins.x + m_infoBoundsMaxX, newMins.y + m_infoBoundsMaxY );
// 	m_infoBounds_jointName	= AABB2( newMins, newMaxs );
// 
// 	ClampInfoBounds();
}


//----------------------------------------------------------------------------------------------------------------------
void AnimationTimeline::ClampInfoBounds()
{
	// Clamp m_infoBounds_caretTime maxes
	float width						= m_infoBounds_caretTime.m_maxs.x - m_infoBounds_caretTime.m_mins.x;
	m_infoBounds_caretTime.m_maxs.x = GetClamped( m_infoBounds_caretTime.m_maxs.x, 0.0f, m_infoBounds_caretTime.m_maxs.x );
	m_infoBounds_caretTime.m_mins.x = m_infoBounds_caretTime.m_maxs.x - width;

	// Clamp m_infoBounds_jointName maxes
	width							= m_infoBounds_jointName.m_maxs.x - m_infoBounds_jointName.m_mins.x;
	m_infoBounds_jointName.m_maxs.x	= GetClamped( m_infoBounds_jointName.m_maxs.x, 0.0f, m_infoBounds_jointName.m_maxs.x );
	m_infoBounds_jointName.m_mins.x	= m_infoBounds_jointName.m_maxs.x - width;

	// Clamp m_infoBounds_translation maxes
	width								= m_infoBounds_translation.m_maxs.x - m_infoBounds_translation.m_mins.x;
	m_infoBounds_translation.m_maxs.x	= GetClamped( m_infoBounds_translation.m_maxs.x, 0.0f, m_infoBounds_translation.m_maxs.x );
	m_infoBounds_translation.m_mins.x	= m_infoBounds_translation.m_maxs.x - width;

	// Clamp m_infoBounds_rotation maxes
	width								= m_infoBounds_rotation.m_maxs.x - m_infoBounds_rotation.m_mins.x;
	m_infoBounds_rotation.m_maxs.x		= GetClamped( m_infoBounds_rotation.m_maxs.x, 0.0f, m_infoBounds_rotation.m_maxs.x );
	m_infoBounds_rotation.m_mins.x		= m_infoBounds_rotation.m_maxs.x - width;

	// Clamp m_infoBounds_scale maxes
	width								= m_infoBounds_scale.m_maxs.x - m_infoBounds_scale.m_mins.x;
	m_infoBounds_scale.m_maxs.x			= GetClamped( m_infoBounds_scale.m_maxs.x, 0.0f, m_infoBounds_scale.m_maxs.x );
	m_infoBounds_scale.m_mins.x			= m_infoBounds_scale.m_maxs.x - width;
}


//----------------------------------------------------------------------------------------------------------------------
void AnimationTimeline::ToggleVisibility()
{
	m_isVisible = !m_isVisible;
}


//----------------------------------------------------------------------------------------------------------------------
bool AnimationTimeline::IsVisible() const
{
	return m_isVisible;
}


//----------------------------------------------------------------------------------------------------------------------
// Init pointers to animClips in memory to avoid duplicating data
void AnimationTimeline::InitAnimClipList( std::vector<AnimationClip>& animClipList )
{
	int sizeOfAnimClipList = int( animClipList.size() );
	m_animClipList.reserve( sizeOfAnimClipList );
	for ( int i = 0; i < sizeOfAnimClipList; i++ )
	{
		AnimationClip& curAnimClip = animClipList[ i ];
		m_animClipList.push_back( &curAnimClip );
	}
}


//----------------------------------------------------------------------------------------------------------------------
void AnimationTimeline::InitAnimPoseList( std::vector<AnimationPose>& animPoseList )
{
	int sizeOfAnimPoseList = int( animPoseList.size() );
	m_animPoseList.reserve( sizeOfAnimPoseList );
	for ( int i = 0; i < sizeOfAnimPoseList; i++ )
	{
		AnimationPose& curAnimPose = animPoseList[i];
		m_animPoseList.push_back( &curAnimPose );
	}
}


//----------------------------------------------------------------------------------------------------------------------
void AnimationTimeline::IncrementAnimClipIndex()
{
	m_curAnimClipIndex++;
	// "Out of bounds" check
	if ( m_curAnimClipIndex >= m_animClipList.size() )
	{
		m_curAnimClipIndex = 0;
	}
	SetAnimClipAndBindPose( m_animClipList[ m_curAnimClipIndex ], *m_animPoseList[ m_curAnimClipIndex ] );
}

//----------------------------------------------------------------------------------------------------------------------
void AnimationTimeline::DecrementAnimClipIndex()
{
	m_curAnimClipIndex--;
	// "Out of bounds" check
	if ( m_curAnimClipIndex < 0 )
	{
		m_curAnimClipIndex = ( int(m_animClipList.size()) - 1);
	}
	SetAnimClipAndBindPose( m_animClipList[ m_curAnimClipIndex ], *m_animPoseList[ m_curAnimClipIndex ] );
}

//----------------------------------------------------------------------------------------------------------------------
void AnimationTimeline::SelectCaret()
{
	m_isCaretSelected = true;
}


//----------------------------------------------------------------------------------------------------------------------
void AnimationTimeline::DeselectCaret()
{
	m_isCaretSelected = false;
}