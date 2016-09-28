#ifndef Quaternion_hpp
#define Quaternion_hpp

#include "Vector3.hpp"

namespace FishEngine {

class Quaternion
{
public:
	union {
        struct { float x, y, z, w; };
        float m[4];
    };

    Quaternion(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}
    Quaternion() : Quaternion(0, 0, 0, 1) {}

    Vector3 eulerAngles() const;

    float roll() const {
    	return Mathf::Rad2Deg * ::std::atan2(2.0f * (x*y+w*z), w*w+x*x-y*y-z*z);
    }

    float pitch() const {
		return Mathf::Rad2Deg * ::std::atan2(2.0f * (y*z+w*x), w*w-x*x-y*y+z*z);
    }

    float yaw() const {
    	return Mathf::Rad2Deg * ::std::asin(Mathf::Clamp(-2.f*(x*z-w*y), -1.f, 1.f));
    }
    
    void setEulerAngles(const Vector3& angles)
    {
    	*this = Quaternion::Euler(angles);
    }

    void Set(float new_x, float new_y, float new_z, float new_w)
    {
        x = new_x;
        y = new_y;
        z = new_z;
        w = new_w;
    }

    float operator[](int index) const {
        return m[index];
    }

    float& operator[](int index) {
        return m[index];
    }

    friend Quaternion operator *(Quaternion lhs, Quaternion rhs)
    {
        return Quaternion(lhs.w * rhs.x + lhs.x * rhs.w + lhs.y * rhs.z - lhs.z * rhs.y, lhs.w * rhs.y + lhs.y * rhs.w + lhs.z * rhs.x - lhs.x * rhs.z, lhs.w * rhs.z + lhs.z * rhs.w + lhs.x * rhs.y - lhs.y * rhs.x, lhs.w * rhs.w - lhs.x * rhs.x - lhs.y * rhs.y - lhs.z * rhs.z);
    }

    friend Vector3 operator *(Quaternion rotation, Vector3 point)
    {
        //return glm::quat(rotation) * glm::vec3(point);
        float num = rotation.x * 2.f;
        float num2 = rotation.y * 2.f;
        float num3 = rotation.z * 2.f;
        float num4 = rotation.x * num;
        float num5 = rotation.y * num2;
        float num6 = rotation.z * num3;
        float num7 = rotation.x * num2;
        float num8 = rotation.x * num3;
        float num9 = rotation.y * num3;
        float num10 = rotation.w * num;
        float num11 = rotation.w * num2;
        float num12 = rotation.w * num3;
        Vector3 result;
        result.x = (1.f - (num5 + num6)) * point.x + (num7 - num12) * point.y + (num8 + num11) * point.z;
        result.y = (num7 + num12) * point.x + (1.f - (num4 + num6)) * point.y + (num9 - num10) * point.z;
        result.z = (num8 - num11) * point.x + (num9 + num10) * point.y + (1.f - (num4 + num5)) * point.z;
        return result;
    }


    static float Angle(const Quaternion& a, const Quaternion& b)
    {
        float f = Quaternion::Dot(a, b);
        return Mathf::Acos(Mathf::Min(Mathf::Abs(f), 1.f)) * 2.f * 57.29578f;
    }

    static Quaternion AngleAxis(float angle, const Vector3& axis)
    {
        //return glm::angleAxis(Mathf::Radians(angle), (glm::vec3)axis.normalized());
        Quaternion Result;

        float s = Mathf::Sin(angle * 0.5f);

        Result.w = Mathf::Cos(angle * 0.5f);
        Result.x = axis.x * s;
        Result.y = axis.y * s;
        Result.z = axis.z * s;
        return Result;
    }

    void ToAngleAxis(float* angle, Vector3* axis);

    static float Dot(const Quaternion& a, const Quaternion& b)
    {
        return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
    }

    static Quaternion Euler(float x, float y, float z)
    {
        return Euler(Vector3(x, y, z));
    }

    static Quaternion Euler(const Vector3& euler);

    static Quaternion FromToRotation(const Vector3& fromDirection, const Vector3& toDirection);
    
    void SetFromToRotation(const Vector3& fromDirection, const Vector3& toDirection)
    {
        *this = Quaternion::FromToRotation(fromDirection, toDirection);
    }


    static Quaternion Inverse(const Quaternion& rotation)
    {
        float num = 1.0f / Dot(rotation, rotation);
        return Quaternion(-rotation.x * num, -rotation.y * num, -rotation.z*num, rotation.w*num);
    }

    static Quaternion Lerp(const Quaternion& a, const Quaternion& b, float t);
    static Quaternion LerpUnclamped(const Quaternion& a, const Quaternion& b, float t);

    static Quaternion Slerp(const Quaternion& a, const Quaternion& b, float t);
    static Quaternion SlerpUnclamped(const Quaternion& a, const Quaternion& b, float t);

    static Quaternion LookRotation(const Vector3& forward, const Vector3& upwards);
    
    void SetLookRotation(const Vector3& view)
    {
        Vector3 up = Vector3::up;
        SetLookRotation(view, up);
    }

    void SetLookRotation(const Vector3& view, const Vector3& up)
    {
        *this = Quaternion::LookRotation(view, up);
    }

    static Quaternion LookRotation(const Vector3& forward);

    static Quaternion RotateTowards(const Quaternion& from, const Quaternion& to, float maxDegreesDelta)
    {
        float num = Quaternion::Angle(from, to);
        if (num == 0.f)
        {
            return to;
        }
        float t = Mathf::Min(1.f, maxDegreesDelta / num);
        return Quaternion::SlerpUnclamped(from, to, t);
    }

    static const Quaternion identity;
};

}

#endif // Quaternion_hpp
