/*
 Copyright (c) 2013 MeherTJ G. All rights reserved.
 License: Everybody can use these code freely.
 */

#pragma once

#include <cmath>
#include <functional>
#include "zen_numerical.h"
#include "zen_vector.h"

namespace Zen {

	union Quaternion {
		float q[4];
		float x, y, z, w;
	};
}

inline Zen::Quaternion operator + (Zen::Quaternion const & left, Zen::Quaternion const & right);
inline Zen::Quaternion operator - (Zen::Quaternion const & left, Zen::Quaternion const & right);
inline Zen::Quaternion operator * (Zen::Quaternion const & left, Zen::Quaternion const & right);
inline Zen::Quaternion operator / (Zen::Quaternion const & left, Zen::Quaternion const & right);
inline bool operator == (Zen::Quaternion const & left, Zen::Quaternion const & right);

namespace Zen {

#define QuaternionIdentity Quaternion{0, 0, 0, 1}

	/*
	 x, y, and z represent the imaginary values.
	 */
	inline Quaternion QuaternionMake(float x, float y, float z, float w);

	/*
	 vector represents the imaginary values.
	 */
	inline Quaternion QuaternionMake(Vector3 vector, float scalar);

	/*
	 Assumes the axis is already normalized.
	 */
	inline Quaternion QuaternionMakeWithAngleAndAxis(float radians, float x, float y, float z);
	/*
	 Assumes the axis is already normalized.
	 */
	inline Quaternion QuaternionMakeWithAngleAndVector3Axis(float radians, Vector3 axisVector);

	/*
	 Calculate and return the angle component of the angle and axis form.
	 */
	float QuaternionAngle(Quaternion quaternion);

	/*
	 Calculate and return the axis component of the angle and axis form.
	 */
	Vector3 QuaternionAxis(Quaternion quaternion);

	inline Quaternion QuaternionAdd(Quaternion quaternionLeft, Quaternion quaternionRight);
	inline Quaternion QuaternionSubtract(Quaternion quaternionLeft, Quaternion quaternionRight);
	inline Quaternion QuaternionMultiply(Quaternion quaternionLeft, Quaternion quaternionRight);

	Quaternion QuaternionSlerp(Quaternion quaternionStart, Quaternion quaternionEnd, float t);

	inline float QuaternionLength(Quaternion quaternion);

	inline Quaternion QuaternionConjugate(Quaternion quaternion);
	inline Quaternion QuaternionInvert(Quaternion quaternion);
	inline Quaternion QuaternionNormalize(Quaternion quaternion);

	inline Vector3 QuaternionRotateVector3(Quaternion quaternion, Vector3 vector);
	void QuaternionRotateVector3Array(Quaternion quaternion, Vector3 * vectors, size_t vectorCount);

	/*
	 The fourth component of the vector is ignored when calculating the rotation.
	 */
	inline Vector4 QuaternionRotateVector4(Quaternion quaternion, Vector4 vector);
	void QuaternionRotateVector4Array(Quaternion quaternion, Vector4 * vectors, size_t vectorCount);

#pragma mark -
#pragma mark Implementations
#pragma mark -

	inline Quaternion QuaternionMake(float x, float y, float z, float w)
	{
		Quaternion q = { x, y, z, w };
		return q;
	}

	inline Quaternion QuaternionMake(Vector3 vector, float scalar)
	{
		Quaternion q = { vector.v[0], vector.v[1], vector.v[2], scalar };
		return q;
	}

	inline Quaternion QuaternionMakeWithAngleAndAxis(float radians, float x, float y, float z)
	{
		float halfAngle = radians * 0.5f;
		float scale = sinf(halfAngle);
		Quaternion q = { scale * x, scale * y, scale * z, cosf(halfAngle) };
		return q;
	}

	inline Quaternion QuaternionMakeWithAngleAndVector3Axis(float radians, Vector3 axisVector)
	{
		return QuaternionMakeWithAngleAndAxis(radians, axisVector.v[0], axisVector.v[1], axisVector.v[2]);
	}

	inline Quaternion QuaternionAdd(Quaternion quaternionLeft, Quaternion quaternionRight)
	{
		Quaternion q = { quaternionLeft.q[0] + quaternionRight.q[0],
			quaternionLeft.q[1] + quaternionRight.q[1],
			quaternionLeft.q[2] + quaternionRight.q[2],
			quaternionLeft.q[3] + quaternionRight.q[3] };
		return q;
	}

	inline Quaternion QuaternionSubtract(Quaternion quaternionLeft, Quaternion quaternionRight)
	{
		Quaternion q = { quaternionLeft.q[0] - quaternionRight.q[0],
			quaternionLeft.q[1] - quaternionRight.q[1],
			quaternionLeft.q[2] - quaternionRight.q[2],
			quaternionLeft.q[3] - quaternionRight.q[3] };
		return q;
	}

	inline Quaternion QuaternionMultiply(Quaternion quaternionLeft, Quaternion quaternionRight)
	{

		Quaternion q = { quaternionLeft.q[3] * quaternionRight.q[0] +
			quaternionLeft.q[0] * quaternionRight.q[3] +
			quaternionLeft.q[1] * quaternionRight.q[2] -
			quaternionLeft.q[2] * quaternionRight.q[1],

			quaternionLeft.q[3] * quaternionRight.q[1] +
			quaternionLeft.q[1] * quaternionRight.q[3] +
			quaternionLeft.q[2] * quaternionRight.q[0] -
			quaternionLeft.q[0] * quaternionRight.q[2],

			quaternionLeft.q[3] * quaternionRight.q[2] +
			quaternionLeft.q[2] * quaternionRight.q[3] +
			quaternionLeft.q[0] * quaternionRight.q[1] -
			quaternionLeft.q[1] * quaternionRight.q[0],

			quaternionLeft.q[3] * quaternionRight.q[3] -
			quaternionLeft.q[0] * quaternionRight.q[0] -
			quaternionLeft.q[1] * quaternionRight.q[1] -
			quaternionLeft.q[2] * quaternionRight.q[2] };
		return q;
	}

	inline float QuaternionLength(Quaternion quaternion)
	{
		return sqrt(quaternion.q[0] * quaternion.q[0] +
					quaternion.q[1] * quaternion.q[1] +
					quaternion.q[2] * quaternion.q[2] +
					quaternion.q[3] * quaternion.q[3]);
	}

	inline Quaternion QuaternionConjugate(Quaternion quaternion)
	{
		Quaternion q = { -quaternion.q[0], -quaternion.q[1], -quaternion.q[2], quaternion.q[3] };
		return q;
	}

	inline Quaternion QuaternionInvert(Quaternion quaternion)
	{
		float scale = 1.0f / (quaternion.q[0] * quaternion.q[0] +
							  quaternion.q[1] * quaternion.q[1] +
							  quaternion.q[2] * quaternion.q[2] +
							  quaternion.q[3] * quaternion.q[3]);
		Quaternion q = { -quaternion.q[0] * scale, -quaternion.q[1] * scale, -quaternion.q[2] * scale, quaternion.q[3] * scale };
		return q;
	}

	inline Quaternion QuaternionNormalize(Quaternion quaternion)
	{
		float scale = 1.0f / QuaternionLength(quaternion);
		Quaternion q = { quaternion.q[0] * scale, quaternion.q[1] * scale, quaternion.q[2] * scale, quaternion.q[3] * scale };
		return q;
	}

	inline Vector3 QuaternionRotateVector3(Quaternion quaternion, Vector3 vector)
	{
		Quaternion rotatedQuaternion = QuaternionMake(vector.v[0], vector.v[1], vector.v[2], 0.0f);
		rotatedQuaternion = QuaternionMultiply(QuaternionMultiply(quaternion, rotatedQuaternion), QuaternionInvert(quaternion));

		return Vector3Make(rotatedQuaternion.q[0], rotatedQuaternion.q[1], rotatedQuaternion.q[2]);
	}

	inline Vector4 QuaternionRotateVector4(Quaternion quaternion, Vector4 vector)
	{
		Quaternion rotatedQuaternion = QuaternionMake(vector.v[0], vector.v[1], vector.v[2], 0.0f);
		rotatedQuaternion = QuaternionMultiply(QuaternionMultiply(quaternion, rotatedQuaternion), QuaternionInvert(quaternion));

		return Vector4Make(rotatedQuaternion.q[0], rotatedQuaternion.q[1], rotatedQuaternion.q[2], vector.v[3]);
	}
}


inline Zen::Quaternion operator + (Zen::Quaternion const & left, Zen::Quaternion const & right)
{
	return Zen::QuaternionMake(left.x+right.x, left.y+right.y, left.z+right.z, left.w+right.w);
}
inline Zen::Quaternion operator - (Zen::Quaternion const & left, Zen::Quaternion const & right)
{
	return Zen::QuaternionMake(left.x-right.x, left.y-right.y, left.z-right.z, left.w-right.w);
}
inline Zen::Quaternion operator * (Zen::Quaternion const & left, Zen::Quaternion const & right)
{
	return Zen::QuaternionMake(left.x*right.x, left.y*right.y, left.z*right.z, left.w*right.w);
}
inline Zen::Quaternion operator / (Zen::Quaternion const & left, Zen::Quaternion const & right)
{
	return Zen::QuaternionMake(left.x/right.x, left.y/right.y, left.z/right.z, left.w/right.w);
}
inline bool operator == (Zen::Quaternion const & left, Zen::Quaternion const & right)
{
	return left.x == right.x && left.y == right.y && left.z == right.z && left.w == right.w;
}
