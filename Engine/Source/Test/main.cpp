#include <Texture.hpp>
#include <Mathf.hpp>
#include <Vector2.hpp>
#include <Vector3.hpp>
#include <Vector4.hpp>
#include <Color.hpp>
#include <Debug.hpp>

#include <boost/filesystem.hpp>
#include <gli/gli.hpp>

using namespace FishEngine;
using namespace std;

typedef FishEngine::Vector2 float2;
typedef FishEngine::Vector3 float3;
typedef FishEngine::Vector4 float4;

void ToAddress(CubemapFace& face, uint32_t& x, uint32_t& y, float3 const & dir, uint32_t size)
{
    float3 n_dir = dir.normalized();
    float3 abs_dir;
    for (int i = 0; i < 3; ++i)
        abs_dir[i] = Mathf::Abs(n_dir[i]);

    if (abs_dir.x > abs_dir.y)
    {
        if (abs_dir.x > abs_dir.z)
        {
            face = n_dir.x > 0 ? CubemapFace::PositiveX : CubemapFace::NegativeX;
        }
        else
        {
            face = n_dir.z > 0 ? CubemapFace::PositiveZ : CubemapFace::NegativeZ;
        }
    }
    else
    {
        if (abs_dir.y > abs_dir.z)
        {
            face = n_dir.y > 0 ? CubemapFace::PositiveY : CubemapFace::NegativeY;
        }
        else
        {
            face = n_dir.z > 0 ? CubemapFace::PositiveZ : CubemapFace::NegativeZ;
        }
    }

    switch (face)
    {
    case CubemapFace::PositiveX:
        n_dir /= abs_dir.x;
        x = Mathf::Clamp<uint32_t>(static_cast<uint32_t>((-n_dir.z * 0.5f + 0.5f) * size), 0U, size - 1);
        y = Mathf::Clamp<uint32_t>(static_cast<uint32_t>((-n_dir.y * 0.5f + 0.5f) * size), 0U, size - 1);
        break;

    case CubemapFace::NegativeX:
        n_dir /= abs_dir.x;
        x = Mathf::Clamp<uint32_t>(static_cast<uint32_t>((+n_dir.z * 0.5f + 0.5f) * size), 0U, size - 1);
        y = Mathf::Clamp<uint32_t>(static_cast<uint32_t>((-n_dir.y * 0.5f + 0.5f) * size), 0U, size - 1);
        break;

    case CubemapFace::PositiveY:
        n_dir /= abs_dir.y;
        x = Mathf::Clamp<uint32_t>(static_cast<uint32_t>((+n_dir.x * 0.5f + 0.5f) * size), 0U, size - 1);
        y = Mathf::Clamp<uint32_t>(static_cast<uint32_t>((+n_dir.z * 0.5f + 0.5f) * size), 0U, size - 1);
        break;

    case CubemapFace::NegativeY:
        n_dir /= abs_dir.y;
        x = Mathf::Clamp<uint32_t>(static_cast<uint32_t>((+n_dir.x * 0.5f + 0.5f) * size), 0U, size - 1);
        y = Mathf::Clamp<uint32_t>(static_cast<uint32_t>((-n_dir.z * 0.5f + 0.5f) * size), 0U, size - 1);
        break;

    case CubemapFace::PositiveZ:
        n_dir /= abs_dir.z;
        x = Mathf::Clamp<uint32_t>(static_cast<uint32_t>((+n_dir.x * 0.5f + 0.5f) * size), 0U, size - 1);
        y = Mathf::Clamp<uint32_t>(static_cast<uint32_t>((-n_dir.y * 0.5f + 0.5f) * size), 0U, size - 1);
        break;

    case CubemapFace::NegativeZ:
    default:
        n_dir /= abs_dir.z;
        x = Mathf::Clamp<uint32_t>(static_cast<uint32_t>((-n_dir.x * 0.5f + 0.5f) * size), 0U, size - 1);
        y = Mathf::Clamp<uint32_t>(static_cast<uint32_t>((-n_dir.y * 0.5f + 0.5f) * size), 0U, size - 1);
        break;
    }

}

uint32_t ReverseBits(uint32_t bits)
{
    bits = (bits << 16) | (bits >> 16);
    bits = ((bits & 0x55555555) << 1) | ((bits & 0xAAAAAAAA) >> 1);
    bits = ((bits & 0x33333333) << 2) | ((bits & 0xCCCCCCCC) >> 2);
    bits = ((bits & 0x0F0F0F0F) << 4) | ((bits & 0xF0F0F0F0) >> 4);
    bits = ((bits & 0x00FF00FF) << 8) | ((bits & 0xFF00FF00) >> 8);
    return bits;
}

float RadicalInverseVdC(uint32_t bits)
{
    return ReverseBits(bits) * 2.3283064365386963e-10f; // / 0x100000000
}

float2 Hammersley2D(uint32_t i, uint32_t N)
{
    return float2(static_cast<float>(i) / N, RadicalInverseVdC(i));
}

float4 ImportanceSampleGGX(float2 E, float Roughness)
{
    float m = Roughness * Roughness;
    float m2 = m * m;

    float Phi = 2 * Mathf::PI * E.x;
    float CosTheta = sqrt((1 - E.y) / (1 + (m2 - 1) * E.y));
    float SinTheta = sqrt(1 - CosTheta * CosTheta);

    float3 H;
    H.x = SinTheta * cos(Phi);
    H.y = SinTheta * sin(Phi);
    H.z = CosTheta;

    float d = (CosTheta * m2 - CosTheta) * CosTheta + 1;
    float D = m2 / (Mathf::PI*d*d);
    float PDF = D * CosTheta;

    return float4(H, PDF);
}

// adapted from shader code in UE4
// also see PrefilterCube.cpp in KlayGE
// float3 PrefilterEnvMap( uint2 Random, float Roughness, float3 R )
float3 PrefilterEnvMapSpecular(float roughness, const float3& R, float3* env_map[6], uint32_t size)
{
    auto& N = R;  // normal
    auto& V = R;  // view

    float3 FilteredColor = float3(0);
    float Weight = 0;

    constexpr uint32_t NumSamples = 128;
    for (uint32_t i = 0; i < NumSamples; i++)
    {
        float2 E = Hammersley2D(i, NumSamples);
        float3 H = ImportanceSampleGGX(E, roughness);
        float3 L = 2 * float3::Dot(R, H) * H - R;

        float NoL = Mathf::Clamp01(float3::Dot(R, L));
        if (NoL > 0)
        {
            CubemapFace face;
            uint32_t x, y;
            ToAddress(face, x, y, L, size);
            FilteredColor += env_map[static_cast<uint32_t>(face)][y * size + x] * NoL;
            Weight += NoL;
        }
    }
    return FilteredColor / Mathf::Max(Weight, 0.001);
}

void PrefilterCube(const std::string & in_file, const std::string & out_file)
{
    gli::texture Texture = gli::load(in_file);
    if (Texture.empty())
    {
        Debug::LogError("Texture %s not found", in_file.c_str());
        abort();
    }
    gli::gl GL(gli::gl::PROFILE_GL33);
    gli::gl::format const Format = GL.translate(Texture.format(), Texture.swizzles());
    auto Target = GL.translate(Texture.target());
}

int main()
{
    const char* input = R"(D:\program\github\Cinder-Experiments\common\textures\BolongaRadiance2.dds)";
    
    boost::filesystem::path output_path(input);
    std::string output = output_path.stem().string() + "_filtered.dds";

    PrefilterCube(input, output);

    return 0;
} 