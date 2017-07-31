#ifndef CAMERA_H
#define CAMERA_H

#include <iostream>
#include <math.h>

#include <GL\glew.h>
#include <glfw3.h>
#include <glm-0.9.9.0\glm\glm.hpp>
#include <glm-0.9.9.0\glm\gtc\matrix_transform.hpp>

class Camera {

public:

	Camera(GLFWwindow* window, float screenWidth, float screenHeight, float fov, float drawDistance);

	void MouseCallback(GLFWwindow* window, double xPos, double yPos);
	void ScrollCallback(GLFWwindow * window, double xOffset, double yOffset);
	void ShowCursor(GLFWwindow* window, bool show);
	void SetSensitivity(float value);

	void xPositionAdd(float value);
	void zPositionAdd(float value);

	void SetPosition(glm::vec3 position);
	glm::vec3 GetPosition();

	glm::vec3 GetFront();

	glm::mat4 GetProjectionMatrix();
	glm::mat4 GetViewMatrix();

	// Yaw is the rotation around the x_axis
	void SetYaw(float value);
	float GetYaw();
	// Pitch is the rotation around the y_axis
	void SetPitch(float value);
	float GetPitch();

	// lookAt returns the view matrix but rotated towards a given position
	glm::mat4 LookAt(glm::vec3 position);

private:

	void CalculateRotation(float yaw, float pitch);

	glm::mat4 view;
	glm::mat4 projection;

	float aspect;
	float drawDistance;
	float fov;

	glm::vec3 cameraPos;
	glm::vec3 cameraFront;
	glm::vec3 cameraUp;

	bool firstMouse;
	float sensitivity;
	float lastX, lastY;

	// yaw is rotation aroud the x_axis, pitch around y_axis and roll and the z_axis
	float yaw, pitch;

};

#endif // !CAMERA_H