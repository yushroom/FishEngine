#include <CGSupport.inc>

#ifdef VERTEX_SHADER
	#define USE_DEFAULT_VS
	#include <AppData.inc>
#endif

#ifdef FRAGMENT_SHADER
	#include <FragmentShaderShadow.inc>
	#define SURFACE_SHADER
#endif
