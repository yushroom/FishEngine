#ifndef Ray_hpp
#define Ray_hpp

#include "Vector3.hpp"

namespace FishEngine
{
    struct Ray
    {
        Vector3 direction;
        Vector3 origin;

        Ray(const Vector3& origin, const Vector3& direction) : origin(origin), direction(direction.normalized())
        {

        }

        Vector3 GetPoint(float distance)
        {
            return origin + direction * distance;
        }
    };
}

#endif // Ray_hpp