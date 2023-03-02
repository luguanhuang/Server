#include "pch.h"
#include "UtilMath.h"
#include "math/XMath.h"

const float EPSINON = (1e-6);

double UtilMath::XSin[721];
double UtilMath::XCos[721];

void UtilMath::Init()
{
	int x = 0; 

	double v = 0;
	double d2r = 0.01745329251994329576923690768489;

	while(x < 721)
	{
		XSin[x] = sin(d2r * (v));
		XCos[x] = cos(d2r * (v));

		++x; v += 0.5;
	}
}

bool UtilMath::IsLineSegmentCross(const Vector3& p1, const Vector3& p2, const Vector3& q1, const Vector3& q2)
{
	//fast detect
	if (std::min(p1.x, p2.x) <= std::max(q1.x, q2.x) &&
		std::min(q1.x, q2.x) <= std::max(p1.x, p2.x) &&
		std::min(p1.z, p2.z) <= std::max(q1.z, q2.z) &&
		std::min(q1.z, q2.z) <= std::max(p1.z, p2.z))
	{
		//( p1 - q1 ) * ( q2 - q1 )
		float p1xq = Vector3::Dot(p1.x - q1.x, p1.z - q1.z,
			q2.x - q1.x, q2.z - q1.z);
		//( p2 - q1 ) * ( q2 - q1 )
		float p2xq = Vector3::Dot(p2.x - q1.x, p2.z - q1.z,
			q2.x - q1.x, q2.z - q1.z);

		//( q1 - p1 ) * ( p2 - p1 )
		float q1xp = Vector3::Dot(q1.x - p1.x, q1.z - p1.z,
			p2.x - p1.x, p2.z - p1.z);
		//( q2 - p1 ) * ( p2 - p1 )
		float q2xp = Vector3::Dot(q2.x - p1.x, q2.z - p1.z,
			p2.x - p1.x, p2.z - p1.z);

		return ((p1xq * p2xq <= 0) && (q1xp * q2xp <= 0));
	}

	return false;
}

bool UtilMath::IsInRect(const Vector3& point, const Vector3* pvecs, const Vector3& center, float yRot)
{
	const Vector3& v = Vector3::HorizontalRotate(point - center, -yRot, false);
	return (v.x > pvecs[0].x && v.x < pvecs[3].x && v.z > pvecs[0].z && v.z < pvecs[2].z);
}

bool UtilMath::IsFloatZero(float value)
{
	return abs(value) <= EPSINON;
}

bool UtilMath::Isdoublezero(double value)
{
	return abs(value) <= EPSINON;
}