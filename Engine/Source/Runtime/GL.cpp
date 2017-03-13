#include "GL.hpp"
#include "GLEnvironment.hpp"

namespace FishEngine
{
	void GL::Clear(bool clearDepth, bool clearColor, Color backgroundColor, float depth /*= 1.0f*/)
	{
		if (clearDepth)
		{
			glClearBufferfv(GL_COLOR, 0, backgroundColor.data());
		}
		if (clearDepth)
		{
			glClearBufferfv(GL_DEPTH, 0, &depth);
		}
	}

	void GL::Flush()
	{
		glFlush();
	}
}

