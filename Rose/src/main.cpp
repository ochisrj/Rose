#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "shaderclass.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"

#include <string>
#ifdef _WIN32
#include <windows.h>
#include <intrin.h>
#include "imgui_markdown.h"
#endif

const unsigned int WIDTH = 800;
const unsigned int HEIGHT = 600;

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "YoutubeOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	gladLoadGL();
	glViewport(0, 0, WIDTH, HEIGHT);
	
	Shader ShaderProgram("shaders/shader.vert", "shaders/shader.frag");
	GLfloat vertices[] =
	{
		-0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f,     1.0f, 0.0f, 0.0f,
		 0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f,     0.0f, 1.0f, 0.0f,
		 0.0f,  0.5f * float(sqrt(3)) * 2 / 3, 0.0f, 0.0f, 0.0f, 1.0f
	};
	
	GLuint indices[] = { 0, 1, 2 };
	
	VAO VAO1;
	VAO1.Bind();
	VBO VBO1(vertices, sizeof(vertices));
	EBO EBO1(indices, sizeof(indices));
	VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 6 * sizeof(float), (void*)0);
	VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	VAO1.UnBind();
	VBO1.UnBind();
	
	EBO1.UnBind();
	GLuint uniID = glGetUniformLocation(ShaderProgram.ID, "scale");
	GLuint timeID = glGetUniformLocation(ShaderProgram.ID, "time");

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 460");
	
	int major, minor, revision;
	glfwGetVersion(&major, &minor, &revision);
	const GLubyte* glslVersion = glGetString(GL_SHADING_LANGUAGE_VERSION);
	const GLubyte* renderer = glGetString(GL_RENDERER);
	printf("Renderer : %s\n", renderer);
	printf("GLFW Version: %d.%d.%d\n", major, minor, revision);
	printf("GLSL Version: %s\n", glslVersion);
	
	static bool Wireframe_mode = false;
	static float color[4] = { 0.07f, 0.13f, 0.17f, 1.0f };
	
	while (!glfwWindowShouldClose(window))
	{
		if (glfwGetKey(window,GLFW_KEY_ESCAPE) == GLFW_PRESS)
		{
			glfwSetWindowShouldClose(window, true);
		}
		glClearColor(color[0], color[1], color[2], color[3]);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		ShaderProgram.Activate();
		glUniform1f(uniID, 0.5f);
		glUniform1f(timeID, (float)glfwGetTime());
		VAO1.Bind();
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		{
			ImGui::Begin("Properties");
			ImGui::Text("Renderer: %s",renderer);
			ImGui::Text("GLFW Version: %d.%d.%d",major,minor,revision);
			ImGui::Text("GLSL Version: %s", glslVersion);
			ImGui::Separator();
			if (ImGui::Checkbox("Wireframe Mode", &Wireframe_mode))
			{
				if (Wireframe_mode)
				{
					glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				}
				else
				{
					glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				}
			}
			ImGui::ColorEdit4("Background Color", color, ImGuiColorEditFlags_PickerHueBar);
			ImGui::End();
		}

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	VAO1.Delete();
	VBO1.Delete();
	EBO1.Delete();

	ShaderProgram.Delete();

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}
