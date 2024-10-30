#pragma once

#include <memory>
#include <globals.hpp>
#include <vector>
#include <light.hpp>

class Scene
{
public:
	virtual void OnDisable() = 0;
	virtual void Start() = 0;
	virtual void Update() {};
	virtual void FixedUpdate(double deltaTime) {};
};

class SceneManager
{
public:
	SceneManager();
	SceneManager(std::shared_ptr<Scene> scene);
	std::vector<Entity*> entityList;
	std::vector<Light*> lights;
	Entity* skybox_ent;
	SceneLight sceneLighting;
	void openScene(std::shared_ptr<Scene> scene);
	void closeScene();
	void Update();
	void FixedUpdate(double deltaTime);
private:
	std::shared_ptr<Scene> currentScene;
};

extern SceneManager currentScene;