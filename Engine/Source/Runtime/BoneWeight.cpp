#include <FishEngine/BoneWeight.hpp>
#include <FishEngine/Debug.hpp>

FishEngine::BoneWeight::BoneWeight()
{
	for (int i = 0; i < MaxBoneForEachVertex; ++i)
	{
		boneIndex[i] = 0;
		weight[i] = 0.f;
	}
}

void FishEngine::BoneWeight::AddBoneData(uint32_t boneIndex, float weight)
{
	for (int i = 0; i < MaxBoneForEachVertex; ++i)
	{
		if (this->weight[i] == 0.0f)
		{
			this->boneIndex[i] = boneIndex;
			this->weight[i] = weight;
			return;
		}
	}
	// more than MaxBoneForEachVertex
	LogWarning(Format("more than %1% bones", MaxBoneForEachVertex));

	int minId = 0;
	float minWeight = this->weight[0];
	for (int i = 1; i < MaxBoneForEachVertex; ++i)
	{
		if (this->weight[i] < minWeight)
		{
			minId = i;
			minWeight = this->weight[i];
		}
	}
	if (minWeight < weight)
	{
		this->boneIndex[minId] = boneIndex;
		this->weight[minId] = weight;
	}
}