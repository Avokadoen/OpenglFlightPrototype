#include "camera.hpp"

// Constructor with vectors
Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch) 
	: Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
{
	Position = position;
	WorldUp = up;
	Yaw = yaw;
	Pitch = pitch;
	updateCameraVectors();
	state = FREEMOVE;
}

// Constructor with scalar values
Camera::Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) 
	: Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
{
	Position = glm::vec3(posX, posY, posZ);
	WorldUp = glm::vec3(upX, upY, upZ);
	Yaw = yaw;
	Pitch = pitch;
	updateCameraVectors();
	state = FREEMOVE;
}

// Returns the view matrix calculated using Euler Angles and the LookAt Matrix
glm::mat4 Camera::GetViewMatrix()
{
	return glm::lookAt(Position, Position + Front, Up);
}

void Camera::setPosition(glm::vec3 position) {
	this->Position = position;
}

void Camera::rotateState() {
	if (state == FREEMOVE) {
		state = LOCK_TO_TARGET;
	}
	else if (state == LOCK_TO_TARGET) {
		state = RESTRICTED;
	}
	else if (state == RESTRICTED) {
		state = FREEMOVE;
	}
	else state = FREEMOVE;
}

// Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
void Camera::ProcessKeyboard(Camera_Movement direction, float deltaTime)
{
	if (state == FREEMOVE) {
		float velocity = MovementSpeed * deltaTime;
		if (direction == FORWARD)
			Position += Front * velocity;
		else if (direction == BACKWARD)
			Position -= Front * velocity;
		else if (direction == LEFT)
			Position -= Right * velocity;
		else if (direction == RIGHT)
			Position += Right * velocity;
	}
	else if (state == RESTRICTED) {
		float velocity = MovementSpeed * deltaTime;

		glm::vec3 x(1, 0, 0);
		glm::vec3 y(0, 1, 0);
		glm::vec3 z(0, 0, 1);

		if (direction == FORWARD)
			Position += x * velocity;
		else if (direction == BACKWARD)
			Position -= x * velocity;
		else if (direction == LEFT)
			Position -= z * velocity;
		else if (direction == RIGHT)
			Position += z * velocity;
		else if (direction == UP)
			Position += y * velocity;
		else if (direction == RIGHT)
			Position += y * velocity;

	}
	else {// nothing
	}
}

// Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
void Camera::ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch)
{
	xoffset *= MouseSensitivity;
	yoffset *= MouseSensitivity;

	Yaw += xoffset;
	Pitch += yoffset;

	// Make sure that when pitch is out of bounds, screen doesn't get flipped
	if (constrainPitch)
	{
		if (Pitch > 89.0f)
			Pitch = 89.0f;
		if (Pitch < -89.0f)
			Pitch = -89.0f;
	}

	// Update Front, Right and Up Vectors using the updated Euler angles
	updateCameraVectors();
}


// Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
void Camera::ProcessMouseScroll(float yoffset)
{
	if (Zoom >= 1.0f && Zoom <= 45.0f)
		Zoom -= yoffset;
	if (Zoom <= 1.0f)
		Zoom = 1.0f;
	if (Zoom >= 45.0f)
		Zoom = 45.0f;
}

// Calculates the front vector from the Camera's (updated) Euler Angles
void Camera::updateCameraVectors()
{
	// Calculate the new Front vector
	glm::vec3 front;
	front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	front.y = sin(glm::radians(Pitch));
	front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	Front = glm::normalize(front);
	// Also re-calculate the Right and Up vector
	Right = glm::normalize(glm::cross(Front, WorldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
	Up = glm::normalize(glm::cross(Right, Front));
}