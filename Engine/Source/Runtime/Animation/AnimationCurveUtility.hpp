#pragma once
#include "AnimationCurve.hpp"

namespace FishEngine
{
	class Meta(NonSerializable) AnimationCurveUtility
	{
	public:

		/**
		* Wraps or clamps the provided time value between the provided range.
		*
		* @param[in,out]	time	Time value to wrap/clamp.
		* @param[in]		start	Start of the range.
		* @param[in]		end		End of the range.
		* @param[in]		loop	If true the value will be wrapped, otherwise clamped to range.
		*/
		FE_EXPORT static void WrapTime(float& time, float start, float end, bool loop);

		/** Converts a curve in euler angles (in degrees) into a curve using quaternions. */
		FE_EXPORT static TAnimationCurve<Quaternion> EulerToQuaternionCurve(const TAnimationCurve<Vector3>& eulerCurve, RotationOrder rotationOrder);


		/** Scales all curve values and tangents by the specified scale factor. */
		template<class T>
		static TAnimationCurve<T> ScaleCurve(const TAnimationCurve<T>& curve, float factor);

		/** Adds a time offset to all keyframes in the provided curve. */
		template<class T>
		static TAnimationCurve<T> OffsetCurve(const TAnimationCurve<T>& curve, float offset);

	};
}