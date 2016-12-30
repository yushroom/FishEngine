#ifndef Mathf_hpp
#define Mathf_hpp

#include "FishEngine.hpp"
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <limits>
#include <algorithm>
#include "ReflectClass.hpp"

namespace FishEngine {

class FE_EXPORT Meta(NonSerializable) Mathf
{

public:
    Mathf() = delete;

    /********** **********/

    static constexpr float PositiveInfinity = std::numeric_limits<float>::infinity();
    static constexpr float Infinity = PositiveInfinity;
    static constexpr float NegativeInfinity = -PositiveInfinity;    // TODO
    static constexpr float PI = static_cast<float>(3.1415926535897932384626);
    static constexpr float Epsilon = ::std::numeric_limits<float>::epsilon();   // TODO
    static constexpr float Deg2Rad = static_cast<float>(0.01745329251994329576923690768489);
    static constexpr float Rad2Deg = static_cast<float>(57.295779513082320876798154814105);

    /********** **********/

    static float Abs(float f) {
        return ::std::fabsf(f);
    }

    static int Abs(int value) {
        return ::std::abs(value);
    }

    static float Sign(float f)
    {
        return (f < 0.f) ? -1.f : 1.f;
    }

    static bool Approximately(float a, float b)
    {
        return Mathf::Abs(b - a) < Mathf::Max(1E-06f * Mathf::Max(Mathf::Abs(a), Mathf::Abs(b)), Mathf::Epsilon * 8.f);
    }

    static float Ceil(float f)
    {
        return ::std::ceil(f);
    }

    static int CeilToInt(float f)
    {
        return (int)::std::ceil(f);
    }

    static float Floor(float f)
    {
        return ::std::floor(f);
    }

    static int FloorToInt(float f)
    {
        return (int)::std::floor(f);
    }

    static float Round(float f)
    {
        return ::std::round(f);
    }

    static int RoundToInt(float f)
    {
        return (int)::std::round(f);
    }

    // ushort FloatToHalf(float val);
    // float HalfToFloat(ushort val);

    template<typename T>
    static T Clamp(T value, T min_value, T max_value)
    {
        if (value < min_value) value = min_value;
        else if (value > max_value) value = max_value;
        return value;
    }

    static float Clamp01(float value)
    {
        if (value < 0.f) {
            return 0.f;
        }
        else if (value > 1.f) {
            return 1.f;
        }
        return value;
    }

    static int ClosestPowerOfTwo(int value);

    inline bool IsPowerOfTwo(uint32_t value)
    {
        return (value & (value-1)) == 0;
    }

    static uint32_t NextPowerOfTwo(uint32_t v)
    {
        // http://graphics.stanford.edu/~seander/bithacks.html#RoundUpPowerOf2
        // PBRT P1031 RoundUpPow2
        v--;
        v |= v >> 1;
        v |= v >> 2;
        v |= v >> 4;
        v |= v >> 8;
        v |= v >> 16;
        return v+1;
    }
    
    inline static float Square(float f)
    {
        return f*f;
    }

    inline static float Sqrt(float f)
    {
        return ::std::sqrtf(f);
    }
    
    // return
    static float RcpSqrt(float f)
    {
        float const threehalfs = 1.5f;
        
        float x2 = f * 0.5f;
        union {
            float f;
            int32_t i;
        } fni;
        
        fni.f = f;											// evil floating point bit level hacking
        fni.i = 0x5f375a86 - (fni.i >> 1);						// what the fuck?
        fni.f = fni.f * (threehalfs - (x2 * fni.f * fni.f));	// 1st iteration
        fni.f = fni.f * (threehalfs - (x2 * fni.f * fni.f));		// 2nd iteration, this can be removed
        
        return fni.f;
    }

    static float Pow(float f, float p)
    {
        return ::std::pow(f, p);
    }

    static float Exp(float power)
    {
        return ::std::exp(power);
    }

    static float Log(float f)
    {
        return ::std::log(f);
    }

    static float Log(float f, float p)
    {
        return ::std::log(f) / ::std::log(p);
    }

    static float Log10(float f)
    {
        return ::std::log10(f);
    }

    static float Gamma(float value, float absmax, float gamma)
    {
        bool flag = false;
        if (value < 0.f)
        {
            flag = true;
        }
        float num = Mathf::Abs(value);
        if (num > absmax)
        {
            return (!flag) ? num : (-num);
        }
        float num2 = Mathf::Pow(num / absmax, gamma) * absmax;
        return (!flag) ? num2 : (-num2);
    }

    static float GammaToLinearSpace(float value);
    static float LinearToGammaSpace(float value);

    static float Repeat(float t, float length)
    {
        return t - ::std::floorf(t / length) * length;
    }

    static float Lerp(float a, float b, float t) {
        return a + (b - a)*Clamp01(t);
    }


    template<typename T>
    static T LerpUnclamped(const T& a, const T& b, const float t) {
        return a + (b - a) * t;
    }

    static float LerpUnclamped(float a, float b, float t)
    {
        return a + (b - a) * t;
    }

    inline float LerpAngle(float a, float b, float t)
    {
        float num = Mathf::Repeat(b - a, 360.f);
        if (num > 180.f)
        {
            num -= 360.f;
        }
        return a + num * Mathf::Clamp01(t);
    }

    static float InverseLerp(float a, float b, float value)
    {
        if (a != b)
        {
            return Mathf::Clamp01((value - a) / (b - a));
        }
        return 0.f;
    }

    static float PingPong(float t, float length)
    {
        t = Mathf::Repeat(t, length * 2.f);
        return length - Mathf::Abs(t - length);
    }

    /********** **********/
    // TODO: radians or degrees?
    static float Sin(float f)
    {
        return ::std::sin(f*Deg2Rad);
    }

    static float Cos(float f)
    {
        return ::std::cos(f*Deg2Rad);
    }

    static float Tan(float f)
    {
        return ::std::tan(f*Deg2Rad);
    }

    static float Acos(float f)
    {
        return ::std::acos(f*Deg2Rad) * Rad2Deg;
    }

    static float Asin(float f)
    {
        return ::std::asin(f)*Rad2Deg;
    }

    static float Atan(float f)
    {
        return ::std::atan(f)*Rad2Deg;
    }

    static float Atan2(float y, float x)
    {
        return ::std::atan2(y, x)*Rad2Deg;
    }

    static float DeltaAngle(float current, float target)
    {
        float num = Mathf::Repeat(target - current, 360.f);
        if (num > 180.f)
        {
            num -= 360.f;
        }
        return num;
    }

    /********** **********/

    static float MoveTowards(float current, float target, float maxDelta)
    {
        if (::fabsf(target - current) <= maxDelta)
        {
            return target;
        }
        return current + Mathf::Sign(target - current) * maxDelta;
    }

    static float MoveTowardsAngle(float current, float target, float maxDelta)
    {
        float num = Mathf::DeltaAngle(current, target);
        if (-maxDelta < num && num < maxDelta)
        {
            return target;
        }
        target = current + num;
        return Mathf::MoveTowards(current, target, maxDelta);
    }


    static float Degrees(float radians)
    {
        return radians * Rad2Deg;
    }

    static float Radians(float degrees)
    {
        return degrees * Deg2Rad;
    }

    static float Max(float a, float b) {
        return ::std::max(a, b);
    }

    static float Min(float a, float b) {
        return ::std::min(a, b);
    }

    /********** misc **********/
    static float PerlinNoise(float x, float y);

}; // class Mathf


template float Mathf::Clamp<float>(float value, float min_value, float max_value);
template uint32_t Mathf::Clamp<uint32_t>(uint32_t value, uint32_t min_value, uint32_t max_value);

}

#endif // Mathf_hpp
