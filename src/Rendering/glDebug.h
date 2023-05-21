#pragma once

#include <iomanip>

#include <GL/glew.h>

#include "../utilities/log.h"

#ifdef _WIN32
	#define ASSERT(x) \
		if(!(x)) __debugbreak();
#else
	#define ASSERT(x) \
		if(!(x)) __builtintrap();
#endif

#define GlCall(x)	\
		GlClearError(); \
		x;              \
		ASSERT(GLLogCall(#x, __FILE__, __LINE__))

static void GlClearError()
{
	while (glGetError() != GL_NO_ERROR)
	{

	};
}

static bool GLLogCall(const char* function, const char* file, int line)
{
	while (GLenum error = glGetError())
	{

		std::stringstream es;

		es << std::hex << error;

		std::stringstream ss;

		ss << "OpenGL error (" << es.str() << "): " << function;
		Log::crit(ss.str().c_str(), file, line);
		return false;
	}
	return true;
}

