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

const unsigned int screenWidth = 800, screenHeight = 600;

Shader* lightingShader;
Shader* lampShader;
Shader* directionalLight;
Shader* pointLight;
Shader* spotLight;

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

GLfloat vertices[] = {
	// vertex coord		// normal coord		// texture coord
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
	0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
	0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
	0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
	0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
	0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
	0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
	-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

	0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
	0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
	0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
	0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
	0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
	0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
	0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
	0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
	0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
	0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
	0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
	0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
};

glm::vec3 positions[]{
	glm::vec3(4.0f,  0.0f,  0.0f),
	glm::vec3(3.0f, 0.0f, 0.0f),
	glm::vec3(2.0f, 0.0f, 0.0f),
	glm::vec3(1.0f, 1.0f, 0.0f),
	glm::vec3(0.0f, 2.0f, 0.0f),
	glm::vec3(0.0f, 3.0f, 0.0f),
	glm::vec3(0.0f, 4.0f, 0.0f),
	glm::vec3(0.0f, 5.0f, 0.0f),
	glm::vec3(1.0f, 6.0f, 0.0f),
	glm::vec3(2.0f, 7.0f, 0.0f),
	glm::vec3(3.0f, 7.0f, 0.0f),
	glm::vec3(4.0f, 7.0f, 0.0f),// c 12

	glm::vec3(9.0f, 0.0f, 0.0f),
	glm::vec3(9.0f, 1.0f, 0.0f),
	glm::vec3(9.0f, 2.0f, 0.0f),
	glm::vec3(9.0f, 3.0f, 0.0f),
	glm::vec3(9.0f, 4.0f, 0.0f),
	glm::vec3(9.0f, 5.0f, 0.0f),
	glm::vec3(9.0f, 6.0f, 0.0f),
	glm::vec3(6.0f, 3.0f, 0.0f),
	glm::vec3(7.0f, 3.0f, 0.0f),
	glm::vec3(8.0f, 3.0f, 0.0f),
	glm::vec3(10.0f, 3.0f, 0.0f),
	glm::vec3(11.0f, 3.0f, 0.0f),
	glm::vec3(12.0f, 3.0f, 0.0f),// + 13

	glm::vec3(17.0f, 0.0f, 0.0f),
	glm::vec3(17.0f, 1.0f, 0.0f),
	glm::vec3(17.0f, 2.0f, 0.0f),
	glm::vec3(17.0f, 3.0f, 0.0f),
	glm::vec3(17.0f, 4.0f, 0.0f),
	glm::vec3(17.0f, 5.0f, 0.0f),
	glm::vec3(17.0f, 6.0f, 0.0f),
	glm::vec3(14.0f, 3.0f, 0.0f),
	glm::vec3(15.0f, 3.0f, 0.0f),
	glm::vec3(16.0f, 3.0f, 0.0f),
	glm::vec3(18.0f, 3.0f, 0.0f),
	glm::vec3(19.0f, 3.0f, 0.0f),
	glm::vec3(20.0f, 3.0f, 0.0f),// + 13    total of 38
};

glm::vec3 pointLightPositions[] = {
	glm::vec3(4.0f, 0.0f, 2.0f),
	glm::vec3(4.0f, 7.0f, 2.0f),
	glm::vec3(0.0f, 3.5f, 2.0f),// c 3

	glm::vec3(9.0f, 0.0f,  2.0f),
	glm::vec3(9.0f, 3.0f, 2.0f),
	glm::vec3(9.0f, 6.0f, 2.0f),
	glm::vec3(6.0f, 3.0f, 2.0f),
	glm::vec3(12.0f, 3.0f, 2.0f),// + 5

	glm::vec3(17.0f, 0.0f,  2.0f),
	glm::vec3(17.0f, 3.0f, 2.0f),
	glm::vec3(17.0f, 6.0f, 2.0f),
	glm::vec3(14.0f, 3.0f, 2.0f),
	glm::vec3(20.0f, 3.0f, 2.0f)// + 5    total = 13
};

glm::vec3 lightColors[] = {
	glm::vec3(2.0f, 0.7f, 1.3f),
	glm::vec3(1.0f, 1.4f, 1.0f),
	glm::vec3(0.1f, 0.1f, 0.1f),// c 3

	glm::vec3(0.3f, 0.6f,  0.9f),
	glm::vec3(0.9f, 0.6f, 0.3f),
	glm::vec3(1.0f, 1.0f, 0.1f),
	glm::vec3(0.1f, 0.8f, 2.0f),
	glm::vec3(1.5f, 0.4f, 1.5f),// + 5

	glm::vec3(0.7f, 0.1f,  0.7f),
	glm::vec3(1.2f, 1.2f, 0.0f),
	glm::vec3(0.0f, 2.0f, 0.4f),
	glm::vec3(1.5f, 1.1f, 0.2f),
	glm::vec3(1.0f, 0.5f, 1.8f)// + 5    total = 13
};

Camera* camera;
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
	Texture diffuseMap = Tex::LoadTexture("textures//container2.png", TextureWrap::repeat, TextureFilter::linear, TextureType::diffuse);
	Texture specularMap = Tex::LoadTexture("textures//container2_specular.png", TextureWrap::repeat, TextureFilter::linear, TextureType::specular);
	Texture emissionMap = Tex::LoadTexture("textures//matrix.jpg", TextureWrap::repeat, TextureFilter::linear, TextureType::emission);


	GLuint VBO[3], VAO[3];
	// bind vertex array and vertex buffer and then configure vertex attributes
	glGenVertexArrays(3, VAO);
	glGenBuffers(3, VBO);
	// first triangle setup
	glBindVertexArray(VAO[0]);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)(6* sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
	

	glBindVertexArray(VAO[1]);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)0);
	glEnableVertexAttribArray(0);


	camera = new Camera(window, 800.0f, 600.0f, 45.0f, 100.0f);
	camera->ShowCursor(window, false);
	glfwSetCursorPosCallback(window, MouseCallback);
	glfwSetScrollCallback(window, ScrollCallback);


	lightingShader->Use();
	// cyan plastic material. Found in this link http://devernay.free.fr/cours/opengl/materials.html
	lightingShader->SetInt("material.diffuse", diffuseMap.id);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, diffuseMap.id);
	lightingShader->SetInt("material.specular", specularMap.id);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, specularMap.id);
	lightingShader->SetInt("material.emission", emissionMap.id);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, emissionMap.id);
	lightingShader->SetFloat("material.shininess", 0.4f * 128.0f);
	lightingShader->SetUnsignedInt("material.diffuseMaps", 1);
	lightingShader->SetUnsignedInt("material.specularMaps", 1);
	lightingShader->SetUnsignedInt("material.emissionMaps", 1);

	camera->SetPosition(glm::vec3(9.0f, 3.75f, 22.0f));
	float vis = 0.0f;
	// render/update loop
	while (!glfwWindowShouldClose(window)) {
		if (loop) {
			// input
			ProcessInput(window);

			// rendering commands
			glClearColor(0, 0.5, 0.5, 1);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			// draw first triangle

			lightingShader->Use();
			glBindVertexArray(VAO[0]);
			lightingShader->SetMatrix4("view", camera->GetViewMatrix());
			lightingShader->SetMatrix4("projection", camera->GetProjectionMatrix());
			lightingShader->SetVec3Float("viewPos", camera->GetPosition());

			// normal cube with color
			float time = glfwGetTime();
			glm::vec3 lightColor = glm::vec3(1.0f);
			glm::vec3 diffuseColor = lightColor * glm::vec3(0.8f);
			glm::vec3 ambientColor = diffuseColor * glm::vec3(0.15f);

			lightingShader->SetUnsignedInt("amountOfPointLights", 13);
			for (int k = 0; k < 13; k++) {
				lightingShader->Use();
				glBindVertexArray(VAO[0]);
				lightColor.x = sin(time * lightColors[k].x);
				lightColor.y = sin(time * lightColors[k].y);
				lightColor.z = sin(time * lightColors[k].z);
				diffuseColor = lightColor * glm::vec3(0.8f);
				ambientColor = diffuseColor * glm::vec3(0.15f);

				std::string name;
				name = "pointLight[" + std::to_string(k) + "].position";
				lightingShader->SetVec3Float(name.c_str(), pointLightPositions[k]);
				name = "pointLight[" + std::to_string(k) + "].ambient";
				lightingShader->SetVec3Float(name.c_str(), ambientColor);
				name = "pointLight[" + std::to_string(k) + "].diffuse";
				lightingShader->SetVec3Float(name.c_str(), diffuseColor);
				name = "pointLight[" + std::to_string(k) + "].specular";
				lightingShader->SetVec3Float(name.c_str(), glm::vec3(1.0f));
				name = "pointLight[" + std::to_string(k) + "].constant";
				lightingShader->SetFloat(name.c_str(), 1.0f);
				name = "pointLight[" + std::to_string(k) + "].linear";
				lightingShader->SetFloat(name.c_str(), 0.09f);
				name = "pointLight[" + std::to_string(k) + "].quadratic";
				lightingShader->SetFloat(name.c_str(), 0.032f);

				// lamp object
				lampShader->Use();
				glBindVertexArray(VAO[1]);
				lampShader->SetMatrix4("view", camera->GetViewMatrix());
				lampShader->SetMatrix4("projection", camera->GetProjectionMatrix());
				lampShader->SetVec3Float("col", lightColor);
				glm::mat4 pos;
				pos = glm::translate(pos, pointLightPositions[k]);
				pos = glm::scale(pos, glm::vec3(0.2f));
				lampShader->SetMatrix4("transform", pos);
				glDrawArrays(GL_TRIANGLES, 0, 36);
			}
			lightingShader->Use();
			glBindVertexArray(VAO[0]);

			lightingShader->SetVec3Float("dirLight.direction", glm::vec3(-0.2f, -1.0f, -0.3f));
			lightingShader->SetVec3Float("dirLight.ambient", glm::vec3(0.8f * 0.15f));
			lightingShader->SetVec3Float("dirLight.diffuse", glm::vec3(0.8f));
			lightingShader->SetVec3Float("dirLight.specular", glm::vec3(1.0f));

			//vis += 0.05f;
			float visFinal = sin(vis);
			lightingShader->SetFloat("visibility", visFinal);

			float radius = 4;
			float xPos = 3.0f;
			float zPos = 3.0f;
			//xPos = sin(glfwGetTime()) * radius;
			//zPos = cos(glfwGetTime()) * radius;
			glm::vec3 lightPos = glm::vec3(xPos, 2.5f, zPos);

			lightingShader->SetVec3Float("spotLight.position", camera->GetPosition());
			lightingShader->SetVec3Float("spotLight.direction", camera->GetFront());
			lightingShader->SetFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
			lightingShader->SetFloat("spotLight.outerCutOff", glm::cos(glm::radians(17.5f)));
			lightingShader->SetFloat("spotLight.constant", 1.0f);
			lightingShader->SetFloat("spotLight.linear", 0.09f);
			lightingShader->SetFloat("spotLight.quadratic", 0.032f);
			lightingShader->SetVec3Float("spotLight.ambient", glm::vec3(0.8f * 0.15f));
			lightingShader->SetVec3Float("spotLight.diffuse", glm::vec3(0.8f));
			lightingShader->SetVec3Float("spotLight.specular", glm::vec3(1.0f));

			glm::vec3 _dir;
			//_dir = camera->GetFront();
			//std::cout << "CamDir: x: " << _dir.x << " y: " << _dir.y << " z: " << _dir.z << std::endl;

			for (int i = 0; i < 38; i++) {
				glm::mat4 pos;
				pos = glm::translate(pos, positions[i]);
				float angle = 20.0f * i;
				pos = glm::rotate(pos, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
				lightingShader->SetMatrix4("transform", pos);
				glDrawArrays(GL_TRIANGLES, 0, 36);
			}

			// reset the deltaTime
			deltaTime = 0;

			// check/call events and swap the buffers
			glfwSwapBuffers(window);
			glfwPollEvents();
		}
		// calculate the frameRate
		CalculateFrameRate();
	}

	glDeleteVertexArrays(3, VAO);
	glDeleteBuffers(3, VBO);

	// clear all allocated glfw resources
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
		if (visibility < 1) {
			visibility += 0.01;
		}
	}
	else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		if (visibility > 0) {
			visibility -= 0.01;
		}
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
	lightingShader = new Shader("shaders\\vertex.vs", "shaders\\lightingShader.fs");
	lampShader = new Shader("shaders\\vertex.vs", "shaders\\lampShader.fs");
	directionalLight = new Shader("shaders\\vertex.vs", "shaders\\directionalLight.fs");
	pointLight = new Shader("shaders\\vertex.vs", "shaders\\pointLight.fs");
	spotLight = new Shader("shaders\\vertex.vs", "shaders\\spotLight.fs");
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