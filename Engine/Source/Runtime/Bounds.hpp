#ifndef Bounds_hpp
#define Bounds_hpp

#include "FishEngine.hpp"
#include "Vector3.hpp"

namespace FishEngine
{
    class FE_EXPORT Bounds
    {
	public:
        Bounds() : m_center(0, 0, 0), m_extents(Mathf::NegativeInfinity, Mathf::NegativeInfinity, Mathf::NegativeInfinity) {}
        
        Bounds(const Vector3& center, const Vector3& size) : m_center(center), m_extents(size*0.5f)
        {
        }
        
        // The center of the bounding box.
        Vector3 center() const { return m_center; }
        void setCenter(const Vector3& center) { m_center = center; }

        // The total size of the box. This is always twice as large as the extents.
        Vector3 size() const { return m_extents * 2.f; }
        void setSize(const Vector3& size) { m_extents = size * 0.5f; }

        // The extents of the box. This is always half of the size.
        Vector3 extents() const { return m_extents; }
        void setExtents(const Vector3& extents) { m_extents = extents; }

        // The minimal point of the box. This is always equal to center-extents.
        Vector3 min() const { return m_center - m_extents; }
        void setMin(const Vector3& min) { SetMinMax(min, max()); }

        // The maximal point of the box. This is always equal to center+extents.
        Vector3 max() const { return m_center + m_extents; }
        void setMax(const Vector3& max) { SetMinMax(min(), max); }

        void SetMinMax(const Vector3& min, const Vector3& max) {
            m_extents = (max - min) * 0.5f;
            m_center = min + m_extents;
        } 


        // Grows the Bounds to include the point.
        void Encapsulate(const Vector3& point)
        {
            SetMinMax(Vector3::Min(min(), point), Vector3::Max(max(), point));
        }

        // Grow the bounds to encapsulate the bounds.
        void Encapsulate(const Bounds& bounds)
        {
            Encapsulate(bounds.m_center - bounds.m_extents);
            Encapsulate(bounds.m_center + bounds.m_extents);
        }

        // Expand the bounds by increasing its size by amount along each side.
        void Expand(float amount)
        {
            amount *= 0.5f;
            m_extents += Vector3(amount, amount, amount);
        }

        // Expand the bounds by increasing its size by amount along each side.
        void Expand(const Vector3& amount)
        {
            m_extents += amount * 0.5f;
        }

        // Does another bounding box intersect with this bounding box ?
        bool Intersects(const Bounds& bounds)
        {
            auto pmin = min();
            auto pmax = max();
            auto bpmin = bounds.min();
            auto bpmax = bounds.max();
            return pmin.x <= bpmax.x && pmax.x >= bpmin.x && pmin.y <= bpmax.y && pmax.y >= bpmin.y && pmin.z <= bpmax.z && pmax.z >= bpmin.z;
        }
        
//        bool Overlaps(const Bounds& b) const
//        {
//            auto pMin = min();
//            auto pMax = max();
//            auto bpMin = b.min();
//            auto bpMax = b.max();
//            bool x = (pMax.x >= bpMin.x) && (pMin.x <= bpMax.x);
//            bool y = (pMax.y >= bpMin.y) && (pMin.y <= bpMax.y);
//            bool z = (pMax.z >= bpMin.z) && (pMin.z <= bpMax.z);
//            return (x && y && z);
//        }

        // Is point contained in the bounding box?
        bool Contains(const Vector3& point)
        {
            auto pmin = min();
            auto pmax = max();
            return (point.x > pmin.x) && (point.y < pmax.x) && (point.y > pmin.y) && (point.y < pmax.y) && (point.z > pmin.z) && (point.z < pmax.z);
        }

        // The smallest squared distance between the point and this bounding box.
        float SqrDistance(const Vector3& point);

        // Does ray intersect this bounding box?
        bool IntersectRay(const Ray& ray, float* outDistance = nullptr);


        // The closest point on the bounding box.
        // param point Arbitrary point.
        // return The point on the bounding box or inside the bounding box.
        Vector3 ClosestPoint(const Vector3& point);
        
        bool IsEmpty() const
        {
            return m_extents.x < 0 || m_extents.y < 0 || m_extents.z < 0;
        }

    private:
		friend class Serialization;
        Vector3 m_center;
        Vector3 m_extents;
    };
}

#endif // Bounds_hpp
