#include "GLEnvironment.hpp"
#include "Debug.hpp"

using namespace FishEngine;

void _checkOpenGLError(const char *file, int line)
{
	GLenum err(glGetError());

	while (err != GL_NO_ERROR)
	{
		std::string error;

		switch (err)
		{
		case GL_INVALID_OPERATION:      error = "INVALID_OPERATION";      break;
		case GL_INVALID_ENUM:           error = "INVALID_ENUM";           break;
		case GL_INVALID_VALUE:          error = "INVALID_VALUE";          break;
		case GL_OUT_OF_MEMORY:          error = "OUT_OF_MEMORY";          break;
		case GL_INVALID_FRAMEBUFFER_OPERATION:  error = "INVALID_FRAMEBUFFER_OPERATION";  break;
		default:                        error = "UNKNOWN_ERROR";
		}

		LogError(Format("GL_%1% - %2%:%3%", error, file, line));
		err = glGetError();
	}
}
