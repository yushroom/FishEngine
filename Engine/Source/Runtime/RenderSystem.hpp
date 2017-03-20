#ifndef RenderSystem_hpp
#define RenderSystem_hpp

#include "RenderTexture.hpp"

namespace FishEngine
{
	class FE_EXPORT Meta(NonSerializable) RenderSystem
	{
	public:
		RenderSystem() = delete;

		static void InitializeGL();

		static void Init();

		static void Render();

		static void Clean();

		static void ResizeBufferSize(const int width, const int height);

		static ColorBufferPtr   m_GBuffer[3];
		static DepthBufferPtr   m_mainDepthBuffer;
		static RenderTargetPtr  m_deferredRenderTarget;

		static ColorBufferPtr   m_screenShadowMap;
		static RenderTargetPtr  m_screenShadowMapRenderTarget;

		static ColorBufferPtr   m_mainColorBuffer;
		static RenderTargetPtr  m_mainRenderTarget;

		static ColorBufferPtr   m_blurredScreenShadowMap;
		static RenderTargetPtr  m_blurScreenShadowMapRenderTarget1;
		static RenderTargetPtr  m_blurScreenShadowMapRenderTarget2;
	};
}

#endif // RenderSystem_hpp
