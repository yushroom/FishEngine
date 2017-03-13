#ifndef GLEnvironment_hpp
#define GLEnvironment_hpp

#include "FishEngine.hpp"

#if FISHENGINE_PLATFORM_WINDOWS
#if 1
	// GLEW
	#if !defined(GLEW_STATIC) && !defined(FishEngine_SHARED_LIB)
	#define GLEW_STATIC
	#endif
	#include <GL/glew.h>
#else
	#ifndef APIENTRY
	#define APIENTRY __stdcall
	#endif
	#ifndef WINGDIAPI
	#define WINGDIAPI extern
	#endif
	#include <GL/GL.h>
	#define GL_GLEXT_PROTOTYPES 1
	#include "glext.h"
	#include "glcorearb.h"
#endif
#else
	#include <OpenGL/gl3.h>
	#include <OpenGL/gl3ext.h>
#endif

//#ifdef _DEBUG
#define glCheckError() _checkOpenGLError(__FILE__,__LINE__)
//#else
//#define glCheckError()
//#endif

FE_EXPORT void _checkOpenGLError(const char *file, int line);

#endif // GLEnvironment_hpp
