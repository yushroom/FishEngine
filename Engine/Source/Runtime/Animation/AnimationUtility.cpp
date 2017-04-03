#include "AnimationUtility.hpp"
#include "TAnimationCurve.hpp"

using namespace FishEngine;

void AnimationUtility::wrapTime(float& time, float start, float end, bool loop)
{
	float length = end - start;

	if (Mathf::CompareApproximately(length, 0.0f))
	{
		time = 0.0f;
		return;
	}

	// Clamp to start or loop
	if (time < start)
	{
		if (loop)
			time = time + (std::floor(end - time) / length) * length;
		else // Clamping
			time = start;
	}

	// Clamp to end or loop
	if (time > end)
	{
		if (loop)
			time = time - std::floor((time - start) / length) * length;
		else // Clamping
			time = end;
	}
}

void setStepTangent(const TKeyframe<Vector3>& lhsIn, const TKeyframe<Vector3>& rhsIn,
	TKeyframe<Quaternion>& lhsOut, TKeyframe<Quaternion>& rhsOut)
{
	for (UINT32 i = 0; i < 3; i++)
	{
		if (lhsIn.outTangent[i] != std::numeric_limits<float>::infinity() &&
			rhsIn.inTangent[i] != std::numeric_limits<float>::infinity())
			continue;

		lhsOut.outTangent[i] = std::numeric_limits<float>::infinity();
		rhsOut.inTangent[i] = std::numeric_limits<float>::infinity();
	}
}

TAnimationCurve<Quaternion> AnimationUtility::eulerToQuaternionCurve(const TAnimationCurve<Vector3>& eulerCurve)
{
	// TODO: We calculate tangents by sampling which can introduce error in the tangents. The error can be exacerbated
	// by the fact we constantly switch between the two representations, possibly losing precision every time. Instead 
	// there must be an analytical way to calculate tangents when converting a curve, or a better way of dealing with
	// tangents.
	// Consider: 
	//  - Sampling multiple points to calculate tangents to improve precision
	//  - Store the original quaternion curve with the euler curve
	//    - This way conversion from euler to quaternion can be done while individual keyframes are being modified
	//      ensuring the conversion results are immediately visible, and that no accumulation error happens are curves
	//		are converted between two formats back and forth.
	//  - Don't store rotation tangents directly, instead store tangent parameters (TCB) which can be shared between
	//    both curves, and used for tangent calculation.
	//
	// If we decide to keep tangents in the current form, then we should also enforce that all euler curve tangents are
	// the same.
	const float FIT_TIME = 0.001f;

	auto eulerToQuaternion = [&](INT32 keyIdx, Vector3& angles, const Quaternion& lastQuat)
	{
		//Quaternion quat(
		//	Degree(angles.x),
		//	Degree(angles.y),
		//	Degree(angles.z));

		Quaternion quat = Quaternion::Euler(Mathf::Degrees(angles.x), Mathf::Degrees(angles.y), Mathf::Degrees(angles.z));

		// Flip quaternion in case rotation is over 180 degrees (use shortest path)
		if (keyIdx > 0)
		{
			float dot = Quaternion::Dot(quat, lastQuat);
			if (dot < 0.0f)
			{
				//quat = -quat;
				quat.x = -quat.x;
				quat.y = -quat.y;
				quat.z = -quat.z;
				quat.w = -quat.w;
			}
				
		}

		return quat;
	};

	INT32 numKeys = (INT32)eulerCurve.getNumKeyFrames();
	std::vector<TKeyframe<Quaternion>> quatKeyframes(numKeys);

	// Calculate key values
	Quaternion lastQuat;
	for (INT32 i = 0; i < numKeys; i++)
	{
		float time = eulerCurve.getKeyFrame(i).time;
		Vector3 angles = eulerCurve.getKeyFrame(i).value;
		Quaternion quat = eulerToQuaternion(i, angles, lastQuat);

		quatKeyframes[i].time = time;
		quatKeyframes[i].value = quat;
		quatKeyframes[i].inTangent = Quaternion::identity;
		quatKeyframes[i].outTangent = Quaternion::identity;

		lastQuat = quat;
	}

	// Calculate extra values between keys so we can approximate tangents. If we're sampling very close to the key
	// the values should pretty much exactly match the tangent (assuming the curves are cubic hermite)
	for (INT32 i = 0; i < numKeys - 1; i++)
	{
		TKeyframe<Quaternion>& currentKey = quatKeyframes[i];
		TKeyframe<Quaternion>& nextKey = quatKeyframes[i + 1];

		const TKeyframe<Vector3>& currentEulerKey = eulerCurve.getKeyFrame(i);
		const TKeyframe<Vector3>& nextEulerKey = eulerCurve.getKeyFrame(i + 1);

		float dt = nextKey.time - currentKey.time;
		float startFitTime = currentKey.time + dt * FIT_TIME;
		float endFitTime = currentKey.time + dt * (1.0f - FIT_TIME);

		Vector3 anglesStart = eulerCurve.evaluate(startFitTime, false);
		Vector3 anglesEnd = eulerCurve.evaluate(endFitTime, false);
		Quaternion startFitValue = eulerToQuaternion(i, anglesStart, currentKey.value);
		Quaternion endFitValue = eulerToQuaternion(i, anglesEnd, startFitValue);

		float invFitTime = 1.0f / (dt * FIT_TIME);
		currentKey.outTangent = (startFitValue - currentKey.value) * invFitTime;
		nextKey.inTangent = (nextKey.value - endFitValue) * invFitTime;

		setStepTangent(currentEulerKey, nextEulerKey, currentKey, nextKey);
	}

	return TAnimationCurve<Quaternion>(quatKeyframes);
}

template<class T>
TAnimationCurve<T> AnimationUtility::scaleCurve(const TAnimationCurve<T>& curve, float factor)
{
	INT32 numKeys = (INT32)curve.getNumKeyFrames();

	std::vector<TKeyframe<T>> newKeyframes(numKeys);
	for (INT32 i = 0; i < numKeys; i++)
	{
		const TKeyframe<T>& key = curve.getKeyFrame(i);
		newKeyframes[i].time = key.time;
		newKeyframes[i].value = key.value * factor;
		newKeyframes[i].inTangent = key.inTangent * factor;
		newKeyframes[i].outTangent = key.outTangent * factor;
	}

	return TAnimationCurve<T>(newKeyframes);
}

template<class T>
TAnimationCurve<T> AnimationUtility::offsetCurve(const TAnimationCurve<T>& curve, float offset)
{
	INT32 numKeys = (INT32)curve.getNumKeyFrames();

	std::vector<TKeyframe<T>> newKeyframes(numKeys);
	for (INT32 i = 0; i < numKeys; i++)
	{
		const TKeyframe<T>& key = curve.getKeyFrame(i);
		newKeyframes[i].time = key.time + offset;
		newKeyframes[i].value = key.value;
		newKeyframes[i].inTangent = key.inTangent;
		newKeyframes[i].outTangent = key.outTangent;
	}

	return TAnimationCurve<T>(newKeyframes);
}

template FE_EXPORT TAnimationCurve<Vector3> AnimationUtility::scaleCurve(const TAnimationCurve<Vector3>& curve, float factor);
template FE_EXPORT TAnimationCurve<Quaternion> AnimationUtility::scaleCurve(const TAnimationCurve<Quaternion>& curve, float factor);
template FE_EXPORT TAnimationCurve<float> AnimationUtility::scaleCurve(const TAnimationCurve<float>& curve, float factor);

template FE_EXPORT TAnimationCurve<Vector3> AnimationUtility::offsetCurve(const TAnimationCurve<Vector3>& curve, float offset);
template FE_EXPORT TAnimationCurve<Quaternion> AnimationUtility::offsetCurve(const TAnimationCurve<Quaternion>& curve, float offset);
template FE_EXPORT TAnimationCurve<float> AnimationUtility::offsetCurve(const TAnimationCurve<float>& curve, float offset);
