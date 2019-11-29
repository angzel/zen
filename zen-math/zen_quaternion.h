/*
 Copyright (c) 2013 ClearSky G.
 
 Permission is hereby granted, free of charge, to any person obtaining a copy of
 this software and associated documentation files (the "Software"), to deal in
 the Software without restriction, including without limitation the rights to
 use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 the Software, and to permit persons to whom the Software is furnished to do so,
 subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
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
	 values[0], values[1], and values[2] represent the imaginary values.
	 */
	inline Quaternion QuaternionMake(float *values/*[4]*/);
	
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
	void QuaternionRotateVector3Array(Quaternion quaternion, Vector3 *__nonnull vectors, size_t vectorCount);

	/*
	 The fourth component of the vector is ignored when calculating the rotation.
	 */
	inline Vector4 QuaternionRotateVector4(Quaternion quaternion, Vector4 vector);
	void QuaternionRotateVector4Array(Quaternion quaternion, Vector4 *__nonnull vectors, size_t vectorCount);

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

	inline Quaternion QuaternionMake(float * values /* [4] */)
	{
#if defined(_SSE3_INTRINSICS)
		__m128 v = _mm_load_ps(values);
		return *(Quaternion *)&v;
#else
		Quaternion q = { values[0], values[1], values[2], values[3] };
		return q;
#endif
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
#if   defined(_SSE3_INTRINSICS)
		__m128 v = _mm_load_ps(&quaternionLeft.q[0]) + _mm_load_ps(&quaternionRight.q[0]);
		return *(Quaternion *)&v;
#else
		Quaternion q = { quaternionLeft.q[0] + quaternionRight.q[0],
			quaternionLeft.q[1] + quaternionRight.q[1],
			quaternionLeft.q[2] + quaternionRight.q[2],
			quaternionLeft.q[3] + quaternionRight.q[3] };
		return q;
#endif
	}

	inline Quaternion QuaternionSubtract(Quaternion quaternionLeft, Quaternion quaternionRight)
	{
#if   defined(_SSE3_INTRINSICS)
		__m128 v = _mm_load_ps(&quaternionLeft.q[0]) - _mm_load_ps(&quaternionRight.q[0]);
		return *(Quaternion *)&v;
#else
		Quaternion q = { quaternionLeft.q[0] - quaternionRight.q[0],
			quaternionLeft.q[1] - quaternionRight.q[1],
			quaternionLeft.q[2] - quaternionRight.q[2],
			quaternionLeft.q[3] - quaternionRight.q[3] };
		return q;
#endif
	}

	inline Quaternion QuaternionMultiply(Quaternion quaternionLeft, Quaternion quaternionRight)
	{
#if defined(_SSE3_INTRINSICS)
		const __m128 ql = _mm_load_ps(&quaternionLeft.q[0]);
		const __m128 qr = _mm_load_ps(&quaternionRight.q[0]);

		const __m128 ql3012 = _mm_shuffle_ps(ql, ql, _MM_SHUFFLE(2, 1, 0, 3));
		const __m128 ql3120 = _mm_shuffle_ps(ql, ql, _MM_SHUFFLE(0, 2, 1, 3));
		const __m128 ql3201 = _mm_shuffle_ps(ql, ql, _MM_SHUFFLE(1, 0, 2, 3));

		const __m128 qr0321 = _mm_shuffle_ps(qr, qr, _MM_SHUFFLE(1, 2, 3, 0));
		const __m128 qr1302 = _mm_shuffle_ps(qr, qr, _MM_SHUFFLE(2, 0, 3, 1));
		const __m128 qr2310 = _mm_shuffle_ps(qr, qr, _MM_SHUFFLE(0, 1, 3, 2));
		const __m128 qr3012 = _mm_shuffle_ps(qr, qr, _MM_SHUFFLE(2, 1, 0, 3));

		uint32_t signBit = 0x80000000;
		uint32_t zeroBit = 0x0;
		uint32_t __attribute__((aligned(16))) mask0001[4] = {zeroBit, zeroBit, zeroBit, signBit};
		uint32_t __attribute__((aligned(16))) mask0111[4] = {zeroBit, signBit, signBit, signBit};
		const __m128 m0001 = _mm_load_ps((float *)mask0001);
		const __m128 m0111 = _mm_load_ps((float *)mask0111);

		const __m128 aline = ql3012 * _mm_xor_ps(qr0321, m0001);
		const __m128 bline = ql3120 * _mm_xor_ps(qr1302, m0001);
		const __m128 cline = ql3201 * _mm_xor_ps(qr2310, m0001);
		const __m128 dline = ql3012 * _mm_xor_ps(qr3012, m0111);
		const __m128 r = _mm_hadd_ps(_mm_hadd_ps(aline, bline), _mm_hadd_ps(cline, dline));

		return *(Quaternion *)&r;
#else

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
#endif
	}

	inline float QuaternionLength(Quaternion quaternion)
	{
#if   defined(_SSE3_INTRINSICS)
		const __m128 q = _mm_load_ps(&quaternion.q[0]);
		const __m128 product = q * q;
		const __m128 halfsum = _mm_hadd_ps(product, product);
		return _mm_cvtss_f32(_mm_sqrt_ss(_mm_hadd_ps(halfsum, halfsum)));
#else
		return sqrt(quaternion.q[0] * quaternion.q[0] +
					quaternion.q[1] * quaternion.q[1] +
					quaternion.q[2] * quaternion.q[2] +
					quaternion.q[3] * quaternion.q[3]);
#endif
	}

	inline Quaternion QuaternionConjugate(Quaternion quaternion)
	{
#if   defined(_SSE3_INTRINSICS)
			// Multiply first three elements by -1
		const uint32_t signBit = 0x80000000;
		const uint32_t zeroBit = 0x0;
		const uint32_t __attribute__((aligned(16))) mask[4] = {signBit, signBit, signBit, zeroBit};
		__m128 v_mask = _mm_load_ps((float *)mask);
		const __m128 q = _mm_load_ps(&quaternion.q[0]);
		__m128 v = _mm_xor_ps(q, v_mask);

		return *(Quaternion *)&v;
#else
		Quaternion q = { -quaternion.q[0], -quaternion.q[1], -quaternion.q[2], quaternion.q[3] };
		return q;
#endif
	}

	inline Quaternion QuaternionInvert(Quaternion quaternion)
	{
#if   defined(_SSE3_INTRINSICS)
		const __m128 q = _mm_load_ps(&quaternion.q[0]);
		const uint32_t signBit = 0x80000000;
		const uint32_t zeroBit = 0x0;
		const uint32_t __attribute__((aligned(16))) mask[4] = {signBit, signBit, signBit, zeroBit};
		const __m128 v_mask = _mm_load_ps((float *)mask);
		const __m128 product = q * q;
		const __m128 halfsum = _mm_hadd_ps(product, product);
		const __m128 v = _mm_xor_ps(q, v_mask) / _mm_hadd_ps(halfsum, halfsum);
		return *(Quaternion *)&v;
#else
		float scale = 1.0f / (quaternion.q[0] * quaternion.q[0] +
							  quaternion.q[1] * quaternion.q[1] +
							  quaternion.q[2] * quaternion.q[2] +
							  quaternion.q[3] * quaternion.q[3]);
		Quaternion q = { -quaternion.q[0] * scale, -quaternion.q[1] * scale, -quaternion.q[2] * scale, quaternion.q[3] * scale };
		return q;
#endif
	}

	inline Quaternion QuaternionNormalize(Quaternion quaternion)
	{
		float scale = 1.0f / QuaternionLength(quaternion);
#if   defined(_SSE3_INTRINSICS)
		const __m128 q = _mm_load_ps(&quaternion.q[0]);
		__m128 v = q * _mm_set1_ps(scale);
		return *(Quaternion *)&v;
#else
		Quaternion q = { quaternion.q[0] * scale, quaternion.q[1] * scale, quaternion.q[2] * scale, quaternion.q[3] * scale };
		return q;
#endif
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
