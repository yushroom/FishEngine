#include "Ray.hpp"
#include <cassert>

// solve x in: Ax^2 + Bx + C = 0, t0 < t1
// see PBRT 3.2.3
inline bool Quadraic(float A, float B, float C, float *t0, float *t1)
{
	float discrim = B*B - 4.f*A*C;
	if (discrim <= 0.f)
		return false;
	float root_discrim = std::sqrtf(discrim);
	float q;
	if (B < 0) q = -0.5f * (B - root_discrim);
	else       q = -0.5f * (B + root_discrim);
	assert(t0 != t1 && t0 != nullptr);
	*t0 = q / A;
	*t1 = C / q;
	if (*t0 > *t1) std::swap(*t0, *t1);
	return true;
}

namespace FishEngine
{

	bool Ray::IntersectSphere(const Vector3& center, const float radius, float* t) const
	{
		auto o = origin - center;
		float A = direction.sqrMagnitude();
		float B = 2.0f * Vector3::Dot(direction, o);
		float C = o.sqrMagnitude() - radius*radius;
		float t1;
		return Quadraic(A, B, C, t, &t1);
	}
	
	bool Ray::IntersectPlane(const Vector3& normal, const Vector3& point, float* t) const
	{
		// Dot((p-point), normal) = 0
		// p = origin + t*direction
		float test = Vector3::Dot(normal, direction);
		if (Mathf::Approximately(test, 0))
			return false;
		*t = Vector3::Dot(point, normal) - Vector3::Dot(normal, origin);
		*t /= test;
		return true;
	}
}

