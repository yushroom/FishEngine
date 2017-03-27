#pragma once

#include "Texture2D.hpp"

namespace FishEngine
{
	class FE_EXPORT Meta(NonSerializable) ColorBuffer : public Texture2D
	{
	public:
		static std::shared_ptr<ColorBuffer> Create(const int width, const int height, TextureFormat format = TextureFormat::RGBA32);
		virtual void Resize(const int newWidth, const int newHeight) override;
	protected:
		//TextureFormat m_format;
	};
	
	//class LayeredColorBuffer : public ColorBuffer
	//{
	//public:
	//    static std::shared_ptr<LayeredColorBuffer> Create(const int width, const int height, const int layers, TextureFormat format = TextureFormat::RGBA32);
	//    //virtual void Resize(const int newWidth, const int newHeight) override;
	//protected:
	//    int m_layers;
	//};
	
	
	class FE_EXPORT Meta(NonSerializable) DepthBuffer : public Texture2D
	{
	public:
		static std::shared_ptr<DepthBuffer> Create(const int width, const int height);
		virtual void Resize(const int newWidth, const int newHeight) override;
		bool m_useStencil = true;
	};
	
	
	class FE_EXPORT LayeredDepthBuffer : public DepthBuffer
	{
	public:
		static std::shared_ptr<LayeredDepthBuffer> Create(const int width, const int height, const int depth, bool useStencil = true);
		virtual void Resize(const int newWidth, const int newHeight) override;
	protected:
		int m_depth;
	};
}
