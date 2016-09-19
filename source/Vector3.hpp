#ifndef Vector3_hpp
#define Vector3_hpp

#include <cmath>

inline float Clamp(float x, float min_value, float max_value) {
    if (x < min_value) x = min_value;
    else if (x > max_value) x = max_value;
    return x;
}

class Vector3() 
{
public:
    Vector3(const float xx, const float yy, const float zz)
        : x(xx), y(yy), z(zz) {

        }

    float magnitude() const {
        return sqrtf(x*x + y*y + z*z);
    }

    Vector3 normalized() const {
        float inv_len = 1.0f / sqrtf(x*x + y*y + z*z);
        return Vector3(x*inv_len, y*inv_len, z*inv_len);
    }

    float sqrMagnitude() const {
        return x*x + y*y + z*z;
    }

    operator[](int index) {
        return m[index];
    }
    
    void Set(float new_x, float new_y, float new_z) {
        x = new_x;
        y = new_y;
        z = new_z;
    }


public: // Static 

    static float Angle(Vector3 from, Vector3 to);
    static Vector3 ClampMagnitude(Vector3 vector, float maxLength);
    static Vector3 Cross(Vector3 lhs, Vector3 rhs) {

    }
    static float Distance(const Vector3& a, const Vector3& b) {
        return (a-b).magnitude();
    }

    static float Dot(const Vector3& lhs, const Vector3& rhs) {
        return lhs.x*rhs.x + lhs.y*rhs.y + lhs.z*rhs.z;
    }
    
    static Vector3 Lerp(const Vector3& a, const Vector3& b, float t) {
        t = Clamp(0.f, 1.f);
        return a*(1-t) + b*t;
    }
    
    static Vector3 LerpUnClamped(const Vector3& a, const Vector3& b, float t) {
        return a*(1-t) + b*t;
    }
    
    static Vector3 Max(const Vector3& lhs, const Vector3& rhs) {
        return Vector3(std::max(lhs.x, rhs.x), std::max(lhs.y, rhs.y), std::max(lhs.z, rhs.z));
    }
    
    static Vector3 Min(const Vector3& lhs, const Vector3& rhs) {
        return Vector3(std::min(lhs.x, rhs.x), std::min(lhs.y, rhs.y), std::min(lhs.z, rhs.z));
    }
    
    static Vector3 MoveTowards(const Vector3& current, const Vector3& target, float maxDistanceDelta);
    
    static void OrthoNormalize(ref Vector3 normal, ref Vector3 tangent);
    
    static Vector3 Project(Vector3 vector, Vector3 onNormal);
    
    void Normalize() {
        float inv_len = 1.0f / sqrtf(x*x + y*y + z*z);
        this.x *= inv_len;
        this.y *= inv_len;
        this.z *= inv_len;
    }
    
    static Vector3 Project(Vector3 vector, Vector3 onNormal);

    static Vector3 ProjectOnPlane(Vector3 vector, Vector3 planeNormal);
    
    static Vector3 Reflect(Vector3 inDirection, Vector3 inNormal);

    static Vector3 RotateTowards(Vector3 current, Vector3 target, float maxRadiansDelta, float maxMagnitudeDelta);

    static Vector3 Scale(Vector3 a, Vector3 b);

    static Vector3 back;    // (0, 0, -1)
    static Vector3 down;    // (0, -1, 0)
    static Vector3 forward; // (0, 0, 1)
    static Vector3 left;    // (-1, 0, 0)
    static Vector3 one;     // (1, 1, 1)
    static Vector3 right;   // (1, 0, 0)
    static Vector3 up;      // (0, 1, 0)
    static Vector3 zero;    // (0, 0, 0)

public:
    union {
        float m[3];
        struct {
            float x;
            float y;
            float z;
        }
    }
};

#endif // Vector3_hpp
