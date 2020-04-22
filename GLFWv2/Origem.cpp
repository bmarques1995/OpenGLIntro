#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "src/classes/ErrorHandler.h"
#include <fstream>
#include <sstream>
#include "src/classes/VertexBuffer.h"
#include "src/classes/IndexBuffer.h"
#include "src/classes/VertexArray.h"
#include "src/classes/Shader.h"
#include "src/classes/Renderer.h"

int main(void)
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(720, 720, u8"Olá Mundo", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	glfwGetWindowAttrib(window, GLFW_ACCUM_ALPHA_BITS);

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	glfwSwapInterval(1);

	if (glewInit() != GLEW_OK)
		std::cout << "GLEW Error" << std::endl;

	std::cout << glGetString(GL_VERSION) << std::endl;
	{
		float positions[] = {
		   -0.5f, -0.5f,
			0.5f, -0.5f,
			0.5f,  0.5f,
		   -0.5f,  0.5f
		};

		unsigned indices[] = {
			0,1,2,
			2,3,0
		};

		VertexArray vertexArray;
		VertexBufferLayout vertexBufferLayout;
		VertexBuffer vertexBuffer(positions, 4 * 2 * sizeof(float));
		vertexBufferLayout.Push<float>(2);
		vertexArray.AddBuffer(vertexBuffer, vertexBufferLayout);
		IndexBuffer indexBuffer(indices, 6);

		Shader shader("src/res/shaders/basic.shader");
		shader.Bind();

		Renderer renderer;

		shader.Unbind();
		vertexArray.Unbind();
		vertexBuffer.Unbind();
		vertexArray.Unbind();

		float g = 0.0f;
		float increment = .05f;
		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(window))
		{
			/* Render here */
			renderer.Clear();

			shader.Bind();
			shader.SetUniform4f("u_Color", 0.0f, g, .25f, 1.0f);

			vertexArray.Bind();
			indexBuffer.Bind();

			renderer.Draw(vertexArray, indexBuffer, shader);
			
			if (g > 1.0f)
				increment = -.05f;
			else if (g < 0.0f)
				increment = .05f;

			g += increment;

			/* Swap front and back buffers */
			glfwSwapBuffers(window);

			/* Poll for and process events */
			glfwPollEvents();
		}
	}

	glfwTerminate();

	return 0;
}