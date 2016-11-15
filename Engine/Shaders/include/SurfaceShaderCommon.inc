#include <CGSupport.inc>

#ifdef VERTEX_SHADER
	#define USE_DEFAULT_VS
	#include "AppData.inc"
#endif

#ifdef FRAGMENT_SHADER
	#include <FragmentShaderShadow.inc>
	//uniform sampler2D _MainTex;
	#define SURFACE_SHADER
#endif


