#include "Camera.h"

void Camera::rotate(float deltaYaw, float deltaPitch)
{
	// Rotate based on deltaYaw
	rotation = glm::rotate(rotation, glm::degrees(deltaYaw), glm::vec3(0.0f, 1.0f, 0.0f));
	// Rotate based on deltaPitch
	rotation = glm::rotate(rotation, glm::degrees(deltaPitch), glm::vec3(1.0f, 0.0f, 0.0f));
	// Normalise rotation quaternion
	rotation = glm::normalize(rotation);
}

void Camera::move(const glm::vec3& translation)
{
	this->translation += translation;
}

void Camera::update(float deltaTime)
{
	glm::mat4 mat = glm::mat4_cast(rotation);		// Cast rotation quat as a matrix
	glm::vec4 vec(translation, 1.0f);
	vec = mat * vec;
	translation = glm::vec3(vec.x, vec.y, vec.z);
	position += translation;
	translation = glm::vec3(0.0f, 0.0f, 0.0f);

	glm::vec4 forward(0.0f, 0.0f, 1.0f, 1.0f);	// Define forward direction for camera
	forward = mat * forward;					// Adjust forward based on rotation matrix
	target = position + glm::vec3(forward.x, forward.y, forward.z);	// Set target as position + forward

	glm::vec4 tempUp(0.0f, 1.0f, 0.0f, 1.0f);		// Create temporary up direction
	tempUp = mat * tempUp;							// Apply rotation matrix to tempUp
	up = glm::vec3(tempUp.x, tempUp.y, tempUp.z);	// Set up direction to tempUp

	// Set view matrix
	view = glm::lookAt(position, target, up);
}