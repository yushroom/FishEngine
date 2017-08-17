#pragma once

//#include "Keyframe.hpp"
#include "WrapMode.hpp"
#include <vector>
#include "../Vector3.hpp"
#include "../Quaternion.hpp"
#include <cassert>

namespace FishEngine
{
	template <class T>
	struct TKeyframe
	{
		float time;
		T value;
		T inTangent;
		T outTangent;
	};


	template <class T>
	class FE_EXPORT TAnimationCurve
	{
	public:
		typedef TKeyframe<T> KeyframeType;

		TAnimationCurve();

		TAnimationCurve(const std::vector<KeyframeType>& keyframes);

		size_t keyframeCount() const { return m_keyframes.size(); }

		const KeyframeType & keyframeAt(uint32_t index) const { return m_keyframes[index]; }

		T Evaluate(float time, bool loop = true) const;
		
		/**
		* Returns a pair of keys that can be used for interpolating to field the value at the provided time.
		*
		* @param[in]	time			Time for which to find the relevant keys from. It is expected to be clamped to a
		*								valid range within the curve.
		* @param[out]	leftKey			Index of the key to interpolate from.
		* @param[out]	rightKey		Index of the key to interpolate to.
		*/
		void FindKeys(float time, uint32_t& leftKey, uint32_t& rightKey) const;

		/** Returns a key frame index nearest to the provided time. */
		uint32_t FindKey(float time);

		std::vector<KeyframeType> m_keyframes;
		float m_start;
		float m_end;
		float m_length;
	};

}
