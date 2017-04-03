#pragma once

#include "AnimationClip.hpp"
#include "AnimationClipState.hpp"
#include "AnimPlaybackType.hpp"

namespace FishEngine
{
	/** Internal information about a single playing animation clip within Animation. */
	struct AnimationClipInfo
	{
		AnimationClipInfo::AnimationClipInfo()
			: fadeDirection(0.0f), fadeTime(0.0f), fadeLength(0.0f), curveVersion(0), layerIdx((UINT32)-1)
			, stateIdx((UINT32)-1), playbackType(AnimPlaybackType::Normal)
		{ }

		AnimationClipInfo::AnimationClipInfo(const HAnimationClip& clip)
			: fadeDirection(0.0f), fadeTime(0.0f), fadeLength(0.0f), clip(clip), curveVersion(0), layerIdx((UINT32)-1)
			, stateIdx((UINT32)-1), playbackType(AnimPlaybackType::Normal)
		{ }

		HAnimationClip clip;
		AnimationClipState state;
		AnimPlaybackType playbackType;

		float fadeDirection;
		float fadeTime;
		float fadeLength;

		/**
		* Version of the animation curves used by the AnimationProxy. Used to detecting the internal animation curves
		* changed.
		*/
		uint64_t curveVersion;
		uint32_t layerIdx; /**< Layer index this clip belongs to in AnimationProxy structure. */
		uint32_t stateIdx; /**< State index this clip belongs to in AnimationProxy structure. */
	};
}