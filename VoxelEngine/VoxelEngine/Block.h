#pragma once

#ifndef BLOCK_H
#define BLOCK_H

#include <GL\glew.h>
#include <GL\glfw.h>
#include <glm\glm.hpp>

// Basic block class
class Block
{
private:
	bool active;
public:
	Block(void);	// Default constructor
	~Block(void);	// Deconstructor

	bool isActive()			{ return active; }
	void setActive(bool a)	{ active = a; }
};

#endif