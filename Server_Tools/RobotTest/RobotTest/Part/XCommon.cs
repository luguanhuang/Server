//#define PRECISION_USED

using System;
using UnityEngine;

namespace XUtliPoolLib
{
    public class XCommon : XSingleton<XCommon>
    {
        public XCommon()
        {
            _idx = 5;   // _random.Next(4, 8);
        }

        public readonly float FrameStep = (1 / 30.0f);
        private static readonly float _eps = 0.0001f;

        public static float XEps
        {
            get { return _eps; }
        }

        private System.Random _random = new System.Random(DateTime.Now.Millisecond);

        private int _idx = 0;

        private uint[] _seeds = { 17, 33, 65, 129 };

        private int _new_id = 0;

        public int New_id
        {
            get { return ++_new_id; }
        }

        public long UniqueToken
        {
            get { return DateTime.Now.Ticks + New_id; }
        }

        //Daniel J. Bernstein, Times 33 with Addition Hash Function
        public uint XHash(string str)
        {
            if (str == null) return 0;

            uint hash = 0;
            for (int i = 0; i < str.Length; i++ )
            {
                hash = (hash << _idx) + hash + str[i];
            }

            return hash;
        }

        public bool IsEqual(float a, float b)
        {
#if (PRECISION_USED)
            //return Mathf.Approximately(Mathf.Abs(a - b), _eps);
            return Mathf.Abs(a - b) < _eps;
#else
            return a == b;
#endif
        }

        public bool IsLess(float a, float b)
        {
#if (PRECISION_USED)
            float v = a - b;
            return !IsEqual(v, 0) && (v < 0/*Mathf.Epsilon*/);
#else
            return a < b;
#endif
        }

        public bool IsGreater(float a, float b)
        {
#if (PRECISION_USED)
            float v = a - b;
            return !IsEqual(v, 0) && (v > 0/*Mathf.Epsilon*/);
#else
            return a > b;
#endif
        }

        public bool IsEqualLess(float a, float b)
        {
#if (PRECISION_USED)
            return IsEqual(a, b) || IsLess(a, b);
#else
            return a <= b;
#endif
        }

        public bool IsEqualGreater(float a, float b)
        {
#if (PRECISION_USED)
            return IsEqual(a, b) || IsGreater(a, b);
#else
            return a >= b;
#endif
        }

        public uint GetToken()
        {
            return (uint)DateTime.Now.Millisecond;
        }

        public void ProcessValueDamp(ref float values, float target, ref float factor, float deltaT)
        {
            if (XCommon.singleton.IsEqual(values, target))
            {
                values = target;
                factor = 0;
            }
            else
            {
                values += (target - values) * Mathf.Min(1.0f, deltaT * factor);
            }
        }

        public void ProcessValueEvenPace(ref float value, float target, float speed, float deltaT)
        {
            float dif = target - value;
            float temp = target - (dif - speed * deltaT);

            if (XCommon.singleton.IsGreater((target - temp) * dif, 0)) value = temp;
            else value = target;
        }

        /*
         * check whether the specific circle is intersected by the segment.
         */
        public bool Intersection(Vector2 begin, Vector2 end, Vector2 center, float radius)
        {
            /*
             * (y1 - y2)x - (x1 - x2)y + x1y2 -x2y1 = 0;
             * end(x1, y1)
             * begin(x2, y2)
             */

            float A = end.y - begin.y;
            float B = -(end.x - begin.y);
            float C = end.x * begin.y - begin.x * end.y;

            float sqare = A * A + B * B;
            if (IsGreater(sqare, 0))
            {
                float D = Mathf.Abs(A * center.x + B * center.y + C);
                float dist = D / Mathf.Sqrt(sqare);

                if (IsLess(dist, radius))
                {
                    return true;
                }
            }

            return false;
        }

        /*
         * check whether the specific circle is intersected by the segment.
         * the first intersection point from begin is returned by value t,
         * which means the distance from begin-point.
         */
        public bool Intersection(Vector2 begin, Vector2 end, Vector2 center, float radius, out float t)
        {
            t = 0;

            /*
             * (y1 - y2)x - (x1 - x2)y + x1y2 -x2y1 = 0;
             * end(x1, y1)
             * begin(x2, y2)
             */

            float A = end.y - begin.y;
            float B = -(end.x - begin.y);
            float C = end.x * begin.y - begin.x * end.y;

            float sqare = A * A + B * B;
            if (IsGreater(sqare, 0))
            {
                float D = Mathf.Abs(A * center.x + B * center.y + C);
                float dist = D / Mathf.Sqrt(sqare);

                if (IsLess(dist, radius))
                {
                    float t1 = center.x - begin.x;
                    float t2 = center.y - begin.y;
                    float cdist = Mathf.Sqrt(t1 * t1 + t2 * t2);

                    if (IsLess(cdist, 0))
                    {
                        t = radius;
                    }
                    else
                    {
                        float ang1 = Mathf.Asin(dist / cdist);
                        float t3 = cdist * Mathf.Cos(ang1);
                        float ang2 = Mathf.Asin(dist / radius);
                        float t4 = radius * Mathf.Cos(ang2);

                        t = t3 - t4;
                    }

                    return true;
                }
            }

            return false;
        }

        private float CrossProduct(float x1, float z1, float x2, float z2)
        {
            return x1 * z2 - x2 * z1;
        }

        public bool IsLineSegmentCross(Vector3 p1, Vector3 p2, Vector3 q1, Vector3 q2)
        {
            //fast detect
            if (Mathf.Min(p1.x, p2.x) <= Mathf.Max(q1.x, q2.x) &&
                Mathf.Min(q1.x, q2.x) <= Mathf.Max(p1.x, p2.x) &&
                Mathf.Min(p1.z, p2.z) <= Mathf.Max(q1.z, q2.z) &&
                Mathf.Min(q1.z, q2.z) <= Mathf.Max(p1.z, p2.z))
            {
                //( p1 - q1 ) * ( q2 - q1 )
                float p1xq = CrossProduct(p1.x - q1.x, p1.z - q1.z,
                                           q2.x - q1.x, q2.z - q1.z);
                //( p2 - q1 ) * ( q2 - q1 )
                float p2xq = CrossProduct(p2.x - q1.x, p2.z - q1.z,
                                           q2.x - q1.x, q2.z - q1.z);

                //( q1 - p1 ) * ( p2 - p1 )
                float q1xp = CrossProduct(q1.x - p1.x, q1.z - p1.z,
                                           p2.x - p1.x, p2.z - p1.z);
                //( q2 - p1 ) * ( p2 - p1 )
                float q2xp = CrossProduct(q2.x - p1.x, q2.z - p1.z,
                                           p2.x - p1.x, p2.z - p1.z);

                return ((p1xq * p2xq <= 0) && (q1xp * q2xp <= 0));
            }

            return false;
        }

        public Vector3 Horizontal(Vector3 v)
        {
            v.y = 0;
            return v.normalized;
        }

        public float AngleNormalize(float basic, float degree)
        {
            Vector3 v1 = FloatToAngle(basic);
            Vector3 v2 = FloatToAngle(degree);

            float angle = Vector3.Angle(v1, v2);

            return Clockwise(v1, v2) ? basic + angle : basic - angle;
        }

        public Vector2 HorizontalRotateVetor2(Vector2 v, float degree, bool normalized = true)
        {
            degree = -degree;

            float rad = degree * Mathf.Deg2Rad;
            float sinA = Mathf.Sin(rad);
            float cosA = Mathf.Cos(rad);

            float x = v.x * cosA - v.y * sinA;
            float y = v.x * sinA + v.y * cosA;

            v.x = x;
            v.y = y;
            return normalized ? v.normalized : v;
        }

        public Vector3 HorizontalRotateVetor3(Vector3 v, float degree, bool normalized = true)
        {
            degree = -degree;

            float rad = degree * Mathf.Deg2Rad;
            float sinA = Mathf.Sin(rad);
            float cosA = Mathf.Cos(rad);

            float x = v.x * cosA - v.z * sinA;
            float z = v.x * sinA + v.z * cosA;

            v.x = x;
            v.z = z;
            return normalized ? v.normalized : v;
        }

        public float TicksToSeconds(long tick)
        {
            long ms = tick / 10000;
            return ms / 1000.0f;
        }

        public long SecondsToTicks(float time)
        {
            long ms = (long)(time * 1000.0f);
            return ms * 10000;
        }

        public float AngleToFloat(Vector3 dir)
        {
            float face = Vector3.Angle(Vector3.forward, dir);
            return XCommon.singleton.Clockwise(Vector3.forward, dir) ? face : -face;
        }

        public float AngleWithSign(Vector3 from, Vector3 to)
        {
            float angle = Vector3.Angle(from, to);
            return Clockwise(from, to) ? angle : -angle;
        }

        public Vector3 FloatToAngle(float angle)
        {
            return Quaternion.Euler(0, angle, 0) * Vector3.forward;
        }

        public Quaternion VectorToQuaternion(Vector3 v)
        {
            return XCommon.singleton.FloatToQuaternion(XCommon.singleton.AngleWithSign(Vector3.forward, v));
        }

        public Quaternion FloatToQuaternion(float angle)
        {
            return Quaternion.Euler(0, angle, 0);
        }

        public Quaternion RotateToGround(Vector3 pos, Vector3 forward)
        {
            //get normal
            RaycastHit hit;

            if (Physics.Raycast(new Ray(pos + Vector3.up, Vector3.down), out hit, 5.0f, 1 | 1 << 9))
            {
                Vector3 v1 = hit.normal; Vector3 v2 = forward;
                Vector3.OrthoNormalize(ref v1, ref v2);
                return Quaternion.LookRotation(v2, v1);
            }

            return Quaternion.identity;
        }

        /*
         * whether vec-r is in the clockwise direction of vec-f in x-z plane.
         */
        public bool Clockwise(Vector3 fiduciary, Vector3 relativity)
        {
            float r = fiduciary.z * relativity.x - fiduciary.x * relativity.z;
            return r > 0;
        }

        public bool Clockwise(Vector2 fiduciary, Vector2 relativity)
        {
            float r = fiduciary.y * relativity.x - fiduciary.x * relativity.y;
            return r > 0;
        }

        /*
         * rect point sequence:
         * left-bottom-->left-top-->right-top-->right-bottom
         * 
         * the center of rect is the (0, 0) for axis
         */
        public bool IsInRect(Vector3 point, Vector3[] rect, Vector3 center, Quaternion rotation)
        {
            float y = -(rotation.eulerAngles.y % 360) / 180.0f * Mathf.PI;
            /*
             * Quaternion = (xi + yj + zk + w ) = (x, y, z, w)
             * Q = cos (a/2) + i (x * sin(a/2)) + j (y * sin(a/2)) + k (z * sin(a/2))    (a 为旋转角度)
             * Q.w = cos (angle / 2) 
             * Q.x = axis.x * sin (angle / 2)
             * Q.y = axis.y * sin (angle / 2)
             * Q.z = axis.z * sin (angle / 2)
             */
            Quaternion q = Quaternion.identity;
            q.w = Mathf.Cos(y / 2.0f);
            q.x = 0;
            q.y = Mathf.Sin(y / 2.0f);
            q.z = 0;

            point = q * (point - center);
            return (point.x > rect[0].x && point.x < rect[3].x && point.z > rect[0].z && point.z < rect[2].z);
        }

        //return float between 0 ~ 1.0
        public float RandomPercentage()
        {
            return (float)_random.NextDouble();
        }

        /*
         * return float between min ~ 1.0
         * if min > 1.0 return 1.0
         */
        public float RandomPercentage(float min)
        {
            if (IsEqualGreater(min, 1.0f)) return 1.0f;

            float f = (float)_random.NextDouble();

            if (IsGreater(f, min))
                return f;
            else
                return (f / min) * (1 - min) + min;
        }

        public float RandomFloat(float max)
        {
            return RandomPercentage() * max;
        }

        public float RandomFloat(float min, float max)
        {
            return min + RandomFloat(max - min);
        }

        public int RandomInt(int min, int max)
        {
            return _random.Next(min, max);
        }

        public int RandomInt(int max)
        {
            return _random.Next(max);
        }

        public int RandomInt()
        {
            return _random.Next();
        }

        //floating between 0 ~ 1.0
        public float GetFloatingValue(float value, float floating)
        {
            if (IsEqualLess(floating, 0) || IsEqualGreater(floating, 1)) return value;

            float random = IsLess(RandomPercentage(), 0.5f) ? 1 - floating : 1 + floating;

            return value * random;
        }

        /*
         * distance: the distance to be move.
         * timespan: the time to use
         * nearenough: the value considered as we got the destination
         */
        public float GetSmoothFactor(float distance, float timespan, float nearenough)
        {
            float _hitFactor = 0;

            distance = Mathf.Abs(distance);

            if (distance > XEps)
            {
                float deltaT = Time.smoothDeltaTime;
                float div = nearenough / distance;
                float frame = timespan / deltaT;

                if (frame > 1)
                {
                    float q = Mathf.Pow(div, 1.0f / frame);
                    _hitFactor = (1 - q) / deltaT;
                }
                else
                {
                    _hitFactor = Mathf.Infinity;
                }
            }

            return _hitFactor;
        }

        //get the jump velocity from air time
        public float GetJumpForce(float airTime, float g)
        {
            return 0;
        }

        public string SecondsToString(int time)
        {
            int minute = time/60;
            int second = time%60;

            return string.Format("{0:D2}:{1}", minute, second);
        }

        // note: if two child has same name, just return first
        public Transform FindChildRecursively(Transform t, string name)
        {
            if (t.name == name)
                return t;

            for (int i = 0; i < t.childCount; i++)
            {
                Transform ct = FindChildRecursively(t.GetChild(i), name);
                if (ct != null)
                    return ct;
            }
            return null;
        }

        public override bool Init()
        {
            return true;
        }

        public override void Uninit()
        {

        }
    }
}