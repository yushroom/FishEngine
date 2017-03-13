#include "RenderSettings.hpp"
#include "Material.hpp"
//#include "TextureImporter.hpp"
#include "Texture2D.hpp"

namespace FishEngine
{
	MaterialPtr RenderSettings::m_skybox;
	TexturePtr RenderSettings::m_ambientCubemap;

	uint32_t ReverseBits(uint32_t Bits)
	{
		Bits = (Bits << 16) | (Bits >> 16);
		Bits = ((Bits & 0x00ff00ff) << 8) | ((Bits & 0xff00ff00) >> 8);
		Bits = ((Bits & 0x0f0f0f0f) << 4) | ((Bits & 0xf0f0f0f0) >> 4);
		Bits = ((Bits & 0x33333333) << 2) | ((Bits & 0xcccccccc) >> 2);
		Bits = ((Bits & 0x55555555) << 1) | ((Bits & 0xaaaaaaaa) >> 1);
		return Bits;
	}

	TexturePtr MakePreintegratedGF()
	{
		auto texture_format = TextureFormat::RG32;
		constexpr uint32_t bytes_per_pixel = 4;    // 2 * 2 for RG32, and 4*2 for RGFloat
		constexpr uint32_t size = 128;

		auto byte_count = size * size * bytes_per_pixel;
		uint8_t DestBuffer[size * size * bytes_per_pixel];
		constexpr uint16_t DestStride = size * bytes_per_pixel;

		// x is NoV, y is roughness
		for (uint32_t y = 0; y < size; y++)
		{
			float Roughness = (float)(y + 0.5f) / size;
			float m = Roughness * Roughness;
			float m2 = m*m;

			for (uint32_t x = 0; x < size; x++)
			{
				float NoV = (float)(x + 0.5f) / size;

				Vector3 V;
				V.x = Mathf::Sqrt(1.0f - NoV * NoV);	// sin
				V.y = 0.0f;
				V.z = NoV;								// cos

				float A = 0.0f;
				float B = 0.0f;

				constexpr uint32_t NumSamples = 512;
				for (uint32_t i = 0; i < NumSamples; i++)
				{
					float E1 = (float)i / NumSamples;
					float E2 = static_cast<float>((double)ReverseBits(i) / (double)0x100000000LL);

					{
						float Phi = 2.0f * Mathf::PI * E1;
						float CosPhi = std::cosf(Phi);
						float SinPhi = std::sinf(Phi);
						float CosTheta = Mathf::Sqrt((1.0f - E2) / (1.0f + (m2 - 1.0f) * E2));
						float SinTheta = Mathf::Sqrt(1.0f - CosTheta * CosTheta);

						Vector3 H(SinTheta * CosPhi, SinTheta * SinPhi, CosTheta);
						float VoH = Vector3::Dot(V, H);
						Vector3 L = 2.0f * VoH * H - V;

						float NoL = Mathf::Max(L.z, 0.0f);
						float NoH = Mathf::Max(H.z, 0.0f);
						VoH = Mathf::Max(VoH, 0.0f);

						if (NoL > 0.0f)
						{
							float Vis_SmithV = NoL * (NoV * (1 - m) + m);
							float Vis_SmithL = NoV * (NoL * (1 - m) + m);
							float Vis = 0.5f / (Vis_SmithV + Vis_SmithL);

							float NoL_Vis_PDF = NoL * Vis * (4.0f * VoH / NoH);
							float Fc = 1.0f - VoH;
							Fc *= Mathf::Square(Fc*Fc);
							A += NoL_Vis_PDF * (1.0f - Fc);
							B += NoL_Vis_PDF * Fc;
						}
					}
				}
				A /= NumSamples;
				B /= NumSamples;

				if (texture_format == TextureFormat::RG32)
				{
					uint16_t* Dest = (uint16_t*)(DestBuffer + x * bytes_per_pixel + y * DestStride);
					Dest[0] = (uint32_t)(Mathf::Clamp01(A) * 65535.0f + 0.5f);
					Dest[1] = (uint32_t)(Mathf::Clamp01(B) * 65535.0f + 0.5f);
				}
				else if (texture_format == TextureFormat::RGFloat)
				{
					float* Dest = (float*)(DestBuffer + x * bytes_per_pixel + y * DestStride);
					Dest[0] = A;
					Dest[1] = B;
				}
			}
		}
#if 0
		TextureImporter importer;
		importer.setWrapMode(TextureWrapMode::Clamp);
		importer.setFilterMode(FilterMode::Bilinear);
		return importer.FromRawData((uint8_t*)DestBuffer, size, size, texture_format);
#else
		return std::make_shared<Texture2D>(size, size, texture_format, (uint8_t*)DestBuffer, byte_count);
#endif
	}

	void RenderSettings::setSkybox(MaterialPtr& skybox)
	{
		m_skybox = skybox;
		m_skybox->DisableKeyword(ShaderKeyword::All);
	}

	TexturePtr RenderSettings::preintegratedGF()
	{
		static auto PreintegratedGF = MakePreintegratedGF();
		return PreintegratedGF;
	}
}

