#include "pch.h"
#include "XVector3.h"
#include "utility/UtilMath.h"

const Vector3 Vector3::back(0, 0, -1);
const Vector3 Vector3::down(0, -1, 0);
const Vector3 Vector3::forward(0, 0, 1);
const Vector3 Vector3::up(0, 1, 0);
const Vector3 Vector3::left(-1, 0, 0);
const Vector3 Vector3::right(1, 0, 0);
const Vector3 Vector3::one(1, 1, 1);
const Vector3 Vector3::zero(0, 0, 0);

Vector3 operator *(float d, const Vector3& v)
{
	return Vector3 (v.x * d, v.y * d, v.z * d); 
}

std::ostream & operator<<(std::ostream &stream, const Vector3 &pos)
{
	stream << "(" << pos.x << ", " << pos.y << ", " << pos.z << ")";
	return stream;
}

float Vector3::Angle(const Vector3& v, const Vector3& u)
{
	//assert(!v.IsZero() && !u.IsZero());

	if(v.IsZero() || u.IsZero())
	{
		LogError("Error Vector3 to calc Angle.");
		return 0;
	}

	float f = Dot(v, u) / sqrt(sqrtMagnitude(v) * sqrtMagnitude(u));
	/*
	 * in case f == (+-)1.00000001
	 */
	if((double)f > 1.0) f = 1.0f;
	else if((double)f < -1.0) f = -1.0f;

	return XRad2Deg(acosf(f));
}

Vector3 Vector3::ClampMagnitude(const Vector3& v, float maxLength)
{
	if(sqrtMagnitude(v) <= maxLength * maxLength)
		return v;
	else
	{
		const Vector3& nor = Vector3::Normalized(v);
		const Vector3& clamp = nor * maxLength;

		return v + clamp;
	}
}

Vector3 Vector3::Cross(const Vector3& v, const Vector3& u)
{
	return Vector3( v.y * u.z - v.z * u.y,
					v.z * u.x - v.x * u.z,
					v.x * u.y - v.y * u.x );
}

float Vector3::Distance(const Vector3& v, const Vector3& u)
{
	return Magnitude(v - u);
}

float Vector3::Dot(const Vector3& v, const Vector3& u)
{
	return v.x * u.x + v.y * u.y + v.z * u.z;
}

float Vector3::Magnitude(const Vector3& v)
{
	return v.IsZero() ? 0 : sqrt(Dot(v, v));
}

float Vector3::sqrtMagnitude(const Vector3& v)
{
	return Dot(v, v);
}

Vector3 Vector3::Normalized(const Vector3& v)
{
	if(v.IsZero()) return Vector3::zero;

	float len = Magnitude(v);
	return Vector3(v.x / len, v.y / len, v.z / len);
}

Vector3 Vector3::Horizontaled(const Vector3& v)
{
	Vector3 vec(v);
	vec.Horizontal();

	return vec;
}

void Vector3::Normalize()
{
	if(IsZero()) return;

	float len = Magnitude(*this);

	x /= len;
	y /= len;
	z /= len;
}

Vector3 Vector3::HorizontalRotate(const Vector3& v, float degree, bool normalized)
{
	degree = -degree;

	int deg = (int)floor(XRoundToCircle(degree) + 0.5f);
	if(deg < 0) deg += 360;

	float sinA = UtilMath::XSin[deg << 1];
	float cosA = UtilMath::XCos[deg << 1];

	float x = v.x * cosA - v.z * sinA;
	float z = v.x * sinA + v.z * cosA;

	Vector3 r(x, v.y, z);
	if(normalized) r.Normalize();

	return r;
}

void Vector3::HorizontalRotate(float degree, bool normalized)
{
	degree = -degree;

	int deg = (int)floor(XRoundToCircle(degree) + 0.5f);
	if(deg < 0) deg += 360;

	float sinA = UtilMath::XSin[deg << 1];
	float cosA = UtilMath::XCos[deg << 1];

	float _x = x * cosA - z * sinA;
	float _z = x * sinA + z * cosA;

	x = _x; z = _z;
	if(normalized) Normalize();
}