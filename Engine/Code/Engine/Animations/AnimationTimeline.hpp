 #pragma once

#include "Engine/Animations/AnimationPose.hpp"
#include "Engine/Math/AABB2.hpp"
#include "Engine/Core/Rgba8.hpp"

#include <vector>


//----------------------------------------------------------------------------------------------------------------------
class  AnimationClip;
class  BitmapFont;
struct Vec2;
struct AABB2;
struct Rgba8;
struct Vertex_PCU;


//----------------------------------------------------------------------------------------------------------------------
// Sample a pose at time set by caret
// Render pose
class AnimationTimeline
{
public:
	AnimationTimeline( AABB2 const& bounds );
	~AnimationTimeline();

	void RenderAnimPose( std::vector<Vertex_PCU>& verts ) const;
	void RenderUI( BitmapFont* bitmapFont, std::vector<Vertex_PCU>& textVerts, std::vector<Vertex_PCU>& verts, Rgba8 const& color, float caretThickness, Rgba8 const& caretColor ) const;

	// Update functions
	void UpdateCaretPos( Vec2 const& cursorPosNormalized, float SCREEN_SIZE_X );
	void SetAnimClipAndBindPose( AnimationClip* newAnimClip, AnimationPose& newBindPose );
	void UpdateSampledPose( int numSlices = 48 );
	void UpdateKeyframePositions( int numSlices, float keyframeRadius = 1.0f );
	void UpdateInfoBounds();
	void ClampInfoBounds();
	void ToggleVisibility();
	bool IsVisible() const;
	void InitAnimClipList( std::vector<AnimationClip>& animClipList );
	void InitAnimPoseList( std::vector<AnimationPose>& animPoseList );
	void IncrementAnimClipIndex();
	void DecrementAnimClipIndex();
	// Setters
	void SelectCaret();
	void DeselectCaret();

public:
	AnimationPose	m_animPoseToRender;
	AnimationPose	m_bindPoseToRender;
	AnimationClip*	m_animClipToPlay			= nullptr;
	AABB2			m_bounds					= AABB2::ZERO_TO_ONE;
	int				m_jointID					= 0;		// This can be changed at runtime to inspect different joints
	float			m_textHeight				= 2.0f;
//	AABB2			m_infoBounds_animClipName	= AABB2( Vec2( m_bounds.m_mins.x, m_bounds.m_maxs.y * 1.3f ), Vec2( m_bounds.m_maxs.x * 0.15f, m_bounds.m_maxs.y * 1.5f ) ); 
	AABB2			m_infoBounds_animClipName	= AABB2( Vec2( m_bounds.m_mins.x, m_bounds.m_maxs.y ), Vec2( m_bounds.m_maxs.x * 0.3f, m_bounds.m_maxs.y * 0.3f ) ); 
	AABB2			m_infoBounds_caretTime		= AABB2( Vec2( m_bounds.m_mins.x, m_bounds.m_mins.y ), Vec2( m_bounds.m_maxs.x * 0.3f, m_bounds.m_maxs.y * 0.3f ) ); 
	AABB2			m_infoBounds_translation	= AABB2( Vec2( m_bounds.m_mins.x, m_bounds.m_mins.y ), Vec2( m_bounds.m_maxs.x * 0.3f, m_bounds.m_maxs.y * 0.3f ) ); 
	AABB2			m_infoBounds_rotation		= AABB2( Vec2( m_bounds.m_mins.x, m_bounds.m_mins.y ), Vec2( m_bounds.m_maxs.x * 0.3f, m_bounds.m_maxs.y * 0.3f ) ); 
	AABB2			m_infoBounds_scale			= AABB2( Vec2( m_bounds.m_mins.x, m_bounds.m_mins.y ), Vec2( m_bounds.m_maxs.x * 0.3f, m_bounds.m_maxs.y * 0.3f ) ); 
	AABB2			m_infoBounds_jointName		= AABB2( Vec2( m_bounds.m_mins.x, m_bounds.m_mins.y ), Vec2( m_bounds.m_maxs.x * 0.3f, m_bounds.m_maxs.y * 0.3f ) ); 
	float			m_infoBoundsMaxX			= m_bounds.m_maxs.x * 1.0f; // 0.45f;
	float			m_infoBoundsMaxY			= m_bounds.m_maxs.y * 0.1f;

	std::vector<Vertex_PCU>		m_keyframeVerts;
	std::vector<AnimationClip*> m_animClipList;
	std::vector<AnimationPose*> m_animPoseList;
	int m_curAnimClipIndex = 0;

private:
	bool  m_isVisible		= false;
	Vec2  m_caretPos		= Vec2::ZERO;			// Top of the caret on the timeline
	bool  m_isCaretSelected	= false;
	float m_caretTime		= 0.0f;					// Caret time can also prolly be thought of as 'sampleTime'

	// Keyframe disc positions
	float m_translationHeightScalar = 0.8f;
	float m_rotationHeightScalar	= 0.5f;
	float m_scaleHeightScalar		= 0.2f;
};
