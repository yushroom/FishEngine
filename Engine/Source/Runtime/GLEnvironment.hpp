#ifndef GLEnvironment_hpp
#define GLEnvironment_hpp

#include "FishEngine.hpp"

// GLEW
#if !defined(GLEW_STATIC) && !defined(FishEngine_SHARED_LIB)
#define GLEW_STATIC
#endif
#include <GL/glew.h>
// GLFW
//#include <glfw/glfw3.h>

//#ifdef _DEBUG
#define glCheckError() _checkOpenGLError(__FILE__,__LINE__)
//#else
//#define glCheckError()
//#endif

FE_EXPORT void _checkOpenGLError(const char *file, int line);

#endif // GLEnvironment_hpp
