#pragma once

#ifndef PROJECTILE_H
#define PROJECTILE_H

#include <GL\glew.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <vector>

#include "Entity.h"

// Projectile class - inherits from Entity
// Controlled by EntityManager
class Projectile : public Entity
{
private:
	std::vector<glm::vec3> vertices;	// Vertex data
	std::vector<glm::vec3> normals;		// Normal data
	
	GLuint vertexBuffer;	// Vertex buffer
	GLuint normalBuffer;	// Normal buffer
	GLuint vao;				// Vertex array object
	
	glm::vec3 direction;	// Projectile's direction
	float velocity;			// Projectile's speed
public:
	Projectile(void);	// Default constructor
	Projectile(glm::vec3 position, glm::vec3 direction);
	Projectile(glm::vec3 position, glm::vec3 direction, float velocity);
	~Projectile(void)	// Deconstructor
	{
		return;
	}

	// Get methods
	glm::vec3 getPosition()		{ return position; }
	glm::vec3 getDirection()	{ return direction; }

	// Set methods
	void setAlive(bool aBool)			{ alive = aBool; }
	void setPosition(glm::vec3 newPos)	{ position = newPos; }
	void setDirection(glm::vec3 newDir)	{ direction = newDir; }

	void createCube();	// Creates projectile's geometry
	void kill();		// Does what it says on the tin - kills the projectile

	// Update & render methods - called by EntityManager
	void render();
	void update(float deltaTime);
};

#endif