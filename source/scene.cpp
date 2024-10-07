#include <scene.hpp>
#include <iostream>
#include <globals.hpp>

SceneManager currentScene;

SceneManager::SceneManager() : currentScene(nullptr)
{

}

SceneManager::SceneManager(std::shared_ptr<Scene> scene)
{
	openScene(scene);
}

void SceneManager::openScene(std::shared_ptr<Scene> scene)
{
	if (currentScene)
	{
		currentScene->OnDisable();

		currentScene = scene;

		currentScene->Start();
	}
	else
	{
		currentScene = scene;
		if (currentScene) // Ensure currentScene is not nullptr
		{  
			currentScene->Start();  // Safe to call Start now
		}
		else 
		{
			std::cerr << "Error: currentScene is nullptr!" << std::endl;
		}
	}
}

void SceneManager::closeScene()
{
	if (currentScene)
	{
		currentScene->OnDisable();
		currentScene = nullptr;
	}
}

void SceneManager::Update()
{
	if (currentScene)
	{
		currentScene->Update();
	}
}

void SceneManager::FixedUpdate(double deltaTime)
{
	if (currentScene)
	{
		currentScene->FixedUpdate(deltaTime);
	}
}