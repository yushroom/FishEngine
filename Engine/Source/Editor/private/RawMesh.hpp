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
        std::vector<Vector3> VertexPositions;

        /** Index of the vertex at this wedge. Array[WedgeId] = VertexId */
        std::vector<uint32_t> WedgeIndices;

        std::vector<Vector3> WedgeNormals;

        std::vector<Vector3> WedgeTangents;

        std::vector<Vector2> WedgeTexCoords;

        void SetVertexCount(uint32_t vertexCount)
        {
            m_vertexCount = vertexCount;
            VertexPositions.reserve(vertexCount);
        }

        void SetFaceCount(uint32_t faceCount)
        {
            m_faceCount = faceCount;
            WedgeIndices.reserve(faceCount * 3);    // 3 corners
            WedgeNormals.reserve(faceCount * 3);
            WedgeTangents.reserve(faceCount * 3);
            WedgeTexCoords.reserve(faceCount * 2);
        }


        std::shared_ptr<Mesh> ToMesh()
        {
            std::vector<Vector3> positionBuffer(VertexPositions);
            std::vector<Vector3> normalBuffer;
            std::vector<Vector3> tangentBuffer;
            std::vector<Vector2> uvBuffer;
			std::vector<uint32_t> indexBuffer(WedgeIndices);
			
			// used to combine vertex with the same position/uv/...
			//std::map<uint32_t, uint32_t> indexRemapping;
			
			// TODO: makes indexed meshes more GPU-friendly
			// https://github.com/zeux/meshoptimizer

            //positionBuffer.resize(m_vertexCount);
            normalBuffer.resize(m_vertexCount);    // minimum size
            tangentBuffer.resize(m_vertexCount);
            uvBuffer.resize(m_vertexCount);

            std::vector<bool> vertexVisited(m_vertexCount, false);

            for (int i = 0; i < m_faceCount; ++i)
            {
                for (int j = 0; j < 3; ++j)
                {
                    uint32_t wedgeId = i * 3 + j;
                    uint32_t vertexId = WedgeIndices[wedgeId];
                    if ( ! vertexVisited[vertexId] )
                    {
                        uvBuffer[vertexId] = WedgeTexCoords[wedgeId];
                        normalBuffer[vertexId] = WedgeNormals[wedgeId];
                        tangentBuffer[vertexId] = WedgeTangents[wedgeId];
						vertexVisited[vertexId] = true;
                    }
                    else
                    {
                        bool newVertex = (uvBuffer[vertexId] != WedgeTexCoords[wedgeId]);
                        if (!newVertex)
						{
                            newVertex = (normalBuffer[vertexId] != WedgeNormals[wedgeId]);
						}
                        if (!newVertex)
						{
                            newVertex = (tangentBuffer[vertexId] != WedgeTangents[wedgeId]);
						}
                        if (newVertex)
                        {
							indexBuffer[wedgeId] = positionBuffer.size();
                            positionBuffer.push_back(positionBuffer[vertexId]);
                            uvBuffer.push_back( WedgeTexCoords[wedgeId] );
                            normalBuffer.push_back( WedgeNormals[wedgeId] );
                            tangentBuffer.push_back( WedgeTangents[wedgeId] );
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
