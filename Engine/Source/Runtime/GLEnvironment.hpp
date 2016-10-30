#ifndef GLEnvironment_hpp
#define GLEnvironment_hpp

#include "FishEngine.hpp"

// GLEW
//#define GLEW_STATIC // passed by command line from cmake
#include <GL/glew.h>
// GLFW
#include <glfw/glfw3.h>

//#ifdef _DEBUG
#define glCheckError() _checkOpenGLError(__FILE__,__LINE__)
//#else
//#define glCheckError()
//#endif

void _checkOpenGLError(const char *file, int line);

#endif // GLEnvironment_hpp
