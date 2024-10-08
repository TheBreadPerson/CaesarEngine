#include <gamescene.hpp>
#include <collision.hpp>
#include <transform.hpp>
#include <renderer.hpp>
#include <player.hpp>
#include <mesh.hpp>
#include <iostream>
#include <globals.hpp>
#include <physics.hpp>

Mesh earth;
Mesh skybox;

Mesh box1;
Mesh defaultCube;
Mesh defaultPlane;
Mesh monkey;

Entity player_ent;
Entity box1_ent;
Entity cube_ent;
Entity plane_ent;
Entity monkey_ent;

GameScene::GameScene()
{

}
void GameScene::Start()
{
	std::cout << "Start GameScene!";

	player.entity.AddComponent<Rigidbody>();
	player.entity.AddComponent<Collider>();
	player.entity.name = "Player";

	monkey = mesh::loadModel("assets/models/monkey.glb");
	monkey.texture_path = "assets/Crate.png";
	monkey_ent.transform.position = vec3(0.0f, 1.0f, 0.0f);
	monkey_ent.transform.scale = vec3(2.0f, 2.0f, 2.0f);
	monkey.shader = 6;
	monkey_ent.name = "Monkey";


	defaultCube = Mesh::createCube(1.0f);
	cube_ent.transform.position = vec3(4.0f, 1.0f, 0.0f);
	cube_ent.transform.scale = vec3(1.0f, 1.0f, 1.0f);
	defaultCube.texture_path = "assets/earth.png";
	cube_ent.name = "Cube 1";

	defaultPlane = Mesh::createPlane(1.0f);
	plane_ent.transform.position = vec3(0.0f, 0.0f, 0.0f);
	plane_ent.transform.scale = vec3(200.0f, 1.0f, 200.0f);
	defaultPlane.texture_path = "assets/grass.png";
	plane_ent.name = "Floor";

	box1 = Mesh::createCube(1.0f);
	box1_ent.transform.position = vec3(-30.0f, 90.5f, 0.0f);
	box1_ent.transform.scale = vec3(2.5f, 2.5f, 2.5f);
	box1.texture_path = "assets/grass.png";
	box1_ent.name = "Box";

	plane_ent.AddComponent<Mesh>(defaultPlane);
	box1_ent.AddComponent<Mesh>(box1);
	cube_ent.AddComponent<Mesh>(defaultCube);
	monkey_ent.AddComponent<Mesh>(monkey);

	plane_ent.AddComponent<Collider>();
	plane_ent.GetComponent<Collider>()->transform = plane_ent.transform;

	monkey_ent.AddComponent<Collider>();
	monkey_ent.GetComponent<Collider>()->transform = monkey_ent.transform;

	box1_ent.AddComponent<Collider>();
	box1_ent.AddComponent<Rigidbody>();
	box1_ent.GetComponent<Collider>()->transform = box1_ent.transform;

	currentScene.skybox_ent = new Entity();
	skybox = mesh::loadModel("assets/models/sphere.glb");
	skybox.texture_path = "assets/sky.jpg";
	currentScene.skybox_ent->transform.position = vec3(0.0f, 0.0f, 0.0f);
	currentScene.skybox_ent->transform.scale = vec3(200.0f, 200.0f, 200.0f);
	skybox.shader = 3;

	currentScene.skybox_ent->AddComponent<Mesh>(skybox);

	currentScene.entityList.push_back(&player.entity);
	currentScene.entityList.push_back(&box1_ent);
	currentScene.entityList.push_back(&cube_ent);
	currentScene.entityList.push_back(&plane_ent);
	currentScene.entityList.push_back(&monkey_ent);

	currentScene.entityList.push_back(*&currentScene.skybox_ent);
}

void GameScene::Update()
{
	if (!currentScene.entityList.empty())
	{
		for (Entity* object : currentScene.entityList)
		{
			if (!object->HasComponent<Collider>())
			{
				continue;
			}
			for (Entity* physics_object : currentScene.entityList)
			{
				if (!physics_object->HasComponent<Rigidbody>() || object == physics_object)
				{
					continue;
				}
				std::cout << "Collision between " << object->name << " and " << physics_object->name << std::endl;
				CheckCollision(physics_object, object->GetComponent<Collider>());
			}
		}
	}
	cam.move();
	player.move();
	currentScene.skybox_ent->transform.position = player.entity.transform.position;
}

void GameScene::FixedUpdate(double deltaTime)
{
	for (Entity* physics_object : currentScene.entityList)
	{
		if (!physics_object->HasComponent<Rigidbody>())
		{
			continue;
		}
		PhysicsUpdate(*physics_object, deltaTime);
	}
}

void GameScene::OnDisable()
{

}