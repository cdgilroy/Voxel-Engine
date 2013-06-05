#pragma once

#ifndef CHUNK_H
#define CHUNK_H

#include <GL\glew.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <glm\gtc\quaternion.hpp>
#include <glm\gtx\constants.hpp>
#include <vector>

#include "Block.h"

// Chunk class
class Chunk
{
private:
	Block*** blocks;			// Block data for the chunk

	std::vector<glm::vec3> vertices;	// Vertex data
	std::vector<glm::vec3> normals;		// Normal data

	GLuint vertexBuffer;	// Vertex buffer
	GLuint normalBuffer;	// Normal buffer
	GLuint vao;				// Vertex array object
public:
	static const int CHUNK_SIZE = 32;	// Size of chunk (in blocks)

	bool needsUpdate;

	Chunk(void);	// Constructor
	~Chunk(void);	// Deconstructor

	void createCube(int x, int y, int z);	// Generates polygon cube
	void createMesh();						// Generates mesh

	glm::vec3 getVoxelPosition(glm::vec3 aPosition);	// Returns aPosition as voxel coordinates

	bool isBlockExternal(int x, int y, int z);	// Returns true if block is external
	bool isColliding(glm::vec3 aPosition);		// Returns true if aPosition collides with active block

	void removeBlock(int x, int y, int z);	// Remove specified block

	void setEmpty();		// Set all blocks in chunk to false

	void setupCube();		// Creates cube of voxels
	void setupSphere();		// Creates sphere of voxels

	void render();
	void update(double deltaTime);
};

#endif