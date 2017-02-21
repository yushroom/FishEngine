#pragma once

#include <vector>
#include <cstdint>

#include <Mesh.hpp>
#include <Vector3.hpp>
#include <Vector2.hpp>

namespace FishEngine
{

    /**
     * Raw mesh data used to construct optimized runtime rendering streams.
     *
     * A note on terminology. Information is stored at various frequencies as defined here:
     *     Face - A single polygon in the mesh. Currently all code assumes this is a triangle but
     *            conceptually any polygon would do.
     *     Corner - Each face has N corners. As all faces are currently triangles, N=3.
     *     Wedge - Properties stored for each corner of each face. Index with FaceIndex * NumCorners + CornerIndex.
     *     Vertex - Properties shared by overlapping wedges of adjacent polygons. Typically these properties
     *              relate to position. Index with VertexIndices[WedgeIndex].
     *
     * Additionally, to ease in backwards compatibility all properties should use only primitive types!
     */
    class RawMesh
    {
    public:

        uint32_t m_vertexCount = 0;
        uint32_t m_faceCount = 0;

        /** Position in local space. Array[VertexId] = float3(x,y,z) */
        std::vector<Vector3> m_vertexPositions;

        /** Index of the vertex at this wedge. Array[WedgeId] = VertexId */
        std::vector<uint32_t> m_wedgeIndices;

        std::vector<Vector3> m_wedgeNormals;

        std::vector<Vector3> m_wedgeTangents;

        std::vector<Vector2> m_wedgeTexCoords;

		// disjoint set
		std::map<uint32_t, uint32_t> m_vertexIndexRemapping;

        void SetVertexCount(uint32_t vertexCount)
        {
            m_vertexCount = vertexCount;
            m_vertexPositions.reserve(vertexCount);
        }

        void SetFaceCount(uint32_t faceCount)
        {
            m_faceCount = faceCount;
            m_wedgeIndices.reserve(faceCount * 3);    // 3 corners
            m_wedgeNormals.reserve(faceCount * 3);
            m_wedgeTangents.reserve(faceCount * 3);
            m_wedgeTexCoords.reserve(faceCount * 2);
        }


        std::shared_ptr<Mesh> ToMesh()
        {
            std::vector<Vector3> positionBuffer(m_vertexPositions);
            std::vector<Vector3> normalBuffer;
            std::vector<Vector3> tangentBuffer;
            std::vector<Vector2> uvBuffer;
			std::vector<uint32_t> indexBuffer(m_wedgeIndices);
			
			// used to combine vertex with the same position/uv/...
			//std::map<uint32_t, uint32_t> indexRemapping;
			
			// TODO: makes indexed meshes more GPU-friendly
			// https://github.com/zeux/meshoptimizer

            //positionBuffer.resize(m_vertexCount);
            normalBuffer.resize(m_vertexCount);    // minimum size
            tangentBuffer.resize(m_vertexCount);
            uvBuffer.resize(m_vertexCount);

            std::vector<bool> vertexVisited(m_vertexCount, false);

            for (uint32_t faceId = 0; faceId < m_faceCount; ++faceId)
            {
                for (int cornerId = 0; cornerId < 3; ++cornerId)
                {
                    uint32_t wedgeId = faceId * 3 + cornerId;
                    uint32_t vertexId = m_wedgeIndices[wedgeId];
                    if ( ! vertexVisited[vertexId] )
                    {
                        uvBuffer[vertexId] = m_wedgeTexCoords[wedgeId];
                        normalBuffer[vertexId] = m_wedgeNormals[wedgeId];
                        tangentBuffer[vertexId] = m_wedgeTangents[wedgeId];
						vertexVisited[vertexId] = true;
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
								indexBuffer[wedgeId] = pid;
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
									uint32_t newVertexId = static_cast<uint32_t>( positionBuffer.size() - 1 );
									indexBuffer[wedgeId] = newVertexId;
									m_vertexIndexRemapping[pid] = newVertexId;
									break;
								}
								pid = it->second;
							}
						}
                    }
                }
            }
			
			// now positionBuffer.size() == normalBuffer.size() == uvBuffer.size() == tangentBuffer.size()

            //std::vector<uint32_t> indexBuffer(WedgeIndices);
			return std::make_shared<Mesh>(std::move(positionBuffer), std::move(normalBuffer), std::move(uvBuffer), std::move(tangentBuffer), std::move(indexBuffer));
        }
    };
}
