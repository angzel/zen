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

namespace Zen {
	union Vector2 {
		float v[2];
		struct {
			float x, y;
		} s;
	};
	union Vector3 {
		float v[3];
		struct {
			float x, y, z;
		} s;
	};
	union Vector4 {
		float v[4];
		struct {
			float x, y, z, w;
		} s;
	};
}

inline Zen::Vector2 operator-(Zen::Vector2 vector);
inline Zen::Vector2 operator+(Zen::Vector2 vectorLeft, Zen::Vector2 vectorRight);
inline Zen::Vector2 operator-(Zen::Vector2 vectorLeft, Zen::Vector2 vectorRight);
inline Zen::Vector2 operator*(Zen::Vector2 vectorLeft, Zen::Vector2 vectorRight);
inline Zen::Vector2 operator/(Zen::Vector2 vectorLeft, Zen::Vector2 vectorRight);
inline Zen::Vector2 operator+(Zen::Vector2 vector, float value);
inline Zen::Vector2 operator-(Zen::Vector2 vector, float value);
inline Zen::Vector2 operator*(Zen::Vector2 vector, float value);
inline Zen::Vector2 operator/(Zen::Vector2 vector, float value);
inline bool operator==(Zen::Vector2 vectorLeft, Zen::Vector2 vectorRight);
inline bool operator==(Zen::Vector2 vector, float value);

inline Zen::Vector3 operator-(Zen::Vector3 vector);
inline Zen::Vector3 operator+(Zen::Vector3 vectorLeft, Zen::Vector3 vectorRight);
inline Zen::Vector3 operator-(Zen::Vector3 vectorLeft, Zen::Vector3 vectorRight);
inline Zen::Vector3 operator*(Zen::Vector3 vectorLeft, Zen::Vector3 vectorRight);
inline Zen::Vector3 operator/(Zen::Vector3 vectorLeft, Zen::Vector3 vectorRight);
inline Zen::Vector3 operator+(Zen::Vector3 vector, float value);
inline Zen::Vector3 operator-(Zen::Vector3 vector, float value);
inline Zen::Vector3 operator*(Zen::Vector3 vector, float value);
inline Zen::Vector3 operator/(Zen::Vector3 vector, float value);
inline bool operator==(Zen::Vector3 vectorLeft, Zen::Vector3 vectorRight);
inline bool operator==(Zen::Vector3 vector, float value);

inline Zen::Vector4 operator-(Zen::Vector4 vector);
inline Zen::Vector4 operator+(Zen::Vector4 vectorLeft, Zen::Vector4 vectorRight);
inline Zen::Vector4 operator-(Zen::Vector4 vectorLeft, Zen::Vector4 vectorRight);
inline Zen::Vector4 operator*(Zen::Vector4 vectorLeft, Zen::Vector4 vectorRight);
inline Zen::Vector4 operator/(Zen::Vector4 vectorLeft, Zen::Vector4 vectorRight);
inline Zen::Vector4 operator+(Zen::Vector4 vector, float value);
inline Zen::Vector4 operator-(Zen::Vector4 vector, float value);
inline Zen::Vector4 operator*(Zen::Vector4 vector, float value);
inline Zen::Vector4 operator/(Zen::Vector4 vector, float value);
inline bool operator==(Zen::Vector4 vectorLeft, Zen::Vector4 vectorRight);
inline bool operator==(Zen::Vector4 vector, float value);

namespace Zen {

	inline Vector2 Vector2Make(float x, float y);
	inline Vector2 Vector2Make(float * values /* [2] */);
	inline Vector2 Vector2Normalize(Vector2 vector);
	inline float Vector2DotProduct(Vector2 vectorLeft, Vector2 vectorRight);
	inline float Vector2Length(Vector2 vector);
	inline float Vector2Distance(Vector2 vectorStart, Vector2 vectorEnd);
	inline Vector2 Vector2Lerp(Vector2 vectorStart, Vector2 vectorEnd, float t);
	inline Vector2 Vector2Project(Vector2 vectorToProject, Vector2 projectionVector);

	inline Vector3 Vector3Make(float x, float y, float z);
	inline Vector3 Vector3Make(float *values /*[3]*/);
	inline Vector3 Vector3Normalize(Vector3 vector);
	inline float Vector3DotProduct(Vector3 vectorLeft, Vector3 vectorRight);
	inline float Vector3Length(Vector3 vector);
	inline float Vector3Distance(Vector3 vectorStart, Vector3 vectorEnd);
	inline Vector3 Vector3Lerp(Vector3 vectorStart, Vector3 vectorEnd, float t);
	inline Vector3 Vector3CrossProduct(Vector3 vectorLeft, Vector3 vectorRight);
	inline Vector3 Vector3Project(Vector3 vectorToProject, Vector3 projectionVector);

	inline Vector4 Vector4Make(float x, float y, float z, float w);
	inline Vector4 Vector4Make(float * values /*[4]*/);
	inline Vector4 Vector4Make(Vector3 vector, float w);
	inline Vector4 Vector4Normalize(Vector4 vector);
	inline float Vector4DotProduct(Vector4 vectorLeft, Vector4 vectorRight);
	inline float Vector4Length(Vector4 vector);
	inline float Vector4Distance(Vector4 vectorStart, Vector4 vectorEnd);
	inline Vector4 Vector4Lerp(Vector4 vectorStart, Vector4 vectorEnd, float t);
	/*
	 Performs a 3D cross product. The last component of the resulting cross product will be zeroed out.
	 */
	inline Vector4 Vector4CrossProduct(Vector4 vectorLeft, Vector4 vectorRight);
	inline Vector4 Vector4Project(Vector4 vectorToProject, Vector4 projectionVector);
	#pragma mark Implementations

	inline Vector2 Vector2Make(float x, float y)
	{
		Vector2 v = { x, y };
		return v;
	}

	inline Vector2 Vector2Make(float * values /* [2] */)
	{
		Vector2 v = { values[0], values[1] };
		return v;
	}

	inline Vector2 Vector2Normalize(Vector2 vector)
	{
		float scale = 1.0f / Vector2Length(vector);
		Vector2 v = vector * scale;
		return v;
	}

	inline float Vector2DotProduct(Vector2 vectorLeft, Vector2 vectorRight)
	{
		return vectorLeft.v[0] * vectorRight.v[0] + vectorLeft.v[1] * vectorRight.v[1];
	}

	inline float Vector2Length(Vector2 vector)
	{
		return sqrt(vector.v[0] * vector.v[0] + vector.v[1] * vector.v[1]);
	}

	inline float Vector2Distance(Vector2 vectorStart, Vector2 vectorEnd)
	{
		return Vector2Length(vectorEnd - vectorStart);
	}

	inline Vector2 Vector2Lerp(Vector2 vectorStart, Vector2 vectorEnd, float t)
	{
		Vector2 v = { vectorStart.v[0] + ((vectorEnd.v[0] - vectorStart.v[0]) * t),
						 vectorStart.v[1] + ((vectorEnd.v[1] - vectorStart.v[1]) * t) };
		return v;
	}

	inline Vector2 Vector2Project(Vector2 vectorToProject, Vector2 projectionVector)
	{
		float scale = Vector2DotProduct(projectionVector, vectorToProject) / Vector2DotProduct(projectionVector, projectionVector);
		Vector2 v = projectionVector * scale;
		return v;
	}

	#pragma mark Implementations

	inline Vector3 Vector3Make(float x, float y, float z)
	{
		Vector3 v = { x, y, z };
		return v;
	}

	inline Vector3 Vector3Make(float *values /*[3]*/)
	{
		Zen::Vector3 v = { values[0], values[1], values[2] };
		return v;
	}
	inline Vector3 Vector3Normalize(Vector3 vector)
	{
		float scale = 1.0f / Vector3Length(vector);
		Vector3 v = { vector.v[0] * scale, vector.v[1] * scale, vector.v[2] * scale };
		return v;
	}

	inline float Vector3DotProduct(Vector3 vectorLeft, Vector3 vectorRight)
	{
		return vectorLeft.v[0] * vectorRight.v[0] + vectorLeft.v[1] * vectorRight.v[1] + vectorLeft.v[2] * vectorRight.v[2];
	}

	inline float Vector3Length(Vector3 vector)
	{
		return sqrt(vector.v[0] * vector.v[0] + vector.v[1] * vector.v[1] + vector.v[2] * vector.v[2]);
	}

	inline float Vector3Distance(Vector3 vectorStart, Vector3 vectorEnd)
	{
		return Vector3Length(vectorEnd - vectorStart);
	}

	inline Vector3 Vector3Lerp(Vector3 vectorStart, Vector3 vectorEnd, float t)
	{
		Vector3 v = { vectorStart.v[0] + ((vectorEnd.v[0] - vectorStart.v[0]) * t),
						 vectorStart.v[1] + ((vectorEnd.v[1] - vectorStart.v[1]) * t),
						 vectorStart.v[2] + ((vectorEnd.v[2] - vectorStart.v[2]) * t) };
		return v;
	}

	inline Vector3 Vector3CrossProduct(Vector3 vectorLeft, Vector3 vectorRight)
	{
		Vector3 v = { vectorLeft.v[1] * vectorRight.v[2] - vectorLeft.v[2] * vectorRight.v[1],
						 vectorLeft.v[2] * vectorRight.v[0] - vectorLeft.v[0] * vectorRight.v[2],
						 vectorLeft.v[0] * vectorRight.v[1] - vectorLeft.v[1] * vectorRight.v[0] };
		return v;
	}

	inline Vector3 Vector3Project(Vector3 vectorToProject, Vector3 projectionVector)
	{
		float scale = Vector3DotProduct(projectionVector, vectorToProject) / Vector3DotProduct(projectionVector, projectionVector);
		Vector3 v = projectionVector * scale;
		return v;
	}

	#pragma mark Implementations

	inline Vector4 Vector4Make(float x, float y, float z, float w)
	{
		Vector4 v = { x, y, z, w };
		return v;
	}

	inline Vector4 Vector4Make(float * values /*[4]*/)
	{
		Vector4 v = { values[0], values[1], values[2], values[3] };
		return v;
	}

	inline Vector4 Vector4Make(Vector3 vector, float w)
	{
		Vector4 v = { vector.v[0], vector.v[1], vector.v[2], w };
		return v;
	}

	inline Vector4 Vector4Normalize(Vector4 vector)
	{
		float scale = 1.0f / Vector4Length(vector);
		Vector4 v = vector * scale;
		return v;
	}

	inline float Vector4DotProduct(Vector4 vectorLeft, Vector4 vectorRight)
	{
		return vectorLeft.v[0] * vectorRight.v[0] +
			   vectorLeft.v[1] * vectorRight.v[1] +
			   vectorLeft.v[2] * vectorRight.v[2] +
			   vectorLeft.v[3] * vectorRight.v[3];
	}

	inline float Vector4Length(Vector4 vector)
	{
		return sqrt(vector.v[0] * vector.v[0] +
					vector.v[1] * vector.v[1] +
					vector.v[2] * vector.v[2] +
					vector.v[3] * vector.v[3]);
	}

	inline float Vector4Distance(Vector4 vectorStart, Vector4 vectorEnd)
	{
		return Vector4Length(operator-(vectorEnd, vectorStart));
	}

	inline Vector4 Vector4Lerp(Vector4 vectorStart, Vector4 vectorEnd, float t)
	{
		Vector4 v = { vectorStart.v[0] + ((vectorEnd.v[0] - vectorStart.v[0]) * t),
						 vectorStart.v[1] + ((vectorEnd.v[1] - vectorStart.v[1]) * t),
						 vectorStart.v[2] + ((vectorEnd.v[2] - vectorStart.v[2]) * t),
						 vectorStart.v[3] + ((vectorEnd.v[3] - vectorStart.v[3]) * t) };
		return v;
	}

	inline Vector4 Vector4CrossProduct(Vector4 vectorLeft, Vector4 vectorRight)
	{
		Vector4 v = { vectorLeft.v[1] * vectorRight.v[2] - vectorLeft.v[2] * vectorRight.v[1],
						 vectorLeft.v[2] * vectorRight.v[0] - vectorLeft.v[0] * vectorRight.v[2],
						 vectorLeft.v[0] * vectorRight.v[1] - vectorLeft.v[1] * vectorRight.v[0],
						 0.0f };
		return v;
	}

	inline Vector4 Vector4Project(Vector4 vectorToProject, Vector4 projectionVector)
	{
		float scale = Vector4DotProduct(projectionVector, vectorToProject) / Vector4DotProduct(projectionVector, projectionVector);
		Vector4 v = operator*(projectionVector, scale);
		return v;
	}
}

/* Zen::Vector2 operator **/

inline Zen::Vector2 operator-(Zen::Vector2 vector)
{
    Zen::Vector2 v = { -vector.v[0] , -vector.v[1] };
    return v;
}

inline Zen::Vector2 operator+(Zen::Vector2 vectorLeft, Zen::Vector2 vectorRight)
{
    Zen::Vector2 v = { vectorLeft.v[0] + vectorRight.v[0],
                     vectorLeft.v[1] + vectorRight.v[1] };
    return v;
}

inline Zen::Vector2 operator-(Zen::Vector2 vectorLeft, Zen::Vector2 vectorRight)
{
    Zen::Vector2 v = { vectorLeft.v[0] - vectorRight.v[0],
                     vectorLeft.v[1] - vectorRight.v[1] };
    return v;
}

inline Zen::Vector2 operator*(Zen::Vector2 vectorLeft, Zen::Vector2 vectorRight)
{
    Zen::Vector2 v = { vectorLeft.v[0] * vectorRight.v[0],
                     vectorLeft.v[1] * vectorRight.v[1] };
    return v;
}

inline Zen::Vector2 operator/(Zen::Vector2 vectorLeft, Zen::Vector2 vectorRight)
{
    Zen::Vector2 v = { vectorLeft.v[0] / vectorRight.v[0],
                     vectorLeft.v[1] / vectorRight.v[1] };
    return v;
}

inline Zen::Vector2 operator+(Zen::Vector2 vector, float value)
{
    Zen::Vector2 v = { vector.v[0] + value,
                     vector.v[1] + value };
    return v;
}

inline Zen::Vector2 operator-(Zen::Vector2 vector, float value)
{
    Zen::Vector2 v = { vector.v[0] - value,
                     vector.v[1] - value };
    return v;
}

inline Zen::Vector2 operator*(Zen::Vector2 vector, float value)
{
    Zen::Vector2 v = { vector.v[0] * value,
                     vector.v[1] * value };
    return v;
}

inline Zen::Vector2 operator/(Zen::Vector2 vector, float value)
{
    Zen::Vector2 v = { vector.v[0] / value,
                     vector.v[1] / value };
    return v;
}

inline bool operator==(Zen::Vector2 vectorLeft, Zen::Vector2 vectorRight)
{
    bool compare = false;
    if (vectorLeft.v[0] == vectorRight.v[0] &&
        vectorLeft.v[1] == vectorRight.v[1])
        compare = true;
    return compare;
}

inline bool operator==(Zen::Vector2 vector, float value)
{
    bool compare = false;
    if (vector.v[0] == value &&
        vector.v[1] == value)
        compare = true;
    return compare;
}

/* Zen::Vector3 operator **/
inline Zen::Vector3 operator-(Zen::Vector3 vector)
{
	Zen::Vector3 v = { -vector.v[0], -vector.v[1], -vector.v[2] };
	return v;
}

inline Zen::Vector3 operator+(Zen::Vector3 vectorLeft, Zen::Vector3 vectorRight)
{
	Zen::Vector3 v = { vectorLeft.v[0] + vectorRight.v[0],
					 vectorLeft.v[1] + vectorRight.v[1],
					 vectorLeft.v[2] + vectorRight.v[2] };
	return v;
}

inline Zen::Vector3 operator-(Zen::Vector3 vectorLeft, Zen::Vector3 vectorRight)
{
	Zen::Vector3 v = { vectorLeft.v[0] - vectorRight.v[0],
					 vectorLeft.v[1] - vectorRight.v[1],
					 vectorLeft.v[2] - vectorRight.v[2] };
	return v;
}

inline Zen::Vector3 operator*(Zen::Vector3 vectorLeft, Zen::Vector3 vectorRight)
{
	Zen::Vector3 v = { vectorLeft.v[0] * vectorRight.v[0],
					 vectorLeft.v[1] * vectorRight.v[1],
					 vectorLeft.v[2] * vectorRight.v[2] };
	return v;
}

inline Zen::Vector3 operator/(Zen::Vector3 vectorLeft, Zen::Vector3 vectorRight)
{
	Zen::Vector3 v = { vectorLeft.v[0] / vectorRight.v[0],
					 vectorLeft.v[1] / vectorRight.v[1],
					 vectorLeft.v[2] / vectorRight.v[2] };
	return v;
}

inline Zen::Vector3 operator+(Zen::Vector3 vector, float value)
{
	Zen::Vector3 v = { vector.v[0] + value,
					 vector.v[1] + value,
					 vector.v[2] + value };
	return v;
}

inline Zen::Vector3 operator-(Zen::Vector3 vector, float value)
{
	Zen::Vector3 v = { vector.v[0] - value,
					 vector.v[1] - value,
					 vector.v[2] - value };
	return v;
}

inline Zen::Vector3 operator*(Zen::Vector3 vector, float value)
{
	Zen::Vector3 v = { vector.v[0] * value,
					 vector.v[1] * value,
					 vector.v[2] * value };
	return v;
}

inline Zen::Vector3 operator/(Zen::Vector3 vector, float value)
{
	Zen::Vector3 v = { vector.v[0] / value,
					 vector.v[1] / value,
					 vector.v[2] / value };
	return v;
}


inline bool operator==(Zen::Vector3 vectorLeft, Zen::Vector3 vectorRight)
{
	bool compare = false;
	if (vectorLeft.v[0] == vectorRight.v[0] &&
		vectorLeft.v[1] == vectorRight.v[1] &&
		vectorLeft.v[2] == vectorRight.v[2])
		compare = true;
	return compare;
}

inline bool operator==(Zen::Vector3 vector, float value)
{
	bool compare = false;
	if (vector.v[0] == value &&
		vector.v[1] == value &&
		vector.v[2] == value)
		compare = true;
	return compare;
}

inline Zen::Vector4 operator-(Zen::Vector4 vector)
{
    Zen::Vector4 v = { -vector.v[0], -vector.v[1], -vector.v[2], -vector.v[3] };
    return v;
}

inline Zen::Vector4 operator+(Zen::Vector4 vectorLeft, Zen::Vector4 vectorRight)
{
    Zen::Vector4 v = { vectorLeft.v[0] + vectorRight.v[0],
                     vectorLeft.v[1] + vectorRight.v[1],
                     vectorLeft.v[2] + vectorRight.v[2],
                     vectorLeft.v[3] + vectorRight.v[3] };
    return v;
}

inline Zen::Vector4 operator-(Zen::Vector4 vectorLeft, Zen::Vector4 vectorRight)
{
    Zen::Vector4 v = { vectorLeft.v[0] - vectorRight.v[0],
                     vectorLeft.v[1] - vectorRight.v[1],
                     vectorLeft.v[2] - vectorRight.v[2],
                     vectorLeft.v[3] - vectorRight.v[3] };
    return v;
}

inline Zen::Vector4 operator*(Zen::Vector4 vectorLeft, Zen::Vector4 vectorRight)
{
    Zen::Vector4 v = { vectorLeft.v[0] * vectorRight.v[0],
                     vectorLeft.v[1] * vectorRight.v[1],
                     vectorLeft.v[2] * vectorRight.v[2],
                     vectorLeft.v[3] * vectorRight.v[3] };
    return v;
}

inline Zen::Vector4 operator/(Zen::Vector4 vectorLeft, Zen::Vector4 vectorRight)
{
    Zen::Vector4 v = { vectorLeft.v[0] / vectorRight.v[0],
                     vectorLeft.v[1] / vectorRight.v[1],
                     vectorLeft.v[2] / vectorRight.v[2],
                     vectorLeft.v[3] / vectorRight.v[3] };
    return v;
}

inline Zen::Vector4 operator+(Zen::Vector4 vector, float value)
{
    Zen::Vector4 v = { vector.v[0] + value,
                     vector.v[1] + value,
                     vector.v[2] + value,
                     vector.v[3] + value };
    return v;
}

inline Zen::Vector4 operator-(Zen::Vector4 vector, float value)
{
    Zen::Vector4 v = { vector.v[0] - value,
                     vector.v[1] - value,
                     vector.v[2] - value,
                     vector.v[3] - value };
    return v;
}

inline Zen::Vector4 operator*(Zen::Vector4 vector, float value)
{
    Zen::Vector4 v = { vector.v[0] * value,
                     vector.v[1] * value,
                     vector.v[2] * value,
                     vector.v[3] * value };
    return v;
}

inline Zen::Vector4 operator/(Zen::Vector4 vector, float value)
{
    Zen::Vector4 v = { vector.v[0] / value,
                     vector.v[1] / value,
                     vector.v[2] / value,
                     vector.v[3] / value };
    return v;
}

inline bool operator==(Zen::Vector4 vectorLeft, Zen::Vector4 vectorRight)
{
    bool compare = false;
    if (vectorLeft.v[0] == vectorRight.v[0] &&
        vectorLeft.v[1] == vectorRight.v[1] &&
        vectorLeft.v[2] == vectorRight.v[2] &&
        vectorLeft.v[3] == vectorRight.v[3])
        compare = true;
    return compare;
}

inline bool operator==(Zen::Vector4 vector, float value)
{
    bool compare = false;
    if (vector.v[0] == value &&
        vector.v[1] == value &&
        vector.v[2] == value &&
        vector.v[3] == value)
        compare = true;
    return compare;
}

std::ostream & operator << (std::ostream & o, Zen::Vector4 const & m)
{
	o << m.v[0] << ',' << m.v[1] << ',' << m.v[2] << ',' << m.v[3];
	return o;
}
std::ostream & operator << (std::ostream & o, Zen::Vector3 const & m)
{
	o << m.v[0] << ',' << m.v[1] << ',' << m.v[2];
	return o;
}

