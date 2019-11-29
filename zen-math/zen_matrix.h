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

#include "zen_vector.h"
#include "zen_quaternion.h"
#include <memory>

namespace Zen {
	union Matrix2 {
		float m[4];
		float c[2][2];
	};
	union Matrix3 {
		float m[9];
		float c[3][3];
	};
	union Matrix4 {
		float m[16];
		float c[4][4];
	};
#define Matrix3Identity Matrix3{1, 0, 0, 0, 1, 0, 0, 0, 1}
#define Matrix4Identity Matrix4{1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1}
}

inline Zen::Matrix3 operator+(Zen::Matrix3 matrixLeft, Zen::Matrix3 matrixRight);
inline Zen::Matrix3 operator-(Zen::Matrix3 matrixLeft, Zen::Matrix3 matrixRight);
inline Zen::Matrix3 operator*(Zen::Matrix3 matrixLeft, Zen::Matrix3 matrixRight);
inline Zen::Matrix3 operator/(Zen::Matrix3 matrixLeft, Zen::Matrix3 matrixRight);
inline Zen::Matrix4 operator+(Zen::Matrix4 matrixLeft, Zen::Matrix4 matrixRight);
inline Zen::Matrix4 operator-(Zen::Matrix4 matrixLeft, Zen::Matrix4 matrixRight);
inline Zen::Matrix4 operator*(Zen::Matrix4 matrixLeft, Zen::Matrix4 matrixRight);
inline Zen::Matrix4 operator/(Zen::Matrix4 matrixLeft, Zen::Matrix4 matrixRight);

namespace Zen {
	inline Matrix3 Matrix3Make(float m00, float m01, float m02,
							   float m10, float m11, float m12,
							   float m20, float m21, float m22);
	
	inline Matrix3 Matrix3MakeAndTranspose(float m00, float m01, float m02,
										   float m10, float m11, float m12,
										   float m20, float m21, float m22);
	
	inline Matrix3 Matrix3Make(float const * values /* [9] */);
	
	inline Matrix3 Matrix3MakeAndTranspose(float const * values /* [9] */);
	
	inline Matrix3 Matrix3MakeWithRows(Vector3 row0,
									   Vector3 row1,
									   Vector3 row2);
	
	inline Matrix3 Matrix3MakeWithColumns(Vector3 column0,
										  Vector3 column1,
										  Vector3 column2);
	
	inline Matrix3 Matrix3MakeWithQuaternion(Quaternion quaternion);
	
	inline Matrix3 Matrix3MakeScale(float sx, float sy, float sz);
	inline Matrix3 Matrix3MakeRotation(float radians, float x, float y, float z);
	
	inline Matrix3 Matrix3MakeXRotation(float radians);
	inline Matrix3 Matrix3MakeYRotation(float radians);
	inline Matrix3 Matrix3MakeZRotation(float radians);
	
	/*
	 Returns the upper left 2x2 portion of the 3x3 matrix.
	 */
	inline Matrix2 Matrix3GetMatrix2(Matrix3 matrix);
	
	inline Vector3 Matrix3GetRow(Matrix3 matrix, int row);
	inline Vector3 Matrix3GetColumn(Matrix3 matrix, int column);
	
	inline Matrix3 Matrix3SetRow(Matrix3 matrix, int row, Vector3 vector);
	inline Matrix3 Matrix3SetColumn(Matrix3 matrix, int column, Vector3 vector);
	
	inline Matrix3 Matrix3Transpose(Matrix3 matrix);
	
	Matrix3 Matrix3Invert(Matrix3 matrix, bool *isInvertible);
	Matrix3 Matrix3InvertAndTranspose(Matrix3 matrix, bool *isInvertible);
	
	inline Matrix3 Matrix3Multiply(Matrix3 matrixLeft, Matrix3 matrixRight);
	
	inline Matrix3 Matrix3Scale(Matrix3 matrix, float sx, float sy, float sz);
	inline Matrix3 Matrix3Scale(Matrix3 matrix, Vector3 scaleVector);
	/*
	 The last component of the Vector4, scaleVector, is ignored.
	 */
	inline Matrix3 Matrix3Scale(Matrix3 matrix, Vector4 scaleVector);
	
	inline Matrix3 Matrix3Rotate(Matrix3 matrix, float radians, float x, float y, float z);
	inline Matrix3 Matrix3Rotate(Matrix3 matrix, float radians, Vector3 axisVector);
	/*
	 The last component of the Vector4, axisVector, is ignored.
	 */
	inline Matrix3 Matrix3Rotate(Matrix3 matrix, float radians, Vector4 axisVector);
	
	inline Matrix3 Matrix3RotateX(Matrix3 matrix, float radians);
	inline Matrix3 Matrix3RotateY(Matrix3 matrix, float radians);
	inline Matrix3 Matrix3RotateZ(Matrix3 matrix, float radians);
	
	inline Vector3 Matrix3Multiply(Matrix3 matrixLeft, Vector3 vectorRight);

	/*
	 m30, m31, and m32 correspond to the translation values tx, ty, tz, respectively.
	 */
	inline Matrix4 Matrix4Make(float m00, float m01, float m02, float m03,
							   float m10, float m11, float m12, float m13,
							   float m20, float m21, float m22, float m23,
							   float m30, float m31, float m32, float m33);

	/*
	 m03, m13, and m23 correspond to the translation values tx, ty, tz, respectively.
	 */
	inline Matrix4 Matrix4MakeAndTranspose(float m00, float m01, float m02, float m03,
										   float m10, float m11, float m12, float m13,
										   float m20, float m21, float m22, float m23,
										   float m30, float m31, float m32, float m33);

	/*
	 m[12], m[13], and m[14] correspond to the translation values tx, ty, and tz, respectively.
	 */
	inline Matrix4 Matrix4Make(float * values /* [16] */);

	/*
	 m[3], m[7], and m[11] correspond to the translation values tx, ty, and tz, respectively.
	 */
	inline Matrix4 Matrix4MakeAndTranspose(float * values /* [16] */);

	/*
	 row0, row1, and row2's last component should correspond to the translation values tx, ty, and tz, respectively.
	 */
	inline Matrix4 Matrix4MakeWithRows(Vector4 row0,
									   Vector4 row1,
									   Vector4 row2,
									   Vector4 row3);

	/*
	 column3's first three components should correspond to the translation values tx, ty, and tz.
	 */
	inline Matrix4 Matrix4MakeWithColumns(Vector4 column0,
										  Vector4 column1,
										  Vector4 column2,
										  Vector4 column3);

	/*
	 The quaternion will be normalized before conversion.
	 */
	inline Matrix4 Matrix4MakeWithQuaternion(Quaternion quaternion);

	inline Matrix4 Matrix4MakeTranslation(float tx, float ty, float tz);
	inline Matrix4 Matrix4MakeScale(float sx, float sy, float sz);
	inline Matrix4 Matrix4MakeRotation(float radians, float x, float y, float z);

	inline Matrix4 Matrix4MakeXRotation(float radians);
	inline Matrix4 Matrix4MakeYRotation(float radians);
	inline Matrix4 Matrix4MakeZRotation(float radians);

	/*
	 Equivalent to gluPerspective.
	 */
	inline Matrix4 Matrix4MakePerspective(float fovyRadians, float aspect, float nearZ, float farZ);

	/*
	 Equivalent to glFrustum.
	 */
	inline Matrix4 Matrix4MakeFrustum(float left, float right,
									  float bottom, float top,
									  float nearZ, float farZ);

	/*
	 Equivalent to glOrtho.
	 */
	inline Matrix4 Matrix4MakeOrtho(float left, float right,
									float bottom, float top,
									float nearZ, float farZ);

	/*
	 Equivalent to gluLookAt.
	 */
	inline Matrix4 Matrix4MakeLookAt(float eyeX, float eyeY, float eyeZ,
									 float centerX, float centerY, float centerZ,
									 float upX, float upY, float upZ);

	/*
	 Returns the upper left 3x3 portion of the 4x4 matrix.
	 */
	inline Matrix3 Matrix4GetMatrix3(Matrix4 matrix);
	/*
	 Returns the upper left 2x2 portion of the 4x4 matrix.
	 */
	inline Matrix2 Matrix4GetMatrix2(Matrix4 matrix);

	/*
	 Matrix4GetRow returns vectors for rows 0, 1, and 2 whose last component will be the translation value tx, ty, and tz, respectively.
	 Valid row values range from 0 to 3, inclusive.
	 */
	inline Vector4 Matrix4GetRow(Matrix4 matrix, int row);
	/*
	 Matrix4GetColumn returns a vector for column 3 whose first three components will be the translation values tx, ty, and tz.
	 Valid column values range from 0 to 3, inclusive.
	 */
	inline Vector4 Matrix4GetColumn(Matrix4 matrix, int column);

	/*
	 Matrix4SetRow expects that the vector for row 0, 1, and 2 will have a translation value as its last component.
	 Valid row values range from 0 to 3, inclusive.
	 */
	inline Matrix4 Matrix4SetRow(Matrix4 matrix, int row, Vector4 vector);
	/*
	 Matrix4SetColumn expects that the vector for column 3 will contain the translation values tx, ty, and tz as its first three components, respectively.
	 Valid column values range from 0 to 3, inclusive.
	 */
	inline Matrix4 Matrix4SetColumn(Matrix4 matrix, int column, Vector4 vector);

	inline Matrix4 Matrix4Transpose(Matrix4 matrix);

	Matrix4 Matrix4Invert(Matrix4 matrix, bool * __nullable isInvertible);
	Matrix4 Matrix4InvertAndTranspose(Matrix4 matrix, bool * __nullable isInvertible);

	inline Matrix4 Matrix4Multiply(Matrix4 matrixLeft, Matrix4 matrixRight);

	inline Matrix4 Matrix4Add(Matrix4 matrixLeft, Matrix4 matrixRight);
	inline Matrix4 Matrix4Subtract(Matrix4 matrixLeft, Matrix4 matrixRight);

	inline Matrix4 Matrix4Translate(Matrix4 matrix, float tx, float ty, float tz);
	inline Matrix4 Matrix4TranslateWithVector3(Matrix4 matrix, Vector3 translationVector);
	/*
	 The last component of the Vector4, translationVector, is ignored.
	 */
	inline Matrix4 Matrix4TranslateWithVector4(Matrix4 matrix, Vector4 translationVector);

	inline Matrix4 Matrix4Scale(Matrix4 matrix, float sx, float sy, float sz);
	inline Matrix4 Matrix4ScaleWithVector3(Matrix4 matrix, Vector3 scaleVector);
	/*
	 The last component of the Vector4, scaleVector, is ignored.
	 */
	inline Matrix4 Matrix4ScaleWithVector4(Matrix4 matrix, Vector4 scaleVector);

	inline Matrix4 Matrix4Rotate(Matrix4 matrix, float radians, float x, float y, float z);
	inline Matrix4 Matrix4RotateWithVector3(Matrix4 matrix, float radians, Vector3 axisVector);
	/*
	 The last component of the Vector4, axisVector, is ignored.
	 */
	inline Matrix4 Matrix4RotateWithVector4(Matrix4 matrix, float radians, Vector4 axisVector);

	inline Matrix4 Matrix4RotateX(Matrix4 matrix, float radians);
	inline Matrix4 Matrix4RotateY(Matrix4 matrix, float radians);
	inline Matrix4 Matrix4RotateZ(Matrix4 matrix, float radians);

	/*
	 Assumes 0 in the w component.
	 */
	inline Vector3 Matrix4MultiplyVector3(Matrix4 matrixLeft, Vector3 vectorRight);
	/*
	 Assumes 1 in the w component.
	 */
	inline Vector3 Matrix4MultiplyVector3WithTranslation(Matrix4 matrixLeft, Vector3 vectorRight);
	/*
	 Assumes 1 in the w component and divides the resulting vector by w before returning.
	 */
	inline Vector3 Matrix4MultiplyAndProjectVector3(Matrix4 matrixLeft, Vector3 vectorRight);

	/*
	 Assumes 0 in the w component.
	 */
	inline void Matrix4MultiplyVector3Array(Matrix4 matrix, Vector3 *__nonnull vectors, size_t vectorCount);
	/*
	 Assumes 1 in the w component.
	 */
	inline void Matrix4MultiplyVector3ArrayWithTranslation(Matrix4 matrix, Vector3 *__nonnull vectors, size_t vectorCount);
	/*
	 Assumes 1 in the w component and divides the resulting vector by w before returning.
	 */
	inline void Matrix4MultiplyAndProjectVector3Array(Matrix4 matrix, Vector3 *__nonnull vectors, size_t vectorCount);

	inline Vector4 Matrix4MultiplyVector4(Matrix4 matrixLeft, Vector4 vectorRight);


	inline Matrix3 Matrix3Make(float m00, float m01, float m02,
							   float m10, float m11, float m12,
							   float m20, float m21, float m22)
	{
		Matrix3 m = { m00, m01, m02,
			m10, m11, m12,
			m20, m21, m22 };
		return m;
	}
	
	inline Matrix3 Matrix3MakeAndTranspose(float m00, float m01, float m02,
										   float m10, float m11, float m12,
										   float m20, float m21, float m22)
	{
		Matrix3 m = { m00, m10, m20,
			m01, m11, m21,
			m02, m12, m22 };
		return m;
	}
	
	inline Matrix3 Matrix3Make(float const * values /* [9] */)
	{
		Matrix3 m = { values[0], values[1], values[2],
			values[3], values[4], values[5],
			values[6], values[7], values[8] };
		return m;
	}
	
	inline Matrix3 Matrix3MakeAndTranspose(float *values /* [9] */)
	{
		Matrix3 m = { values[0], values[3], values[6],
			values[1], values[4], values[7],
			values[2], values[5], values[8] };
		return m;
	}
	
	inline Matrix3 Matrix3MakeWithRows(Vector3 row0,
									   Vector3 row1,
									   Vector3 row2)
	{
		Matrix3 m = { row0.v[0], row1.v[0], row2.v[0],
			row0.v[1], row1.v[1], row2.v[1],
			row0.v[2], row1.v[2], row2.v[2] };
		return m;
	}
	
	inline Matrix3 Matrix3MakeWithColumns(Vector3 column0,
										  Vector3 column1,
										  Vector3 column2)
	{
		Matrix3 m = { column0.v[0], column0.v[1], column0.v[2],
			column1.v[0], column1.v[1], column1.v[2],
			column2.v[0], column2.v[1], column2.v[2] };
		return m;
	}
#if 0
	inline Matrix3 Matrix3MakeWithQuaternion(Quaternion quaternion)
	{
		quaternion = QuaternionNormalize(quaternion);
		
		float x = quaternion.q[0];
		float y = quaternion.q[1];
		float z = quaternion.q[2];
		float w = quaternion.q[3];
		
		float _2x = x + x;
		float _2y = y + y;
		float _2z = z + z;
		float _2w = w + w;
		
		Matrix3 m = { 1.0f - _2y * y - _2z * z,
			_2x * y + _2w * z,
			_2x * z - _2w * y,
			
			_2x * y - _2w * z,
			1.0f - _2x * x - _2z * z,
			_2y * z + _2w * x,
			
			_2x * z + _2w * y,
			_2y * z - _2w * x,
			1.0f - _2x * x - _2y * y };
		
		return m;
	}
#endif
	
	inline Matrix3 Matrix3MakeScale(float sx, float sy, float sz)
	{
		Matrix3 m = Matrix3Identity;
		m.m[0] = sx;
		m.m[4] = sy;
		m.m[8] = sz;
		return m;
	}
	
	inline Matrix3 Matrix3MakeRotation(float radians, float x, float y, float z)
	{
		Vector3 v = Vector3Normalize(Vector3Make(x, y, z));
		float cos = cosf(radians);
		float cosp = 1.0f - cos;
		float sin = sinf(radians);
		
		Matrix3 m = { cos + cosp * v.v[0] * v.v[0],
			cosp * v.v[0] * v.v[1] + v.v[2] * sin,
			cosp * v.v[0] * v.v[2] - v.v[1] * sin,
			
			cosp * v.v[0] * v.v[1] - v.v[2] * sin,
			cos + cosp * v.v[1] * v.v[1],
			cosp * v.v[1] * v.v[2] + v.v[0] * sin,
			
			cosp * v.v[0] * v.v[2] + v.v[1] * sin,
			cosp * v.v[1] * v.v[2] - v.v[0] * sin,
			cos + cosp * v.v[2] * v.v[2] };
		
		return m;
	}
	
	inline Matrix3 Matrix3MakeXRotation(float radians)
	{
		float cos = cosf(radians);
		float sin = sinf(radians);
		
		Matrix3 m = { 1.0f, 0.0f, 0.0f,
			0.0f, cos, sin,
			0.0f, -sin, cos };
		
		return m;
	}
	
	inline Matrix3 Matrix3MakeYRotation(float radians)
	{
		float cos = cosf(radians);
		float sin = sinf(radians);
		
		Matrix3 m = { cos, 0.0f, -sin,
			0.0f, 1.0f, 0.0f,
			sin, 0.0f, cos };
		
		return m;
	}
	
	inline Matrix3 Matrix3MakeZRotation(float radians)
	{
		float cos = cosf(radians);
		float sin = sinf(radians);
		
		Matrix3 m = { cos, sin, 0.0f,
			-sin, cos, 0.0f,
			0.0f, 0.0f, 1.0f };
		
		return m;
	}
	
	inline Matrix2 Matrix3GetMatrix2(Matrix3 matrix)
	{
		Matrix2 m = { matrix.m[0], matrix.m[1],
			matrix.m[3], matrix.m[4] };
		return m;
	}
	
	inline Vector3 Matrix3GetRow(Matrix3 matrix, int row)
	{
		Vector3 v = { matrix.m[row], matrix.m[3 + row], matrix.m[6 + row] };
		return v;
	}
	
	inline Vector3 Matrix3GetColumn(Matrix3 matrix, int column)
	{
		Vector3 v = { matrix.m[column * 3 + 0], matrix.m[column * 3 + 1], matrix.m[column * 3 + 2] };
		return v;
	}
	
	inline Matrix3 Matrix3SetRow(Matrix3 matrix, int row, Vector3 vector)
	{
		matrix.m[row] = vector.v[0];
		matrix.m[row + 3] = vector.v[1];
		matrix.m[row + 6] = vector.v[2];
		
		return matrix;
	}
	
	inline Matrix3 Matrix3SetColumn(Matrix3 matrix, int column, Vector3 vector)
	{
		matrix.m[column * 3 + 0] = vector.v[0];
		matrix.m[column * 3 + 1] = vector.v[1];
		matrix.m[column * 3 + 2] = vector.v[2];
		
		return matrix;
	}
	
	inline Matrix3 Matrix3Transpose(Matrix3 matrix)
	{
		Matrix3 m = { matrix.m[0], matrix.m[3], matrix.m[6],
			matrix.m[1], matrix.m[4], matrix.m[7],
			matrix.m[2], matrix.m[5], matrix.m[8] };
		return m;
	}
	
	inline Matrix3 Matrix3Multiply(Matrix3 matrixLeft, Matrix3 matrixRight)
	{
#if   defined(GLK_SSE3_INTRINSICS)
		struct {
			Matrix3 m;
			char pad[16*4 - sizeof(Matrix3)];
		} ret;
		
		const __m128 iMatrixLeft0 = _mm_loadu_ps(&matrixLeft.m[0]); // 0 1 2 3 // unaligned load
		const __m128 iMatrixLeft1 = _mm_loadu_ps(&matrixLeft.m[3]); // 3 4 5 6 // unaligned load
		const __m128 iMatrixLeft2Tmp = _mm_loadu_ps(&matrixLeft.m[5]); // 5 6 7 8 // unaligned load
		const __m128 iMatrixLeft2 = _mm_shuffle_ps(iMatrixLeft2Tmp, iMatrixLeft2Tmp, _MM_SHUFFLE(0, 3, 2, 1)); // 6 7 8 x
		
		const __m128 iMatrixRight0 = _mm_loadu_ps(&matrixRight.m[0]);
		const __m128 iMatrixRight1 = _mm_loadu_ps(&matrixRight.m[3]);
		const __m128 iMatrixRight2 = _mm_loadu_ps(&matrixRight.m[5]);
		
		const __m128 mm0 = iMatrixLeft0 * _mm_shuffle_ps(iMatrixRight0, iMatrixRight0, _MM_SHUFFLE(0, 0, 0, 0))  // mm0 = L0*R0 L1*R0 L2*R0 L3*R0
		+ iMatrixLeft1 * _mm_shuffle_ps(iMatrixRight0, iMatrixRight0, _MM_SHUFFLE(1, 1, 1, 1))  // mm0 = L0*R0+L3*R1 L1*R0+L4*R1 L2*R0+L5*R1 L3*R0+L6*R1
		+ iMatrixLeft2 * _mm_shuffle_ps(iMatrixRight0, iMatrixRight0, _MM_SHUFFLE(2, 2, 2, 2));
		
		const __m128 mm1 = iMatrixLeft0 * _mm_shuffle_ps(iMatrixRight0, iMatrixRight0, _MM_SHUFFLE(3, 3, 3, 3))  // mm1 = L0*R3 L1*R3 L2*R3 L3*R3
		+ iMatrixLeft1 * _mm_shuffle_ps(iMatrixRight1, iMatrixRight1, _MM_SHUFFLE(1, 1, 1, 1))  // mm1 = L0*R3+L3*R4 L1*R3+L4*R4 L2*R3+L5*R4 L3*R3+
		+ iMatrixLeft2 * _mm_shuffle_ps(iMatrixRight1, iMatrixRight1, _MM_SHUFFLE(2, 2, 2, 2));
		
		const __m128 mm2 = iMatrixLeft0 * _mm_shuffle_ps(iMatrixRight1, iMatrixRight1, _MM_SHUFFLE(3, 3, 3, 3)) // mm2 = L0*R6 L1*R6 L2*R6 L3*R6
		+ iMatrixLeft1 * _mm_shuffle_ps(iMatrixRight2, iMatrixRight2, _MM_SHUFFLE(2, 2, 2, 2))
		+ iMatrixLeft2 * _mm_shuffle_ps(iMatrixRight2, iMatrixRight2, _MM_SHUFFLE(3, 3, 3, 3));
		
		_mm_storeu_ps(&ret.m.m[0], mm0); //unaligned store to indices: 0 1 2 3
		_mm_storeu_ps(&ret.m.m[3], mm1); //unaligned store to indices: 3 4 5 6
		_mm_storeu_ps(&ret.m.m[6], mm2); //unaligned store to indices: 6 7 8
		
		return ret.m;
#else
		Matrix3 m;
		
		m.m[0] = matrixLeft.m[0] * matrixRight.m[0] + matrixLeft.m[3] * matrixRight.m[1] + matrixLeft.m[6] * matrixRight.m[2];
		m.m[3] = matrixLeft.m[0] * matrixRight.m[3] + matrixLeft.m[3] * matrixRight.m[4] + matrixLeft.m[6] * matrixRight.m[5];
		m.m[6] = matrixLeft.m[0] * matrixRight.m[6] + matrixLeft.m[3] * matrixRight.m[7] + matrixLeft.m[6] * matrixRight.m[8];
		
		m.m[1] = matrixLeft.m[1] * matrixRight.m[0] + matrixLeft.m[4] * matrixRight.m[1] + matrixLeft.m[7] * matrixRight.m[2];
		m.m[4] = matrixLeft.m[1] * matrixRight.m[3] + matrixLeft.m[4] * matrixRight.m[4] + matrixLeft.m[7] * matrixRight.m[5];
		m.m[7] = matrixLeft.m[1] * matrixRight.m[6] + matrixLeft.m[4] * matrixRight.m[7] + matrixLeft.m[7] * matrixRight.m[8];
		
		m.m[2] = matrixLeft.m[2] * matrixRight.m[0] + matrixLeft.m[5] * matrixRight.m[1] + matrixLeft.m[8] * matrixRight.m[2];
		m.m[5] = matrixLeft.m[2] * matrixRight.m[3] + matrixLeft.m[5] * matrixRight.m[4] + matrixLeft.m[8] * matrixRight.m[5];
		m.m[8] = matrixLeft.m[2] * matrixRight.m[6] + matrixLeft.m[5] * matrixRight.m[7] + matrixLeft.m[8] * matrixRight.m[8];
		
		return m;
#endif
	}
	
	inline Matrix3 Matrix3Scale(Matrix3 matrix, float sx, float sy, float sz)
	{
		Matrix3 m = { matrix.m[0] * sx, matrix.m[1] * sx, matrix.m[2] * sx,
			matrix.m[3] * sy, matrix.m[4] * sy, matrix.m[5] * sy,
			matrix.m[6] * sz, matrix.m[7] * sz, matrix.m[8] * sz };
		return m;
	}
	
	inline Matrix3 Matrix3Scale(Matrix3 matrix, Vector3 scaleVector)
	{
		Matrix3 m = { matrix.m[0] * scaleVector.v[0], matrix.m[1] * scaleVector.v[0], matrix.m[2] * scaleVector.v[0],
			matrix.m[3] * scaleVector.v[1], matrix.m[4] * scaleVector.v[1], matrix.m[5] * scaleVector.v[1],
			matrix.m[6] * scaleVector.v[2], matrix.m[7] * scaleVector.v[2], matrix.m[8] * scaleVector.v[2] };
		return m;
	}
	
	inline Matrix3 Matrix3Scale(Matrix3 matrix, Vector4 scaleVector)
	{
		Matrix3 m = { matrix.m[0] * scaleVector.v[0], matrix.m[1] * scaleVector.v[0], matrix.m[2] * scaleVector.v[0],
			matrix.m[3] * scaleVector.v[1], matrix.m[4] * scaleVector.v[1], matrix.m[5] * scaleVector.v[1],
			matrix.m[6] * scaleVector.v[2], matrix.m[7] * scaleVector.v[2], matrix.m[8] * scaleVector.v[2] };
		return m;
	}
	
	inline Matrix3 Matrix3Rotate(Matrix3 matrix, float radians, float x, float y, float z)
	{
		Matrix3 rm = Matrix3MakeRotation(radians, x, y, z);
		return Matrix3Multiply(matrix, rm);
	}
	
	inline Matrix3 Matrix3Rotate(Matrix3 matrix, float radians, Vector3 axisVector)
	{
		Matrix3 rm = Matrix3MakeRotation(radians, axisVector.v[0], axisVector.v[1], axisVector.v[2]);
		return Matrix3Multiply(matrix, rm);
	}
	
	inline Matrix3 Matrix3Rotate(Matrix3 matrix, float radians, Vector4 axisVector)
	{
		Matrix3 rm = Matrix3MakeRotation(radians, axisVector.v[0], axisVector.v[1], axisVector.v[2]);
		return Matrix3Multiply(matrix, rm);
	}
	
	inline Matrix3 Matrix3RotateX(Matrix3 matrix, float radians)
	{
		Matrix3 rm = Matrix3MakeXRotation(radians);
		return Matrix3Multiply(matrix, rm);
	}
	
	inline Matrix3 Matrix3RotateY(Matrix3 matrix, float radians)
	{
		Matrix3 rm = Matrix3MakeYRotation(radians);
		return Matrix3Multiply(matrix, rm);
	}
	
	inline Matrix3 Matrix3RotateZ(Matrix3 matrix, float radians)
	{
		Matrix3 rm = Matrix3MakeZRotation(radians);
		return Matrix3Multiply(matrix, rm);
	}
	
	inline Vector3 Matrix3Multiply(Matrix3 matrixLeft, Vector3 vectorRight)
	{
		Vector3 v = { matrixLeft.m[0] * vectorRight.v[0] + matrixLeft.m[3] * vectorRight.v[1] + matrixLeft.m[6] * vectorRight.v[2],
			matrixLeft.m[1] * vectorRight.v[0] + matrixLeft.m[4] * vectorRight.v[1] + matrixLeft.m[7] * vectorRight.v[2],
			matrixLeft.m[2] * vectorRight.v[0] + matrixLeft.m[5] * vectorRight.v[1] + matrixLeft.m[8] * vectorRight.v[2] };
		return v;
	}

	#pragma mark Implementations

	inline Matrix4 Matrix4Make(float m00, float m01, float m02, float m03,
							   float m10, float m11, float m12, float m13,
							   float m20, float m21, float m22, float m23,
							   float m30, float m31, float m32, float m33)
	{
		Matrix4 m = { m00, m01, m02, m03,
			m10, m11, m12, m13,
			m20, m21, m22, m23,
			m30, m31, m32, m33 };
		return m;
	}

	inline Matrix4 Matrix4MakeAndTranspose(float m00, float m01, float m02, float m03,
										   float m10, float m11, float m12, float m13,
										   float m20, float m21, float m22, float m23,
										   float m30, float m31, float m32, float m33)
	{
		Matrix4 m = { m00, m10, m20, m30,
			m01, m11, m21, m31,
			m02, m12, m22, m32,
			m03, m13, m23, m33 };
		return m;
	}

	inline Matrix4 Matrix4Make(float * values /* [16] */)
	{
		Matrix4 m = { values[0], values[1], values[2], values[3],
			values[4], values[5], values[6], values[7],
			values[8], values[9], values[10], values[11],
			values[12], values[13], values[14], values[15] };
		return m;
	}

	inline Matrix4 Matrix4MakeAndTranspose(float * values /* [16] */)
	{
		Matrix4 m = { values[0], values[4], values[8], values[12],
			values[1], values[5], values[9], values[13],
			values[2], values[6], values[10], values[14],
			values[3], values[7], values[11], values[15] };
		return m;
	}

	inline Matrix4 Matrix4MakeWithRows(Vector4 row0,
									   Vector4 row1,
									   Vector4 row2,
									   Vector4 row3)
	{
		Matrix4 m = { row0.v[0], row1.v[0], row2.v[0], row3.v[0],
			row0.v[1], row1.v[1], row2.v[1], row3.v[1],
			row0.v[2], row1.v[2], row2.v[2], row3.v[2],
			row0.v[3], row1.v[3], row2.v[3], row3.v[3] };
		return m;
	}

	inline Matrix4 Matrix4MakeWithColumns(Vector4 column0,
										  Vector4 column1,
										  Vector4 column2,
										  Vector4 column3)
	{
	#if   defined(_SSE3_INTRINSICS)
		Matrix4 m;
		*((__m128*)&m.m[0])  = *(__m128*)&column0;
		*((__m128*)&m.m[4])  = *(__m128*)&column1;
		*((__m128*)&m.m[8])  = *(__m128*)&column2;
		*((__m128*)&m.m[12]) = *(__m128*)&column3;
		return m;
	#else
		Matrix4 m = { column0.v[0], column0.v[1], column0.v[2], column0.v[3],
			column1.v[0], column1.v[1], column1.v[2], column1.v[3],
			column2.v[0], column2.v[1], column2.v[2], column2.v[3],
			column3.v[0], column3.v[1], column3.v[2], column3.v[3] };
		return m;
	#endif
	}

	inline Matrix4 Matrix4MakeWithQuaternion(Quaternion quaternion)
	{
		quaternion = QuaternionNormalize(quaternion);

		float x = quaternion.q[0];
		float y = quaternion.q[1];
		float z = quaternion.q[2];
		float w = quaternion.q[3];

		float _2x = x + x;
		float _2y = y + y;
		float _2z = z + z;
		float _2w = w + w;

		Matrix4 m = { 1.0f - _2y * y - _2z * z,
			_2x * y + _2w * z,
			_2x * z - _2w * y,
			0.0f,
			_2x * y - _2w * z,
			1.0f - _2x * x - _2z * z,
			_2y * z + _2w * x,
			0.0f,
			_2x * z + _2w * y,
			_2y * z - _2w * x,
			1.0f - _2x * x - _2y * y,
			0.0f,
			0.0f,
			0.0f,
			0.0f,
			1.0f };

		return m;
	}

	inline Matrix4 Matrix4MakeTranslation(float tx, float ty, float tz)
	{
		Matrix4 m = Matrix4Identity;
		m.m[12] = tx;
		m.m[13] = ty;
		m.m[14] = tz;
		return m;
	}

	inline Matrix4 Matrix4MakeScale(float sx, float sy, float sz)
	{
		Matrix4 m = Matrix4Identity;
		m.m[0] = sx;
		m.m[5] = sy;
		m.m[10] = sz;
		return m;
	}

	inline Matrix4 Matrix4MakeRotation(float radians, float x, float y, float z)
	{
		Vector3 v = Vector3Normalize(Vector3Make(x, y, z));
		float cos = cosf(radians);
		float cosp = 1.0f - cos;
		float sin = sinf(radians);

		Matrix4 m = { cos + cosp * v.v[0] * v.v[0],
			cosp * v.v[0] * v.v[1] + v.v[2] * sin,
			cosp * v.v[0] * v.v[2] - v.v[1] * sin,
			0.0f,
			cosp * v.v[0] * v.v[1] - v.v[2] * sin,
			cos + cosp * v.v[1] * v.v[1],
			cosp * v.v[1] * v.v[2] + v.v[0] * sin,
			0.0f,
			cosp * v.v[0] * v.v[2] + v.v[1] * sin,
			cosp * v.v[1] * v.v[2] - v.v[0] * sin,
			cos + cosp * v.v[2] * v.v[2],
			0.0f,
			0.0f,
			0.0f,
			0.0f,
			1.0f };

		return m;
	}

	inline Matrix4 Matrix4MakeXRotation(float radians)
	{
		float cos = cosf(radians);
		float sin = sinf(radians);

		Matrix4 m = { 1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, cos, sin, 0.0f,
			0.0f, -sin, cos, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f };

		return m;
	}

	inline Matrix4 Matrix4MakeYRotation(float radians)
	{
		float cos = cosf(radians);
		float sin = sinf(radians);

		Matrix4 m = { cos, 0.0f, -sin, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			sin, 0.0f, cos, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f };

		return m;
	}

	inline Matrix4 Matrix4MakeZRotation(float radians)
	{
		float cos = cosf(radians);
		float sin = sinf(radians);

		Matrix4 m = { cos, sin, 0.0f, 0.0f,
			-sin, cos, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f };

		return m;
	}

	inline Matrix4 Matrix4MakePerspective(float fovyRadians, float aspect, float nearZ, float farZ)
	{
		float cotan = 1.0f / tanf(fovyRadians / 2.0f);

		Matrix4 m = { cotan / aspect, 0.0f, 0.0f, 0.0f,
			0.0f, cotan, 0.0f, 0.0f,
			0.0f, 0.0f, (farZ + nearZ) / (nearZ - farZ), -1.0f,
			0.0f, 0.0f, (2.0f * farZ * nearZ) / (nearZ - farZ), 0.0f };

		return m;
	}

	inline Matrix4 Matrix4MakeFrustum(float left, float right,
									  float bottom, float top,
									  float nearZ, float farZ)
	{
		float ral = right + left;
		float rsl = right - left;
		float tsb = top - bottom;
		float tab = top + bottom;
		float fan = farZ + nearZ;
		float fsn = farZ - nearZ;

		Matrix4 m = { 2.0f * nearZ / rsl, 0.0f, 0.0f, 0.0f,
			0.0f, 2.0f * nearZ / tsb, 0.0f, 0.0f,
			ral / rsl, tab / tsb, -fan / fsn, -1.0f,
			0.0f, 0.0f, (-2.0f * farZ * nearZ) / fsn, 0.0f };

		return m;
	}

	inline Matrix4 Matrix4MakeOrtho(float left, float right,
									float bottom, float top,
									float nearZ, float farZ)
	{
		float ral = right + left;
		float rsl = right - left;
		float tab = top + bottom;
		float tsb = top - bottom;
		float fan = farZ + nearZ;
		float fsn = farZ - nearZ;

		Matrix4 m = { 2.0f / rsl, 0.0f, 0.0f, 0.0f,
			0.0f, 2.0f / tsb, 0.0f, 0.0f,
			0.0f, 0.0f, -2.0f / fsn, 0.0f,
			-ral / rsl, -tab / tsb, -fan / fsn, 1.0f };

		return m;
	}

	inline Matrix4 Matrix4MakeLookAt(float eyeX, float eyeY, float eyeZ,
									 float centerX, float centerY, float centerZ,
									 float upX, float upY, float upZ)
	{
		Vector3 ev = { eyeX, eyeY, eyeZ };
		Vector3 cv = { centerX, centerY, centerZ };
		Vector3 uv = { upX, upY, upZ };
		Vector3 n = Vector3Normalize(ev - cv);
		Vector3 u = Vector3Normalize(Vector3CrossProduct(uv, n));
		Vector3 v = Vector3CrossProduct(n, u);

		Matrix4 m = { u.v[0], v.v[0], n.v[0], 0.0f,
			u.v[1], v.v[1], n.v[1], 0.0f,
			u.v[2], v.v[2], n.v[2], 0.0f,
			Vector3DotProduct(-(u), ev),
			Vector3DotProduct(-(v), ev),
			Vector3DotProduct(-(n), ev),
			1.0f };

		return m;
	}

	inline Matrix3 Matrix4GetMatrix3(Matrix4 matrix)
	{
		Matrix3 m = { matrix.m[0], matrix.m[1], matrix.m[2],
			matrix.m[4], matrix.m[5], matrix.m[6],
			matrix.m[8], matrix.m[9], matrix.m[10] };
		return m;
	}

	inline Matrix2 Matrix4GetMatrix2(Matrix4 matrix)
	{
		Matrix2 m = { matrix.m[0], matrix.m[1],
			matrix.m[4], matrix.m[5] };
		return m;
	}

	inline Vector4 Matrix4GetRow(Matrix4 matrix, int row)
	{
		Vector4 v = { matrix.m[row], matrix.m[4 + row], matrix.m[8 + row], matrix.m[12 + row] };
		return v;
	}

	inline Vector4 Matrix4GetColumn(Matrix4 matrix, int column)
	{
	#if   defined(_SSE3_INTRINSICS)
		__m128 v = _mm_load_ps(&matrix.m[column * 4]);
		return *(Vector4 *)&v;
	#else
		Vector4 v = { matrix.m[column * 4 + 0], matrix.m[column * 4 + 1], matrix.m[column * 4 + 2], matrix.m[column * 4 + 3] };
		return v;
	#endif
	}

	inline Matrix4 Matrix4SetRow(Matrix4 matrix, int row, Vector4 vector)
	{
		matrix.m[row] = vector.v[0];
		matrix.m[row + 4] = vector.v[1];
		matrix.m[row + 8] = vector.v[2];
		matrix.m[row + 12] = vector.v[3];

		return matrix;
	}

	inline Matrix4 Matrix4SetColumn(Matrix4 matrix, int column, Vector4 vector)
	{
	#if   defined(_SSE3_INTRINSICS)
		*((__m128*)&matrix.m[column*4]) = *(__m128*)&vector;
		return matrix;
	#else
		matrix.m[column * 4 + 0] = vector.v[0];
		matrix.m[column * 4 + 1] = vector.v[1];
		matrix.m[column * 4 + 2] = vector.v[2];
		matrix.m[column * 4 + 3] = vector.v[3];

		return matrix;
	#endif
	}

	inline Matrix4 Matrix4Transpose(Matrix4 matrix)
	{
		Matrix4 m = { matrix.m[0], matrix.m[4], matrix.m[8], matrix.m[12],
			matrix.m[1], matrix.m[5], matrix.m[9], matrix.m[13],
			matrix.m[2], matrix.m[6], matrix.m[10], matrix.m[14],
			matrix.m[3], matrix.m[7], matrix.m[11], matrix.m[15] };
		return m;
	}

	inline Matrix4 Matrix4Multiply(Matrix4 matrixLeft, Matrix4 matrixRight)
	{
	#if   defined(_SSE3_INTRINSICS)

		const __m128 l0 = _mm_load_ps(&matrixLeft.m[0]);
		const __m128 l1 = _mm_load_ps(&matrixLeft.m[4]);
		const __m128 l2 = _mm_load_ps(&matrixLeft.m[8]);
		const __m128 l3 = _mm_load_ps(&matrixLeft.m[12]);

		const __m128 r0 = _mm_load_ps(&matrixRight.m[0]);
		const __m128 r1 = _mm_load_ps(&matrixRight.m[4]);
		const __m128 r2 = _mm_load_ps(&matrixRight.m[8]);
		const __m128 r3 = _mm_load_ps(&matrixRight.m[12]);

		const __m128 m0 = l0 * _mm_shuffle_ps(r0, r0, _MM_SHUFFLE(0, 0, 0, 0))
		+ l1 * _mm_shuffle_ps(r0, r0, _MM_SHUFFLE(1, 1, 1, 1))
		+ l2 * _mm_shuffle_ps(r0, r0, _MM_SHUFFLE(2, 2, 2, 2))
		+ l3 * _mm_shuffle_ps(r0, r0, _MM_SHUFFLE(3, 3, 3, 3));

		const __m128 m1 = l0 * _mm_shuffle_ps(r1, r1, _MM_SHUFFLE(0, 0, 0, 0))
		+ l1 * _mm_shuffle_ps(r1, r1, _MM_SHUFFLE(1, 1, 1, 1))
		+ l2 * _mm_shuffle_ps(r1, r1, _MM_SHUFFLE(2, 2, 2, 2))
		+ l3 * _mm_shuffle_ps(r1, r1, _MM_SHUFFLE(3, 3, 3, 3));

		const __m128 m2 = l0 * _mm_shuffle_ps(r2, r2, _MM_SHUFFLE(0, 0, 0, 0))
		+ l1 * _mm_shuffle_ps(r2, r2, _MM_SHUFFLE(1, 1, 1, 1))
		+ l2 * _mm_shuffle_ps(r2, r2, _MM_SHUFFLE(2, 2, 2, 2))
		+ l3 * _mm_shuffle_ps(r2, r2, _MM_SHUFFLE(3, 3, 3, 3));

		const __m128 m3 = l0 * _mm_shuffle_ps(r3, r3, _MM_SHUFFLE(0, 0, 0, 0))
		+ l1 * _mm_shuffle_ps(r3, r3, _MM_SHUFFLE(1, 1, 1, 1))
		+ l2 * _mm_shuffle_ps(r3, r3, _MM_SHUFFLE(2, 2, 2, 2))
		+ l3 * _mm_shuffle_ps(r3, r3, _MM_SHUFFLE(3, 3, 3, 3));

		Matrix4 m;
		_mm_store_ps(&m.m[0], m0);
		_mm_store_ps(&m.m[4], m1);
		_mm_store_ps(&m.m[8], m2);
		_mm_store_ps(&m.m[12], m3);
		return m;

	#else
		Matrix4 m;

		m.m[0]  = matrixLeft.m[0] * matrixRight.m[0]  + matrixLeft.m[4] * matrixRight.m[1]  + matrixLeft.m[8] * matrixRight.m[2]   + matrixLeft.m[12] * matrixRight.m[3];
		m.m[4]  = matrixLeft.m[0] * matrixRight.m[4]  + matrixLeft.m[4] * matrixRight.m[5]  + matrixLeft.m[8] * matrixRight.m[6]   + matrixLeft.m[12] * matrixRight.m[7];
		m.m[8]  = matrixLeft.m[0] * matrixRight.m[8]  + matrixLeft.m[4] * matrixRight.m[9]  + matrixLeft.m[8] * matrixRight.m[10]  + matrixLeft.m[12] * matrixRight.m[11];
		m.m[12] = matrixLeft.m[0] * matrixRight.m[12] + matrixLeft.m[4] * matrixRight.m[13] + matrixLeft.m[8] * matrixRight.m[14]  + matrixLeft.m[12] * matrixRight.m[15];

		m.m[1]  = matrixLeft.m[1] * matrixRight.m[0]  + matrixLeft.m[5] * matrixRight.m[1]  + matrixLeft.m[9] * matrixRight.m[2]   + matrixLeft.m[13] * matrixRight.m[3];
		m.m[5]  = matrixLeft.m[1] * matrixRight.m[4]  + matrixLeft.m[5] * matrixRight.m[5]  + matrixLeft.m[9] * matrixRight.m[6]   + matrixLeft.m[13] * matrixRight.m[7];
		m.m[9]  = matrixLeft.m[1] * matrixRight.m[8]  + matrixLeft.m[5] * matrixRight.m[9]  + matrixLeft.m[9] * matrixRight.m[10]  + matrixLeft.m[13] * matrixRight.m[11];
		m.m[13] = matrixLeft.m[1] * matrixRight.m[12] + matrixLeft.m[5] * matrixRight.m[13] + matrixLeft.m[9] * matrixRight.m[14]  + matrixLeft.m[13] * matrixRight.m[15];

		m.m[2]  = matrixLeft.m[2] * matrixRight.m[0]  + matrixLeft.m[6] * matrixRight.m[1]  + matrixLeft.m[10] * matrixRight.m[2]  + matrixLeft.m[14] * matrixRight.m[3];
		m.m[6]  = matrixLeft.m[2] * matrixRight.m[4]  + matrixLeft.m[6] * matrixRight.m[5]  + matrixLeft.m[10] * matrixRight.m[6]  + matrixLeft.m[14] * matrixRight.m[7];
		m.m[10] = matrixLeft.m[2] * matrixRight.m[8]  + matrixLeft.m[6] * matrixRight.m[9]  + matrixLeft.m[10] * matrixRight.m[10] + matrixLeft.m[14] * matrixRight.m[11];
		m.m[14] = matrixLeft.m[2] * matrixRight.m[12] + matrixLeft.m[6] * matrixRight.m[13] + matrixLeft.m[10] * matrixRight.m[14] + matrixLeft.m[14] * matrixRight.m[15];

		m.m[3]  = matrixLeft.m[3] * matrixRight.m[0]  + matrixLeft.m[7] * matrixRight.m[1]  + matrixLeft.m[11] * matrixRight.m[2]  + matrixLeft.m[15] * matrixRight.m[3];
		m.m[7]  = matrixLeft.m[3] * matrixRight.m[4]  + matrixLeft.m[7] * matrixRight.m[5]  + matrixLeft.m[11] * matrixRight.m[6]  + matrixLeft.m[15] * matrixRight.m[7];
		m.m[11] = matrixLeft.m[3] * matrixRight.m[8]  + matrixLeft.m[7] * matrixRight.m[9]  + matrixLeft.m[11] * matrixRight.m[10] + matrixLeft.m[15] * matrixRight.m[11];
		m.m[15] = matrixLeft.m[3] * matrixRight.m[12] + matrixLeft.m[7] * matrixRight.m[13] + matrixLeft.m[11] * matrixRight.m[14] + matrixLeft.m[15] * matrixRight.m[15];

		return m;
	#endif
	}

	inline Matrix4 Matrix4Translate(Matrix4 matrix, float tx, float ty, float tz)
	{
		Matrix4 m = { matrix.m[0], matrix.m[1], matrix.m[2], matrix.m[3],
			matrix.m[4], matrix.m[5], matrix.m[6], matrix.m[7],
			matrix.m[8], matrix.m[9], matrix.m[10], matrix.m[11],
			matrix.m[0] * tx + matrix.m[4] * ty + matrix.m[8] * tz + matrix.m[12],
			matrix.m[1] * tx + matrix.m[5] * ty + matrix.m[9] * tz + matrix.m[13],
			matrix.m[2] * tx + matrix.m[6] * ty + matrix.m[10] * tz + matrix.m[14],
			matrix.m[3] * tx + matrix.m[7] * ty + matrix.m[11] * tz + matrix.m[15] };
		return m;
	}

	inline Matrix4 Matrix4TranslateWithVector3(Matrix4 matrix, Vector3 translationVector)
	{
		Matrix4 m = { matrix.m[0], matrix.m[1], matrix.m[2], matrix.m[3],
			matrix.m[4], matrix.m[5], matrix.m[6], matrix.m[7],
			matrix.m[8], matrix.m[9], matrix.m[10], matrix.m[11],
			matrix.m[0] * translationVector.v[0] + matrix.m[4] * translationVector.v[1] + matrix.m[8] * translationVector.v[2] + matrix.m[12],
			matrix.m[1] * translationVector.v[0] + matrix.m[5] * translationVector.v[1] + matrix.m[9] * translationVector.v[2] + matrix.m[13],
			matrix.m[2] * translationVector.v[0] + matrix.m[6] * translationVector.v[1] + matrix.m[10] * translationVector.v[2] + matrix.m[14],
			matrix.m[3] * translationVector.v[0] + matrix.m[7] * translationVector.v[1] + matrix.m[11] * translationVector.v[2] + matrix.m[15] };
		return m;
	}

	inline Matrix4 Matrix4TranslateWithVector4(Matrix4 matrix, Vector4 translationVector)
	{
		Matrix4 m = { matrix.m[0], matrix.m[1], matrix.m[2], matrix.m[3],
			matrix.m[4], matrix.m[5], matrix.m[6], matrix.m[7],
			matrix.m[8], matrix.m[9], matrix.m[10], matrix.m[11],
			matrix.m[0] * translationVector.v[0] + matrix.m[4] * translationVector.v[1] + matrix.m[8] * translationVector.v[2] + matrix.m[12],
			matrix.m[1] * translationVector.v[0] + matrix.m[5] * translationVector.v[1] + matrix.m[9] * translationVector.v[2] + matrix.m[13],
			matrix.m[2] * translationVector.v[0] + matrix.m[6] * translationVector.v[1] + matrix.m[10] * translationVector.v[2] + matrix.m[14],
			matrix.m[3] * translationVector.v[0] + matrix.m[7] * translationVector.v[1] + matrix.m[11] * translationVector.v[2] + matrix.m[15] };
		return m;
	}

	inline Matrix4 Matrix4Scale(Matrix4 matrix, float sx, float sy, float sz)
	{
	#if   defined(_SSE3_INTRINSICS)
		Matrix4 m;

		_mm_store_ps(&m.m[0],  _mm_load_ps(&matrix.m[0])  * _mm_load1_ps(&sx));
		_mm_store_ps(&m.m[4],  _mm_load_ps(&matrix.m[4])  * _mm_load1_ps(&sy));
		_mm_store_ps(&m.m[8],  _mm_load_ps(&matrix.m[8])  * _mm_load1_ps(&sz));
		_mm_store_ps(&m.m[12], _mm_load_ps(&matrix.m[12]));

		return m;
	#else
		Matrix4 m = { matrix.m[0] * sx, matrix.m[1] * sx, matrix.m[2] * sx, matrix.m[3] * sx,
			matrix.m[4] * sy, matrix.m[5] * sy, matrix.m[6] * sy, matrix.m[7] * sy,
			matrix.m[8] * sz, matrix.m[9] * sz, matrix.m[10] * sz, matrix.m[11] * sz,
			matrix.m[12], matrix.m[13], matrix.m[14], matrix.m[15] };
		return m;
	#endif
	}

	inline Matrix4 Matrix4ScaleWithVector3(Matrix4 matrix, Vector3 scaleVector)
	{
	#if   defined(_SSE3_INTRINSICS)
		Matrix4 m;

		_mm_store_ps(&m.m[0],  _mm_load_ps(&matrix.m[0])  * _mm_load1_ps(&scaleVector.v[0]));
		_mm_store_ps(&m.m[4],  _mm_load_ps(&matrix.m[4])  * _mm_load1_ps(&scaleVector.v[1]));
		_mm_store_ps(&m.m[8],  _mm_load_ps(&matrix.m[8])  * _mm_load1_ps(&scaleVector.v[2]));
		_mm_store_ps(&m.m[12], _mm_load_ps(&matrix.m[12]));

		return m;
	#else
		Matrix4 m = { matrix.m[0] * scaleVector.v[0], matrix.m[1] * scaleVector.v[0], matrix.m[2] * scaleVector.v[0], matrix.m[3] * scaleVector.v[0],
			matrix.m[4] * scaleVector.v[1], matrix.m[5] * scaleVector.v[1], matrix.m[6] * scaleVector.v[1], matrix.m[7] * scaleVector.v[1],
			matrix.m[8] * scaleVector.v[2], matrix.m[9] * scaleVector.v[2], matrix.m[10] * scaleVector.v[2], matrix.m[11] * scaleVector.v[2],
			matrix.m[12], matrix.m[13], matrix.m[14], matrix.m[15] };
		return m;
	#endif
	}

	inline Matrix4 Matrix4ScaleWithVector4(Matrix4 matrix, Vector4 scaleVector)
	{
	#if   defined(_SSE3_INTRINSICS)
		Matrix4 m;

		_mm_store_ps(&m.m[0],  _mm_load_ps(&matrix.m[0])  * _mm_load1_ps(&scaleVector.v[0]));
		_mm_store_ps(&m.m[4],  _mm_load_ps(&matrix.m[4])  * _mm_load1_ps(&scaleVector.v[1]));
		_mm_store_ps(&m.m[8],  _mm_load_ps(&matrix.m[8])  * _mm_load1_ps(&scaleVector.v[2]));
		_mm_store_ps(&m.m[12], _mm_load_ps(&matrix.m[12]));

		return m;
	#else
		Matrix4 m = { matrix.m[0] * scaleVector.v[0], matrix.m[1] * scaleVector.v[0], matrix.m[2] * scaleVector.v[0], matrix.m[3] * scaleVector.v[0],
			matrix.m[4] * scaleVector.v[1], matrix.m[5] * scaleVector.v[1], matrix.m[6] * scaleVector.v[1], matrix.m[7] * scaleVector.v[1],
			matrix.m[8] * scaleVector.v[2], matrix.m[9] * scaleVector.v[2], matrix.m[10] * scaleVector.v[2], matrix.m[11] * scaleVector.v[2],
			matrix.m[12], matrix.m[13], matrix.m[14], matrix.m[15] };
		return m;
	#endif
	}

	inline Matrix4 Matrix4Rotate(Matrix4 matrix, float radians, float x, float y, float z)
	{
		Matrix4 rm = Matrix4MakeRotation(radians, x, y, z);
		return Matrix4Multiply(matrix, rm);
	}

	inline Matrix4 Matrix4RotateWithVector3(Matrix4 matrix, float radians, Vector3 axisVector)
	{
		Matrix4 rm = Matrix4MakeRotation(radians, axisVector.v[0], axisVector.v[1], axisVector.v[2]);
		return Matrix4Multiply(matrix, rm);
	}

	inline Matrix4 Matrix4RotateWithVector4(Matrix4 matrix, float radians, Vector4 axisVector)
	{
		Matrix4 rm = Matrix4MakeRotation(radians, axisVector.v[0], axisVector.v[1], axisVector.v[2]);
		return Matrix4Multiply(matrix, rm);
	}

	inline Matrix4 Matrix4RotateX(Matrix4 matrix, float radians)
	{
		Matrix4 rm = Matrix4MakeXRotation(radians);
		return Matrix4Multiply(matrix, rm);
	}

	inline Matrix4 Matrix4RotateY(Matrix4 matrix, float radians)
	{
		Matrix4 rm = Matrix4MakeYRotation(radians);
		return Matrix4Multiply(matrix, rm);
	}

	inline Matrix4 Matrix4RotateZ(Matrix4 matrix, float radians)
	{
		Matrix4 rm = Matrix4MakeZRotation(radians);
		return Matrix4Multiply(matrix, rm);
	}

	inline Vector3 Matrix4MultiplyVector3(Matrix4 matrixLeft, Vector3 vectorRight)
	{
		Vector4 v4 = Matrix4MultiplyVector4(matrixLeft, Vector4Make(vectorRight.v[0], vectorRight.v[1], vectorRight.v[2], 0.0f));
		return Vector3Make(v4.v[0], v4.v[1], v4.v[2]);
	}

	inline Vector3 Matrix4MultiplyVector3WithTranslation(Matrix4 matrixLeft, Vector3 vectorRight)
	{
		Vector4 v4 = Matrix4MultiplyVector4(matrixLeft, Vector4Make(vectorRight.v[0], vectorRight.v[1], vectorRight.v[2], 1.0f));
		return Vector3Make(v4.v[0], v4.v[1], v4.v[2]);
	}

	inline Vector3 Matrix4MultiplyAndProjectVector3(Matrix4 matrixLeft, Vector3 vectorRight)
	{
		Vector4 v4 = Matrix4MultiplyVector4(matrixLeft, Vector4Make(vectorRight.v[0], vectorRight.v[1], vectorRight.v[2], 1.0f));
		return Vector3Make(v4.v[0], v4.v[1], v4.v[2]) * 1.0f / v4.v[3];
	}

	inline void Matrix4MultiplyVector3ArrayWithTranslation(Matrix4 matrix, Vector3 *__nonnull vectors, size_t vectorCount)
	{
		int i;
		for (i=0; i < vectorCount; i++)
			vectors[i] = Matrix4MultiplyVector3WithTranslation(matrix, vectors[i]);
	}

	inline void Matrix4MultiplyAndProjectVector3Array(Matrix4 matrix, Vector3 *__nonnull vectors, size_t vectorCount)
	{
		int i;
		for (i=0; i < vectorCount; i++)
			vectors[i] = Matrix4MultiplyAndProjectVector3(matrix, vectors[i]);
	}

	inline Vector4 Matrix4MultiplyVector4(Matrix4 matrixLeft, Vector4 vectorRight)
	{
	#if   defined(_SSE3_INTRINSICS)
		const __m128 v = _mm_load_ps(&vectorRight.v[0]);

		const __m128 r = _mm_load_ps(&matrixLeft.m[0])  * _mm_shuffle_ps(v, v, _MM_SHUFFLE(0, 0, 0, 0))
		+ _mm_load_ps(&matrixLeft.m[4])  * _mm_shuffle_ps(v, v, _MM_SHUFFLE(1, 1, 1, 1))
		+ _mm_load_ps(&matrixLeft.m[8])  * _mm_shuffle_ps(v, v, _MM_SHUFFLE(2, 2, 2, 2))
		+ _mm_load_ps(&matrixLeft.m[12]) * _mm_shuffle_ps(v, v, _MM_SHUFFLE(3, 3, 3, 3));

		Vector4 ret;
		*(__m128*)&ret = r;
		return ret;
	#else
		Vector4 v = { matrixLeft.m[0] * vectorRight.v[0] + matrixLeft.m[4] * vectorRight.v[1] + matrixLeft.m[8] * vectorRight.v[2] + matrixLeft.m[12] * vectorRight.v[3],
			matrixLeft.m[1] * vectorRight.v[0] + matrixLeft.m[5] * vectorRight.v[1] + matrixLeft.m[9] * vectorRight.v[2] + matrixLeft.m[13] * vectorRight.v[3],
			matrixLeft.m[2] * vectorRight.v[0] + matrixLeft.m[6] * vectorRight.v[1] + matrixLeft.m[10] * vectorRight.v[2] + matrixLeft.m[14] * vectorRight.v[3],
			matrixLeft.m[3] * vectorRight.v[0] + matrixLeft.m[7] * vectorRight.v[1] + matrixLeft.m[11] * vectorRight.v[2] + matrixLeft.m[15] * vectorRight.v[3] };
		return v;
	#endif
	}

	inline void Matrix4MultiplyVector4Array(Matrix4 matrix, Vector4 *__nonnull vectors, size_t vectorCount)
	{
		int i;
		for (i=0; i < vectorCount; i++)
			vectors[i] = Matrix4MultiplyVector4(matrix, vectors[i]);
	}

}

inline Zen::Matrix3 operator+(Zen::Matrix3 matrixLeft, Zen::Matrix3 matrixRight)
{
#if defined(GLK_SSE3_INTRINSICS)
	Zen::Matrix3 m;

	_mm_storeu_ps(&m.m[0], _mm_loadu_ps(&matrixLeft.m[0]) + _mm_loadu_ps(&matrixRight.m[0]));
	_mm_storeu_ps(&m.m[4], _mm_loadu_ps(&matrixLeft.m[4]) + _mm_loadu_ps(&matrixRight.m[4]));
	m.m[8] = matrixLeft.m[8] + matrixRight.m[8];

	return m;
#else
	Zen::Matrix3 m;

	m.m[0] = matrixLeft.m[0] + matrixRight.m[0];
	m.m[1] = matrixLeft.m[1] + matrixRight.m[1];
	m.m[2] = matrixLeft.m[2] + matrixRight.m[2];
	
	m.m[3] = matrixLeft.m[3] + matrixRight.m[3];
	m.m[4] = matrixLeft.m[4] + matrixRight.m[4];
	m.m[5] = matrixLeft.m[5] + matrixRight.m[5];

	m.m[6] = matrixLeft.m[6] + matrixRight.m[6];
	m.m[7] = matrixLeft.m[7] + matrixRight.m[7];
	m.m[8] = matrixLeft.m[8] + matrixRight.m[8];

	return m;
#endif
}

inline Zen::Matrix3 operator-(Zen::Matrix3 matrixLeft, Zen::Matrix3 matrixRight)
{
#if defined(GLK_SSE3_INTRINSICS)
	Zen::Matrix3 m;

	_mm_storeu_ps(&m.m[0], _mm_loadu_ps(&matrixLeft.m[0]) - _mm_loadu_ps(&matrixRight.m[0]));
	_mm_storeu_ps(&m.m[4], _mm_loadu_ps(&matrixLeft.m[4]) - _mm_loadu_ps(&matrixRight.m[4]));
	m.m[8] = matrixLeft.m[8] - matrixRight.m[8];

	return m;
#else
	Zen::Matrix3 m;

	m.m[0] = matrixLeft.m[0] - matrixRight.m[0];
	m.m[1] = matrixLeft.m[1] - matrixRight.m[1];
	m.m[2] = matrixLeft.m[2] - matrixRight.m[2];

	m.m[3] = matrixLeft.m[3] - matrixRight.m[3];
	m.m[4] = matrixLeft.m[4] - matrixRight.m[4];
	m.m[5] = matrixLeft.m[5] - matrixRight.m[5];

	m.m[6] = matrixLeft.m[6] - matrixRight.m[6];
	m.m[7] = matrixLeft.m[7] - matrixRight.m[7];
	m.m[8] = matrixLeft.m[8] - matrixRight.m[8];

	return m;
#endif
}
inline Zen::Matrix3 operator*(Zen::Matrix3 matrixLeft, Zen::Matrix3 matrixRight)
{
#if defined(GLK_SSE3_INTRINSICS)
	Zen::Matrix3 m;

	_mm_storeu_ps(&m.m[0], _mm_loadu_ps(&matrixLeft.m[0]) * _mm_loadu_ps(&matrixRight.m[0]));
	_mm_storeu_ps(&m.m[4], _mm_loadu_ps(&matrixLeft.m[4]) * _mm_loadu_ps(&matrixRight.m[4]));
	m.m[8] = matrixLeft.m[8] * matrixRight.m[8];

	return m;
#else
	Zen::Matrix3 m;

	m.m[0] = matrixLeft.m[0] * matrixRight.m[0];
	m.m[1] = matrixLeft.m[1] * matrixRight.m[1];
	m.m[2] = matrixLeft.m[2] * matrixRight.m[2];

	m.m[3] = matrixLeft.m[3] * matrixRight.m[3];
	m.m[4] = matrixLeft.m[4] * matrixRight.m[4];
	m.m[5] = matrixLeft.m[5] * matrixRight.m[5];

	m.m[6] = matrixLeft.m[6] * matrixRight.m[6];
	m.m[7] = matrixLeft.m[7] * matrixRight.m[7];
	m.m[8] = matrixLeft.m[8] * matrixRight.m[8];

	return m;
#endif
}

inline Zen::Matrix3 operator/(Zen::Matrix3 matrixLeft, Zen::Matrix3 matrixRight)
{
#if defined(GLK_SSE3_INTRINSICS)
	Zen::Matrix3 m;

	_mm_storeu_ps(&m.m[0], _mm_loadu_ps(&matrixLeft.m[0]) / _mm_loadu_ps(&matrixRight.m[0]));
	_mm_storeu_ps(&m.m[4], _mm_loadu_ps(&matrixLeft.m[4]) / _mm_loadu_ps(&matrixRight.m[4]));
	m.m[8] = matrixLeft.m[8] / matrixRight.m[8];

	return m;
#else
	Zen::Matrix3 m;

	m.m[0] = matrixLeft.m[0] / matrixRight.m[0];
	m.m[1] = matrixLeft.m[1] / matrixRight.m[1];
	m.m[2] = matrixLeft.m[2] / matrixRight.m[2];

	m.m[3] = matrixLeft.m[3] / matrixRight.m[3];
	m.m[4] = matrixLeft.m[4] / matrixRight.m[4];
	m.m[5] = matrixLeft.m[5] / matrixRight.m[5];

	m.m[6] = matrixLeft.m[6] / matrixRight.m[6];
	m.m[7] = matrixLeft.m[7] / matrixRight.m[7];
	m.m[8] = matrixLeft.m[8] / matrixRight.m[8];

	return m;
#endif
}

inline Zen::Matrix4 operator+(Zen::Matrix4 matrixLeft, Zen::Matrix4 matrixRight)
{
#if   defined(_SSE3_INTRINSICS)
	Zen::Matrix4 m;

	_mm_store_ps(&m.m[0],  _mm_load_ps(&matrixLeft.m[0])  + _mm_load_ps(&matrixRight.m[0]));
	_mm_store_ps(&m.m[4],  _mm_load_ps(&matrixLeft.m[4])  + _mm_load_ps(&matrixRight.m[4]));
	_mm_store_ps(&m.m[8],  _mm_load_ps(&matrixLeft.m[8])  + _mm_load_ps(&matrixRight.m[8]));
	_mm_store_ps(&m.m[12], _mm_load_ps(&matrixLeft.m[12]) + _mm_load_ps(&matrixRight.m[12]));

	return m;
#else
	Zen::Matrix4 m;

	m.m[0] = matrixLeft.m[0] + matrixRight.m[0];
	m.m[1] = matrixLeft.m[1] + matrixRight.m[1];
	m.m[2] = matrixLeft.m[2] + matrixRight.m[2];
	m.m[3] = matrixLeft.m[3] + matrixRight.m[3];

	m.m[4] = matrixLeft.m[4] + matrixRight.m[4];
	m.m[5] = matrixLeft.m[5] + matrixRight.m[5];
	m.m[6] = matrixLeft.m[6] + matrixRight.m[6];
	m.m[7] = matrixLeft.m[7] + matrixRight.m[7];

	m.m[8] = matrixLeft.m[8] + matrixRight.m[8];
	m.m[9] = matrixLeft.m[9] + matrixRight.m[9];
	m.m[10] = matrixLeft.m[10] + matrixRight.m[10];
	m.m[11] = matrixLeft.m[11] + matrixRight.m[11];

	m.m[12] = matrixLeft.m[12] + matrixRight.m[12];
	m.m[13] = matrixLeft.m[13] + matrixRight.m[13];
	m.m[14] = matrixLeft.m[14] + matrixRight.m[14];
	m.m[15] = matrixLeft.m[15] + matrixRight.m[15];

	return m;
#endif
}

inline Zen::Matrix4 operator-(Zen::Matrix4 matrixLeft, Zen::Matrix4 matrixRight)
{
#if   defined(_SSE3_INTRINSICS)
	Zen::Matrix4 m;

	_mm_store_ps(&m.m[0],  _mm_load_ps(&matrixLeft.m[0])  - _mm_load_ps(&matrixRight.m[0]));
	_mm_store_ps(&m.m[4],  _mm_load_ps(&matrixLeft.m[4])  - _mm_load_ps(&matrixRight.m[4]));
	_mm_store_ps(&m.m[8],  _mm_load_ps(&matrixLeft.m[8])  - _mm_load_ps(&matrixRight.m[8]));
	_mm_store_ps(&m.m[12], _mm_load_ps(&matrixLeft.m[12]) - _mm_load_ps(&matrixRight.m[12]));

	return m;
#else
	Zen::Matrix4 m;

	m.m[0] = matrixLeft.m[0] - matrixRight.m[0];
	m.m[1] = matrixLeft.m[1] - matrixRight.m[1];
	m.m[2] = matrixLeft.m[2] - matrixRight.m[2];
	m.m[3] = matrixLeft.m[3] - matrixRight.m[3];

	m.m[4] = matrixLeft.m[4] - matrixRight.m[4];
	m.m[5] = matrixLeft.m[5] - matrixRight.m[5];
	m.m[6] = matrixLeft.m[6] - matrixRight.m[6];
	m.m[7] = matrixLeft.m[7] - matrixRight.m[7];

	m.m[8] = matrixLeft.m[8] - matrixRight.m[8];
	m.m[9] = matrixLeft.m[9] - matrixRight.m[9];
	m.m[10] = matrixLeft.m[10] - matrixRight.m[10];
	m.m[11] = matrixLeft.m[11] - matrixRight.m[11];

	m.m[12] = matrixLeft.m[12] - matrixRight.m[12];
	m.m[13] = matrixLeft.m[13] - matrixRight.m[13];
	m.m[14] = matrixLeft.m[14] - matrixRight.m[14];
	m.m[15] = matrixLeft.m[15] - matrixRight.m[15];

	return m;
#endif
}

inline Zen::Matrix4 operator*(Zen::Matrix4 matrixLeft, Zen::Matrix4 matrixRight)
{
#if   defined(_SSE3_INTRINSICS)
	Zen::Matrix4 m;

	_mm_store_ps(&m.m[0],  _mm_load_ps(&matrixLeft.m[0])  * _mm_load_ps(&matrixRight.m[0]));
	_mm_store_ps(&m.m[4],  _mm_load_ps(&matrixLeft.m[4])  * _mm_load_ps(&matrixRight.m[4]));
	_mm_store_ps(&m.m[8],  _mm_load_ps(&matrixLeft.m[8])  * _mm_load_ps(&matrixRight.m[8]));
	_mm_store_ps(&m.m[12], _mm_load_ps(&matrixLeft.m[12]) * _mm_load_ps(&matrixRight.m[12]));

	return m;
#else
	Zen::Matrix4 m;

	m.m[0] = matrixLeft.m[0] * matrixRight.m[0];
	m.m[1] = matrixLeft.m[1] * matrixRight.m[1];
	m.m[2] = matrixLeft.m[2] * matrixRight.m[2];
	m.m[3] = matrixLeft.m[3] * matrixRight.m[3];

	m.m[4] = matrixLeft.m[4] * matrixRight.m[4];
	m.m[5] = matrixLeft.m[5] * matrixRight.m[5];
	m.m[6] = matrixLeft.m[6] * matrixRight.m[6];
	m.m[7] = matrixLeft.m[7] * matrixRight.m[7];

	m.m[8] = matrixLeft.m[8] * matrixRight.m[8];
	m.m[9] = matrixLeft.m[9] * matrixRight.m[9];
	m.m[10] = matrixLeft.m[10] * matrixRight.m[10];
	m.m[11] = matrixLeft.m[11] * matrixRight.m[11];

	m.m[12] = matrixLeft.m[12] * matrixRight.m[12];
	m.m[13] = matrixLeft.m[13] * matrixRight.m[13];
	m.m[14] = matrixLeft.m[14] * matrixRight.m[14];
	m.m[15] = matrixLeft.m[15] * matrixRight.m[15];

	return m;
#endif
}

inline Zen::Matrix4 operator/(Zen::Matrix4 matrixLeft, Zen::Matrix4 matrixRight)
{
#if   defined(_SSE3_INTRINSICS)
	Zen::Matrix4 m;

	_mm_store_ps(&m.m[0],  _mm_load_ps(&matrixLeft.m[0])  / _mm_load_ps(&matrixRight.m[0]));
	_mm_store_ps(&m.m[4],  _mm_load_ps(&matrixLeft.m[4])  / _mm_load_ps(&matrixRight.m[4]));
	_mm_store_ps(&m.m[8],  _mm_load_ps(&matrixLeft.m[8])  / _mm_load_ps(&matrixRight.m[8]));
	_mm_store_ps(&m.m[12], _mm_load_ps(&matrixLeft.m[12]) / _mm_load_ps(&matrixRight.m[12]));

	return m;
#else
	Zen::Matrix4 m;

	m.m[0] = matrixLeft.m[0] / matrixRight.m[0];
	m.m[1] = matrixLeft.m[1] / matrixRight.m[1];
	m.m[2] = matrixLeft.m[2] / matrixRight.m[2];
	m.m[3] = matrixLeft.m[3] / matrixRight.m[3];

	m.m[4] = matrixLeft.m[4] / matrixRight.m[4];
	m.m[5] = matrixLeft.m[5] / matrixRight.m[5];
	m.m[6] = matrixLeft.m[6] / matrixRight.m[6];
	m.m[7] = matrixLeft.m[7] / matrixRight.m[7];

	m.m[8] = matrixLeft.m[8] / matrixRight.m[8];
	m.m[9] = matrixLeft.m[9] / matrixRight.m[9];
	m.m[10] = matrixLeft.m[10] / matrixRight.m[10];
	m.m[11] = matrixLeft.m[11] / matrixRight.m[11];

	m.m[12] = matrixLeft.m[12] / matrixRight.m[12];
	m.m[13] = matrixLeft.m[13] / matrixRight.m[13];
	m.m[14] = matrixLeft.m[14] / matrixRight.m[14];
	m.m[15] = matrixLeft.m[15] / matrixRight.m[15];

	return m;
#endif
}

