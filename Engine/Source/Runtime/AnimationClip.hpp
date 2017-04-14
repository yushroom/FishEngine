#pragma once

#include "FishEngine.hpp"
#include "Object.hpp"
#include "Animation/WrapMode.hpp"
#include "Animation/AnimationBlendMode.hpp"
#include "Animation/AnimationEvent.hpp"

#include "Animation/AnimationCurve.hpp"

namespace FishEngine
{
	class FE_EXPORT Motion : public Object
	{
	public:
		InjectClassName(Motion);
	};
	
	struct FE_EXPORT Meta(NonSerializable) QuaternionCurve
	{
		std::string path;
		TAnimationCurve<Quaternion> curve;
	};

	struct FE_EXPORT Meta(NonSerializable) Vector3Curve
	{
		std::string path;
		TAnimationCurve<Vector3> curve;
	};
	
	// Stores keyframe based animations.
	// AnimationClip is used by Animation to play back animations.
	class FE_EXPORT AnimationClip final : public Motion
	{
	public:
		
		InjectClassName(AnimationClip);
		
		// Animation Events for this animation clip.
		std::vector<AnimationEvent> events;
		
		// Frame rate at which keyframes are sampled. (Read Only)
		// This is the frame rate that was used in the animation program you used to create the animation or model.
		float frameRate;
		
		// Animation length in seconds. (Read Only)
		float length;
		
		// Sets the default wrap mode used in the animation state.
		WrapMode wrapMode;

		//TAnimationCurve<Vector3> m_positionCurve;
		//TAnimationCurve<Quaternion> m_rotationCurve;
		//TAnimationCurve<Vector3> m_scaleCurve;
		Meta(NonSerializable)
		std::vector<Vector3Curve> m_positionCurve;

		Meta(NonSerializable)
		std::vector<QuaternionCurve> m_rotationCurves;

		//Meta(NonSerializable)
		//std::vector<Vector3Curve> m_eulersCurves;

		Meta(NonSerializable)
		std::vector<Vector3Curve> m_scaleCurves;

		Meta(NonSerializable)
		AvatarPtr m_avatar;
	};


	// Information about the current or next state.
	struct AnimatorStateInfo
	{

	};
}
