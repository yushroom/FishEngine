#pragma once

#include <string>
#include <vector>

namespace FishEngine
{
	using BoneID = std::hash<std::string>::result_type;

	struct SkeletonNode
	{
		BoneID m_id;
		BoneID m_parentId;
	};

	struct Skeleton
	{
		uint32_t m_NodeCount;
		std::vector<SkeletonNode> m_nodes;
	};
}