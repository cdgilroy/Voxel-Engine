#include "Chunk.h"

Chunk::Chunk(void)
{
	// Create blocks
	blocks = new Block**[CHUNK_SIZE];
	for (int i = 0; i < CHUNK_SIZE; ++i)
	{
		blocks[i] = new Block*[CHUNK_SIZE];
		for (int j = 0; j < CHUNK_SIZE; ++j)
		{
			blocks[i][j] = new Block[CHUNK_SIZE];
		}
	}

	needsUpdate = false;	// Set default state of needsUpdate flag

	setupCube();	// Arrange the chunk into a cube
}

Chunk::~Chunk(void)
{
	// Delete blocks
	for (int i = 0; i < CHUNK_SIZE; ++i)
	{
		for (int j = 0; j < CHUNK_SIZE; ++j)
		{
			delete [] blocks[i][j];
		}
		delete [] blocks[i];
	}
	delete [] blocks;
}

void Chunk::createCube(int x, int y, int z)
{
	// Create vertices
	glm::vec3 v1 = glm::vec3(x - 0.5f, y - 0.5f, z + 0.5f);
	glm::vec3 v2 = glm::vec3(x + 0.5f, y - 0.5f, z + 0.5f);
	glm::vec3 v3 = glm::vec3(x + 0.5f, y + 0.5f, z + 0.5f);
	glm::vec3 v4 = glm::vec3(x - 0.5f, y + 0.5f, z + 0.5f);
	glm::vec3 v5 = glm::vec3(x + 0.5f, y - 0.5f, z - 0.5f);
	glm::vec3 v6 = glm::vec3(x - 0.5f, y - 0.5f, z - 0.5f);
	glm::vec3 v7 = glm::vec3(x - 0.5f, y + 0.5f, z - 0.5f);
	glm::vec3 v8 = glm::vec3(x + 0.5f, y + 0.5f, z - 0.5f);

	// Push normals and vertices into their appropriate vectors
	// Front
	normals.push_back(glm::vec3(0.0f, 0.0f, -1.0f));
	vertices.push_back(v1);
	vertices.push_back(v2);
	vertices.push_back(v3);
	vertices.push_back(v4);

	// Back
	normals.push_back(glm::vec3(0.0f, 0.0f, 1.0f));
	vertices.push_back(v5);
	vertices.push_back(v6);
	vertices.push_back(v7);
	vertices.push_back(v8);

	// Right
	normals.push_back(glm::vec3(-1.0f, 0.0f, 0.0f));
	vertices.push_back(v2);
	vertices.push_back(v5);
	vertices.push_back(v8);
	vertices.push_back(v3);

	// Left
	normals.push_back(glm::vec3(1.0f, 0.0f, 0.0f));
	vertices.push_back(v6);
	vertices.push_back(v1);
	vertices.push_back(v4);
	vertices.push_back(v7);

	// Top
	normals.push_back(glm::vec3(0.0f, -1.0f, 0.0f));
	vertices.push_back(v4);
	vertices.push_back(v3);
	vertices.push_back(v8);
	vertices.push_back(v7);

	// Bottom
	normals.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
	vertices.push_back(v6);
	vertices.push_back(v5);
	vertices.push_back(v2);
	vertices.push_back(v1);
}

void Chunk::createMesh()
{
	for (int x = 0; x < CHUNK_SIZE; ++x)
	{
		for (int y = 0; y < CHUNK_SIZE; ++y)
		{
			for (int z = 0; z < CHUNK_SIZE; ++z)
			{
				if (blocks[x][y][z].isActive() == false)
				{
					// Don't create triangle data for inactive blocks
					continue;
				}
				createCube(x, y, z);
			}
		}
	}

	// Generate the vertex array and bind
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// Generate the vertex buffer and bind
	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	// Generate the normal buffer and bind
	glGenBuffers(1, &normalBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);
	
	// Unbind the vertex array
	glBindVertexArray(0);
}

glm::vec3 Chunk::getVoxelPosition(glm::vec3 aPosition)
{
	// Create voxel coordinates by casting aPosition components as integers
	glm::vec3 voxelPosition = glm::vec3((int)aPosition.x, (int)aPosition.y, (int)aPosition.z);

	return voxelPosition;
}

bool Chunk::isBlockExternal(int x, int y, int z)
{
	// If block has one or more non-active neighbours,
	// block is external

	bool external;
	int activeNeighbours = 0;

	// Check if out of bounds
	if (((0 < x) || (x < CHUNK_SIZE)) && ((0 < y) || (y < CHUNK_SIZE)) && ((0 < z) || (z < CHUNK_SIZE)))
	{
		external = false;
	}
	else
	{
		// Check left
		if (blocks[x+1][y][z].isActive())
			activeNeighbours++;
		// Check right
		if (blocks[x-1][y][z].isActive())
			activeNeighbours++;
		// Check front
		if (blocks[x][y+1][z].isActive())
			activeNeighbours++;
		// Check back
		if (blocks[x][y-1][z].isActive())
			activeNeighbours++;
		// Check up
		if (blocks[x][y][z+1].isActive())
			activeNeighbours++;
		// Check down
		if (blocks[x][y][z-1].isActive())
			activeNeighbours++;
	}

	if (activeNeighbours < 6)
		external = true;

	return external;
}

bool Chunk::isColliding(glm::vec3 aPosition)
{
	bool collides = false;

	// Convert position to voxel coordinates
	glm::vec3 posAsVoxel = getVoxelPosition(aPosition);

	// Loop through blocks, compare position against active blocks
	for (int x = 0; x < CHUNK_SIZE; ++x)
	{
		for (int y = 0; y < CHUNK_SIZE; ++y)
		{
			for (int z = 0; z < CHUNK_SIZE; ++z)
			{
				if (blocks[x][y][z].isActive() && posAsVoxel == glm::vec3(x,y,z))
				{
					collides = true;
					break;
				}
			}
		}
	}

	return collides;
}

void Chunk::removeBlock(int x, int y, int z)
{
	blocks[x][y][z].setActive(false);	// Set block to false
	needsUpdate = true;					// Request chunk updates
}

void Chunk::setEmpty()
{
	// Loop through all blocks, set them to false
	for (int x = 0; x < CHUNK_SIZE; ++x)
	{
		for (int y = 0; y < CHUNK_SIZE; ++y)
		{
			for (int z = 0; z < CHUNK_SIZE; ++z)
			{
				blocks[x][y][z].setActive(false);
			}
		}
	}
	needsUpdate = true;	// Request update
}

void Chunk::setupCube()
{
	// Loop through all blocks and make active
	for (int x = 0; x < CHUNK_SIZE; ++x)
	{
		for (int y = 0; y < CHUNK_SIZE; ++y)
		{
			for (int z = 0; z < CHUNK_SIZE; ++z)
			{
				blocks[x][y][z].setActive(true);
			}
		}
	}
	needsUpdate = true;	// Request update
}

void Chunk::setupSphere()
{
	for (int z = 0; z < CHUNK_SIZE; ++z)
	{
		for (int y = 0; y < CHUNK_SIZE; ++y)
		{
			for (int x = 0; x < CHUNK_SIZE; ++x)
			{
				if (sqrt((float) (x-CHUNK_SIZE/2)*(x-CHUNK_SIZE/2) + (y-CHUNK_SIZE/2)*(y-CHUNK_SIZE/2) + (z-CHUNK_SIZE/2)*(z-CHUNK_SIZE/2)) <= CHUNK_SIZE/2)
				{
					blocks[x][y][z].setActive(true);
				}
				else
				{
					blocks[x][y][z].setActive(false);
				}
			}
		}
	}
	needsUpdate = true;
}

void Chunk::render()
{
	glBindVertexArray(vao);						// Bind vertex array
	glDrawArrays(GL_QUADS, 0, vertices.size());	// Draw vertex array
	glBindVertexArray(0);						// Unbind vertex array
}

void Chunk::update(double deltaTime)
{
	if (needsUpdate)
	{
		vertices.clear();	// Empty vector
		createMesh();		// Rebuild mesh
		needsUpdate = false;
	}
}