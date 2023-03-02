#ifndef __XVECTOR3_H__
#define __XVECTOR3_H__

#include "pb/project.pb.h"
#include <iosfwd>
#include "XMath.h"

class Vector3;
std::ostream &operator<<(std::ostream &stream, const Vector3 &pos); 

class Vector3
{
public:
	float x;
	float y;
	float z;

	Vector3()
		:x(0), y(0), z(0) { }
	Vector3(float x1, float y1, float z1)
		:x(x1), y(y1), z(z1) { }
	Vector3(const Vector3& v)
		:x(v.x), y(v.y), z(v.z) { }
	Vector3(const KKSG::Vec3& v)
		:x(v.x()), y(v.y()), z(v.z()) { }

	inline Vector3 operator -() const { return Vector3(-x, -y, -z); }
	inline void operator =(const Vector3& v){ x = v.x; y = v.y; z = v.z; }
	inline void operator =(const KKSG::Vec3& v){ x = v.x(); y = v.y(); z = v.z(); }
	inline void assignTo(KKSG::Vec3& v) const { v.set_x(x); v.set_y(y); v.set_z(z); }
	inline void set(float _x, float _y, float _z) { x = _x; y = _y; z = _z; }

	inline Vector3 operator -(const Vector3& v) const { return Vector3 (x - v.x, y - v.y, z - v.z); }
	inline Vector3 operator +(const Vector3& v) const { return Vector3 (x + v.x, y + v.y, z + v.z); }
	inline Vector3 operator *(const Vector3& v) const { return Vector3 (v.x * x, v.y * y, v.z * z); }

	friend Vector3 operator *(float d, const Vector3& v);

	inline Vector3 operator *(float d) const { return Vector3 (x * d, y * d, z * d); }
	inline Vector3 operator /(float d) const { return Vector3 (x / d, y / d, z / d); }
	inline bool operator ==(const Vector3& v) const { return !((v.x != x) || (v.y != y) || (v.z != z)); }
	inline bool operator !=(const Vector3& v) const { return (v.x != x) || (v.y != y) || (v.z != z); }

	inline Vector3& operator +=(const Vector3& v)
	{
		x += v.x;
		y += v.y;
		z += v.z;
		return *this;
	}

	inline Vector3& operator -=(const Vector3& v)
	{
		x -= v.x;
		y -= v.y;
		z -= v.z;
		return *this;
	}

	inline Vector3& operator *=(const Vector3& v)
	{
		x *= v.x;
		y *= v.y;
		z *= v.z;
		return *this;
	}

	inline Vector3& operator *=(float d)
	{
		x *= d;
		y *= d;
		z *= d;
		return *this;
	}

	inline Vector3& operator /=(float d)
	{
		x /= d;
		y /= d;
		z /= d;
		return *this;
	}

	void HorizontalRotate(float degree, bool normalized = true);

	//Returns the angle in degrees between v and u.
	static float Angle(const Vector3& v, const Vector3& u);
	//Returns a copy of vector3 with its magnitude clamped to maxLength.
	static Vector3 ClampMagnitude(const Vector3& v, float maxLength);
	static Vector3 Cross(const Vector3& v, const Vector3& u);
	static float Distance(const Vector3& v, const Vector3& u);
	static float Dot(const Vector3& v, const Vector3& u);
	static float Magnitude(const Vector3& v);
	static float sqrtMagnitude(const Vector3& v);
	static Vector3 Normalized(const Vector3& v);
	static Vector3 Horizontaled(const Vector3& v);
	//Rotate around Y-axis ((0, 0) for plane) with degree (clockwise positive)
	static Vector3 HorizontalRotate(const Vector3& v, float degree, bool normalized = true);
	/*
	 * whether vec-r is in the clockwise direction of vec-f in x-z plane.
	 */
	static inline bool Clockwise(const Vector3& fiduciary, const Vector3& relativity)
	{
		return (fiduciary.z * relativity.x - fiduciary.x * relativity.z) > 0;
	}

	static inline float Dot(float v1, float u1, float v2, float u2)
	{
		return v1 * u2 - v2 * u1;
	}

	static float inline AngleToFloat(const Vector3& dir)
	{
		float face = Vector3::Angle(Vector3::forward, dir);
		return Vector3::Clockwise(Vector3::forward, dir) ? face : -face;
	}

	static Vector3 inline FloatToAngle(float angle)
	{
		return HorizontalRotate(Vector3::forward, angle);
	}

	void Normalize();
	inline void Zero(){ x = 0; y = 0; z = 0; }
	inline bool IsZero() const { return x == 0 && y == 0 && z == 0; }

	/*
	 * vector maybe zero after Horizontal
	 */
	inline void Horizontal()
	{
		y = 0;
		Normalize();
	}

	static const Vector3 back;
	static const Vector3 down;
	static const Vector3 forward;
	static const Vector3 up;
	static const Vector3 left;
	static const Vector3 right;
	static const Vector3 one;
	static const Vector3 zero;
};

Vector3 operator *(float d, const Vector3& v);

#endif	//__XVECTOR3_H__