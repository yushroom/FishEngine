#include <FishEngine/Animation/AnimationCurve.hpp>
#include <FishEngine/Animation/AnimationCurveUtility.hpp>

using namespace FishEngine;

template <class T>
TAnimationCurve<T>::TAnimationCurve()
	:m_start(0.0f), m_end(0.0f), m_length(0.0f)
{

}

template <class T>
TAnimationCurve<T>::TAnimationCurve(const std::vector<KeyframeType>& keyframes)
	:m_keyframes(keyframes)
{
#if BS_DEBUG_MODE
	// Ensure keyframes are sorted
	if (keyframes.size() > 0)
	{
		float time = keyframes[0].time;
		for (UINT32 i = 1; i < (UINT32)keyframes.size(); i++)
		{
			assert(keyframes[i].time >= time);
			time = keyframes[i].time;
		}
	}
#endif

	if (keyframes.size() > 0)
		m_end = keyframes.back().time;
	else
		m_end = 0.0f;

	m_start = 0.0f;
	m_length = m_end;
}


/**
* Evaluates a cubic Hermite curve at a specific point.
*
* @param[in]	t			Parameter that at which to evaluate the curve, in range [0, 1].
* @param[in]	pointA		Starting point (at t=0).
* @param[in]	pointB		Ending point (at t=1).
* @param[in]	tangentA	Starting tangent (at t=0).
* @param[in]	tangentB	Ending tangent (at t = 1).
* @return					Evaluated value at @p t.
*/
template<class T>
static T cubicHermite(float t, const T& pointA, const T& pointB, const T& tangentA, const T& tangentB)
{
	float t2 = t * t;
	float t3 = t2 * t;

	float a = 2 * t3 - 3 * t2 + 1;
	float b = t3 - 2 * t2 + t;
	float c = -2 * t3 + 3 * t2;
	float d = t3 - t2;

	return a * pointA + b * tangentA + c * pointB + d * tangentB;
}


/** Checks if any components of the keyframes are constant (step) functions and updates the key value. */
void setStepValue(const TKeyframe<float>& lhs, const TKeyframe<float>& rhs, float& value)
{
	if (lhs.outTangent != std::numeric_limits<float>::infinity() &&
		rhs.inTangent != std::numeric_limits<float>::infinity())
		return;

	value = lhs.value;
}

void setStepValue(const TKeyframe<Vector3>& lhs, const TKeyframe<Vector3>& rhs, Vector3& value)
{
	for (int i = 0; i < 3; i++)
	{
		if (lhs.outTangent[i] != std::numeric_limits<float>::infinity() &&
			rhs.inTangent[i] != std::numeric_limits<float>::infinity())
			continue;

		value[i] = lhs.value[i];
	}
}

void setStepValue(const TKeyframe<Quaternion>& lhs, const TKeyframe<Quaternion>& rhs, Quaternion& value)
{
	for (int i = 0; i < 4; i++)
	{
		if (lhs.outTangent[i] != std::numeric_limits<float>::infinity() &&
			rhs.inTangent[i] != std::numeric_limits<float>::infinity())
			continue;

		value[i] = lhs.value[i];
	}
}

template <class T>
T getZero() { return 0.0f; }

template<>
float getZero<float>() { return 0.0f; }

template<>
Vector3 getZero<Vector3>() { return Vector3(0, 0, 0); }

template<>
Quaternion getZero<Quaternion>() { return Quaternion(0, 0, 0, 0); }


template<class T>
T Lerp(const T & a, const T & b, float t);

template<>
float Lerp<float>(const float & a, const float & b, float t) {
	return Mathf::Lerp(a, b, t);
}

template<>
Vector3 Lerp<Vector3>(const Vector3 & a, const Vector3 & b, float t) {
	return Vector3::Lerp(a, b, t);
}

template<>
Quaternion Lerp<Quaternion>(const Quaternion & a, const Quaternion & b, float t) {
	return Quaternion::Lerp(a, b, t);
}

template <class T>
T TAnimationCurve<T>::Evaluate(float time, bool loop) const
{
	if (m_keyframes.size() == 0)
		return getZero<T>();

	AnimationCurveUtility::WrapTime(time, m_start, m_end, loop);

	uint32_t leftKeyIdx;
	uint32_t rightKeyIdx;

	FindKeys(time, leftKeyIdx, rightKeyIdx);

	// Evaluate curve as hermit cubic spline
	auto & leftKey = m_keyframes[leftKeyIdx];
	auto & rightKey = m_keyframes[rightKeyIdx];

	if (leftKeyIdx == rightKeyIdx)
		return leftKey.value;

	float length = rightKey.time - leftKey.time;
	assert(length > 0.0f);

	float t;
	T leftTangent;
	T rightTangent;

	if (Mathf::CompareApproximately(length, 0.0f))
	{
		t = 0.0f;
		leftTangent = getZero<T>();
		rightTangent = getZero<T>();
	}
	else
	{
		// Scale from arbitrary range to [0, 1]
		t = (time - leftKey.time) / length;
		leftTangent = leftKey.outTangent * length;
		rightTangent = rightKey.inTangent * length;
	}

	//T output = cubicHermite(t, leftKey.value, rightKey.value, leftTangent, rightTangent);
	//setStepValue(leftKey, rightKey, output);
	
	// TODO
	T output = Lerp<T>(leftKey.value, rightKey.value, t);

	return output;
}


template <class T>
void TAnimationCurve<T>::FindKeys(float time, uint32_t& leftKey, uint32_t& rightKey) const
{
	int32_t start = 0;
	int32_t searchLength = (int32_t)m_keyframes.size();

	while (searchLength > 0)
	{
		int32_t half = searchLength >> 1;
		int32_t mid = start + half;

		if (time < m_keyframes[mid].time)
		{
			searchLength = half;
		}
		else
		{
			start = mid + 1;
			searchLength -= (half + 1);
		}
	}

	leftKey = std::max(0, start - 1);
	rightKey = std::min((uint32_t)start, (uint32_t)m_keyframes.size() - 1);
}

template <class T>
uint32_t TAnimationCurve<T>::FindKey(float time)
{
	uint32_t leftKeyIdx;
	uint32_t rightKeyIdx;

	FindKeys(time, leftKeyIdx, rightKeyIdx);

	const auto & leftKey = m_keyframes[leftKeyIdx];
	const auto & rightKey = m_keyframes[rightKeyIdx];

	if (Mathf::Abs(leftKey.time - time) <= Mathf::Abs(rightKey.time - time))
		return leftKeyIdx;

	return rightKeyIdx;
}

template class FishEngine::TAnimationCurve<Vector3>;
template class FishEngine::TAnimationCurve<Quaternion>;
template class FishEngine::TAnimationCurve<float>;
