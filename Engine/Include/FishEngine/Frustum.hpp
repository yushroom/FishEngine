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
			float rad = Mathf::Radians(fov) * 0.5f;
			float tan2 = Mathf::Tan(rad);
			float h1 = tan2 * minRange;
			float w1 = h1 * aspect;
			float h2 = tan2 * maxRange;
			float w2 = h2 * aspect;
			
			//Vector3 v[8];
			v[0].Set(-w1, h1, minRange);    // near top left
			v[1].Set(w1, h1, minRange);     // near top right
			v[2].Set(w1, -h1, minRange);    // near bottom right
			v[3].Set(-w1, -h1, minRange);   // near bottom left
			v[4].Set(-w2, h2, maxRange);    // far top left
			v[5].Set(w2, h2, maxRange);     // far top right
			v[6].Set(w2, -h2, maxRange);    // far bottom right
			v[7].Set(-w2, -h2, maxRange);   // far bottom left
		}
	};
}

#endif // Frustum_hpp
