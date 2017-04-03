#pragma once

#include "TAnimationCurve.hpp"
#include "ReflectClass.hpp"
#include "../Vector3.hpp"
#include "../Quaternion.hpp"
#include "BsCorePrerequisites.h"

namespace FishEngine
{
	class FE_EXPORT AnimationUtility
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
		static void wrapTime(float& time, float start, float end, bool loop);

		/** Converts a curve in euler angles (in degrees) into a curve using quaternions. */
		static TAnimationCurve<Quaternion> eulerToQuaternionCurve(const TAnimationCurve<Vector3>& eulerCurve);
		
		/** Scales all curve values and tangents by the specified scale factor. */
		template<class T>
		static TAnimationCurve<T> scaleCurve(const TAnimationCurve<T>& curve, float factor);

		/** Adds a time offset to all keyframes in the provided curve. */
		template<class T>
		static TAnimationCurve<T> offsetCurve(const TAnimationCurve<T>& curve, float offset);

	};
}