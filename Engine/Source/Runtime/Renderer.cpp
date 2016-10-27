#include "Renderer.hpp"
#include "GameObject.hpp"
#include "MeshFilter.hpp"
#include "Mesh.hpp"
#include "Gizmos.hpp"
#include "Transform.hpp"

namespace FishEngine
{

    Bounds Renderer::bounds() const
    {
//        auto l2w = transform()->localToWorldMatrix();
//        auto lb = localBounds();
//        //Vector3 corners[8];
//        auto center = lb.center();
//        auto extents = lb.extents();
//        float weights[] = { 1, 1, 1,  1, 1, -1,  1, -1, 1,  -1, 1, 1,
//            1, -1, -1,  -1, 1, -1,  -1, -1, 1,  -1, -1, -1};
//        Bounds result;
//        for (int i = 0; i < 8; ++i)
//        {
//            Vector3 corner = center + extents * Vector3(weights+i*3);
//            corner = l2w.MultiplyPoint(corner);
//            result.Encapsulate(corner);
//        }
//        return result;
        return transform()->TransformBounds(localBounds());
    }

    void Renderer::OnDrawGizmosSelected()
    {
        Gizmos::setColor(Color::white);
        Gizmos::setMatrix(transform()->localToWorldMatrix());
        Bounds b = bounds();
        Gizmos::DrawWireCube(b.center(), b.size());
    }
}
