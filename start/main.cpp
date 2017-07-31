#define GLEW_STATIC

#include <iostream>
#include <string>

#include <gl\glew.h>
#include <glfw3.h>
#include <glm-0.9.9.0\glm\glm.hpp>
#include <glm-0.9.9.0\glm\gtc\matrix_transform.hpp>
#include <glm-0.9.9.0\glm\gtc\type_ptr.hpp>

#include "shader.h"
#include "texture.h"
#include "camera.h"
#include "OBJloader.h"

const unsigned int screenWidth = 800, screenHeight = 600;

Shader* directionalLight;

float visibility = 0.2;
float positionX = 0;
bool reloadKey = false;
bool screenshotKey = false;

void FrameBufferSizeCallback(GLFWwindow* window, int width, int height);
void ProcessInput(GLFWwindow* window);
void CreateShaders();

float deltaTime = 0.0f;
float _deltaTime = 0.0f;
float lastFrame = 0.0f;
float totalTime = 0.0f;
int fpsCount = 0;
float fps = 1.0f / 60.0f;
bool loop = true;
void CalculateFrameRate();

float num = 0;

Camera* camera;
Mesh* mesh;
void MouseCallback(GLFWwindow * window, double xPos, double yPos);
void ScrollCallback(GLFWwindow* window, double xOffset, double yOffset);

int main() {
	// initialize and configure glfw
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	// glfw window creation
	GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, "tbfw", NULL, NULL);
	if (window == NULL) {
		std::cout << "glfw window creation failed!" << std::endl;
		glfwTerminate();
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, FrameBufferSizeCallback);

	// set this to true so glew knows to use a modern approache to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;

	// initialize glew
	if (GLEW_OK != glewInit()) {
		std::cout << "Error in initializing glew" << std::endl;
		glfwTerminate();
	}
	CreateShaders();

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	camera = new Camera(window, 800.0f, 600.0f, 45.0f, 100.0f);
	camera->ShowCursor(window, false);
	glfwSetCursorPosCallback(window, MouseCallback);
	glfwSetScrollCallback(window, ScrollCallback);
	mesh = new Mesh("assets//JapaneseFarmHouse7.obj");
	mesh->WireframeMode(false);
	mesh->SetAmountOfTextures(1, 1, 0);
	mesh->AddTexture(Tex::LoadTexture("textures//JapaneseFarmHouseUV.png", TextureWrap::repeat, TextureFilter::linear, TextureType::diffuse));
	mesh->AddTexture(Tex::LoadTexture("textures//JapaneseFarmHouseUV.png", TextureWrap::repeat, TextureFilter::linear, TextureType::specular));
	//mesh->AddTexture(tex::LoadTexture("textures//JapaneseFarmHouseUV.png", tex::TextureWrap::repeat, tex::TextureFilter::linear, tex::TextureType::emission));
	
	directionalLight->Use();
	// cyan plastic material. Found in this link http://devernay.free.fr/cours/opengl/materials.html

	float vis = 0.0f;
	float rotation = 0.0f;
	// render/update loop
	while (!glfwWindowShouldClose(window)) {
		if (loop) {
			// input
			ProcessInput(window);

			// rendering commands
			glClearColor(0, 0.5, 0.5, 1);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			// draw first triangle

			directionalLight->Use();

			glm::mat4 pos;
			pos = glm::translate(pos, glm::vec3(1.0f));
			//rotation += 0.5f;
			pos = glm::rotate(pos, glm::radians(rotation), glm::vec3(0.0f, 1.0f, 0.0f));
			directionalLight->SetMatrix4("transform", pos);
			directionalLight->SetMatrix4("view", camera->GetViewMatrix());
			directionalLight->SetMatrix4("projection", camera->GetProjectionMatrix());

			directionalLight->SetVec3Float("viewPos", camera->GetPosition());
			directionalLight->SetFloat("visibility", vis);

			directionalLight->SetVec3Float("light.direction", glm::vec3(-1.0f, 1.0f, -1.0f));
			directionalLight->SetVec3Float("light.ambient", glm::vec3(0.329412f, 0.223529f, 0.027451f));
			directionalLight->SetVec3Float("light.diffuse", glm::vec3(0.780392f, 0.568627f, 0.113725f));
			directionalLight->SetVec3Float("light.specular", glm::vec3(0.992157f, 0.941176f, 0.807843f));
			directionalLight->SetFloat("material.shininess", 0.4f * 128.0f);
			mesh->Draw(directionalLight);

			// reset the deltaTime
			deltaTime = 0;

			// check/call events and swap the buffers
			glfwSwapBuffers(window);
			glfwPollEvents();
		}
		// calculate the frameRate
		CalculateFrameRate();
	}

	// clear all allocated glfw resources
	delete mesh;
	delete directionalLight;
	delete camera;
	glfwDestroyWindow(window);
	glfwTerminate();
	std::cout << "Program succeeded" << std::endl;
	return 0;
}

void FrameBufferSizeCallback(GLFWwindow * window, int width, int height)
{
	// this makes sure the screen width/height always fits even if the window is resized
	glViewport(0, 0, width, height);
}

void ProcessInput(GLFWwindow * window)
{
	// if escaped is pressed exit the program
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		
	}
	else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		
	}
	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS && !reloadKey) {
		reloadKey = true;
	}
	else if (glfwGetKey(window, GLFW_KEY_R) == GLFW_RELEASE && reloadKey) {
		reloadKey = false;
	}
	if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS && !screenshotKey) {
		screenshotKey = true;
		Tex::SaveImage(&screenWidth, &screenHeight);
	}
	else if (glfwGetKey(window, GLFW_KEY_T) == GLFW_RELEASE && screenshotKey) {
		screenshotKey = false;
	}

	float speed = 2.0f*deltaTime;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		camera->zPositionAdd(speed);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		camera->zPositionAdd(speed * -1);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		camera->xPositionAdd(speed * -1);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		camera->xPositionAdd(speed);
	}
}

void CreateShaders()
{
	directionalLight = new Shader("shaders\\vertex.vs", "shaders\\directionalLight.fs");
}

void CalculateFrameRate()
{
	float currentFrame = glfwGetTime();
	_deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;

	deltaTime += _deltaTime;

	totalTime += _deltaTime;
	if (totalTime - fpsCount * fps >= fps) {
		fpsCount++;
		loop = true;
	}
	else {
		loop = false;
	}
	if (totalTime >= 1.0f) {
		totalTime -= 1.0f;
		std::cout << "fps: " << fpsCount << std::endl;
		fpsCount = 0;
	}
}

void MouseCallback(GLFWwindow * window, double xPos, double yPos)
{
	camera->MouseCallback(window, xPos, yPos);
}

void ScrollCallback(GLFWwindow * window, double xOffset, double yOffset)
{
	camera->ScrollCallback(window, xOffset, yOffset);
}
