//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//**************** Copyright (c) 2016 Marko Pintera (marko.pintera@gmail.com). All rights reserved. **********************//
#pragma once

#include <cstdint>
#include "../Matrix4x4.hpp"
#include "../Vector3.hpp"
#include "../Quaternion.hpp"
#include "../ReflectClass.hpp"
#include "CurveCache.hpp"
//#include "BsCurveCache.h"
#include "BsCorePrerequisites.h"

namespace FishEngine
{
	class SkeletonMask;
	struct AnimationCurves;

	/** @addtogroup Animation-Internal
	 *  @{
	 */

	 /**
	  * Contains indices for position/rotation/scale animation curves. Used for quick mapping of bones in a skeleton to
	  * relevant animation curves.
	  */
	struct AnimationCurveMapping
	{
		uint32_t position;
		uint32_t rotation;
		uint32_t scale;
	};

	/** Information about a single bone used for constructing a skeleton. */
	struct BONE_DESC
	{
		std::string name; /**< Unique name of the bone. */
		uint32_t parent; /**< Index of the parent bone, if any. -1 if root bone. */

		Matrix4x4 invBindPose; /**< Inverse bind pose which transforms vertices from their bind pose into local space. */
	};

	/** Contains information about a single playing animation clip. */
	struct AnimationState
	{
		std::shared_ptr<AnimationCurves> curves; /**< All curves in the animation clip. */
		AnimationCurveMapping* boneToCurveMapping; /**< Mapping of bone indices to curve indices for quick lookup .*/
		AnimationCurveMapping* soToCurveMapping; /**< Mapping of scene object indices to curve indices for quick lookup. */

		TCurveCache<Vector3>* positionCaches; /**< Cache used for evaluating position curves. */
		TCurveCache<Quaternion>* rotationCaches; /**< Cache used for evaluating rotation curves. */
		TCurveCache<Vector3>* scaleCaches; /**< Cache used for evaluating scale curves. */
		TCurveCache<float>* genericCaches; /**< Cache used for evaluating generic curves. */

		float time; /**< Time to evaluate the curve at. */
		float weight; /**< Determines how much of an influence will this clip have in regard to others in the same layer. */
		bool loop; /**< Determines should the animation loop (wrap) once ending or beginning frames are passed. */
		bool disabled; /**< If true the clip state will not be evaluated. */
	};

	/** Contains animation states for a single animation layer. */
	struct AnimationStateLayer
	{
		AnimationState* states; /**< Array of animation states in the layer. */
		uint32_t numStates; /**< Number of states in @p states. */

		unsigned char index; /**< Unique index of the animation layer. */

		/**
		 * If true animations from this layer will be added on top of other layers using the per-state weights. If false
		 * the weights will be normalized, animations will be blended with each other according to the normalized weights
		 * and then added on top of other layers.
		 */
		bool additive;
	};

	/** 
	 * Contains local translation, rotation and scale values for each bone in a skeleton, after being evaluated at a
	 * specific time of an animation.  All values are stored in the same order as the bones in the skeleton they were
	 * created by.
	 */
	struct LocalSkeletonPose
	{
		LocalSkeletonPose();
		LocalSkeletonPose(uint32_t numBones);
		LocalSkeletonPose(uint32_t numPos, uint32_t numRot, uint32_t numScale);
		LocalSkeletonPose(const LocalSkeletonPose& other) = delete;
		LocalSkeletonPose(LocalSkeletonPose&& other);
		~LocalSkeletonPose();

		LocalSkeletonPose& operator=(const LocalSkeletonPose& other) = delete;
		LocalSkeletonPose& operator=(LocalSkeletonPose&& other);

		Vector3* positions; /**< Local bone positions at specific animation time. */
		Quaternion* rotations; /**< Local bone rotations at specific animation time. */
		Vector3* scales; /**< Local bone scales at specific animation time. */
		bool* hasOverride; /**< True if the bone transform was overriden externally (local pose was ignored). */
		uint32_t numBones; /**< Number of bones in the pose. */
	};

	/** Contains internal information about a single bone in a Skeleton. */
	struct SkeletonBoneInfo
	{
		std::string name; /**< Unique name of the bone. */
		uint32_t parent; /**< Index of the bone parent, or -1 if root (no parent). */
	};

	/** 
	 * Contains information about bones required for skeletal animation. Allows caller to evaluate a set of animation
	 * clips at a specific time and output the relevant skeleton pose.
	 */
	class FE_EXPORT Skeleton // Note: Must be immutable in order to be usable on multiple threads
	{
	public:

		Skeleton();
		Skeleton(BONE_DESC* bones, uint32_t numBones);
		~Skeleton();

		/** 
		 * Outputs a skeleton pose containing required transforms for transforming the skeleton to the values specified by
		 * the provided animation clip evaluated at the specified time.
		 *
		 * @param[out]	pose		Output pose containing the requested transforms. Must be pre-allocated with enough space
		 *							to hold all the bone matrices of this skeleton.
		 * @param[in]	mask		Mask that filters which skeleton bones are enabled or disabled.
		 * @param[out]	localPose	Output pose containing the local transforms. Must be pre-allocated with enough space
		 *							to hold all the bone data of this skeleton.
		 * @param[in]	clip		Clip to evaluate.
		 * @param[in]	time		Time to evaluate the clip with.
		 * @param[in]	loop		Determines should the time be looped (wrapped) if it goes past the clip start/end.
		 *
		 * @note	It is more efficient to use the other getPose overload as sequential calls can benefit from animation
		 *			evaluator cache.
		 */
		void getPose(Matrix4x4* pose, LocalSkeletonPose& localPose, const SkeletonMask& mask, const AnimationClip& clip, 
			float time, bool loop = true);

		/** 
		 * Outputs a skeleton pose containing required transforms for transforming the skeleton to the values specified by
		 * the provided set of animation curves.
		 *
		 * @param[out]	pose		Output pose containing the requested transforms. Must be pre-allocated with enough space
		 *							to hold all the bone matrices of this skeleton.
		 * @param[in]	mask		Mask that filters which skeleton bones are enabled or disabled.
		 * @param[out]	localPose	Output pose containing the local transforms. Must be pre-allocated with enough space
		 *							to hold all the bone data of this skeleton.
		 * @param[in]	layers		One or multiple layers, containing one or multiple animation states to evaluate.
		 * @param[in]	numLayers	Number of layers in the @p layers array.
		 */
		void getPose(Matrix4x4* pose, LocalSkeletonPose& localPose, const SkeletonMask& mask, 
			const AnimationStateLayer* layers, uint32_t numLayers);

		/** Returns the total number of bones in the skeleton. */
		uint32_t getNumBones() const { return mNumBones; }

		/** Returns information about a bone at the provided index. */
		const SkeletonBoneInfo& getBoneInfo(uint32_t idx) const { return mBoneInfo[idx]; }

		/** Searches all bones to find a root bone. Returns -1 if no root can be found. */
		uint32_t getRootBoneIndex() const;

		/** Returns the inverse bind pose for the bone at the provided index. */
		const Matrix4x4& getInvBindPose(uint32_t idx) const { return mInvBindPoses[idx]; }

		/** 
		 * Creates a new Skeleton. 
		 *
		 * @param[in]	bones		An array of bones to initialize the skeleton with. Data will be copied.
		 * @param[in]	numBones	Number of bones in the @p bones array.
		 */
		static std::shared_ptr<Skeleton> create(BONE_DESC* bones, uint32_t numBones);

	private:
		

		uint32_t mNumBones;
		std::vector<Matrix4x4> mInvBindPoses;
		std::vector<SkeletonBoneInfo> mBoneInfo;
	};

	/** @} */
}