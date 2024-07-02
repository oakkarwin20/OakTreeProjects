#include "Engine/Math/Quaternion.hpp"
#include "Engine/Math/Vec3.hpp"
#include "Engine/Math/Mat44.hpp"
#include "Engine/Math/MathUtils.hpp"


//----------------------------------------------------------------------------------------------------------------------
Quaternion::Quaternion()
{
}


//----------------------------------------------------------------------------------------------------------------------
Quaternion::Quaternion( float initialX, float initialY, float initialZ, float initialW )
	: x ( initialX ), y ( initialY ), z ( initialZ ), w ( initialW )
{
}


//----------------------------------------------------------------------------------------------------------------------
Quaternion const Quaternion::operator*( Quaternion const& rotationToAppend ) const
{
	float realPartOfThisQuat		= w;
	Vec3  imaginaryPartofThisQuat   = Vec3( x, y, z );

	float realPartOfRotor		= rotationToAppend.w;
	Vec3 imaginaryPartofRotor	= Vec3( rotationToAppend.x, rotationToAppend.y, rotationToAppend.z );

	float w1w2		= realPartOfThisQuat * realPartOfRotor;
	float v1DotV2	= DotProduct3D( imaginaryPartofThisQuat, imaginaryPartofRotor );
	Vec3  w1v2		= realPartOfThisQuat * imaginaryPartofRotor;
	Vec3  w2v1		= realPartOfRotor * imaginaryPartofThisQuat;
	Vec3  v1CrossV2 = CrossProduct3D( imaginaryPartofThisQuat, imaginaryPartofRotor );

	float scalar	= w1w2 - v1DotV2;
	Vec3  vector	= w1v2 + w2v1 + v1CrossV2;

	Quaternion result;
	result.x = vector.x;
	result.y = vector.y;
	result.z = vector.z;
	result.w = scalar;

	return result;
}


//----------------------------------------------------------------------------------------------------------------------
Vec3 const Quaternion::operator*( Vec3 const& vectorToRotate ) const
{
	Vec3 vectorPartOfQuat = Vec3( x, y, z );
	Vec3 rotatedResult;
	rotatedResult = ( 2.0f * DotProduct3D( vectorPartOfQuat, vectorToRotate ) * vectorPartOfQuat ) + 
					( ( w * w - DotProduct3D( vectorPartOfQuat, vectorPartOfQuat ) ) * vectorToRotate ) + 
					( 2.0f * w * CrossProduct3D( vectorPartOfQuat, vectorToRotate ) );
	return rotatedResult;
}


//----------------------------------------------------------------------------------------------------------------------
Quaternion const Quaternion::operator*( float scalar ) const
{
	Quaternion scaledQuad;
	scaledQuad.x = x * scalar;
	scaledQuad.y = y * scalar;
	scaledQuad.z = z * scalar;
	scaledQuad.w = w * scalar;
	return scaledQuad;
}


//----------------------------------------------------------------------------------------------------------------------
Quaternion const Quaternion::operator+( Quaternion const& quatToAdd ) const
{
	Quaternion newQuat;
	newQuat.x = x + quatToAdd.x;
	newQuat.y = y + quatToAdd.y;
	newQuat.z = z + quatToAdd.z;
	newQuat.w = w + quatToAdd.w;
	return newQuat;
}
	

//----------------------------------------------------------------------------------------------------------------------
Quaternion const Quaternion::operator-( Quaternion const& quatToSubtract ) const
{
	Quaternion newQuat;
	newQuat.x = x - quatToSubtract.x;
	newQuat.y = y - quatToSubtract.y;
	newQuat.z = z - quatToSubtract.z;
	newQuat.w = w - quatToSubtract.w;
	return newQuat;
}


//----------------------------------------------------------------------------------------------------------------------
Vec4 Quaternion::GetAsVec4()
{
	Vec4 quatAsVec4 = Vec4( x, y, z, w );
	return quatAsVec4;
}


//----------------------------------------------------------------------------------------------------------------------
void Quaternion::Normalize()
{
    float lengthSquared = (x * x) + (y * y) + (z * z) + (w * w);
    float length		= sqrtf( lengthSquared );
    //	if ( CompareIfFloatsAreEqual( length, 0.0f, 0.001f ) )
    //	{
    //		return Quaternion();
    //	}
    float normalize = 1 / length;
    x = x * normalize;
    y = y * normalize;
    z = z * normalize;
    w = w * normalize;
}


//----------------------------------------------------------------------------------------------------------------------
// Test code from Chat GPT
//----------------------------------------------------------------------------------------------------------------------
Quaternion Quaternion::GetAsQuaternion( Mat44 const& matrix )
{
	// Input matrix:
	float m11 = matrix.m_values[ Mat44::Ix ], m12 = matrix.m_values[ Mat44::Jx ], m13 = matrix.m_values[ Mat44::Kx ];
	float m21 = matrix.m_values[ Mat44::Iy ], m22 = matrix.m_values[ Mat44::Jy ], m23 = matrix.m_values[ Mat44::Ky ];
	float m31 = matrix.m_values[ Mat44::Iz ], m32 = matrix.m_values[ Mat44::Jz ], m33 = matrix.m_values[ Mat44::Kz ];

	// Output quaternion
	Quaternion result;

	// Determine which of w, x, y, or z has the largest absolute value
	float fourWSquaredMinus1	   = m11 + m22 + m33;
	float fourXSquaredMinus1	   = m11 - m22 - m33;
	float fourYSquaredMinus1	   = m22 - m11 - m33;
	float fourZSquaredMinus1	   = m33 - m11 - m22;

	int	  biggestIndex			   = 0;
	float fourBiggestSquaredMinus1 = fourWSquaredMinus1;
	if ( fourXSquaredMinus1 > fourBiggestSquaredMinus1 )
	{
		fourBiggestSquaredMinus1 = fourXSquaredMinus1;
		biggestIndex			 = 1;
	}
	if ( fourYSquaredMinus1 > fourBiggestSquaredMinus1 )
	{
		fourBiggestSquaredMinus1 = fourYSquaredMinus1;
		biggestIndex			 = 2;
	}
	if ( fourZSquaredMinus1 > fourBiggestSquaredMinus1 )
	{
		fourBiggestSquaredMinus1 = fourZSquaredMinus1;
		biggestIndex			 = 3;
	}

	// Perform square root and division
	float biggestVal = sqrtf( fourBiggestSquaredMinus1 + 1.0f ) * 0.5f;
	float mult		 = 0.25f / biggestVal;

	// Apply table to compute quaternion values
	switch ( biggestIndex )
	{
	case 0:
		result.w = biggestVal;
		result.x = ( m32 - m23 ) * mult;
		result.y = ( m13 - m31 ) * mult;
		result.z = ( m21 - m12 ) * mult;
		break;

	case 1:
		result.x = biggestVal;
		result.w = ( m32 - m23 ) * mult;
		result.y = ( m12 + m21 ) * mult;
		result.z = ( m31 + m13 ) * mult;
		break;

	case 2:
		result.y = biggestVal;
		result.w = ( m13 - m31 ) * mult;
		result.x = ( m12 + m21 ) * mult;
		result.z = ( m23 + m32 ) * mult;
		break;

	case 3:
		result.z = biggestVal;
		result.w = ( m21 - m12 ) * mult;
		result.x = ( m31 + m13 ) * mult;
		result.y = ( m23 + m32 ) * mult;
		break;
	}

	return result;


    //----------------------------------------------------------------------------------------------------------------------
    //----------------------------------------------------------------------------------------------------------------------
    // Hack attempts to swizzle coordinate "handed-ness" and transpose
//     Mat44 mat = matrix;
//     mat.Transpose();
// 
//     // SD   conventions: X fwd, -Y right, Z up
//     // book conventions: Z fwd,  X right, Y up
// //     Vec3 xFwd    = mat.GetIBasis3D();
// //     Vec3 yLeft   = mat.GetJBasis3D();
// //     Vec3 kUp     = mat.GetKBasis3D();
// // //  mat.SetIJK3D( kUp, -xFwd, yLeft );
// //     mat.m_values[Mat44::Ix] = kUp.x;
// //     mat.m_values[Mat44::Jx] = kUp.y;
// //     mat.m_values[Mat44::Kx] = kUp.z;
// // 
// //     mat.m_values[Mat44::Iy] = -xFwd.x;
// //     mat.m_values[Mat44::Jy] = -xFwd.y;
// //     mat.m_values[Mat44::Ky] = -xFwd.z;
// // 
// //     mat.m_values[Mat44::Iz] = yLeft.z;
// //     mat.m_values[Mat44::Jz] = yLeft.z;
// //     mat.m_values[Mat44::Kz] = yLeft.z; 
// 
// 
//     // Access matrix elements using the enum identifiers
//     float Ix = mat.m_values[Mat44::Ix];
//     float Iy = mat.m_values[Mat44::Iy];
//     float Iz = mat.m_values[Mat44::Iz];
// 
//     float Jx = mat.m_values[Mat44::Jx];
//     float Jy = mat.m_values[Mat44::Jy];
//     float Jz = mat.m_values[Mat44::Jz];
// 
//     float Kx = mat.m_values[Mat44::Kx];
//     float Ky = mat.m_values[Mat44::Ky];
//     float Kz = mat.m_values[Mat44::Kz];
// 
//     // Calculate squared differences for determining the largest component
//     float fourWSquaredMinus1 = Ix + Jy + Kz;
//     float fourXSquaredMinus1 = Ix - Jy - Kz;
//     float fourYSquaredMinus1 = Jy - Ix - Kz;
//     float fourZSquaredMinus1 = Kz - Ix - Jy;
// 
//     // Determine which component (w, x, y, or z) has the largest absolute value
//     int biggestIndex = 0;
//     float fourBiggestSquaredMinus1 = fourWSquaredMinus1;
//     if (fourXSquaredMinus1 > fourBiggestSquaredMinus1) 
//     {
//         fourBiggestSquaredMinus1 = fourXSquaredMinus1;
//         biggestIndex = 1;
//     }
//     if (fourYSquaredMinus1 > fourBiggestSquaredMinus1) 
//     {
//         fourBiggestSquaredMinus1 = fourYSquaredMinus1;
//         biggestIndex = 2;
//     }
//     if (fourZSquaredMinus1 > fourBiggestSquaredMinus1) 
//     {
//         fourBiggestSquaredMinus1 = fourZSquaredMinus1;
//         biggestIndex = 3;
//     }
// 
//     // Calculate the largest component's value
//     float biggestVal = sqrtf(fourBiggestSquaredMinus1 + 1.0f) * 0.5f;
//     float mult = 0.25f / biggestVal;
// 
//     // Create the quaternion and assign the largest component value
//     Quaternion q;
//     switch (biggestIndex) 
//     {
//         case 0:
//             q.w = biggestVal;
//             q.x = (Jy - Kz) * mult;
//             q.y = (Kx - Iz) * mult;
//             q.z = (Iy - Jx) * mult;
//             break;
// 
//         case 1:
//             q.x = biggestVal;
//             q.w = (Jy - Kz) * mult;
//             q.y = (Iy + Jx) * mult;
//             q.z = (Kx + Iz) * mult;
//             break;
// 
//         case 2:
//             q.y = biggestVal;
//             q.w = (Kx - Iz) * mult;
//             q.x = (Iy + Jx) * mult;
//             q.z = (Jy + Kz) * mult;
//             break;
// 
//         case 3:
//             q.z = biggestVal;
//             q.w = (Iy - Jx) * mult;
//             q.x = (Kx + Iz) * mult;
//             q.y = (Jy + Ky) * mult;
//             break;
//     }
// 
//     // Return the resulting quaternion
//     return q;
}


//----------------------------------------------------------------------------------------------------------------------
Mat44 Quaternion::GetAsMatrix( Vec3 const& position, Quaternion const& quat )
{
    Vec3 iBasis	 = Vec3::X_FWD;
    Vec3 jBasis	 = Vec3::Y_LEFT;
    Vec3 kBasis	 = Vec3::Z_UP;
    iBasis		 = quat * iBasis;
    jBasis		 = quat * jBasis;
    kBasis		 = quat * kBasis;
    Mat44 matrix = Mat44( iBasis, jBasis, kBasis, position );
    return matrix;
}