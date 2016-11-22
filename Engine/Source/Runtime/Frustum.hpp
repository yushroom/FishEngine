#ifndef Frustum_hpp
#define Frustum_hpp

#include "Mathf.hpp"
#include "Vector3.hpp"

namespace FishEngine
{
    struct FE_EXPORT Frustum
    {
        // center is (0, 0, 0)
        //Vector3 center;
        float   fov;
        float   maxRange;
        float   minRange;
        float   aspect;
        
        void getLocalCorners(Vector3 v[8]) const
        {
            float tan2 = Mathf::Tan(fov * 0.5f);
            float h1 = tan2 * minRange;
            float w1 = h1 * aspect;
            float h2 = tan2 * maxRange;
            float w2 = h2 * aspect;
            
            //Vector3 v[8];
            v[0].Set(-w1, h1, minRange);
            v[1].Set(w1, h1, minRange);
            v[2].Set(w1, -h1, minRange);
            v[3].Set(-w1, -h1, minRange);
            v[4].Set(-w2, h2, maxRange);
            v[5].Set(w2, h2, maxRange);
            v[6].Set(w2, -h2, maxRange);
            v[7].Set(-w2, -h2, maxRange);
        }
    };
}

#endif // Frustum_hpp
