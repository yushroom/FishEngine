#ifndef Mathf_hpp
#define Mathf_hpp

#include "FishEngine.hpp"
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <limits>
#include <algorithm>
#include "ReflectClass.hpp"

namespace FishEngine
{
	struct FE_EXPORT Meta(NonSerializable) Mathf
	{
		Mathf() = delete;

	public:

		// The infamous 3.14159265358979... value (Read Only).
		static constexpr float PI = static_cast<float>(3.1415926535897932384626);

		// A representation of positive infinity(Read Only).
		static constexpr float Infinity = std::numeric_limits<float>::infinity();

		// A representation of negative infinity (Read Only).
		static constexpr float NegativeInfinity = -Infinity;    // TODO

		// Degrees-to-radians conversion constant (Read Only).
		static constexpr float Deg2Rad = static_cast<float>(0.01745329251994329576923690768489);

		// Radians-to-degrees conversion constant (Read Only).
		static constexpr float Rad2Deg = static_cast<float>(57.295779513082320876798154814105);
		
		// A tiny floating point value (Read Only).
		static constexpr float Epsilon = ::std::numeric_limits<float>::epsilon();   // TODO
		
	public:

		// Returns the closest power of two value.
		static int ClosestPowerOfTwo(int value);

		// Converts the given value from gamma (sRGB) to linear color space.
		static float GammaToLinearSpace(float value);

		// Converts the given value from linear to gamma (sRGB) color space.
		static float LinearToGammaSpace(float value);

		// Returns true if the value is power of two.
		inline static bool IsPowerOfTwo(uint32_t value)
		{
			return (value & (value - 1)) == 0;
		}

		// Returns the next power of two value.
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
			return v + 1;
		}

		/**
		* Returns the sine of angle f in radians.
		*
		* @param float f The argument as radian
		* @return float The return value between -1 and +1.
		*/
		static float Sin(float f)
		{
			return (float)::std::sin((double)f);
		}

		// Returns the cosine of angle f in radians.
		static float Cos(float f)
		{
			return (float)::std::cos((double)f);
		}


		// Returns the tangent of angle f in radians.
		static float Tan(float f)
		{
			return (float)::std::tan((double)f);
		}

		// Returns the arc-sine of f - the angle in radians whose sine is f.
		static float Asin(float f)
		{
			return (float)::std::asin((double)f);
		}

		// Returns the arc-cosine of f - the angle in radians whose cosine is f.
		static float Acos(float f)
		{
			return (float)::std::acos((double)f);
		}

		// Returns the arc-tangent of f - the angle in radians whose tangent is f.
		static float Atan(float f)
		{
			return (float)::std::atan((double)f);
		}

		// Returns the angle in radians whose Tan is y/x.
		static float Atan2(float y, float x)
		{
			return (float)::std::atan2((double)y, (double)x);
		}

		// Returns square root of f.
		inline static float Sqrt(float f)
		{
			return (float)::std::sqrt((double)f);
		}

		// return 1.0f / sqrt(f)
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

		// Returns the absolute value of f.
		static float Abs(float f)
		{
			return ::std::fabsf(f);
		}

		// Returns the absolute value of value.
		static int Abs(int value)
		{
			return ::std::abs(value);
		}

		// Returns the smallest of two or more values.
		static float Min(float a, float b)
		{
			return ::std::min(a, b);
		}

		static int Min(int a, int b)
		{
			return ::std::min(a, b);
		}

		static float Max(float a, float b)
		{
			return ::std::max(a, b);
		}

		static int Max(int a, int b)
		{
			return ::std::max(a, b);
		}

		inline static float Square(float f)
		{
			return f*f;
		}

		// Returns f raised to power p.
		static float Pow(float f, float p)
		{
			return ::std::pow(f, p);
		}

		// Returns e raised to the specified power.
		static float Exp(float power)
		{
			return ::std::exp(power);
		}

		// Returns the logarithm of a specified number in a specified base.
		static float Log(float f, float p)
		{
			return ::std::log(f) / ::std::log(p);
		}
		
		// Returns the natural (base e) logarithm of a specified number.
		static float Log(float f)
		{
			return ::std::log(f);
		}
		
		// Returns the base 10 logarithm of a specified number.
		static float Log10(float f)
		{
			return ::std::log10(f);
		}

		// Returns the smallest integer greater to or equal to f.
		static float Ceil(float f)
		{
			return ::std::ceil(f);
		}

		// Returns the largest integer smaller to or equal to f.
		static float Floor(float f)
		{
			return ::std::floor(f);
		}

		// Returns f rounded to the nearest integer.
		static float Round(float f)
		{
			return ::std::round(f);
		}

		// Returns the smallest integer greater to or equal to f.
		static int CeilToInt(float f)
		{
			return (int)::std::ceil(f);
		}

		// Returns the largest integer smaller to or equal to f.
		static int FloorToInt(float f)
		{
			return (int)::std::floor(f);
		}
		
		// Returns f rounded to the nearest integer.
		static int RoundToInt(float f)
		{
			return (int)::std::round(f);
		}


		// Returns the sign of f.
		static float Sign(float f)
		{
			return (f < 0.f) ? -1.f : 1.f;
		}


		// Clamps a value between a minimum float and maximum float value.
		template<typename T>
		static T Clamp(T value, T min_value, T max_value)
		{
			if (value < min_value) value = min_value;
			else if (value > max_value) value = max_value;
			return value;
		}

		// Clamps value between 0 and 1 and returns value.
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

		/**
		 * Linearly interpolates between a and b by t.
		 *
		 * @param float a The start value
		 * @param float b The end value
		 * @param float t The interpolation value between the two floats
		 * @return float The interpolated float result between the two float values.
		 */
		static float Lerp(float a, float b, float t)
		{
			return a + (b - a) * Clamp01(t);
		}

		 /**
		 * Linearly interpolates between a and b by t with no limit to t.
		 *
		 * @param float a The start value
		 * @param float b The end value
		 * @param float t The interpolation value between the two floats
		 * @return float The float value as a result from the linear interpolation.
		 */
		template<typename T>
		static T LerpUnclamped(const T& a, const T& b, float t)
		{
			return a + (b - a) * t;
		}

		 /**
		 * Linearly interpolates between a and b by t with no limit to t.
		 *
		 * @param float a The start value
		 * @param float b The end value
		 * @param float t The interpolation value between the two floats
		 * @return float The float value as a result from the linear interpolation.
		 */
		static float LerpUnclamped(float a, float b, float t)
		{
			return a + (b - a) * t;
		}

		// Same as Lerp but makes sure the values interpolate correctly when they wrap around 360 degrees.
		inline float LerpAngle(float a, float b, float t)
		{
			float num = Mathf::Repeat(b - a, 360.f);
			if (num > 180.f)
			{
				num -= 360.f;
			}
			return a + num * Mathf::Clamp01(t);
		}

		// Calculates the linear parameter t that produces the interpolant value within the range [a, b].
		static float InverseLerp(float a, float b, float value)
		{
			if (a != b)
			{
				return Mathf::Clamp01((value - a) / (b - a));
			}
			return 0.f;
		}

		/**
		 * Moves a value current towards target.
		 *
		 * @param float current The current value.
		 * @param float target The value to move towards.
		 * @param float maxDelta The maximum change that should be applied to the value.
		 * @return float
		 */
		static float MoveTowards(float current, float target, float maxDelta)
		{
			if (Mathf::Abs(target - current) <= maxDelta)
			{
				return target;
			}
			return current + Mathf::Sign(target - current) * maxDelta;
		}

		// >Same as MoveTowards but makes sure the values interpolate correctly when they wrap around 360 degrees.
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

		// Interpolates between min and max with smoothing at the limits.
		static float SmoothStep(float from, float to, float t)
		{
			t = Mathf::Clamp01(t);
			t = -2.0f * t * t * t + 3.0f * t * t;
			return to * t + from * (1.0f - t);
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

		// Compares two floating point values if they are similar.
		static bool Approximately(float a, float b)
		{
			return Mathf::Abs(b - a) < Mathf::Max(1E-06f * Mathf::Max(Mathf::Abs(a), Mathf::Abs(b)), Mathf::Epsilon * 8.f);
		}


		// Returns true if the distance between f0 and f1 is smaller than epsilon
		static bool CompareApproximately(float f0, float f1, float epsilon = 0.000001F)
		{
			float dist = (f0 - f1);
			dist = fabsf(dist);
			return dist < epsilon;
		}

		/**
		 * Gradually changes a value towards a desired goal over time.
		 *
		 * The value is smoothed by some spring-damper like function, which will never overshoot. The function can be used to smooth any kind of value, positions, colors, scalars.
		 *
		 * @param float current The current position.
		 * @param float target The position we are trying to reach.
		 * @param float &  currentVelocity The current velocity, this value is modified by the function every time you call it.
		 * @param float smoothTime Approximately the time it will take to reach the target. A smaller value will reach the target faster.
		 * @param float maxSpeed Optionally allows you to clamp the maximum speed.
		 * @param float deltaTime The time since the last call to this function. By default Time.deltaTime.
		 * @return float
		 */
		static float SmoothDamp(float current, float target, float & /*ref*/ currentVelocity, float smoothTime, float maxSpeed, float deltaTime)
		{
			smoothTime = Mathf::Max(0.0001f, smoothTime);
			float num = 2.f / smoothTime;
			float num2 = num * deltaTime;
			float num3 = 1.f / (1.f + num2 + 0.48f * num2 * num2 + 0.235f * num2 * num2 * num2);
			float num4 = current - target;
			float num5 = target;
			float num6 = maxSpeed * smoothTime;
			num4 = Mathf::Clamp(num4, -num6, num6);
			target = current - num4;
			float num7 = (currentVelocity + num * num4) * deltaTime;
			currentVelocity = (currentVelocity - num * num7) * num3;
			float num8 = target + (num4 + num7) * num3;
			if (num5 - current > 0.f == num8 > num5)
			{
				num8 = num5;
				currentVelocity = (num8 - num5) / deltaTime;
			}
			return num8;
		}

		// Compares two floating point values if they are similar.
		static float SmoothDamp(float current, float target, float & /*ref*/ currentVelocity, float smoothTime, float maxSpeed = Mathf::Infinity);

		/**
		 * Gradually changes an angle given in degrees towards a desired goal angle over time.
		 *
		 *The value is smoothed by some spring-damper like function. The function can be used to smooth any kind of value, positions, colors, scalars. The most common use is for smoothing a follow camera.
		 *
		 * @param float current The current position.
		 * @param float target The position we are trying to reach.
		 * @param float &  currentVelocity The current velocity, this value is modified by the function every time you call it.
		 * @param float smoothTime Approximately the time it will take to reach the target. A smaller value will reach the target faster.
		 * @param float maxSpeed Optionally allows you to clamp the maximum speed.
		 * @param float deltaTime The time since the last call to this function. By default Time.deltaTime.
		 * @return float
		 */
		 static float SmoothDampAngle(float current, float target, float& currentVelocity, float smoothTime, float maxSpeed, float deltaTime)
		{
			target = current + Mathf::DeltaAngle(current, target);
			return Mathf::SmoothDamp(current, target, currentVelocity, smoothTime, maxSpeed, deltaTime);
		}

		static float SmoothDampAngle(float current, float target, float& currentVelocity, float smoothTime, float maxSpeed = Mathf::Infinity);


		// Loops the value t, so that it is never larger than length and never smaller than 0.
		static float Repeat(float t, float length)
		{
			return t - Mathf::Floor(t / length) * length;
		}

		static float PingPong(float t, float length)
		{
			t = Mathf::Repeat(t, length * 2.f);
			return length - Mathf::Abs(t - length);
		}

		// ushort FloatToHalf(float val);
		// float HalfToFloat(ushort val);

		// Calculates the shortest difference between two given angles given in degrees.
		static float DeltaAngle(float current, float target)
		{
			float num = Mathf::Repeat(target - current, 360.f);
			if (num > 180.f)
			{
				num -= 360.f;
			}
			return num;
		}


		// convert angle in radians to angle in degrees
		static float Degrees(float radians)
		{
			return radians * Rad2Deg;
		}

		// convert angle in degree to angle in radians
		static float Radians(float degrees)
		{
			return degrees * Deg2Rad;
		}

		/********** misc **********/
		static float PerlinNoise(float x, float y);

	}; // class Mathf


	template float Mathf::Clamp<float>(float value, float min_value, float max_value);
	template uint32_t Mathf::Clamp<uint32_t>(uint32_t value, uint32_t min_value, uint32_t max_value);

}

#endif // Mathf_hpp
