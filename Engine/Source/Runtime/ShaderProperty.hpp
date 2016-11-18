#pragma once

#include "Matrix4x4.hpp"
#include "Vector3.hpp"
#include "Vector4.hpp"
#include "GLEnvironment.hpp"

namespace FishEngine
{
    struct ShaderUniforms
    {
        std::map<std::string, Matrix4x4> mat4s;
        std::map<std::string, Vector3> vec3s;
        std::map<std::string, Vector4> vec4s;
        std::map<std::string, float> floats;
    };

    struct UniformInfo {
        //GLint size; // size of the variable
        GLenum type; // type of the variable (float, vec3 or mat4, etc)
        //GLchar name[32];
        std::string name;  // variable name in GLSL
        GLuint location;
        bool binded;
    };

    enum ShaderKeyword
    {
        None = 0,
        Shadow = 1,
        SkinnedAnimation = 2,
        AmbientIBL = 4,
        All = Shadow & SkinnedAnimation & AmbientIBL,
    };

    typedef std::uint32_t ShaderKeywords;
}