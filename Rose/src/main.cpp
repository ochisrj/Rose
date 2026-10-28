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
#include "texture.h"
#include "camera.h"

#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

#include <stb_image.h>

const unsigned int WIDTH = 800;
const unsigned int HEIGHT = 600;

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = WIDTH / 2.0f;
float lastY = HEIGHT / 2.0f;
bool firstMouse = true;

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	(void)window;
	glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	ImGuiIO& io = ImGui::GetIO();
	if (io.WantCaptureMouse || glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) != GLFW_PRESS)
	{
		firstMouse = true;
		return;
	}
	if (firstMouse)
	{
		lastX = (float)xpos;
		lastY = (float)ypos;
		firstMouse = false;
	}
	float xoffset = (float)xpos - lastX;
	float yoffset = lastY - (float)ypos;
	lastX = (float)xpos;
	lastY = (float)ypos;
	camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	(void)window;
	(void)xoffset;
	if (ImGui::GetIO().WantCaptureMouse)
		return;
	camera.ProcessMouseScroll((float)yoffset);
}

void processInput(GLFWwindow* window, float deltaTime)
{
	if (ImGui::GetIO().WantCaptureKeyboard) 
		return;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		camera.ProcessKeyboard(DOWN, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		camera.ProcessKeyboard(UP, deltaTime);
}

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
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	gladLoadGL();
	glViewport(0, 0, WIDTH, HEIGHT);
	glEnable(GL_DEPTH_TEST);

	Shader ShaderProgram("shaders/shader.vert", "shaders/shader.frag");
	GLfloat vertices[] =
	{
		// positions          // colors           // texture coords
		 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
		 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left
	};

	GLuint indices[] =
	{
		0, 1, 3,
		1, 2, 3
	};

	VAO VAO1;
	VAO1.Bind();
	VBO VBO1(vertices, sizeof(vertices));
	EBO EBO1(indices, sizeof(indices));

	VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
	VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	VAO1.LinkAttrib(VBO1, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));

	VAO1.UnBind();
	VBO1.UnBind();
	EBO1.UnBind();

	GLuint myTexture = Loadtexture("20011.jpg");

	int modelLoc = glGetUniformLocation(ShaderProgram.ID, "model");
	int viewLoc = glGetUniformLocation(ShaderProgram.ID, "view");
	int projLoc = glGetUniformLocation(ShaderProgram.ID, "proj");
	int scaleLoc = glGetUniformLocation(ShaderProgram.ID, "scale");
	int texLoc = glGetUniformLocation(ShaderProgram.ID, "ourTexture");

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
	static bool autoRotate = true;
	static float rotateSpeed = 20.0f;
	static float color[4] = { 0.07f, 0.13f, 0.17f, 1.0f };

	float deltaTime = 0.0f;
	float lastFrame = 0.0f;

	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = (float)glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);
		processInput(window, deltaTime);

		int fbWidth, fbHeight;
		glfwGetFramebufferSize(window, &fbWidth, &fbHeight);
		float aspect = (fbHeight > 0) ? (float)fbWidth / (float)fbHeight : (float)WIDTH / HEIGHT;

		glClearColor(color[0], color[1], color[2], color[3]);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, myTexture);

		ShaderProgram.Activate();
		glUniform1f(scaleLoc, 1.0f);
		glUniform1i(texLoc, 0);

		glm::mat4 model = glm::mat4(1.0f);
		if (autoRotate)
			model = glm::rotate(model, glm::radians(rotateSpeed) * currentFrame, glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4 view = camera.GetViewMatrix();
		glm::mat4 proj = glm::perspective(glm::radians(camera.Zoom), aspect, 0.1f, 100.0f);

		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(proj));

		VAO1.Bind();
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		VAO1.UnBind();

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		{
			ImGui::Begin("Properties");
			ImGui::Text("Renderer: %s", renderer);
			ImGui::Text("GLFW Version: %d.%d.%d", major, minor, revision);
			ImGui::Text("GLSL Version: %s", glslVersion);
			ImGui::Text("Test on NVIM COLOR YAHOO");
				ImGui::Separator();
			if (ImGui::Checkbox("Wireframe Mode", &Wireframe_mode))
			{
				glPolygonMode(GL_FRONT_AND_BACK, Wireframe_mode ? GL_LINE : GL_FILL);
			}
			ImGui::ColorEdit4("Background Color", color, ImGuiColorEditFlags_PickerHueBar);
			ImGui::Separator();
			ImGui::Text("Camera: W/A/S/D + Q/E, hold Right-Mouse to look, wheel = zoom");
			ImGui::Text("Pos: %.2f %.2f %.2f", camera.Position.x, camera.Position.y, camera.Position.z);
			ImGui::Text("Yaw: %.1f Pitch: %.1f Zoom: %.1f", camera.Yaw, camera.Pitch, camera.Zoom);
			ImGui::Checkbox("Auto rotate", &autoRotate);
			if (ImGui::Button("Reset camera"))
				camera = Camera(glm::vec3(0.0f, 0.0f, 3.0f));
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
