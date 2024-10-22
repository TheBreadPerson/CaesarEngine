#pragma once
#include "scene.hpp"

class GameScene : public Scene
{
public:
	GameScene();
	void Start() override;
	void Update() override;
	void FixedUpdate(double deltaTime) override;
	void OnDisable() override;
};