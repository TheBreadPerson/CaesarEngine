#include "entity.hpp"
#include "globals.hpp"
#include <scene.hpp>

Entity::EntityID Entity::nextId = 0;

// Instantiate a new entity
void Instantiate(Entity* entity)
{
	currentScene.entityList.push_back(entity);
}