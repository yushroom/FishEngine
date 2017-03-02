#include "Quaternion.hpp"

namespace FishEngine {

    const Quaternion Quaternion::identity(0, 0, 0, 1);

    Vector3 Quaternion::eulerAngles() const
    {
        // http://www.euclideanspace.com/maths/geometry/rotations/conversions/quaternionToEuler/
        // https://en.wikipedia.org/wiki/Conversion_between_quaternions_and_Euler_angles#Quaternion_to_Euler_Angles_Conversion
        float heading, attitude, bank;  // y, z, x
        float sqx = x*x;
        float sqy = y*y;
        float sqz = z*z;
        float sqw = w*w;
        float unit = sqx + sqy + sqz + sqw;
        float test = x*y + z*w;
        if (test > 0.499f * unit) {
            heading = 2 * atan2(x, w);
            attitude = Mathf::PI / 2.f;
            bank = 0.0f;
        }
        else {
            if (test < -0.499f * unit) {
                heading = -2 * atan2(x, w);
                attitude = -Mathf::PI / 2;
                bank = 0.f;
            }
            else {
                heading = atan2(2 * (y*w - x*z), sqx - sqy - sqz + sqw);
                attitude = asin(2 * test / unit);
                bank = atan2(2 * (x*w - y*z), -sqx + sqy - sqz + sqw);
            }
        }
        return Vector3(bank, heading, attitude) * Mathf::Rad2Deg;
        //return Vector3(pitch(), yaw(), roll());
    }

    void Quaternion::NormalizeSelf()
    {
        float inv_len = 1.0f / std::sqrtf(x*x + y*y + z*z + w*w);
        x *= inv_len;
        y *= inv_len;
        z *= inv_len;
        w *= inv_len;
    }

    FishEngine::Vector3 Quaternion::operator*(const Vector3 & point) const
    {
        float num = x * 2.f;
        float num2 = y * 2.f;
        float num3 = z * 2.f;
        float num4 = x * num;
        float num5 = y * num2;
        float num6 = z * num3;
        float num7 = x * num2;
        float num8 = x * num3;
        float num9 = y * num3;
        float num10 = w * num;
        float num11 = w * num2;
        float num12 = w * num3;
        Vector3 result;
        result.x = (1.f - (num5 + num6)) * point.x + (num7 - num12) * point.y + (num8 + num11) * point.z;
        result.y = (num7 + num12) * point.x + (1.f - (num4 + num6)) * point.y + (num9 - num10) * point.z;
        result.z = (num8 - num11) * point.x + (num9 + num10) * point.y + (1.f - (num4 + num5)) * point.z;
        return result;
    }

//    FishEngine::Quaternion Quaternion::operator*(const Quaternion & rhs) const
//    {
//        // [p.w*q.v + q.w*p.v + corss(p.v, q.v), p.w*q.w-dot(p.v, q.v)]
//        return Quaternion(
//            w * rhs.x + x * rhs.w + y * rhs.z - z * rhs.y,
//            w * rhs.y + y * rhs.w + z * rhs.x - x * rhs.z,
//            w * rhs.z + z * rhs.w + x * rhs.y - y * rhs.x,
//            w * rhs.w - x * rhs.x - y * rhs.y - z * rhs.z);
//    }

    FishEngine::Quaternion Quaternion::AngleAxis(const float angle, const Vector3& axis)
    {
        auto a = axis.normalized();
        Quaternion Result;

		float angleHalf = Mathf::Radians(angle) * 0.5f;

        float s = Mathf::Sin(angleHalf);

        Result.w = Mathf::Cos(angleHalf);
        Result.x = a.x * s;
        Result.y = a.y * s;
        Result.z = a.z * s;
        return Result;
    }

    FishEngine::Quaternion Quaternion::Euler(const Vector3& euler)
    {
        // http://www.euclideanspace.com/maths/geometry/rotations/conversions/eulerToQuaternion/index.htm
//        Quaternion result;
//        auto rad = euler * (Mathf::Deg2Rad * 0.5f);
//        //auto c = Vector3(cos(rad.x), cos(rad.y), cos(rad.z));
//        //auto s = Vector3(sin(rad.x), sin(rad.y), sin(rad.z));
//        //result.w = c.x * c.y * c.z + s.x * s.y * s.z;
//        //result.x = s.x * c.y * c.z - c.x * s.y * s.z;
//        //result.y = c.x * s.y * c.z + s.x * c.y * s.z;
//        //result.z = c.x * c.y * s.z - s.x * s.y * c.z;
//
//        // heading, attitude, bank -> y,z,x
//        auto c = Vector3(cos(rad.y), cos(rad.z), cos(rad.x));
//        auto s = Vector3(sin(rad.y), sin(rad.z), sin(rad.x));
//        result.w = c.x * c.y * c.z - s.x * s.y * s.z;
//        result.x = s.x * s.y * c.z + c.x * c.y * s.z;
//        result.y = s.x * c.y * c.z + c.x * s.y * s.z;
//        result.z = c.x * s.y * c.z - s.x * c.y * s.z;
//        return result;
		return AngleAxis(euler.y, Vector3{0,1,0}) * AngleAxis(euler.x, Vector3{1, 0, 0}) * AngleAxis(euler.z, Vector3{0, 0, 1});
    }

    Quaternion Quaternion::FromToRotation(const Vector3& fromDirection, const Vector3& toDirection)
    {
        const float cos_theta = Vector3::Dot(fromDirection, toDirection);
        if (cos_theta == 1.0f) {
            return Quaternion::identity;
        } else {
            if (cos_theta == -1.0f) {
                return Quaternion(1, 0, 0, 0);
            } else {
                Vector3 w = Vector3::Cross(fromDirection, toDirection);
                return Quaternion(w.x, w.y, w.z, 1+cos_theta);
            }
        }
    }

    FishEngine::Quaternion Quaternion::Slerp(const Quaternion& a, const Quaternion& b, float t)
    {
        return SlerpUnclamped(a, b, Mathf::Clamp01(t));
    }

    FishEngine::Quaternion Quaternion::SlerpUnclamped(const Quaternion& a, const Quaternion& b, float t)
    {
        // from Assimp

        float cosTheta = Quaternion::Dot(a, b);

        // adjust signs (if necessary)
        Quaternion end = b;
        if (cosTheta < 0.0f)
        {
            cosTheta = -cosTheta;
            end.x = -end.x;
            end.y = -end.y;
            end.z = -end.z;
            end.w = -end.w;
        }

        float sclp, sclq;
        if ((1.0f - cosTheta) > 0.0001f) {
            // Standard case (slerp)
            float theta, sinTheta;
            theta    = std::acos(cosTheta);
            sinTheta = std::sin(theta);
            sclp     = std::sin((1.0f - t) * theta) / sinTheta;
            sclq     = std::sin(t * theta) / sinTheta;
        }
        else {
            // Very close, do LERP (because it's faster)
            sclp = 1.0f - t;
            sclq = t;
        }

        return Quaternion(
            sclp * a.x + sclq * end.x,
            sclp * a.y + sclq * end.y,
            sclp * a.z + sclq * end.z,
            sclp * a.w + sclq * end.w);
    }

    FishEngine::Quaternion Quaternion::RotateTowards(const Quaternion& from, const Quaternion& to, float maxDegreesDelta)
    {
        float num = Quaternion::Angle(from, to);
        if (num == 0.f)
        {
            return to;
        }
        float t = Mathf::Min(1.f, maxDegreesDelta / num);
        return Quaternion::SlerpUnclamped(from, to, t);
    }
}
