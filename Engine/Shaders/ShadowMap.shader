#include <ShaderVariables.inc>

#ifdef VERTEX_SHADER
	#include "AppData.inc"

	uniform mat4 MATRIX_LIGHT_MVP;

	void vs_main(AppData appdata)
	{
	    gl_Position = MATRIX_LIGHT_MVP * appdata.position;
	}
#endif

#ifdef FRAGMENT_SHADER
	void main()
	{
	    
	}
#endif