#ifndef _UTIL_MATH_H_
#define _UTIL_MATH_H_

#include "math/XVector3.h"
#include <cmath>

class UtilMath
{
public:
	static bool IsLineSegmentCross(const Vector3& p1, const Vector3& p2, const Vector3& q1, const Vector3& q2);
	/*
	 * ����Բ���Ƿ��ھ��κ�Բ���ɿɷ�˹���͵ķ�Χ��
	 * ���α�������䣬������ԭ��
	 */
	static inline bool IsRectCycleCross(float rectw, float recth, const Vector3& c, float r)
	{
		Vector3 u(std::max(abs(c.x) - rectw, 0.0f), 0, std::max(abs(c.z) - recth, 0.0f));
		return Vector3::sqrtMagnitude(u) < r * r;
	}

	static bool IsInRect(const Vector3& point, const Vector3* pvecs, const Vector3& center, float yRot);

	static bool IsFloatZero(float value);
	static bool Isdoublezero(double value);

	static void Init();

	static double XSin[721];
	static double XCos[721];
};

#endif