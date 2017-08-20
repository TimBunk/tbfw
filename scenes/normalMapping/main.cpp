#define GLEW_STATIC

#include <iostream>
#include <string>

#include <gl\glew.h>
#include <glfw3.h>
#include <glm-0.9.8.4\glm\glm.hpp>
#include <glm-0.9.8.4\glm\gtc\matrix_transform.hpp>
#include <glm-0.9.8.4\glm\gtc\type_ptr.hpp>

#include "shader.h"
#include "texture.h"
#include "camera.h"
#include "OBJloader.h"

const unsigned int screenWidth = 800, screenHeight = 600;

Shader* directionalLight;
Shader* directionalLightWithoutNormalMap;

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
void MouseCallback(GLFWwindow * window, double xPos, double yPos);
void ScrollCallback(GLFWwindow* window, double xOffset, double yOffset);

std::vector<Mesh*> dragon;
std::vector<Mesh*> bastetWithoutNormalMap;
float rotation = 0.0f;

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

	// textures
	Texture diff;
	diff = Tex::LoadTexture("textures//T_N-Bastet_DIF.png", TextureWrap::repeat, TextureFilter::linear, TextureType::diffuse);
	Texture spc;
	spc = Tex::LoadTexture("textures//T_N-Bastet_SPC.png", TextureWrap::repeat, TextureFilter::linear, TextureType::specular);
	Texture nrm;
	nrm = Tex::LoadTexture("textures//T_N-Bastet_NRM.png", TextureWrap::repeat, TextureFilter::linear, TextureType::normalMap);

	// with normal map
	dragon = OBJloader::LoadObject("assets//SM_Bastet.obj", true, true, true, true, true);
	dragon[0]->SetAmountOfTextures(1, 1, 0);
	dragon[0]->AddTexture(diff);
	dragon[0]->AddTexture(spc);
	dragon[0]->AddTexture(nrm);

	// without normal map
	/*bastetWithoutNormalMap = OBJloader::LoadObject("assets//SM_Bastet.obj", false, true, true, true, true);
	bastetWithoutNormalMap[0]->SetAmountOfTextures(1, 1, 0);
	bastetWithoutNormalMap[0]->AddTexture(diff);
	bastetWithoutNormalMap[0]->AddTexture(spc);*/

	// cyan plastic material. Found in this link http://devernay.free.fr/cours/opengl/materials.html
	/*directionalLight->SetVec3Float("material.ambient", glm::vec3(0.05375f, 0.05f, 0.06625f));
	directionalLight->SetVec3Float("material.diffuse", glm::vec3(0.18275f, 0.17f, 0.22525f));
	directionalLight->SetVec3Float("material.specular", glm::vec3(0.332741f, 0.328634f, 0.346435f));*/

	float vis = 1.0f;
	camera->SetPosition(glm::vec3(2.0f, 6.0f, 17.0f));
	// render/update loop
	while (!glfwWindowShouldClose(window)) {
		if (loop) {
			// input
			ProcessInput(window);

			// rendering commands
			glClearColor(0, 0.5, 0.5, 1);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			
			// draw bastet with normal map
			directionalLight->Use();

			glm::mat4 pos;
			pos = glm::translate(pos, glm::vec3(1.0f));
			pos = glm::rotate(pos, glm::radians(rotation), glm::vec3(0.0f, 1.0f, 0.0f));
			pos = glm::scale(pos, glm::vec3(0.1f));
			
			directionalLight->SetMatrix4("transform", pos);
			directionalLight->SetMatrix4("view", camera->GetViewMatrix());
			directionalLight->SetMatrix4("projection", camera->GetProjectionMatrix());

			directionalLight->SetVec3Float("viewPos", camera->GetPosition());

			directionalLight->SetVec3Float("light.direction", glm::vec3(-1.0f, 1.0f, -1.0f));
			directionalLight->SetVec3Float("light.ambient", glm::vec3(1.0f));
			directionalLight->SetVec3Float("light.diffuse", glm::vec3(1.0f));
			directionalLight->SetVec3Float("light.specular", glm::vec3(1.0f));
			directionalLight->SetFloat("material.shininess", 0.4f * 128.0f);
			directionalLight->SetFloat("visibility", vis);
			for (int i = 0; i < dragon.size(); i++) {
				dragon[i]->DrawTextures(directionalLight);
			}

			// draw bastet without normal map
			/*directionalLightWithoutNormalMap->Use();

			glm::mat4 pos2;
			pos2 = glm::translate(pos2, glm::vec3(6.0f, 1.0f, 1.0f));
			pos2 = glm::rotate(pos2, glm::radians(rotation), glm::vec3(0.0f, 1.0f, 0.0f));
			pos2 = glm::scale(pos2, glm::vec3(0.1f));

			directionalLightWithoutNormalMap->SetMatrix4("transform", pos2);
			directionalLightWithoutNormalMap->SetMatrix4("view", camera->GetViewMatrix());
			directionalLightWithoutNormalMap->SetMatrix4("projection", camera->GetProjectionMatrix());

			directionalLightWithoutNormalMap->SetVec3Float("viewPos", camera->GetPosition());

			directionalLightWithoutNormalMap->SetVec3Float("light.direction", glm::vec3(-1.0f, 1.0f, -1.0f));
			directionalLightWithoutNormalMap->SetVec3Float("light.ambient", glm::vec3(1.0f));
			directionalLightWithoutNormalMap->SetVec3Float("light.diffuse", glm::vec3(1.0f));
			directionalLightWithoutNormalMap->SetVec3Float("light.specular", glm::vec3(1.0f));
			directionalLightWithoutNormalMap->SetFloat("material.shininess", 0.4f * 128.0f);
			directionalLightWithoutNormalMap->SetFloat("visibility", vis);
			for (int i = 0; i < bastetWithoutNormalMap.size(); i++) {
				bastetWithoutNormalMap[i]->DrawTextures(directionalLightWithoutNormalMap);
			}*/


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
	delete dragon[0];
	//delete bastetWithoutNormalMap[0];
	delete directionalLight;
	delete directionalLightWithoutNormalMap;
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
		Tex::SaveImage(&screenWidth, &screenHeight);
	}
	else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {

	}
	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS && !reloadKey) {
		reloadKey = true;
	}
	else if (glfwGetKey(window, GLFW_KEY_R) == GLFW_RELEASE && reloadKey) {
		reloadKey = false;
	}
	if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS) {
		rotation += 0.5f;
	}
	else if (glfwGetKey(window, GLFW_KEY_T) == GLFW_RELEASE) {
		
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
	directionalLight = new Shader("shaders\\vertexWithNormalMap.vs", "shaders\\directionalLightWithNormalMap.fs");
	directionalLightWithoutNormalMap = new Shader("shaders\\vertex.vs", "shaders\\directionalLight.fs");
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