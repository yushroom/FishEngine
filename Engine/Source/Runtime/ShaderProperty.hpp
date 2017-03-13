#pragma once

#include "Matrix4x4.hpp"
#include "Vector2.hpp"
#include "Vector3.hpp"
#include "Vector4.hpp"
#include "GLEnvironment.hpp"
#include "Common.hpp"

namespace FishEngine
{
	struct ShaderUniforms
	{
		std::map<std::string, Matrix4x4> mat4s;
		std::map<std::string, Vector2> vec2s;
		std::map<std::string, Vector3> vec3s;
		std::map<std::string, Vector4> vec4s;
		std::map<std::string, float> floats;
	};

	struct UniformInfo
	{
		GLenum      type; // type of the variable (float, vec3 or mat4, etc)
		std::string name;  // variable name in GLSL
		GLuint      location;
		int         textureBindPoint;
		bool        binded;
	};

	//struct UniformTextureInfo
	//{
	//    GLenum type;
	//    string name;
	//    GLuint location;
	//    
	//    bool binded;
	//};

	enum class ShaderKeyword
	{
		None = 0,
		SkinnedAnimation = 1,
		AmbientIBL = 2,
		All = SkinnedAnimation | AmbientIBL,
	};

	typedef std::uint32_t ShaderKeywords;


	// https://docs.unity3d.com/Manual/SL-CullAndDepth.html
	enum class Cullface {
		Back = GL_BACK,
		Front = GL_FRONT,
		Off = GL_NONE,
	};

	enum class ZWrite {
		On,
		Off,
	};

	enum class ZTest {
		Less = GL_LESS,
		Greater = GL_GREATER,
		LEqual = GL_LEQUAL,
		GEqual = GL_GEQUAL,
		Equal = GL_EQUAL,
		NotEqual = GL_NOTEQUAL,
		Always = GL_ALWAYS,
	};
}
