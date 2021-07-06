/*
 Copyright (c) 2013 MeherTJ G. All rights reserved.
 License: LGPL for personnal study or free software.
 */

#pragma once

#include "zen_vector.h"
#include "zen_quaternion.h"
#include <memory>

namespace Zen {
	struct Matrix2 {
		float c[2][2];
		
		float & operator[](size_t i) {
			return c[0][i];
		}
		float const & operator[](size_t i) const {
			return c[0][i];
		}
	};
	
	struct Matrix3 {
		float c[3][3];
		
		float & operator[](size_t i) {
			return c[0][i];
		}
		float const & operator[](size_t i) const {
			return c[0][i];
		}
	};
	
	struct Matrix4 {
		float c[4][4];
		
		float & operator[](size_t i) {
			return c[0][i];
		}
		float const & operator[](size_t i) const {
			return c[0][i];
		}
	};
}

inline Zen::Matrix3 operator+(Zen::Matrix3 mL, Zen::Matrix3 mR);
inline Zen::Matrix3 operator-(Zen::Matrix3 mL, Zen::Matrix3 mR);
inline Zen::Matrix3 operator*(Zen::Matrix3 mL, Zen::Matrix3 mR);
inline Zen::Matrix3 operator/(Zen::Matrix3 mL, Zen::Matrix3 mR);
inline Zen::Matrix4 operator+(Zen::Matrix4 mL, Zen::Matrix4 mR);
inline Zen::Matrix4 operator-(Zen::Matrix4 mL, Zen::Matrix4 mR);
inline Zen::Matrix4 operator*(Zen::Matrix4 mL, Zen::Matrix4 mR);
inline Zen::Matrix4 operator/(Zen::Matrix4 mL, Zen::Matrix4 mR);

namespace Zen {
	inline Matrix3 Matrix3Make(float m00, float m01, float m02,
							   float m10, float m11, float m12,
							   float m20, float m21, float m22);
	
	inline Matrix3 Matrix3MakeIdentity(float v = 1);
	
	inline Matrix3 Matrix3MakeAndTranspose(float m00, float m01, float m02,
										   float m10, float m11, float m12,
										   float m20, float m21, float m22);
	
	inline Matrix3 Matrix3MakeWithRows(Vector3 row0,
									   Vector3 row1,
									   Vector3 row2);
	
	inline Matrix3 Matrix3MakeWithColumns(Vector3 column0,
										  Vector3 column1,
										  Vector3 column2);
	
	inline Matrix3 Matrix3MakeWithQuaternion(Quaternion qua);
	
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
	
	inline Matrix3 Matrix3Multiply(Matrix3 mL, Matrix3 mR);
	
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
	
	inline Vector3 Matrix3Multiply(Matrix3 mL, Vector3 vR);
	
	/*
	 m30, m31, and m32 correspond to the translation values tx, ty, tz, respectively.
	 */
	inline Matrix4 Matrix4Make(float m00, float m01, float m02, float m03,
							   float m10, float m11, float m12, float m13,
							   float m20, float m21, float m22, float m23,
							   float m30, float m31, float m32, float m33);
	
	
	inline Matrix4 Matrix4MakeIdentity(float v = 1);
	/*
	 m03, m13, and m23 correspond to the translation values tx, ty, tz, respectively.
	 */
	inline Matrix4 Matrix4MakeAndTranspose(float m00, float m01, float m02, float m03,
										   float m10, float m11, float m12, float m13,
										   float m20, float m21, float m22, float m23,
										   float m30, float m31, float m32, float m33);
	
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
	 The qua will be normalized before conversion.
	 */
	inline Matrix4 Matrix4MakeWithQuaternion(Quaternion qua);
	
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
	
	inline Matrix4 Matrix4Multiply(Matrix4 mL, Matrix4 mR);
	
	inline Matrix4 Matrix4Add(Matrix4 mL, Matrix4 mR);
	inline Matrix4 Matrix4Subtract(Matrix4 mL, Matrix4 mR);
	
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
	inline Vector3 Matrix4MultiplyVector3(Matrix4 mL, Vector3 vR);
	/*
	 Assumes 1 in the w component.
	 */
	inline Vector3 Matrix4MultiplyVector3WithTranslation(Matrix4 mL, Vector3 vR);
	/*
	 Assumes 1 in the w component and divides the resulting vector by w before returning.
	 */
	inline Vector3 Matrix4MultiplyAndProjectVector3(Matrix4 mL, Vector3 vR);
	
	/*
	 Assumes 0 in the w component.
	 */
	inline void Matrix4MultiplyVector3Array(Matrix4 matrix, Vector3 * vectors, size_t vectorCount);
	/*
	 Assumes 1 in the w component.
	 */
	inline void Matrix4MultiplyVector3ArrayWithTranslation(Matrix4 matrix, Vector3 * vectors, size_t vectorCount);
	/*
	 Assumes 1 in the w component and divides the resulting vector by w before returning.
	 */
	inline void Matrix4MultiplyAndProjectVector3Array(Matrix4 matrix, Vector3 * vectors, size_t vectorCount);
	
	inline Vector4 Matrix4MultiplyVector4(Matrix4 mL, Vector4 vR);
	
	
	inline Matrix3 Matrix3Make(float m00, float m01, float m02,
							   float m10, float m11, float m12,
							   float m20, float m21, float m22)
	{
		Matrix3 m = { m00, m01, m02,
			m10, m11, m12,
			m20, m21, m22 };
		return m;
	}
	
	inline Matrix3 Matrix3MakeIdentity(float v)
	{
		return Matrix3{v, 0, 0, 0, v, 0, 0, 0, v};
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
	
	inline Matrix3 Matrix3MakeWithRows(Vector3 row0,
									   Vector3 row1,
									   Vector3 row2)
	{
		Matrix3 m = { row0[0], row1[0], row2[0],
			row0[1], row1[1], row2[1],
			row0[2], row1[2], row2[2] };
		return m;
	}
	
	inline Matrix3 Matrix3MakeWithColumns(Vector3 column0,
										  Vector3 column1,
										  Vector3 column2)
	{
		Matrix3 m = { column0[0], column0[1], column0[2],
			column1[0], column1[1], column1[2],
			column2[0], column2[1], column2[2] };
		return m;
	}
	inline Matrix3 Matrix3MakeWithQuaternion(Quaternion qua)
	{
		qua = QuaternionNormalize(qua);
		
		float x = qua.q[0];
		float y = qua.q[1];
		float z = qua.q[2];
		float w = qua.q[3];
		
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
	
	inline Matrix3 Matrix3MakeScale(float sx, float sy, float sz)
	{
		Matrix3 m = Matrix3MakeIdentity();
		m[0] = sx;
		m[4] = sy;
		m[8] = sz;
		return m;
	}
	
	inline Matrix3 Matrix3MakeRotation(float radians, float x, float y, float z)
	{
		Vector3 v = Vector3Normalize(Vector3Make(x, y, z));
		float cos = cosf(radians);
		float cosp = 1.0f - cos;
		float sin = sinf(radians);
		
		Matrix3 m = { cos + cosp * v[0] * v[0],
			cosp * v[0] * v[1] + v[2] * sin,
			cosp * v[0] * v[2] - v[1] * sin,
			
			cosp * v[0] * v[1] - v[2] * sin,
			cos + cosp * v[1] * v[1],
			cosp * v[1] * v[2] + v[0] * sin,
			
			cosp * v[0] * v[2] + v[1] * sin,
			cosp * v[1] * v[2] - v[0] * sin,
			cos + cosp * v[2] * v[2] };
		
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
		Matrix2 m = { matrix[0], matrix[1],
			matrix[3], matrix[4] };
		return m;
	}
	
	inline Vector3 Matrix3GetRow(Matrix3 matrix, int row)
	{
		Vector3 v = { matrix[row], matrix[3 + row], matrix[6 + row] };
		return v;
	}
	
	inline Vector3 Matrix3GetColumn(Matrix3 matrix, int column)
	{
		Vector3 v = { matrix[column * 3 + 0], matrix[column * 3 + 1], matrix[column * 3 + 2] };
		return v;
	}
	
	inline Matrix3 Matrix3SetRow(Matrix3 matrix, int row, Vector3 vector)
	{
		matrix[row] = vector[0];
		matrix[row + 3] = vector[1];
		matrix[row + 6] = vector[2];
		
		return matrix;
	}
	
	inline Matrix3 Matrix3SetColumn(Matrix3 matrix, int column, Vector3 vector)
	{
		matrix[column * 3 + 0] = vector[0];
		matrix[column * 3 + 1] = vector[1];
		matrix[column * 3 + 2] = vector[2];
		
		return matrix;
	}
	
	inline Matrix3 Matrix3Transpose(Matrix3 matrix)
	{
		Matrix3 m = { matrix[0], matrix[3], matrix[6],
			matrix[1], matrix[4], matrix[7],
			matrix[2], matrix[5], matrix[8] };
		return m;
	}
	
	inline Matrix3 Matrix3Multiply(Matrix3 mL, Matrix3 mR)
	{
		Matrix3 m;
		
		m[0] = mL[0] * mR[0] + mL[3] * mR[1] + mL[6] * mR[2];
		m[3] = mL[0] * mR[3] + mL[3] * mR[4] + mL[6] * mR[5];
		m[6] = mL[0] * mR[6] + mL[3] * mR[7] + mL[6] * mR[8];
		
		m[1] = mL[1] * mR[0] + mL[4] * mR[1] + mL[7] * mR[2];
		m[4] = mL[1] * mR[3] + mL[4] * mR[4] + mL[7] * mR[5];
		m[7] = mL[1] * mR[6] + mL[4] * mR[7] + mL[7] * mR[8];
		
		m[2] = mL[2] * mR[0] + mL[5] * mR[1] + mL[8] * mR[2];
		m[5] = mL[2] * mR[3] + mL[5] * mR[4] + mL[8] * mR[5];
		m[8] = mL[2] * mR[6] + mL[5] * mR[7] + mL[8] * mR[8];
		
		return m;
	}
	
	inline Matrix3 Matrix3Scale(Matrix3 matrix, float sx, float sy, float sz)
	{
		Matrix3 m = { matrix[0] * sx, matrix[1] * sx, matrix[2] * sx,
			matrix[3] * sy, matrix[4] * sy, matrix[5] * sy,
			matrix[6] * sz, matrix[7] * sz, matrix[8] * sz };
		return m;
	}
	
	inline Matrix3 Matrix3Scale(Matrix3 matrix, Vector3 scaleVector)
	{
		Matrix3 m = { matrix[0] * scaleVector[0], matrix[1] * scaleVector[0], matrix[2] * scaleVector[0],
			matrix[3] * scaleVector[1], matrix[4] * scaleVector[1], matrix[5] * scaleVector[1],
			matrix[6] * scaleVector[2], matrix[7] * scaleVector[2], matrix[8] * scaleVector[2] };
		return m;
	}
	
	inline Matrix3 Matrix3Scale(Matrix3 matrix, Vector4 scaleVector)
	{
		Matrix3 m = { matrix[0] * scaleVector[0], matrix[1] * scaleVector[0], matrix[2] * scaleVector[0],
			matrix[3] * scaleVector[1], matrix[4] * scaleVector[1], matrix[5] * scaleVector[1],
			matrix[6] * scaleVector[2], matrix[7] * scaleVector[2], matrix[8] * scaleVector[2] };
		return m;
	}
	
	inline Matrix3 Matrix3Rotate(Matrix3 matrix, float radians, float x, float y, float z)
	{
		Matrix3 rm = Matrix3MakeRotation(radians, x, y, z);
		return Matrix3Multiply(matrix, rm);
	}
	
	inline Matrix3 Matrix3Rotate(Matrix3 matrix, float radians, Vector3 axisVector)
	{
		Matrix3 rm = Matrix3MakeRotation(radians, axisVector[0], axisVector[1], axisVector[2]);
		return Matrix3Multiply(matrix, rm);
	}
	
	inline Matrix3 Matrix3Rotate(Matrix3 matrix, float radians, Vector4 axisVector)
	{
		Matrix3 rm = Matrix3MakeRotation(radians, axisVector[0], axisVector[1], axisVector[2]);
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
	
	inline Vector3 Matrix3Multiply(Matrix3 mL, Vector3 vR)
	{
		Vector3 v = { mL[0] * vR[0] + mL[3] * vR[1] + mL[6] * vR[2],
			mL[1] * vR[0] + mL[4] * vR[1] + mL[7] * vR[2],
			mL[2] * vR[0] + mL[5] * vR[1] + mL[8] * vR[2] };
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
	
	inline Matrix4 Matrix4MakeIdentity(float v)
	{
		return Matrix4{ 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 };
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
	
	inline Matrix4 Matrix4MakeWithRows(Vector4 row0,
									   Vector4 row1,
									   Vector4 row2,
									   Vector4 row3)
	{
		Matrix4 m = { row0[0], row1[0], row2[0], row3[0],
			row0[1], row1[1], row2[1], row3[1],
			row0[2], row1[2], row2[2], row3[2],
			row0[3], row1[3], row2[3], row3[3] };
		return m;
	}
	
	inline Matrix4 Matrix4MakeWithColumns(Vector4 column0,
										  Vector4 column1,
										  Vector4 column2,
										  Vector4 column3)
	{
		Matrix4 m = { column0[0], column0[1], column0[2], column0[3],
			column1[0], column1[1], column1[2], column1[3],
			column2[0], column2[1], column2[2], column2[3],
			column3[0], column3[1], column3[2], column3[3] };
		return m;
	}
	
	inline Matrix4 Matrix4MakeWithQuaternion(Quaternion qua)
	{
		qua = QuaternionNormalize(qua);
		
		float x = qua.q[0];
		float y = qua.q[1];
		float z = qua.q[2];
		float w = qua.q[3];
		
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
		Matrix4 m = {
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			tx, ty, tz, 1,
		};
		return m;
	}
	
	inline Matrix4 Matrix4MakeScale(float sx, float sy, float sz)
	{
		Matrix4 m = {
			sx, 0, 0, 0,
			0, sy, 0, 0,
			0, 0, sz, 0,
			0, 0, 0, 1,
		};
		return m;
	}
	
	inline Matrix4 Matrix4MakeRotation(float radians, float x, float y, float z)
	{
		//		Vector3 v = Vector3Normalize(axis);
		//		float cos = cosf(radians);
		//		float cosp = 1.0f - cos;
		//		float sin = sinf(radians);
		
		Vector3 axis = Vector3Normalize(Vector3{x, y, z});
		float ct = cosf(radians);
		float st = sinf(radians);
		float ci = 1 - ct;
		x = axis.x; y = axis.y; z = axis.z;
		
		return Matrix4{
			ct + x * x * ci, y * x * ci + z * st, z * x * ci - y * st, 0,
			x * y * ci - z * st, ct + y * y * ci, z * y * ci + x * st, 0,
			x * z * ci + y * st, y * z * ci - x * st, ct + z * z * ci, 0,
			0, 0, 0, 1,
		};
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
	
	inline Matrix4 Matrix4MakePerspective(float fovyRadians, float aspect, float /*nearZ*/ n, float /*farZ*/ f)
	{
		float ys = 1 / tanf(fovyRadians * 0.5);
		float xs = ys / aspect;
		float dz = n - f;
		float zs = (n+f) / dz;
		
		return {
			xs, 0, 0, 0,
			0, ys, 0, 0,
			0, 0, zs, -1,
			0, 0, 2 * f * n / dz, 0// 2 * f * n / dz - zs * n, +n,
		};
		
		/* move nearZ to zero.
		 return {
		 xs, 0, 0, 0,
		 0, ys, 0, 0,
		 0, 0, zs, -1,
		 0, 0, 2 * f * n / dz + 2 * f * n / dz - zs * n, +n,
		 };
		 */
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
		
		Matrix4 m = { u[0], v[0], n[0], 0.0f,
			u[1], v[1], n[1], 0.0f,
			u[2], v[2], n[2], 0.0f,
			Vector3DotProduct(-(u), ev),
			Vector3DotProduct(-(v), ev),
			Vector3DotProduct(-(n), ev),
			1.0f };
		
		return m;
	}
	
	inline Matrix3 Matrix4GetMatrix3(Matrix4 matrix)
	{
		Matrix3 m = { matrix[0], matrix[1], matrix[2],
			matrix[4], matrix[5], matrix[6],
			matrix[8], matrix[9], matrix[10] };
		return m;
	}
	
	inline Matrix2 Matrix4GetMatrix2(Matrix4 matrix)
	{
		Matrix2 m = { matrix[0], matrix[1],
			matrix[4], matrix[5] };
		return m;
	}
	
	inline Vector4 Matrix4GetRow(Matrix4 matrix, int row)
	{
		Vector4 v = { matrix[row], matrix[4 + row], matrix[8 + row], matrix[12 + row] };
		return v;
	}
	
	inline Vector4 Matrix4GetColumn(Matrix4 matrix, int column)
	{
		Vector4 v = { matrix[column * 4 + 0], matrix[column * 4 + 1], matrix[column * 4 + 2], matrix[column * 4 + 3] };
		return v;
	}
	
	inline Matrix4 Matrix4SetRow(Matrix4 matrix, int row, Vector4 vector)
	{
		matrix[row] = vector[0];
		matrix[row + 4] = vector[1];
		matrix[row + 8] = vector[2];
		matrix[row + 12] = vector[3];
		
		return matrix;
	}
	
	inline Matrix4 Matrix4SetColumn(Matrix4 matrix, int column, Vector4 vector)
	{
		matrix[column * 4 + 0] = vector[0];
		matrix[column * 4 + 1] = vector[1];
		matrix[column * 4 + 2] = vector[2];
		matrix[column * 4 + 3] = vector[3];
		
		return matrix;
	}
	
	inline Matrix4 Matrix4Transpose(Matrix4 matrix)
	{
		Matrix4 m = { matrix[0], matrix[4], matrix[8], matrix[12],
			matrix[1], matrix[5], matrix[9], matrix[13],
			matrix[2], matrix[6], matrix[10], matrix[14],
			matrix[3], matrix[7], matrix[11], matrix[15] };
		return m;
	}
	
	inline Matrix4 Matrix4Multiply(Matrix4 mL, Matrix4 mR)
	{
		Matrix4 m;
		
		m[0] = mL[0] * mR[0] + mL[4] * mR[1] + mL[8] * mR[2] + mL[12] * mR[3];
		m[4] = mL[0] * mR[4] + mL[4] * mR[5] + mL[8] * mR[6] + mL[12] * mR[7];
		m[8] = mL[0] * mR[8] + mL[4] * mR[9] + mL[8] * mR[10] + mL[12] * mR[11];
		m[12] = mL[0] * mR[12] + mL[4] * mR[13] + mL[8] * mR[14] + mL[12] * mR[15];
		
		m[1] = mL[1] * mR[0] + mL[5] * mR[1] + mL[9] * mR[2] + mL[13] * mR[3];
		m[5] = mL[1] * mR[4] + mL[5] * mR[5] + mL[9] * mR[6] + mL[13] * mR[7];
		m[9] = mL[1] * mR[8] + mL[5] * mR[9] + mL[9] * mR[10] + mL[13] * mR[11];
		m[13] = mL[1] * mR[12] + mL[5] * mR[13] + mL[9] * mR[14] + mL[13] * mR[15];
		
		m[2] = mL[2] * mR[0] + mL[6] * mR[1] + mL[10] * mR[2] + mL[14] * mR[3];
		m[6] = mL[2] * mR[4] + mL[6] * mR[5] + mL[10] * mR[6] + mL[14] * mR[7];
		m[10] = mL[2] * mR[8] + mL[6] * mR[9] + mL[10] * mR[10] + mL[14] * mR[11];
		m[14] = mL[2] * mR[12] + mL[6] * mR[13] + mL[10] * mR[14] + mL[14] * mR[15];
		
		m[3] = mL[3] * mR[0] + mL[7] * mR[1] + mL[11] * mR[2] + mL[15] * mR[3];
		m[7] = mL[3] * mR[4] + mL[7] * mR[5] + mL[11] * mR[6] + mL[15] * mR[7];
		m[11] = mL[3] * mR[8] + mL[7] * mR[9] + mL[11] * mR[10] + mL[15] * mR[11];
		m[15] = mL[3] * mR[12] + mL[7] * mR[13] + mL[11] * mR[14] + mL[15] * mR[15];
		
		return m;
	}
	
	inline Matrix4 Matrix4Translate(Matrix4 matrix, float tx, float ty, float tz)
	{
		Matrix4 m = { matrix[0], matrix[1], matrix[2], matrix[3],
			matrix[4], matrix[5], matrix[6], matrix[7],
			matrix[8], matrix[9], matrix[10], matrix[11],
			matrix[0] * tx + matrix[4] * ty + matrix[8] * tz + matrix[12],
			matrix[1] * tx + matrix[5] * ty + matrix[9] * tz + matrix[13],
			matrix[2] * tx + matrix[6] * ty + matrix[10] * tz + matrix[14],
			matrix[3] * tx + matrix[7] * ty + matrix[11] * tz + matrix[15] };
		return m;
	}
	
	inline Matrix4 Matrix4TranslateWithVector3(Matrix4 matrix, Vector3 translationVector)
	{
		Matrix4 m = { matrix[0], matrix[1], matrix[2], matrix[3],
			matrix[4], matrix[5], matrix[6], matrix[7],
			matrix[8], matrix[9], matrix[10], matrix[11],
			matrix[0] * translationVector[0] + matrix[4] * translationVector[1] + matrix[8] * translationVector[2] + matrix[12],
			matrix[1] * translationVector[0] + matrix[5] * translationVector[1] + matrix[9] * translationVector[2] + matrix[13],
			matrix[2] * translationVector[0] + matrix[6] * translationVector[1] + matrix[10] * translationVector[2] + matrix[14],
			matrix[3] * translationVector[0] + matrix[7] * translationVector[1] + matrix[11] * translationVector[2] + matrix[15] };
		return m;
	}
	
	inline Matrix4 Matrix4TranslateWithVector4(Matrix4 matrix, Vector4 translationVector)
	{
		Matrix4 m = { matrix[0], matrix[1], matrix[2], matrix[3],
			matrix[4], matrix[5], matrix[6], matrix[7],
			matrix[8], matrix[9], matrix[10], matrix[11],
			matrix[0] * translationVector[0] + matrix[4] * translationVector[1] + matrix[8] * translationVector[2] + matrix[12],
			matrix[1] * translationVector[0] + matrix[5] * translationVector[1] + matrix[9] * translationVector[2] + matrix[13],
			matrix[2] * translationVector[0] + matrix[6] * translationVector[1] + matrix[10] * translationVector[2] + matrix[14],
			matrix[3] * translationVector[0] + matrix[7] * translationVector[1] + matrix[11] * translationVector[2] + matrix[15] };
		return m;
	}
	
	inline Matrix4 Matrix4Scale(Matrix4 matrix, float sx, float sy, float sz)
	{
		Matrix4 m = { matrix[0] * sx, matrix[1] * sx, matrix[2] * sx, matrix[3] * sx,
			matrix[4] * sy, matrix[5] * sy, matrix[6] * sy, matrix[7] * sy,
			matrix[8] * sz, matrix[9] * sz, matrix[10] * sz, matrix[11] * sz,
			matrix[12], matrix[13], matrix[14], matrix[15] };
		return m;
	}
	
	inline Matrix4 Matrix4ScaleWithVector3(Matrix4 matrix, Vector3 scaleVector)
	{
		Matrix4 m = { matrix[0] * scaleVector[0], matrix[1] * scaleVector[0], matrix[2] * scaleVector[0], matrix[3] * scaleVector[0],
			matrix[4] * scaleVector[1], matrix[5] * scaleVector[1], matrix[6] * scaleVector[1], matrix[7] * scaleVector[1],
			matrix[8] * scaleVector[2], matrix[9] * scaleVector[2], matrix[10] * scaleVector[2], matrix[11] * scaleVector[2],
			matrix[12], matrix[13], matrix[14], matrix[15] };
		return m;
	}
	
	inline Matrix4 Matrix4ScaleWithVector4(Matrix4 matrix, Vector4 scaleVector)
	{
		Matrix4 m = { matrix[0] * scaleVector[0], matrix[1] * scaleVector[0], matrix[2] * scaleVector[0], matrix[3] * scaleVector[0],
			matrix[4] * scaleVector[1], matrix[5] * scaleVector[1], matrix[6] * scaleVector[1], matrix[7] * scaleVector[1],
			matrix[8] * scaleVector[2], matrix[9] * scaleVector[2], matrix[10] * scaleVector[2], matrix[11] * scaleVector[2],
			matrix[12], matrix[13], matrix[14], matrix[15] };
		return m;
	}
	
	inline Matrix4 Matrix4Rotate(Matrix4 matrix, float radians, float x, float y, float z)
	{
		Matrix4 rm = Matrix4MakeRotation(radians, x, y, z);
		return Matrix4Multiply(matrix, rm);
	}
	
	inline Matrix4 Matrix4RotateWithVector3(Matrix4 matrix, float radians, Vector3 axisVector)
	{
		Matrix4 rm = Matrix4MakeRotation(radians, axisVector[0], axisVector[1], axisVector[2]);
		return Matrix4Multiply(matrix, rm);
	}
	
	inline Matrix4 Matrix4RotateWithVector4(Matrix4 matrix, float radians, Vector4 axisVector)
	{
		Matrix4 rm = Matrix4MakeRotation(radians, axisVector[0], axisVector[1], axisVector[2]);
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
	
	inline Vector3 Matrix4MultiplyVector3(Matrix4 mL, Vector3 vR)
	{
		Vector4 v4 = Matrix4MultiplyVector4(mL, Vector4Make(vR[0], vR[1], vR[2], 0.0f));
		return Vector3Make(v4[0], v4[1], v4[2]);
	}
	
	inline Vector3 Matrix4MultiplyVector3WithTranslation(Matrix4 mL, Vector3 vR)
	{
		Vector4 v4 = Matrix4MultiplyVector4(mL, Vector4Make(vR[0], vR[1], vR[2], 1.0f));
		return Vector3Make(v4[0], v4[1], v4[2]);
	}
	
	inline Vector3 Matrix4MultiplyAndProjectVector3(Matrix4 mL, Vector3 vR)
	{
		Vector4 v4 = Matrix4MultiplyVector4(mL, Vector4Make(vR[0], vR[1], vR[2], 1.0f));
		return Vector3Make(v4[0], v4[1], v4[2]) * 1.0f / v4[3];
	}
	
	inline void Matrix4MultiplyVector3ArrayWithTranslation(Matrix4 matrix, Vector3 * vectors, size_t vectorCount)
	{
		int i;
		for (i=0; i < vectorCount; i++)
			vectors[i] = Matrix4MultiplyVector3WithTranslation(matrix, vectors[i]);
	}
	
	inline void Matrix4MultiplyAndProjectVector3Array(Matrix4 matrix, Vector3 * vectors, size_t vectorCount)
	{
		int i;
		for (i=0; i < vectorCount; i++)
			vectors[i] = Matrix4MultiplyAndProjectVector3(matrix, vectors[i]);
	}
	
	inline Vector4 Matrix4MultiplyVector4(Matrix4 mL, Vector4 vR)
	{
		Vector4 v = { mL[0] * vR[0] + mL[4] * vR[1] + mL[8] * vR[2] + mL[12] * vR[3],
			mL[1] * vR[0] + mL[5] * vR[1] + mL[9] * vR[2] + mL[13] * vR[3],
			mL[2] * vR[0] + mL[6] * vR[1] + mL[10] * vR[2] + mL[14] * vR[3],
			mL[3] * vR[0] + mL[7] * vR[1] + mL[11] * vR[2] + mL[15] * vR[3] };
		return v;
	}
	
	inline void Matrix4MultiplyVector4Array(Matrix4 matrix, Vector4 * vectors, size_t vectorCount)
	{
		int i;
		for (i=0; i < vectorCount; i++)
			vectors[i] = Matrix4MultiplyVector4(matrix, vectors[i]);
	}
	
}

inline Zen::Matrix3 operator+(Zen::Matrix3 mL, Zen::Matrix3 mR)
{
	Zen::Matrix3 m;
	
	m[0] = mL[0] + mR[0];
	m[1] = mL[1] + mR[1];
	m[2] = mL[2] + mR[2];
	
	m[3] = mL[3] + mR[3];
	m[4] = mL[4] + mR[4];
	m[5] = mL[5] + mR[5];
	
	m[6] = mL[6] + mR[6];
	m[7] = mL[7] + mR[7];
	m[8] = mL[8] + mR[8];
	
	return m;
}

inline Zen::Matrix3 operator-(Zen::Matrix3 mL, Zen::Matrix3 mR)
{
	Zen::Matrix3 m;
	
	m[0] = mL[0] - mR[0];
	m[1] = mL[1] - mR[1];
	m[2] = mL[2] - mR[2];
	
	m[3] = mL[3] - mR[3];
	m[4] = mL[4] - mR[4];
	m[5] = mL[5] - mR[5];
	
	m[6] = mL[6] - mR[6];
	m[7] = mL[7] - mR[7];
	m[8] = mL[8] - mR[8];
	
	return m;
}
inline Zen::Matrix3 operator*(Zen::Matrix3 mL, Zen::Matrix3 mR)
{
	Zen::Matrix3 m;
	
	m[0] = mL[0] * mR[0];
	m[1] = mL[1] * mR[1];
	m[2] = mL[2] * mR[2];
	
	m[3] = mL[3] * mR[3];
	m[4] = mL[4] * mR[4];
	m[5] = mL[5] * mR[5];
	
	m[6] = mL[6] * mR[6];
	m[7] = mL[7] * mR[7];
	m[8] = mL[8] * mR[8];
	
	return m;
}

inline Zen::Matrix3 operator/(Zen::Matrix3 mL, Zen::Matrix3 mR)
{
	Zen::Matrix3 m;
	
	m[0] = mL[0] / mR[0];
	m[1] = mL[1] / mR[1];
	m[2] = mL[2] / mR[2];
	
	m[3] = mL[3] / mR[3];
	m[4] = mL[4] / mR[4];
	m[5] = mL[5] / mR[5];
	
	m[6] = mL[6] / mR[6];
	m[7] = mL[7] / mR[7];
	m[8] = mL[8] / mR[8];
	
	return m;
}

inline Zen::Matrix4 operator+(Zen::Matrix4 mL, Zen::Matrix4 mR)
{
	Zen::Matrix4 m;
	
	m[0] = mL[0] + mR[0];
	m[1] = mL[1] + mR[1];
	m[2] = mL[2] + mR[2];
	m[3] = mL[3] + mR[3];
	
	m[4] = mL[4] + mR[4];
	m[5] = mL[5] + mR[5];
	m[6] = mL[6] + mR[6];
	m[7] = mL[7] + mR[7];
	
	m[8] = mL[8] + mR[8];
	m[9] = mL[9] + mR[9];
	m[10] = mL[10] + mR[10];
	m[11] = mL[11] + mR[11];
	
	m[12] = mL[12] + mR[12];
	m[13] = mL[13] + mR[13];
	m[14] = mL[14] + mR[14];
	m[15] = mL[15] + mR[15];
	
	return m;
}

inline Zen::Matrix4 operator-(Zen::Matrix4 mL, Zen::Matrix4 mR)
{
	Zen::Matrix4 m;
	
	m[0] = mL[0] - mR[0];
	m[1] = mL[1] - mR[1];
	m[2] = mL[2] - mR[2];
	m[3] = mL[3] - mR[3];
	
	m[4] = mL[4] - mR[4];
	m[5] = mL[5] - mR[5];
	m[6] = mL[6] - mR[6];
	m[7] = mL[7] - mR[7];
	
	m[8] = mL[8] - mR[8];
	m[9] = mL[9] - mR[9];
	m[10] = mL[10] - mR[10];
	m[11] = mL[11] - mR[11];
	
	m[12] = mL[12] - mR[12];
	m[13] = mL[13] - mR[13];
	m[14] = mL[14] - mR[14];
	m[15] = mL[15] - mR[15];
	
	return m;
}

inline Zen::Matrix4 operator*(Zen::Matrix4 mL, Zen::Matrix4 mR)
{
	Zen::Matrix4 m;
	
	m[0] = mL[0] * mR[0];
	m[1] = mL[1] * mR[1];
	m[2] = mL[2] * mR[2];
	m[3] = mL[3] * mR[3];
	
	m[4] = mL[4] * mR[4];
	m[5] = mL[5] * mR[5];
	m[6] = mL[6] * mR[6];
	m[7] = mL[7] * mR[7];
	
	m[8] = mL[8] * mR[8];
	m[9] = mL[9] * mR[9];
	m[10] = mL[10] * mR[10];
	m[11] = mL[11] * mR[11];
	
	m[12] = mL[12] * mR[12];
	m[13] = mL[13] * mR[13];
	m[14] = mL[14] * mR[14];
	m[15] = mL[15] * mR[15];
	
	return m;
}

inline Zen::Matrix4 operator/(Zen::Matrix4 mL, Zen::Matrix4 mR)
{
	Zen::Matrix4 m;
	
	m[0] = mL[0] / mR[0];
	m[1] = mL[1] / mR[1];
	m[2] = mL[2] / mR[2];
	m[3] = mL[3] / mR[3];
	
	m[4] = mL[4] / mR[4];
	m[5] = mL[5] / mR[5];
	m[6] = mL[6] / mR[6];
	m[7] = mL[7] / mR[7];
	
	m[8] = mL[8] / mR[8];
	m[9] = mL[9] / mR[9];
	m[10] = mL[10] / mR[10];
	m[11] = mL[11] / mR[11];
	
	m[12] = mL[12] / mR[12];
	m[13] = mL[13] / mR[13];
	m[14] = mL[14] / mR[14];
	m[15] = mL[15] / mR[15];
	
	return m;
}

inline std::ostream & operator << (std::ostream & o, Zen::Matrix3 const & m)
{
	for(int i = 0; i < 3; ++i)
	{
		o << m.c[i][0];
		for(int j = 1; j < 3; ++j)
		{
			o << m.c[i][j] << ',';
		}
		o << '\n';
	}
	return o;
}

inline std::ostream & operator << (std::ostream & o, Zen::Matrix4 const & m)
{
	for(int i = 0; i < 4; ++i)
	{
		o << m.c[i][0];
		for(int j = 1; j < 4; ++j)
		{
			o << m.c[i][j] << ',';
		}
		o << '\n';
	}
	return o;
}
