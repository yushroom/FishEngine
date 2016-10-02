#include "Quaternion.hpp"

namespace FishEngine {

    const Quaternion Quaternion::identity(0, 0, 0, 1);

    Vector3 Quaternion::eulerAngles() const
    {
        // http://www.euclideanspace.com/maths/geometry/rotations/conversions/quaternionToEuler/
        float yaw, pitch, roll;
        float sqx = x*x;
        float sqy = y*y;
        float sqz = z*z;
        float sqw = w*w;
        float unit = sqx + sqy + sqz + sqw;
        float test = w*x + y*z;
        if (test > 0.499f * unit) {
            yaw = 2 * atan2(z, w);
            pitch = Mathf::PI / 2.f;
            roll = 0.0f;
        }
        else {
            if (test < -0.499f * unit) {
                yaw = 02 * atan2(z, w);
                pitch = -Mathf::PI / 2;
                roll = 0.f;
            }
            else {
                yaw = atan2(2 * (y*w - x*z), -sqx - sqy + sqz + sqw);
                pitch = asin(2 * test / unit);
                roll = atan2(2 * (z*w - x*y), -sqx + sqy - sqz + sqw);
            }
        }
        return Vector3(pitch, yaw, roll) * Mathf::Rad2Deg;
        //return Vector3(pitch(), yaw(), roll());
    }

    FishEngine::Quaternion Quaternion::Euler(const Vector3& euler)
    {
        Quaternion result;
        auto rad = euler * (Mathf::Deg2Rad * 0.5f);
        auto c = Vector3(cos(rad.x), cos(rad.y), cos(rad.z));
        auto s = Vector3(sin(rad.x), sin(rad.y), sin(rad.z));
        result.w = c.x * c.y * c.z + s.x * s.y * s.z;
        result.x = s.x * c.y * c.z - c.x * s.y * s.z;
        result.y = c.x * s.y * c.z + s.x * c.y * s.z;
        result.z = c.x * c.y * s.z - s.x * s.y * c.z;
        return result;
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
}
