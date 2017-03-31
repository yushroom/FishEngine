#ifndef Ray_hpp
#define Ray_hpp

#include "Vector3.hpp"
//#include "Macro.hpp"

namespace FishEngine
{
	class FE_EXPORT Ray
	{
	public:
		//InjectSerializationFunctionsNonVirtual(Ray)
		
		// The origin point of the ray.
		Vector3 origin;

		// The direction of the ray.
		Vector3 direction;

		// Creates a ray starting at origin along direction.
		Ray(const Vector3& origin, const Vector3& direction)
			: origin(origin), direction(direction.normalized())
		{

		}

		// Returns a point at distance units along the ray.
		Vector3 GetPoint(float distance)
		{
			return origin + direction * distance;
		}

		bool IntersectSphere(const Vector3& center, const float radius, float* t) const;
		
		bool IntersectPlane(const Vector3& normal, const Vector3& point, float* t) const;
	};
}

#endif // Ray_hpp
