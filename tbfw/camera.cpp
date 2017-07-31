#include "camera.h"

Camera::Camera(GLFWwindow* window, float screenWidth, float screenHeight, float fov, float drawDistance)
{
	// Initialize all of the variables
	cameraPos = glm::vec3(0.0f, 0.0f, 0.0f);
	cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

	lastX = screenWidth / 2;
	lastY = screenHeight / 2;

	yaw = -90.0f;
	pitch = 0.0f;

	firstMouse = true;
	sensitivity = 0.05f;

	aspect = screenWidth / screenHeight;
	this->fov = fov;
	this->drawDistance = drawDistance;

	// Enable depth test
	glEnable(GL_DEPTH_TEST);
}

void Camera::MouseCallback(GLFWwindow * window, double xPos, double yPos)
{
	// initialize the camera rotation if used for first time
	if (firstMouse)
	{
		lastX = xPos;
		lastY = yPos;
		firstMouse = false;
	}
	// calculate the offset between the last mouse position and current mouse position
	float xOffset = xPos - lastX;
	float yOffset = lastY - yPos;
	lastX = xPos;
	lastY = yPos;

	// apply the sensitivity to the offset
	xOffset *= sensitivity;
	yOffset *= sensitivity;

	// add the offset to the yaw/pitch
	yaw += xOffset;
	pitch += yOffset;

	CalculateRotation(yaw, pitch);
}

void Camera::ScrollCallback(GLFWwindow * window, double xOffset, double yOffset)
{
	if (fov >= 1.0f && fov <= 45.0f) {
		fov -= yOffset;
	}
	if (fov < 1.0f) {
		fov = 1.0f;
	}
	if (fov > 45.0f) {
		fov = 45.0f;
	}
}

void Camera::ShowCursor(GLFWwindow* window, bool show)
{
	// If true show mouse else dont show
	if (show) {
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
	else {
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}
}

void Camera::SetSensitivity(float value)
{
	sensitivity = value;
}

void Camera::xPositionAdd(float value)
{
	cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * value;
}

void Camera::zPositionAdd(float value)
{
	cameraPos += value  * cameraFront;
}

void Camera::SetPosition(glm::vec3 position)
{
	cameraPos = position;
}

glm::vec3 Camera::GetPosition()
{
	return cameraPos;
}

glm::vec3 Camera::GetFront()
{
	return cameraFront;
}

glm::mat4 Camera::GetProjectionMatrix()
{
	projection = glm::perspective(glm::radians(fov), aspect, 0.1f, drawDistance);
	return projection;
}

glm::mat4 Camera::GetViewMatrix()
{
	view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
	return view;
}

void Camera::SetYaw(float value)
{
	CalculateRotation(value, this->pitch);
}

float Camera::GetYaw()
{
	return yaw;
}

void Camera::SetPitch(float value)
{
	CalculateRotation(this->yaw, value);
}

float Camera::GetPitch()
{
	return pitch;
}

glm::mat4 Camera::LookAt(glm::vec3 position)
{
	view = glm::lookAt(cameraPos, position, cameraUp);
	return view;
}

void Camera::CalculateRotation(float yaw, float pitch)
{
	// clamp the pitch between 89 and -89 degrees
	if (pitch > 89.0f) {
		pitch = 89.0f;
	}
	else if (pitch < -89.0f) {
		pitch = -89.0f;
	}
	
	// constrain yaw to be between 360 and -360 degrees
	yaw = std::fmod(yaw, (GLfloat)360.0f);

	// set the pitch and yaw values
	this->yaw = yaw;
	this->pitch = pitch;

	// apply the rotation to the camera
	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(front);
}
