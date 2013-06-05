#include "Projectile.h"

// Constructor
// Takes in position & direction
// Sets name and speed to nothing
Projectile::Projectile(glm::vec3 position, glm::vec3 direction)
{
	this->name = "";
	this->position = position;
	this->direction = direction;
	this->velocity = 0;

	this->alive = true;	// Starts alive

	createCube();	// Create geometry
}

// Constructor
// Takes in position, direction & speed
// Sets name to nothing
Projectile::Projectile(glm::vec3 position, glm::vec3 direction, float velocity)
{
	this->name = "";
	this->position = position;
	this->direction = direction;
	this->velocity = velocity;

	this->alive = true;	// Starts alive

	createCube();	// Create geometry
}

void Projectile::createCube()
{
	// Create vertex data
	glm::vec3 v1 = glm::vec3(position.x - 0.5f, position.y - 0.5f, position.z + 0.5f);
	glm::vec3 v2 = glm::vec3(position.x + 0.5f, position.y - 0.5f, position.z + 0.5f);
	glm::vec3 v3 = glm::vec3(position.x + 0.5f, position.y + 0.5f, position.z + 0.5f);
	glm::vec3 v4 = glm::vec3(position.x - 0.5f, position.y + 0.5f, position.z + 0.5f);
	glm::vec3 v5 = glm::vec3(position.x + 0.5f, position.y - 0.5f, position.z - 0.5f);
	glm::vec3 v6 = glm::vec3(position.x - 0.5f, position.y - 0.5f, position.z - 0.5f);
	glm::vec3 v7 = glm::vec3(position.x - 0.5f, position.y + 0.5f, position.z - 0.5f);
	glm::vec3 v8 = glm::vec3(position.x + 0.5f, position.y + 0.5f, position.z - 0.5f);

	// Push normals and vertices into their respective vectors
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

void Projectile::kill()
{
	if (alive == true)
	{
		alive = false;
	}
}

void Projectile::render()
{
	// Draw the projectile
	glBindVertexArray(vao);
	glDrawArrays(GL_QUADS, 0, vertices.size());
	glBindVertexArray(0);
}

void Projectile::update(float deltaTime)
{
	// Update projectile's position
	// Bit of a hack - all projectiles move in the same direction
	glm::vec3 newPosition = glm::vec3(position.x - velocity, position.y - (velocity/4), position.z - velocity);
	position = newPosition;

	// Redraw the cube
	vertices.clear();
	createCube();
}