#pragma once

//#include "Keyframe.hpp"
#include "WrapMode.hpp"
#include <vector>
#include "Vector3.hpp"
#include "Quaternion.hpp"
#include <cassert>

namespace FishEngine
{

#if 0
	class AnimationCurve
	{
	public:
		AnimationCurve()
		{
			Init({});
		}

		AnimationCurve(std::vector<Keyframe> const & keys)
		{
			Init(keys);
		}

		~AnimationCurve()
		{
			Cleanup();
		}

		std::vector<Keyframe> & keys()
		{
			return m_keys;
		}

		void setKeys(std::vector<Keyframe> const & keys)
		{
			m_keys = keys;
		}

		Keyframe operator[](int index)
		{
			return m_keys[index];
		}

		int length() const
		{
			return m_length;
		}

		float Evaluate(float time);
		float AddKey(float time, float value);
		int AddKey(Keyframe const & key);
		int MoveKey(int index, Keyframe const & key);
		void RemoveKey(int index);
		void SetKeys(std::vector<Keyframe> const & keys);
		std::vector<Keyframe> & GetKeys() { return m_keys; }

		void SmoothTangents(int index, float weight);

		static AnimationCurve Linear(float timeStart, float valueStart, float timeEnd, float valueEnd)
		{
			float num = (valueEnd - valueStart) / (timeEnd - timeStart);
			Keyframe[] keys = new Keyframe[]
			{
				new Keyframe(timeStart, valueStart, 0f, num),
				new Keyframe(timeEnd, valueEnd, num, 0f)
			};
			return new AnimationCurve(keys);
		}

		static AnimationCurve EaseInOut(float timeStart, float valueStart, float timeEnd, float valueEnd)
		{
			Keyframe[] keys = new Keyframe[]
			{
				new Keyframe(timeStart, valueStart, 0f, 0f),
				new Keyframe(timeEnd, valueEnd, 0f, 0f)
			};
			return new AnimationCurve(keys);
		}


	private:
		void Cleanup();
		void Init(std::vector<Keyframe> const & keys);

	private:
		std::vector<Keyframe> m_keys;
		int m_length;
		WrapMode m_preWrapMode;
		WrapMode m_postWrapMode;
	};
#endif

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