#include "Pipeline.hpp"

namespace FishEngine
{
    PerDraw Pipeline::perDrawUniformData;
    PerFrame Pipeline::perFrameUniformData;
    GLuint Pipeline::perDrawUBO = 0;
    GLuint Pipeline::perFrameUBO = 0;
}
