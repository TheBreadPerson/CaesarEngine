#include "entity.hpp"
#include "globals.hpp"
#include <scene.hpp>

Entity::EntityID Entity::nextId = 0;

// Instantiate a new entity
Entity* Instantiate(Entity* entity)
{
	if (entity->GetComponent<Light>())
	{
		currentScene.lights.push_back(entity->GetComponent<Light>());
	}
	currentScene.entityList.push_back(entity);
	return currentScene.entityList.back();
}