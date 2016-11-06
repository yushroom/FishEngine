#ifndef Ray_hpp
#define Ray_hpp

#include "Vector3.hpp"

namespace FishEngine
{
    struct Ray
    {
        Vector3 origin;
        Vector3 direction;

        Ray(const Vector3& origin, const Vector3& direction) : origin(origin), direction(direction.normalized())
        {

        }

        Vector3 GetPoint(float distance)
        {
            return origin + direction * distance;
        }

        bool IntersectSphere(const Vector3& center, const float radius, float* t) const;
        
        bool IntersectPlane(const Vector3& normal, const Vector3& point, float* t) const;
    };
}

#endif // Ray_hpp
