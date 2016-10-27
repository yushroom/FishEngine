#include "Pipeline.hpp"

namespace FishEngine
{
    PerDraw Pipeline::perDrawUniformData;
    PerFrame Pipeline::perFrameUniformData;
    //Bones Pipeline::bonesUniformData;
    GLuint Pipeline::perDrawUBO = 0;
    GLuint Pipeline::perFrameUBO = 0;
    //GLuint Pipeline::bonesUBO = 0;
}
