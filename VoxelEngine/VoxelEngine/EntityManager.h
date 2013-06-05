#pragma once

#ifndef ENTITYMANAGER_H
#define ENTITYMANAGER_H

#include <vector>
#include <map>
#include <string>

#include "Entity.h"

// Entity manager class
class EntityManager
{
private:
	std::vector<Entity*> entities;					// List of entities
	std::map<std::string, Entity*> namedEntities;	// List of named entities
public:
	EntityManager(void);	// Default constructor
	~EntityManager(void);	// Deconstructor

	void registerEntity(Entity* entity);	// Adds entity to list
	void removeEntity(Entity* entity);		// Removes entity from list

	void update(float deltaTime);	// Updates entities
	void render();					// Renders entities
};

#endif