#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cstdlib>
#include <iostream>

#define ASSERT(x) if (!(x)) __debugbreak()

#define GLCall(x) GLClearError();\
	x;\
	ASSERT(GLLogCall(#x, __FILE__, __LINE__, glGetError()))

static void GLClearError() 
{
	while (glGetError() != GL_NO_ERROR);
}

static bool GLLogCall
(
	const char * function, 
	const char * file, 
	size_t line,
	GLenum error
) 
{
	while (GLenum error = glGetError()) 
	{
		std::cout << 
			"[Erro OpenGL] (" << error << ") em " << 
			function << 
			" " << 
			file << 
			":" << 
			line << 
			std::endl;
		return false;
	}
	return true;
}