#pragma once

#include "Engine/Math/Vec4.hpp"

struct Vec3;
struct Mat44;


//----------------------------------------------------------------------------------------------------------------------
struct Quaternion
{
	float x = 0.0f;
	float y = 0.0f;
	float z = 0.0f;
	float w = 1.0f;

	Quaternion();	// Identity
	Quaternion( float x, float y, float z, float w );


	// Append rotation 
	Quaternion const operator*( Quaternion const& rotationToAppend  ) const;
	Vec3 const		 operator*( Vec3 const& vectorToRotate )		  const;
	Quaternion const operator*( float scalar )						  const;
	Quaternion const operator+( Quaternion const& quatToAdd			) const;
	Quaternion const operator-( Quaternion const& quatToSubtract	) const;

	Vec4 GetAsVec4();
	void Normalize();
	static Quaternion GetAsQuaternion( Mat44 const& matrix );
	static Mat44	  GetAsMatrix( Vec3 const& position, Quaternion const& quat );
};
