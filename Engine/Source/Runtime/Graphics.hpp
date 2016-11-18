#pragma once

#include "FishEngine.hpp"

namespace FishEngine
{
    //class RenderBuffer;

    class Graphics
    {
    public:
        Graphics() = delete;

        static void Blit(const TexturePtr& source, const RenderTexturePtr& dest);

        static void DrawMesh(const MeshPtr& mesh, const Matrix4x4& matrix, const MaterialPtr& material);
        static void DrawMesh(const MeshPtr& mesh, const MaterialPtr& material);
        static void DrawTexture();

        static void SetRenderTarget(RenderTexturePtr rt);

        //static RenderBuffer activeColorBuffer;
        //static RenderBuffer activeDepthBuffer;
    };
}