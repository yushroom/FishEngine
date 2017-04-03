#include "RawMesh.hpp"

using namespace FishEngine;

std::shared_ptr<Mesh> FishEngine::RawMesh::ToMesh()
{
	std::vector<Vector3> positionBuffer(m_vertexPositions);
	std::vector<Vector3> normalBuffer;
	std::vector<Vector3> tangentBuffer;
	std::vector<Vector2> uvBuffer;
	std::vector<uint32_t> indexBuffer;
	indexBuffer.reserve(m_wedgeIndices.size());

	//std::vector<std::vector<uint32_t>> submeshIndex;

	// used to combine vertex with the same position/uv/...
	//std::map<uint32_t, uint32_t> indexRemapping;

	// TODO: makes indexed meshes more GPU-friendly
	// https://github.com/zeux/meshoptimizer

	//positionBuffer.resize(m_vertexCount);
	normalBuffer.resize(m_vertexCount);    // minimum size
	tangentBuffer.resize(m_vertexCount);
	uvBuffer.resize(m_vertexCount);

	std::vector<bool> vertexVisited(m_vertexCount, false);

	bool hasSubMesh = m_subMeshCount > 1;
	std::vector<uint32_t> subMeshOffset(m_subMeshCount);
	subMeshOffset[0] = 0;

	for (int subMeshId = 0; subMeshId < m_subMeshCount; ++subMeshId)
	{
		subMeshOffset[subMeshId] = indexBuffer.size();
		for (uint32_t faceId = 0; faceId < m_faceCount; ++faceId)
		{
			for (int cornerId = 0; cornerId < 3; ++cornerId)
			{
				if (hasSubMesh && m_submeshMap[faceId] != subMeshId)
					continue;
				uint32_t wedgeId = faceId * 3 + cornerId;
				uint32_t vertexId = m_wedgeIndices[wedgeId];
				if (!vertexVisited[vertexId])
				{
					uvBuffer[vertexId] = m_wedgeTexCoords[wedgeId];
					normalBuffer[vertexId] = m_wedgeNormals[wedgeId];
					tangentBuffer[vertexId] = m_wedgeTangents[wedgeId];
					vertexVisited[vertexId] = true;
					indexBuffer.push_back(vertexId);
				}
				else
				{
					int pid = vertexId;
					while (true)
					{
						bool isSame = uvBuffer[pid] == m_wedgeTexCoords[wedgeId] &&
							normalBuffer[pid] == m_wedgeNormals[wedgeId] &&
							tangentBuffer[pid] == m_wedgeTangents[wedgeId];
						if (isSame)
						{
							indexBuffer.push_back(pid);
							break;
						}
						else
						{
							auto it = m_vertexIndexRemapping.find(pid);
							if (it == m_vertexIndexRemapping.end())
							{
								// make a new vertex
								positionBuffer.push_back(positionBuffer[vertexId]);
								uvBuffer.push_back(m_wedgeTexCoords[wedgeId]);
								normalBuffer.push_back(m_wedgeNormals[wedgeId]);
								tangentBuffer.push_back(m_wedgeTangents[wedgeId]);
								uint32_t newVertexId = static_cast<uint32_t>(positionBuffer.size() - 1);
								indexBuffer.push_back(newVertexId);
								m_vertexIndexRemapping[pid] = newVertexId;
								break;
							}
							pid = it->second;
						}
					}
				}
			}
		}
	}

	// now positionBuffer.size() == normalBuffer.size() == uvBuffer.size() == tangentBuffer.size()
	auto ret = std::make_shared<Mesh>(std::move(positionBuffer), std::move(normalBuffer), std::move(uvBuffer), std::move(tangentBuffer), std::move(indexBuffer));
	if (m_subMeshCount > 1)
	{
		ret->m_subMeshCount = m_subMeshCount;
		ret->m_subMeshIndexOffset = subMeshOffset;
	}
	return ret;
}
