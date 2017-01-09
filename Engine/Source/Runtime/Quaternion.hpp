#ifndef Quaternion_hpp
#define Quaternion_hpp

#include "Vector3.hpp"

namespace FishEngine {

class FE_EXPORT Quaternion
{
public:
	union {
        struct { float x, y, z, w; };
        float m[4];
    };

    Quaternion(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {
        NormalizeSelf();
    }
    Quaternion() : Quaternion(0, 0, 0, 1) {}

    Vector3 eulerAngles() const;

    void NormalizeSelf();
    
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

    Quaternion operator *(Quaternion rhs) const;

    Vector3 operator *(Vector3 point) const;

    bool operator==(const Quaternion& q) {
        return Mathf::Approximately(x, q.x) 
            && Mathf::Approximately(y, q.y)
            && Mathf::Approximately(z, q.z) 
            && Mathf::Approximately(w, q.w);
    }


    static float Angle(const Quaternion& a, const Quaternion& b)
    {
        float f = Quaternion::Dot(a, b);
        return Mathf::Acos(Mathf::Min(Mathf::Abs(f), 1.f)) * 2.f * 57.29578f;
    }

    /// @brief Creates a rotation which rotates angle degrees around axis.
    static Quaternion AngleAxis(const float angle, const Vector3& axis);

    //void ToAngleAxis(float* angle, Vector3* axis);

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

    Quaternion inverse() const
    {
        return Inverse(*this);
    }

    //static Quaternion Lerp(const Quaternion& a, const Quaternion& b, float t);
    //static Quaternion LerpUnclamped(const Quaternion& a, const Quaternion& b, float t);

    static Quaternion Slerp(const Quaternion& a, const Quaternion& b, float t);
    static Quaternion SlerpUnclamped(const Quaternion& a, const Quaternion& b, float t);

    //static Quaternion LookRotation(const Vector3& forward, const Vector3& upwards);
    
    //void SetLookRotation(const Vector3& view)
    //{
    //    Vector3 up = Vector3::up;
    //    SetLookRotation(view, up);
    //}

    //void SetLookRotation(const Vector3& view, const Vector3& up)
    //{
    //    *this = Quaternion::LookRotation(view, up);
    //}

    //static Quaternion LookRotation(const Vector3& forward);

    static Quaternion RotateTowards(const Quaternion& from, const Quaternion& to, float maxDegreesDelta);

    static const Quaternion identity;
};
    
    
    // Quaternion
    template<typename Archive>
    Archive & operator << ( Archive& archive, Quaternion const & q )
    {
        archive << q.x << q.y << q.z << q.w;
    }
    
    template<typename Archive>
    Archive & operator >> ( Archive& archive, Quaternion & q )
    {
        archive >> q.x >> q.y >> q.z >> q.w;
    }


}

#endif // Quaternion_hpp
