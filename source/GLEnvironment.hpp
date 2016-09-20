#ifndef GLEnvironment_hpp
#define GLEnvironment_hpp

#include "FishEngine.hpp"

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>
// GLFW
#include <glfw/glfw3.h>

// glm
#define GLM_FORCE_LEFT_HANDED
#include <glm/glm.hpp>
#include <glm/vec3.hpp>                 // glm::vec3
#include <glm/vec4.hpp>                 // glm::vec4
#include <glm/mat4x4.hpp>               // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/constants.hpp>        // glm::pi
#include <glm/gtx/quaternion.hpp>       // glm::quat
#include <glm/gtc/type_ptr.hpp>         // glm::value_ptr

NAMESPACE_FISHENGINE_BEGIN

typedef glm::vec4 Vector4;
typedef glm::vec3 Vector3;
typedef glm::vec2 Vector2;
typedef glm::mat4 Matrix4x4;
typedef glm::mat3 float3x3;
typedef glm::quat Quaternion;

NAMESPACE_FISHENGINE_END

#ifdef _DEBUG
#define glCheckError() _checkOpenGLError(__FILE__,__LINE__)
#else
#define glCheckError()
#endif

void _checkOpenGLError(const char *file, int line);

#endif // GLEnvironment_hpp
