#include "EntityManager.h"

EntityManager::EntityManager(void)
{
}

EntityManager::~EntityManager(void)
{
}

void EntityManager::registerEntity(Entity* entity)
{
	// Add to unnamed entity list
	entities.push_back(entity);
}

void EntityManager::removeEntity(Entity* entity)
{
	// Check unnamed entity list and remove
	for (int i = 0; i < entities.size(); ++i)
	{
		if (entities[i]->getName().compare(entity->getName()))
			entities.erase(entities.begin()+i);
	}
}

void EntityManager::update(float deltaTime)
{
	// Update all living entities
	for (int i = 0; i < entities.size(); ++i)
	{
		if (entities[i]->isAlive())
		{
			entities[i]->update(deltaTime);
		}
	}
}

void EntityManager::render()
{
	// Loop through all living entities and render
	for (int i = 0; i < entities.size(); ++i)
	{
		if (entities[i]->isAlive())
		{
			entities[i]->render();
		}
	}
}