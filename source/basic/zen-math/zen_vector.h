/*
 Copyright (c) 2013 MeherTJ G. All rights reserved.
 License: LGPL for personnal study or free software.
 */

#pragma once

#include <cmath>
#include <functional>
#include <iostream>
#include "zen_types.h"

namespace Zen {
	struct Vector2{
		float x, y;
		
		float & operator[](size_t i)
		{
			return (&x)[i];
		}
		float const & operator[](size_t i) const
		{
			return (&x)[i];
		}
	};
	struct Vector3 {
		float x, y, z;
		
		float & operator[](size_t i)
		{
			return (&x)[i];
		}
		float const & operator[](size_t i) const
		{
			return (&x)[i];
		}
	};
	struct Vector4 {
		float x, y, z, w;
		
		float & operator[](size_t i)
		{
			return (&x)[i];
		}
		float const & operator[](size_t i) const
		{
			return (&x)[i];
		}
	};
}

namespace Zen {
	inline Point2 Point2Make(Vector2 v)
	{
		return { v.x, v.y };
	}
	inline Size2 Size2Make(Vector2 v)
	{
		return { v.x, v.y };
	}
}
inline Zen::Vector2 operator-(Zen::Vector2 v);
inline Zen::Vector2 operator+(Zen::Vector2 vL, Zen::Vector2 vR);
inline Zen::Vector2 operator-(Zen::Vector2 vL, Zen::Vector2 vR);
inline Zen::Vector2 operator*(Zen::Vector2 vL, Zen::Vector2 vR);
inline Zen::Vector2 operator/(Zen::Vector2 vL, Zen::Vector2 vR);
inline Zen::Vector2 operator+(Zen::Vector2 v, float value);
inline Zen::Vector2 operator-(Zen::Vector2 v, float value);
inline Zen::Vector2 operator*(Zen::Vector2 v, float value);
inline Zen::Vector2 operator/(Zen::Vector2 v, float value);
inline bool operator==(Zen::Vector2 vL, Zen::Vector2 vR);
inline bool operator==(Zen::Vector2 v, float value);

inline Zen::Vector3 operator-(Zen::Vector3 v);
inline Zen::Vector3 operator+(Zen::Vector3 vL, Zen::Vector3 vR);
inline Zen::Vector3 operator-(Zen::Vector3 vL, Zen::Vector3 vR);
inline Zen::Vector3 operator*(Zen::Vector3 vL, Zen::Vector3 vR);
inline Zen::Vector3 operator/(Zen::Vector3 vL, Zen::Vector3 vR);
inline Zen::Vector3 operator+(Zen::Vector3 v, float value);
inline Zen::Vector3 operator-(Zen::Vector3 v, float value);
inline Zen::Vector3 operator*(Zen::Vector3 v, float value);
inline Zen::Vector3 operator/(Zen::Vector3 v, float value);
inline bool operator==(Zen::Vector3 vL, Zen::Vector3 vR);
inline bool operator==(Zen::Vector3 v, float value);

inline Zen::Vector4 operator-(Zen::Vector4 v);
inline Zen::Vector4 operator+(Zen::Vector4 vL, Zen::Vector4 vR);
inline Zen::Vector4 operator-(Zen::Vector4 vL, Zen::Vector4 vR);
inline Zen::Vector4 operator*(Zen::Vector4 vL, Zen::Vector4 vR);
inline Zen::Vector4 operator/(Zen::Vector4 vL, Zen::Vector4 vR);
inline Zen::Vector4 operator+(Zen::Vector4 v, float value);
inline Zen::Vector4 operator-(Zen::Vector4 v, float value);
inline Zen::Vector4 operator*(Zen::Vector4 v, float value);
inline Zen::Vector4 operator/(Zen::Vector4 v, float value);
inline bool operator==(Zen::Vector4 vL, Zen::Vector4 vR);
inline bool operator==(Zen::Vector4 v, float value);

namespace Zen {
	
	inline Vector2 Vector2Make(float x, float y);
	inline Vector2 Vector2Make(float * values /* [2] */);
	inline Vector2 Vector2Make(Point2 p);
	inline Vector2 Vector2Normalize(Vector2 v);
	inline float Vector2DotProduct(Vector2 vL, Vector2 vR);
	inline float Vector2Length(Vector2 v);
	inline float Vector2Distance(Vector2 vStart, Vector2 vEnd);
	inline float Vector2Length2(Vector2 v);
	inline float Vector2Distance2(Vector2 vStart, Vector2 vEnd);
	inline Vector2 Vector2Lerp(Vector2 vStart, Vector2 vEnd, float t);
	inline Vector2 Vector2Project(Vector2 v, Vector2 p);
	
	inline Vector3 Vector3Make(float x, float y, float z);
	inline Vector3 Vector3Make(float *values /*[3]*/);
	inline Vector3 Vector3Normalize(Vector3 v);
	inline float Vector3DotProduct(Vector3 vL, Vector3 vR);
	inline float Vector3Length(Vector3 v);
	inline float Vector3Distance(Vector3 vStart, Vector3 vEnd);
	inline float Vector3Length2(Vector3 v);
	inline float Vector3Distance2(Vector3 vStart, Vector3 vEnd);
	inline Vector3 Vector3Lerp(Vector3 vStart, Vector3 vEnd, float t);
	inline Vector3 Vector3CrossProduct(Vector3 vL, Vector3 vR);
	inline Vector3 Vector3Project(Vector3 v, Vector3 p);
	
	inline Vector4 Vector4Make(float x, float y, float z, float w);
	inline Vector4 Vector4Make(float * values /*[4]*/);
	inline Vector4 Vector4Make(Vector3 v, float w);
	inline Vector4 Vector4Normalize(Vector4 v);
	inline float Vector4DotProduct(Vector4 vL, Vector4 vR);
	inline float Vector4Length(Vector4 v);
	inline float Vector4Distance(Vector4 vStart, Vector4 vEnd);
	inline float Vector4Length2(Vector4 v);
	inline float Vector4Distance2(Vector4 vStart, Vector4 vEnd);
	inline Vector4 Vector4Lerp(Vector4 vStart, Vector4 vEnd, float t);
	/*
	 Performs a 3D cross product. The last component of the resulting cross product will be zeroed out.
	 */
	inline Vector4 Vector4CrossProduct(Vector4 vL, Vector4 vR);
	inline Vector4 Vector4Project(Vector4 v, Vector4 p);
#pragma mark Implementations
	
	inline Vector2 Vector2Make(float x, float y)
	{
		return { x, y };
	}
	
	inline Vector2 Vector2Make(float * values /* [2] */)
	{
		return { values[0], values[1] };
	}
	inline Vector2 Vector2Make(Point2 p) {
		return { p.x, p.y };
	}
	
	
	inline Vector2 Vector2Normalize(Vector2 v)
	{
		float scale = 1.0f / Vector2Length(v);
		return v * scale;
	}
	
	inline float Vector2DotProduct(Vector2 vL, Vector2 vR)
	{
		return vL[0] * vR[0] + vL[1] * vR[1];
	}
	
	inline float Vector2Length(Vector2 v)
	{
		return sqrt(v[0] * v[0] + v[1] * v[1]);
	}
	
	inline float Vector2Distance(Vector2 vStart, Vector2 vEnd)
	{
		return Vector2Length(vEnd - vStart);
	}
	inline float Vector2Length2(Vector2 v)
	{
		return v[0] * v[0] + v[1] * v[1];
	}
	
	inline float Vector2Distance2(Vector2 vStart, Vector2 vEnd)
	{
		return Vector2Length2(vEnd - vStart);
	}
	
	inline Vector2 Vector2Lerp(Vector2 vStart, Vector2 vEnd, float t)
	{
		Vector2 v = { vStart[0] + ((vEnd[0] - vStart[0]) * t),
			vStart[1] + ((vEnd[1] - vStart[1]) * t) };
		return v;
	}
	
	inline Vector2 Vector2Project(Vector2 v, Vector2 p)
	{
		float scale = Vector2DotProduct(p, v) / Vector2DotProduct(p, p);
		return p * scale;
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
	inline Vector3 Vector3Normalize(Vector3 v)
	{
		float scale = 1.0f / Vector3Length(v);
		return { v[0] * scale, v[1] * scale, v[2] * scale };
	}
	
	inline float Vector3DotProduct(Vector3 vL, Vector3 vR)
	{
		return vL[0] * vR[0] + vL[1] * vR[1] + vL[2] * vR[2];
	}
	
	inline float Vector3Length(Vector3 v)
	{
		return sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
	}
	
	inline float Vector3Distance(Vector3 vStart, Vector3 vEnd)
	{
		return Vector3Length(vEnd - vStart);
	}
	inline float Vector3Length2(Vector3 v)
	{
		return (v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
	}
	
	inline float Vector3Distance2(Vector3 vStart, Vector3 vEnd)
	{
		return Vector3Length2(vEnd - vStart);
	}
	
	inline Vector3 Vector3Lerp(Vector3 vStart, Vector3 vEnd, float t)
	{
		Vector3 v = { vStart[0] + ((vEnd[0] - vStart[0]) * t),
			vStart[1] + ((vEnd[1] - vStart[1]) * t),
			vStart[2] + ((vEnd[2] - vStart[2]) * t) };
		return v;
	}
	
	inline Vector3 Vector3CrossProduct(Vector3 vL, Vector3 vR)
	{
		Vector3 v = { vL[1] * vR[2] - vL[2] * vR[1],
			vL[2] * vR[0] - vL[0] * vR[2],
			vL[0] * vR[1] - vL[1] * vR[0] };
		return v;
	}
	
	inline Vector3 Vector3Project(Vector3 v, Vector3 p)
	{
		float scale = Vector3DotProduct(p, v) / Vector3DotProduct(p, p);
		return p * scale;
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
	
	inline Vector4 Vector4Make(Vector3 v, float w)
	{
		return { v[0], v[1], v[2], w };
	}
	
	inline Vector4 Vector4Normalize(Vector4 v)
	{
		float scale = 1.0f / Vector4Length(v);
		return v * scale;
	}
	
	inline float Vector4DotProduct(Vector4 vL, Vector4 vR)
	{
		return vL[0] * vR[0] +
		vL[1] * vR[1] +
		vL[2] * vR[2] +
		vL[3] * vR[3];
	}
	
	inline float Vector4Length(Vector4 v)
	{
		return sqrt(v[0] * v[0] +
					v[1] * v[1] +
					v[2] * v[2] +
					v[3] * v[3]);
	}
	
	inline float Vector4Distance(Vector4 vStart, Vector4 vEnd)
	{
		return Vector4Length(operator-(vEnd, vStart));
	}
	inline float Vector4Length2(Vector4 v)
	{
		return (v[0] * v[0] +
				v[1] * v[1] +
				v[2] * v[2] +
				v[3] * v[3]);
	}
	
	inline float Vector4Distance2(Vector4 vStart, Vector4 vEnd)
	{
		return Vector4Length2(vEnd - vStart);
	}
	
	inline Vector4 Vector4Lerp(Vector4 vStart, Vector4 vEnd, float t)
	{
		Vector4 v = { vStart[0] + ((vEnd[0] - vStart[0]) * t),
			vStart[1] + ((vEnd[1] - vStart[1]) * t),
			vStart[2] + ((vEnd[2] - vStart[2]) * t),
			vStart[3] + ((vEnd[3] - vStart[3]) * t) };
		return v;
	}
	
	inline Vector4 Vector4CrossProduct(Vector4 vL, Vector4 vR)
	{
		Vector4 v = { vL[1] * vR[2] - vL[2] * vR[1],
			vL[2] * vR[0] - vL[0] * vR[2],
			vL[0] * vR[1] - vL[1] * vR[0],
			0.0f };
		return v;
	}
	
	inline Vector4 Vector4Project(Vector4 v, Vector4 p)
	{
		float scale = Vector4DotProduct(p, v) / Vector4DotProduct(p, p);
		return operator*(p, scale);
	}
}

/* Zen::Vector2 operator **/

inline Zen::Vector2 operator-(Zen::Vector2 v)
{
	return { -v[0] , -v[1] };
}

inline Zen::Vector2 operator+(Zen::Vector2 vL, Zen::Vector2 vR)
{
	return { vL[0] + vR[0], vL[1] + vR[1] };
}

inline Zen::Vector2 operator-(Zen::Vector2 vL, Zen::Vector2 vR)
{
	return { vL[0] - vR[0],
		vL[1] - vR[1] };
}

inline Zen::Vector2 operator*(Zen::Vector2 vL, Zen::Vector2 vR)
{
	return { vL[0] * vR[0],
		vL[1] * vR[1] };
}

inline Zen::Vector2 operator/(Zen::Vector2 vL, Zen::Vector2 vR)
{
	return { vL[0] / vR[0],
		vL[1] / vR[1] };
}

inline Zen::Vector2 operator+(Zen::Vector2 v, float value)
{
	return { v[0] + value,
		v[1] + value };
}

inline Zen::Vector2 operator-(Zen::Vector2 v, float value)
{
	return { v[0] - value,
		v[1] - value };
}

inline Zen::Vector2 operator*(Zen::Vector2 v, float value)
{
	return { v[0] * value,
		v[1] * value };
}

inline Zen::Vector2 operator/(Zen::Vector2 v, float value)
{
	return { v[0] / value,
		v[1] / value };
}

inline bool operator==(Zen::Vector2 vL, Zen::Vector2 vR)
{
	bool compare = false;
	if (vL[0] == vR[0] &&
		vL[1] == vR[1])
		compare = true;
	return compare;
}

inline bool operator==(Zen::Vector2 v, float value)
{
	bool compare = false;
	if (v[0] == value &&
		v[1] == value)
		compare = true;
	return compare;
}

/* Zen::Vector3 operator **/
inline Zen::Vector3 operator-(Zen::Vector3 v)
{
	return { -v[0], -v[1], -v[2] };
}

inline Zen::Vector3 operator+(Zen::Vector3 vL, Zen::Vector3 vR)
{
	return { vL[0] + vR[0],
		vL[1] + vR[1],
		vL[2] + vR[2] };
}

inline Zen::Vector3 operator-(Zen::Vector3 vL, Zen::Vector3 vR)
{
	return { vL[0] - vR[0],
		vL[1] - vR[1],
		vL[2] - vR[2] };
}

inline Zen::Vector3 operator*(Zen::Vector3 vL, Zen::Vector3 vR)
{
	return { vL[0] * vR[0],
		vL[1] * vR[1],
		vL[2] * vR[2] };
}

inline Zen::Vector3 operator/(Zen::Vector3 vL, Zen::Vector3 vR)
{
	return { vL[0] / vR[0],
		vL[1] / vR[1],
		vL[2] / vR[2] };
}

inline Zen::Vector3 operator+(Zen::Vector3 v, float value)
{
	return { v[0] + value,
		v[1] + value,
		v[2] + value };
}

inline Zen::Vector3 operator-(Zen::Vector3 v, float value)
{
	return { v[0] - value,
		v[1] - value,
		v[2] - value };
}

inline Zen::Vector3 operator*(Zen::Vector3 v, float value)
{
	return { v[0] * value,
		v[1] * value,
		v[2] * value };
}

inline Zen::Vector3 operator/(Zen::Vector3 v, float value)
{
	return { v[0] / value,
		v[1] / value,
		v[2] / value };
}


inline bool operator==(Zen::Vector3 vL, Zen::Vector3 vR)
{
	bool compare = false;
	if (vL[0] == vR[0] &&
		vL[1] == vR[1] &&
		vL[2] == vR[2])
		compare = true;
	return compare;
}

inline bool operator==(Zen::Vector3 v, float value)
{
	bool compare = false;
	if (v[0] == value &&
		v[1] == value &&
		v[2] == value)
		compare = true;
	return compare;
}

inline Zen::Vector4 operator-(Zen::Vector4 v)
{
	return { -v[0], -v[1], -v[2], -v[3] };
}

inline Zen::Vector4 operator+(Zen::Vector4 vL, Zen::Vector4 vR)
{
	return { vL[0] + vR[0],
		vL[1] + vR[1],
		vL[2] + vR[2],
		vL[3] + vR[3] };
}

inline Zen::Vector4 operator-(Zen::Vector4 vL, Zen::Vector4 vR)
{
	return { vL[0] - vR[0],
		vL[1] - vR[1],
		vL[2] - vR[2],
		vL[3] - vR[3] };
}

inline Zen::Vector4 operator*(Zen::Vector4 vL, Zen::Vector4 vR)
{
	return { vL[0] * vR[0],
		vL[1] * vR[1],
		vL[2] * vR[2],
		vL[3] * vR[3] };
}

inline Zen::Vector4 operator/(Zen::Vector4 vL, Zen::Vector4 vR)
{
	return { vL[0] / vR[0],
		vL[1] / vR[1],
		vL[2] / vR[2],
		vL[3] / vR[3] };
}

inline Zen::Vector4 operator+(Zen::Vector4 v, float value)
{
	return { v[0] + value,
		v[1] + value,
		v[2] + value,
		v[3] + value };
}

inline Zen::Vector4 operator-(Zen::Vector4 v, float value)
{
	return { v[0] - value,
		v[1] - value,
		v[2] - value,
		v[3] - value };
}

inline Zen::Vector4 operator*(Zen::Vector4 v, float value)
{
	return { v[0] * value,
		v[1] * value,
		v[2] * value,
		v[3] * value };
}

inline Zen::Vector4 operator/(Zen::Vector4 v, float value)
{
	return { v[0] / value,
		v[1] / value,
		v[2] / value,
		v[3] / value };
}

inline bool operator==(Zen::Vector4 vL, Zen::Vector4 vR)
{
	bool compare = false;
	if (vL[0] == vR[0] &&
		vL[1] == vR[1] &&
		vL[2] == vR[2] &&
		vL[3] == vR[3])
		compare = true;
	return compare;
}

inline bool operator==(Zen::Vector4 v, float value)
{
	bool compare = false;
	if (v[0] == value &&
		v[1] == value &&
		v[2] == value &&
		v[3] == value)
		compare = true;
	return compare;
}

inline std::ostream & operator << (std::ostream & o, Zen::Vector4 const & v)
{
	o << v[0] << ',' << v[1] << ',' << v[2] << ',' << v[3];
	return o;
}
inline std::ostream & operator << (std::ostream & o, Zen::Vector3 const & v)
{
	o << v[0] << ',' << v[1] << ',' << v[2];
	return o;
}
inline std::ostream & operator << (std::ostream & o, Zen::Vector2 const & v)
{
	o << v[0] << ',' << v[1];
	return o;
}
