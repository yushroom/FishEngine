#ifndef Vector3_hpp
#define Vector3_hpp

#include "Mathf.hpp"

#if defined(_DEBUG) || defined(DEBUG)
#include <cassert>
#define Assert(exp) assert(exp)
#else
#define Assert(exp) void(0)
#endif

namespace FishEngine{

class Vector3
{
public:
    union {
        struct { float x, y, z; };
        float m[3];
    };


    Vector3(const float x, const float y, const float z)
        : x(x), y(y), z(z)
    {
        Assert(!hasNaNs());
    }
    
    Vector3(float* array) : x(array[0]), y(array[1]), z(array[2]) {}

    Vector3() : Vector3(0, 0, 0) {

    }

    const float* data() const {
        return m;
    }

    float* data() {
        return m;
    }

    Vector3 normalized() const {
        return Normalize(*this);
    }

    float magnitude() const {
        return ::std::sqrtf(x*x + y*y + z*z);
    }

    float sqrMagnitude() const {
        return x*x + y*y + z*z;
    }

    void Set(float new_x, float new_y, float new_z) {
        x = new_x;
        y = new_y;
        z = new_z;
    }

    void Scale(const Vector3& scale) {
        x *= scale.x;
        y *= scale.y;
        z *= scale.z;
    }

    float&      operator[](const int index)         { return m[index]; }
    float       operator[](const int index) const   { return m[index]; }
    Vector3&    operator=(const Vector3& v)         { Assert(!v.hasNaNs()); x = v.x; y = v.y; z = v.z; return *this; }
    Vector3     operator-() const                   { return Vector3(-x, -y, -z); }

    Vector3     operator+(const Vector3& v) const   { Assert(!v.hasNaNs()); return Vector3(x + v.x, y + v.y, z + v.z); }
    Vector3     operator-(const Vector3& v) const   { Assert(!v.hasNaNs()); return Vector3(x - v.x, y - v.y, z - v.z); }
    Vector3     operator*(const Vector3& v) const   { Assert(!v.hasNaNs()); return Vector3(x * v.x, y * v.y, z * v.z); }
    void        operator+=(const Vector3& v)        { Assert(!v.hasNaNs()); x += v.x; y += v.y; z += v.z; }
    void        operator-=(const Vector3& v)        { Assert(!v.hasNaNs()); x -= v.x; y -= v.y; z -= v.z; }
    
    Vector3     operator+(const float f) const      { Assert(!isnan(f)); return Vector3(x + f, y + f, z + f); }
    Vector3     operator-(const float f) const      { Assert(!isnan(f)); return Vector3(x - f, y - f, z - f); }
    Vector3     operator*(const float f) const      { Assert(!isnan(f)); return Vector3(x * f, y * f, z * f); }
    Vector3     operator/(const float f) const      { Assert(!isnan(f) && f!=0.f); return Vector3(x / f, y / f, z / f); }
    void        operator+=(const float f)           { Assert(!isnan(f)); x += f; y += f; z += f; }
    void        operator-=(const float f)           { Assert(!isnan(f)); x -= f; y -= f; z -= f; }
    void        operator*=(const float f)           { Assert(!isnan(f)); x *= f; y *= f; z *= f; }
    void        operator/=(const float f)           { Assert(!isnan(f)); x /= f; y /= f; z /= f; }
    
    friend Vector3  operator*(const float f, const Vector3& v) { Assert(!isnan(f) && !v.hasNaNs()); return Vector3(v.x * f, v.y * f, v.z * f); }
    friend Vector3  operator/(const float f, const Vector3& v) { Assert(!isnan(f) && !v.hasNaNs()); return Vector3(f/v.x, f/v.y, f/v.z); }


    bool          operator==(const Vector3& v) const { Assert(!v.hasNaNs()); return (Mathf::Approximately(x, v.x) && Mathf::Approximately(y, v.y) && Mathf::Approximately(z, v.z)); }
    //bool          operator!=(const Vector3& v) const { Assert(!v.hasNaNs()); return !operator==(v); }


public: // Static 

    static float Angle(const Vector3& from, const Vector3& to)
    {
        return ::std::acosf(Dot(Normalize(from), Normalize(to))) * Mathf::Rad2Deg;
    }

    static float Magnitude(const Vector3& a)
    {
        return ::std::sqrtf(a.x * a.x + a.y * a.y + a.z * a.z);
    }

    static float SqrMagnitude(const Vector3& a)
    {
        return a.x * a.x + a.y * a.y + a.z * a.z;
    }

    static Vector3 ClampMagnitude(const Vector3& vector, float maxLength)
    {
        if (vector.sqrMagnitude() > maxLength * maxLength) {
            return vector.normalized() * maxLength;
        }
        return vector;
    }


    static float Dot(const Vector3& lhs, const Vector3& rhs)
    {
        return lhs.x*rhs.x + lhs.y*rhs.y + lhs.z*rhs.z;
    }

    static Vector3 Cross(const Vector3& lhs, const Vector3& rhs)
    {
        return Vector3(lhs.y * rhs.z - lhs.z * rhs.y, lhs.z * rhs.x - lhs.x * rhs.z, lhs.x * rhs.y - lhs.y * rhs.x);
    }

    static float Distance(const Vector3& a, const Vector3& b)
    {
        return Magnitude(a-b);
    }

    static float DistanceSquared(const Vector3& a, const Vector3& b)
    {
        return SqrMagnitude(a-b);
    }
    
    // a*(1-t) + b*t
    static Vector3 Lerp(const Vector3& a, const Vector3& b, float t)
    {
        t = Mathf::Clamp01(t);
        return Vector3(a.x + (b.x - a.x) * t, a.y + (b.y - a.y) * t, a.z + (b.z - a.z) * t);
    }
    
    // a*(1-t) + b*t
    static Vector3 LerpUnClamped(const Vector3& a, const Vector3& b, float t)
    {
        return Vector3(a.x + (b.x - a.x) * t, a.y + (b.y - a.y) * t, a.z + (b.z - a.z) * t);
    }
    
    static Vector3 Max(const Vector3& lhs, const Vector3& rhs)
    {
        return Vector3(std::max(lhs.x, rhs.x), std::max(lhs.y, rhs.y), std::max(lhs.z, rhs.z));
    }
    
    static Vector3 Min(const Vector3& lhs, const Vector3& rhs)
    {
        return Vector3(std::min(lhs.x, rhs.x), std::min(lhs.y, rhs.y), std::min(lhs.z, rhs.z));
    }
    
    static Vector3 MoveTowards(const Vector3& current, const Vector3& target, float maxDistanceDelta)
    {
        Vector3 a = target - current;
        float magnitude = a.magnitude();
        if (magnitude <= maxDistanceDelta || magnitude == 0.f)
        {
            return target;
        }
        return current + a / magnitude * maxDistanceDelta;
    }
    
    static Vector3 Normalize(const Vector3& value)
    {
        float num = value.magnitude();
        if (num > 1E-05f)
        {
            return value / num;
        }
        return Vector3::zero;
    }

    void Normalize()
    {
        float len = magnitude();
        if (len > 1e-5f) {
            x /= len;
            y /= len;
            z /= len;
        }
        else {
            x = y = z = 0;
        }
    }

    // Projects a vector onto another vector.
    static Vector3 Project(const Vector3& vector, const Vector3& onNormal)
    {
        float num = Dot(onNormal, onNormal);
        if (num < 1e-5f)
        {
            return Vector3::zero;
        }
        return onNormal * Vector3::Dot(vector, onNormal) / num;
    }

    //std::string ToString() const;

    //static void OrthoNormalize(ref Vector3 normal, ref Vector3 tangent);

    static Vector3 ProjectOnPlane(const Vector3& vector, const Vector3& planeNormal);
    
    static Vector3 Reflect(const Vector3& inDirection, const Vector3& inNormal);

    static Vector3 RotateTowards(const Vector3& current, const Vector3& target, float maxRadiansDelta, float maxMagnitudeDelta);

    static Vector3 Scale(const Vector3& a, const Vector3& b) {
        return Vector3(a.x*b.x, a.y*b.y, a.z*b.z);
    }

    static Vector3 Slerp(const Vector3& a, const Vector3& b, float t);

    static Vector3 SlerpUnclamped(const Vector3& a, const Vector3& b, float t);


    //static Vector3 SmoothDamp(const Vector3& current, const Vector3& target, Vector3& currentVelocity/*ref*/, float smoothTime, float maxSpeed = Mathf::PositiveInfinity)
    //{
    //    float deltaTime = Time::deltaTime;
    //    return Vector3::SmoothDamp(current, target, currentVelocity, smoothTime, maxSpeed, deltaTime);
    //}

    static Vector3 SmoothDamp(const Vector3& current, const Vector3& target, Vector3& currentVelocity/*ref*/, float smoothTime, float maxSpeed, float deltaTime)
    {
        smoothTime = Mathf::Max(0.0001f, smoothTime);
        float num = 2.f / smoothTime;
        float num2 = num * deltaTime;
        float d = 1.f / (1.f + num2 + 0.48f * num2 * num2 + 0.235f * num2 * num2 * num2);
        Vector3 vector = current - target;
        Vector3 vector2 = target;
        float maxLength = maxSpeed * smoothTime;
        vector = Vector3::ClampMagnitude(vector, maxLength);
        Vector3 newTarget = current - vector;
        Vector3 vector3 = (currentVelocity + num * vector) * deltaTime;
        currentVelocity = (currentVelocity - num * vector3) * d;
        Vector3 vector4 = newTarget + (vector + vector3) * d;
        if (Vector3::Dot(vector2 - current, vector4 - vector2) > 0.f)
        {
            vector4 = vector2;
            currentVelocity = (vector4 - vector2) / deltaTime;
        }
        return vector4;
    }

    const static Vector3 back;    // (0, 0, -1)
    const static Vector3 down;    // (0, -1, 0)
    const static Vector3 forward; // (0, 0, 1)
    const static Vector3 left;    // (-1, 0, 0)
    const static Vector3 one;     // (1, 1, 1)
    const static Vector3 right;   // (1, 0, 0)
    const static Vector3 up;      // (0, 1, 0)
    const static Vector3 zero;    // (0, 0, 0)

private:
    bool hasNaNs() const { return isnan(x) || isnan(y) || isnan(z); }
};


class Vector4
{
public:
    //const static float kEpsilon = 1e-5f;

    union {
        struct { float x, y, z, w; };
        //struct { Vector3 xyz; float w; };
        float m[4];
    };

    Vector4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {
        Assert(!hasNaNs());
    }
    Vector4() : Vector4(0, 0, 0, 0) {}
    Vector4(const Vector3& v3, float w) : Vector4(v3.x, v3.y, v3.z, w) {}

    operator Vector3() const {
        return Vector3(x, y, z);
    }

    const float* data() const {
        return m;
    }

    float* data() {
        return m;
    }

    float operator[](int index) const {
        return m[index];
    }

    float& operator[](int index) {
        return m[index];
    }

    friend Vector4 operator*(const Vector4& lhs, const Vector4& rhs)
    {
        return Vector4(lhs.x*rhs.x, lhs.y*rhs.y, lhs.z*rhs.z, lhs.w*rhs.w);
    }

    friend Vector4 operator*(const float lhs, const Vector4& rhs)
    {
        return Vector4(lhs*rhs.x, lhs*rhs.y, lhs*rhs.z, lhs*rhs.w);
    }

    friend Vector4 operator*(const Vector4& lhs, const float rhs)
    {
        return Vector4(lhs.x*rhs, lhs.y*rhs, lhs.z*rhs, lhs.w*rhs);
    }

    friend Vector4 operator+(const Vector4& lhs, const Vector4& rhs)
    {
        return Vector4(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z, lhs.w + rhs.w);
    }

    friend Vector4 operator-(const Vector4& lhs, const Vector4& rhs)
    {
        return Vector4(lhs.x-rhs.x, lhs.y-rhs.y, lhs.z-rhs.z, lhs.w-rhs.w);
    }

    Vector4 normalized() const
    {
        return Vector4::Normalize(*this);
    }

    float magnitude() const
    {
        //return Mathf.Sqrt(Vector4.Dot(this, this));
        return ::std::sqrtf(x*x + y*y + z*z + w*w);
    }

    float sqrMagnitude() const
    {
        return x*x + y*y + z*z + w*w;
    }

    float SqrMagnitude() const
    {
        return x*x + y*y + z*z + w*w;
    }

    static float Magnitude(const Vector4& a)
    {
        return a.magnitude();
    }

    static float SqrMagnitude(const Vector4& a)
    {
        return a.sqrMagnitude();
    }

    void Normalize()
    {
        float num = magnitude();
        if (num > 1E-05f)
        {
            x /= num;
            y /= num;
            z /= num;
            w /= num;
        }
        else
        {
            x = y = z = w = 0;
        }
    }

    static Vector4 Normalize(const Vector4& a)
    {
        float num = a.magnitude();
        if (num > 1E-05f)
        {
            return Vector4(a.x/num, a.y/num, a.z/num, a.w/num);
        }
        return Vector4::zero;
    }

    static Vector4 Min(const Vector4& lhs, const Vector4& rhs)
    {
        return Vector4(Mathf::Min(lhs.x, rhs.x), Mathf::Min(lhs.y, rhs.y), Mathf::Min(lhs.z, rhs.z), Mathf::Min(lhs.w, rhs.w));
    }

    static Vector4 Max(const Vector4& lhs, const Vector4& rhs)
    {
        return Vector4(Mathf::Max(lhs.x, rhs.x), Mathf::Max(lhs.y, rhs.y), Mathf::Max(lhs.z, rhs.z), Mathf::Max(lhs.w, rhs.w));
    }

    static float Dot(const Vector4& lhs, const Vector4& rhs)
    {
        return lhs.x*rhs.x + lhs.y*rhs.y + lhs.z*rhs.z + lhs.w*rhs.w;
    }


    static const Vector4 one;
    static const Vector4 zero;


private:
    bool hasNaNs() const { return isnan(x) || isnan(y) || isnan(z) || isnan(w); }
};
    
struct Int2{
    int m[2];
};
    
struct Int3{
    int m[3];
};
    
struct Int4{
    int m[4];
};

}

#endif // Vector3_hpp
