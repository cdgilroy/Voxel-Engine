#pragma once

#ifndef ENTITY_H
#define ENTITY_H

#include <glm\glm.hpp>
#include <string>

// Abstract Entity class
class Entity
{
protected:
	std::string name;	// Entity name
	glm::vec3 position;	// Entity position
	bool alive;			// Entity status (true/false)
public:
	Entity(void);	// Default constructor
	~Entity(void);	// Deconstructor

	// Get methods
	std::string getName()	{ return name; }
	glm::vec3 getPosition()	{ return position; }

	bool isAlive()	{ return alive; }

	virtual void update(float deltaTime);	// Update method
	virtual void render();					// Render method
};

#endif