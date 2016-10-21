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
        auto mf = gameObject()->GetComponent<MeshFilter>();
        if (mf == nullptr)
            return Bounds();
        return mf->mesh()->bounds();
        //return transform()->localToWorldMatrix()
    }

    void Renderer::OnDrawGizmosSelected()
    {
        Gizmos::setColor(Color::white);
        Gizmos::setMatrix(transform()->localToWorldMatrix());
        Bounds b = bounds();
        Gizmos::DrawWireCube(b.center(), b.size());
    }
}
