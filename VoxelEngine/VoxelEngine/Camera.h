#pragma once

#ifndef CAMERA_H
#define CAMERA_H

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtx\quaternion.hpp>

// Camera class
// Taken from Graphics Programming module material by Dr Kevin Chalmers
class Camera
{
protected:
	glm::vec3 position;		// Camera position
	glm::vec3 target;		// Camera target
	glm::vec3 up;			// Definition of "up" in relation to camera
	glm::mat4 view;			// View matrix
	glm::mat4 projection;	// Projection matrix

	// FPS-specific stuff
	glm::quat rotation;		// Camera rotation
	glm::vec3 translation;	// Camera translation
public:
	Camera() {}
	~Camera() {}
	
	// Get methods
	glm::vec3 getPosition()		const { return position; }
	glm::vec3 getTarget()		const { return target; }
	glm::vec3 getUp()			const { return up; }
	glm::mat4 getView()			const { return view; }
	glm::mat4 getProjection()	const { return projection; }

	// FPS specific
	glm::quat getRotation()		const { return rotation; }
	glm::vec3 getTranslation()	const { return translation; }

	// Set methods
	void setPosition(const glm::vec3& value)	{ position = value; }
	void setTarget(const glm::vec3& value)		{ target = value; }
	void setUp(const glm::vec3& value)			{ up = value; }
	void setProjection(float fov, float aspect, float nearPlane, float farPlane)
	{
		projection = glm::perspective(glm::degrees(fov), aspect, nearPlane, farPlane);
	}

	// FPS specific
	void setRotation(const glm::quat& value)	{ rotation = value; }

	void update(float deltaTime);					// Update method
	
	// Again, FPS specific
	void rotate(float deltaYaw, float deltaPitch);	// Rotate the camera
	void move(const glm::vec3& translation);		// Translate the camera
};

#endif