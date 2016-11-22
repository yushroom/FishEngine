#ifndef Vector3_hpp
#define Vector3_hpp

#include "Mathf.hpp"
#include "FishEngine.hpp"

namespace FishEngine
{

class FE_EXPORT Vector3
{
public:
    union
    {
        struct { float x, y, z; };
        float m[3];
    };


    Vector3(const float x, const float y, const float z)
        : x(x), y(y), z(z)
    {
        Assert(!hasNaNs());
    }
    
    Vector3(float* array)
        : x(array[0]), y(array[1]), z(array[2])
    {
    }

    Vector3()
        : Vector3(0, 0, 0)
    {

    }

    const float* data() const
    {
        return m;
    }

    float* data()
    {
        return m;
    }

    Vector3 normalized() const
    {
        return Normalize(*this);
    }

    float magnitude() const
    {
        return ::std::sqrtf(x*x + y*y + z*z);
    }

    float sqrMagnitude() const
    {
        return x*x + y*y + z*z;
    }

    void Set(float newX, float newY, float newZ)
    {
        x = newX;
        y = newY;
        z = newZ;
    }

    void Scale(const Vector3& scale)
    {
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
    
    static Vector3 MoveTowards(const Vector3& current, const Vector3& target, float maxDistanceDelta);
    
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
    static Vector3 Project(const Vector3& vector, const Vector3& onNormal);

    //std::string ToString() const;

    //static void OrthoNormalize(ref Vector3 normal, ref Vector3 tangent);

    // Projects a vector onto a plane defined by a normal orthogonal to the plane.
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

    static Vector3 SmoothDamp(const Vector3& current, const Vector3& target, Vector3& currentVelocity/*ref*/, float smoothTime, float maxSpeed, float deltaTime);

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

struct Int2
{
    int x, y;
};
    
struct Int3{
    //int m[3];
    int x, y, z;
};
    
struct Int4{
    //int m[4];
    int x, y, z, w;
};

}

#endif // Vector3_hpp
