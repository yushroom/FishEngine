#include <FishEngine/Bounds.hpp>
#include <FishEngine/Ray.hpp>

using namespace FishEngine;

bool FishEngine::Bounds::IntersectRay(const Ray& ray, float* outDistance /*= nullptr*/)
{
	if (!IsValid())
		return false;
	// PBRT 4.2.1
	auto pmin = this->min();
	auto pmax = this->max();
	auto inv_d = 1.0f / ray.direction;
	float tmin = Mathf::NegativeInfinity;
	float tmax = Mathf::Infinity;
	for (int i = 0; i < 3; ++i) {
		float t1 = (pmin[i] - ray.origin[i]) * inv_d[i];
		float t2 = (pmax[i] - ray.origin[i]) * inv_d[i];
		if (t1 > t2)
			std::swap(t1, t2);
		tmin = std::max(tmin, t1);
		tmax = std::min(tmax, t2);
		if (tmin > tmax) {
			return false;
		}
	}
	if (outDistance != nullptr) {
		*outDistance = tmin;
	}
	return true;
}
