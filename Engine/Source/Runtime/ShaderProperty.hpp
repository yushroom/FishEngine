#pragma once

#include "Matrix4x4.hpp"
#include "Vector3.hpp"
#include "Vector4.hpp"
#include "GLEnvironment.hpp"
#include "Common.hpp"

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

    enum class ShaderKeyword
    {
        None = 0,
        Shadow = 1,
        SkinnedAnimation = 2,
        AmbientIBL = 4,
        All = Shadow | SkinnedAnimation | AmbientIBL,
    };

    typedef std::uint32_t ShaderKeywords;


    // https://docs.unity3d.com/Manual/SL-CullAndDepth.html
    enum class Cullface {
        Back = GL_BACK,
        Front = GL_FRONT,
        Off = GL_NONE,
    };

    inline const char* ToString(Cullface e)
    {
        switch (e) {
        case Cullface::Back: return "Back"; break;
        case Cullface::Front: return "Front"; break;
        case Cullface::Off: return "Off"; break;
        default: abort(); break;
        }
    }


    template<>
    inline Cullface ToEnum<Cullface>(const std::string& s)
    {
        if (s == "back" || s == "Back") return Cullface::Back;
        if (s == "front" || s == "Front") return Cullface::Front;
        if (s == "off" || s == "Off") return Cullface::Off;
        abort();
    }

    enum class ZWrite {
        On,
        Off,
    };

    inline const char* ToString(ZWrite e)
    {
        switch (e) {
        case ZWrite::On: return "On"; break;
        case ZWrite::Off: return "Off"; break;
        default: abort(); break;
        }
    }

    template<>
    inline ZWrite ToEnum<ZWrite>(const std::string& s)
    {
        if (s == "On") return ZWrite::On;
        if (s == "Off") return ZWrite::Off;
        abort();
    }

    enum class ZTest {
        Less = GL_LESS,
        Greater = GL_GREATER,
        LEqual = GL_LEQUAL,
        GEqual = GL_GEQUAL,
        Equal = GL_EQUAL,
        NotEqual = GL_NOTEQUAL,
        Always = GL_ALWAYS,
    };

    inline const char* ToString(ZTest e)
    {
        switch (e) {
        case ZTest::Less: return "Less"; break;
        case ZTest::Greater: return "Greater"; break;
        case ZTest::LEqual: return "LEqual"; break;
        case ZTest::GEqual: return "GEqual"; break;
        case ZTest::Equal: return "Equal"; break;
        case ZTest::NotEqual: return "NotEqual"; break;
        case ZTest::Always: return "Always"; break;
        default: abort(); break;
        }
    }

    template<>
    inline ZTest ToEnum<ZTest>(const std::string& s)
    {
        if (s == "Less") return ZTest::Less;
        if (s == "Greater") return ZTest::Greater;
        if (s == "LEqual") return ZTest::LEqual;
        if (s == "GEqual") return ZTest::GEqual;
        if (s == "Equal") return ZTest::Equal;
        if (s == "NotEqual") return ZTest::NotEqual;
        if (s == "Always") return ZTest::Always;
        abort();
    }
}