#ifndef Vector4_hpp
#define Vector4_hpp

#include "Mathf.hpp"
#include "FishEngine.hpp"
#include "Vector3.hpp"

namespace FishEngine
{

class FE_EXPORT Vector4
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
    
    void operator*=(const float rhs)
    {
        x *= rhs;
        y *= rhs;
        z *= rhs;
        w *= rhs;
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
    
} // namespace FishEngine

#endif // Vector4_hpp
