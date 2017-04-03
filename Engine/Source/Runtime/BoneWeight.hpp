#pragma once

#include "ReflectClass.hpp"

namespace FishEngine
{
	static constexpr int MaxBoneForEachVertex = 4;

	struct FE_EXPORT BoneWeight
	{
		//union {
		//    struct {
		//        int boneIndex0;
		//        int boneIndex1;
		//        int boneIndex2;
		//        int boneIndex3;
		//    };
		//    int boneIndex[4];
		//};
		//union {
		//    struct {
		//        float weight0;
		//        float weight1;
		//        float weight2;
		//        float weight3;
		//    };
		//    float weight[4];
		//};

		Meta(NonSerializable)
		int boneIndex[MaxBoneForEachVertex];

		Meta(NonSerializable)
		float weight[MaxBoneForEachVertex];

		BoneWeight();

		void AddBoneData(uint32_t boneIndex, float weight);
	};
}