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
#include "src/classes/Texture.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "src/classes/vendor/imgui/imgui.h"
#include "src/classes/vendor/imgui/imgui_impl_glfw.h"
#include "src/classes/vendor/imgui/imgui_impl_opengl3.h"

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
	window = glfwCreateWindow(1280, 720, u8"Olá Mundo", NULL, NULL);
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
		   -0.5f, -0.5f,  0.0f,  0.0f,
			0.5f, -0.5f,  1.0f,  0.0f,
			0.5f,  0.5f,  1.0f,  1.0f,
		   -0.5f,  0.5f,  0.0f,  1.0f
		};

		unsigned indices[] = {
			0,1,2,
			2,3,0
		};

		GLCall(glEnable(GL_BLEND));
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

		VertexArray vertexArray;
		VertexBufferLayout vertexBufferLayout;
		VertexBuffer vertexBuffer(positions, 4 * 4 * sizeof(float));

		vertexBufferLayout.Push<float>(2);
		vertexBufferLayout.Push<float>(2);
		vertexArray.AddBuffer(vertexBuffer, vertexBufferLayout);
		IndexBuffer indexBuffer(indices, 6);

		//representa o espaço
		glm::mat4 projectionMatrix = glm::ortho(-4.0f, 4.0f, -2.25f, 2.25f, -1.0f, 1.0f);
		//representa o elemento de visualização (câmera)
		glm::mat4 viewMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(-2.0f, 0, 0));
		//representa a posição
		glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(2.0f, -1.0f, 0.0f));
		glm::mat4 mvp = projectionMatrix * viewMatrix * modelMatrix;

		Shader shader("src/res/shaders/basic.shader");
		shader.Bind();

		Texture texture("src/res/textures/Gadsden.png");
		texture.Bind(2);
		shader.SetUniform1i("u_Texture", 2);
		shader.SetUniformMat4f("u_MVP", mvp);

		Renderer renderer;

		shader.Unbind();
		vertexArray.Unbind();
		vertexBuffer.Unbind();
		vertexArray.Unbind();

		ImGui::CreateContext();
		ImGui::StyleColorsClassic();
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init("#version 130");

		bool show_demo_window = true;
		bool show_another_window = false;
		ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

		glm::vec3 translation;

		float g = 0.0f;
		float increment = .05f;
		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(window))
		{
			/* Render here */
			renderer.Clear();

			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

			modelMatrix = glm::translate(glm::mat4(1.0f), translation);
			mvp = projectionMatrix * viewMatrix * modelMatrix;

			shader.Bind();
			shader.SetUniform4f("u_Color", 0.0f, g, .25f, 1.0f);
			shader.SetUniformMat4f("u_MVP", mvp);

			vertexArray.Bind();
			indexBuffer.Bind();

			



			renderer.Draw(vertexArray, indexBuffer, shader);

			if (g > 1.0f)
				increment = -.05f;
			else if (g < 0.0f)
				increment = .05f;

			g += increment;

			{
				static float f = 0.0f;
				static int counter = 0;

				ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

				ImGui::Text(u8"Teste de Acentuação");               // Display some text (you can use a format strings too)
				ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
				ImGui::Checkbox("Another Window", &show_another_window);

				float translations[3];

				ImGui::InputFloat3("float", translations);

				translation.x = translations[0];
				translation.y = translations[1];
				translation.z = translations[2];

				
				ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
				ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

				if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
					counter++;
				ImGui::SameLine();
				ImGui::Text("counter = %d", counter);

				ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
				ImGui::End();
			}

			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

			/* Swap front and back buffers */
			glfwSwapBuffers(window);

			/* Poll for and process events */
			glfwPollEvents();
		}
	}

	ImGui_ImplOpenGL3_Shutdown();
	ImGui::DestroyContext();

	glfwTerminate();

	return 0;
}