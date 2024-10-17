#include <gamescene.hpp>
#include <collision.hpp>
#include <transform.hpp>
#include <renderer.hpp>
#include <player.hpp>
#include <mesh.hpp>
#include <iostream>
#include <globals.hpp>
#include <physics.hpp>
#include <light.hpp>

Mesh earth;
Mesh skybox;

Mesh box1;
Mesh sphere_mesh;
Mesh defaultPlane;
Mesh monkey;

Entity player_ent;
Entity box1_ent;
Entity globe_ent;
Entity plane_ent;
Entity monkey_ent;
Entity light_ent;
Entity light2;

GameScene::GameScene()
{

}
void GameScene::Start()
{
	Mesh CubeMesh = mesh::loadModel("assets/models/Cube.glb");
	Mesh SphereMesh = mesh::loadModel("assets/models/Sphere.glb");

	player.entity.transform.scale = vec3(1.0f, 1.0f, 1.0f);
	player.entity.AddComponent<Rigidbody>();
	player.entity.AddComponent<Collider>();
	player.entity.GetComponent<Collider>()->scale = vec3(1.0f, 1.0f, 1.0f);
	player.entity.name = "Player";

	monkey = CubeMesh;
	monkey.texture_path = "assets/Crate.png";
	monkey_ent.transform.position = vec3(0.0f, 0.5f, 0.0f);
	monkey_ent.transform.scale = vec3(0.5f, 0.5f, 0.5f);
	monkey.shader = 6;
	monkey_ent.name = "Monkey";


	sphere_mesh = SphereMesh;
	sphere_mesh.shader = 6;
	sphere_mesh.material.specular = vec3(0.5f);
	sphere_mesh.material.shininess = 20.0f;
	globe_ent.transform.position = vec3(-5.0f, 2.0f, 0.0f);
	globe_ent.transform.rotation = vec3(180.0f, 0.0f, 0.0f);
	globe_ent.transform.scale = vec3(1.0f, 1.0f, 1.0f);
	sphere_mesh.texture_path = "assets/earth.png";
	globe_ent.name = "Earth";

	/*defaultPlane = Mesh::createPlane(1.0f);*/
	defaultPlane = mesh::loadModel("assets/models/Plane.glb");
	plane_ent.transform.position = vec3(0.0f, 0.0f, 0.0f);
	plane_ent.transform.scale = vec3(20.0f, 0.01f, 20.0f);
	defaultPlane.texture_path = "assets/realgrass.jpg";
	defaultPlane.shader = 6;
	plane_ent.name = "Floor";

	box1 = CubeMesh;
	box1.texture_path = "assets/dirt.png";
	box1.shader = 6;
	box1_ent.transform.position = vec3(5.0f, 15.0f, 0.0f);
	box1_ent.transform.scale = vec3(1.0f, 1.0f, 1.0f);

	box1_ent.name = "Box";

	Mesh lightMesh = SphereMesh;
	lightMesh.shader = 3;
	lightMesh.color = vec4(0.0f, 0.0f, 1.0f, 1.0f);
	light_ent.AddComponent<Light>();
	light_ent.GetComponent<Light>()->diffuse = lightMesh.color;
	light_ent.transform.position = vec3(5.0f, 15.0f, 0.0f);
	light_ent.name = "Light";
	//light_ent.AddComponent<Mesh>(lightMesh);

	Mesh light2Mesh = SphereMesh;
	light2Mesh.shader = 3;
	light2Mesh.color = vec4(1.0f, 0.0f, 0.0f, 1.0f);
	light2.AddComponent<Light>();
	light2.GetComponent<Light>()->diffuse = light2Mesh.color;
	light2.transform.position = vec3(-5.0f, 5.0f, 0.0f);
	light2.name = "Light 2";
	light2.AddComponent<Mesh>(light2Mesh);

	plane_ent.AddComponent<Mesh>(defaultPlane);
	box1_ent.AddComponent<Mesh>(box1);
	globe_ent.AddComponent<Mesh>(sphere_mesh);
	monkey_ent.AddComponent<Mesh>(monkey);

	plane_ent.AddComponent<Collider>();
	plane_ent.GetComponent<Collider>()->scale = vec3(40.0f, 0.001f, 40.0f);

	globe_ent.AddComponent<Collider>();
	globe_ent.GetComponent<Collider>()->scale = globe_ent.transform.scale;

	monkey_ent.AddComponent<Collider>();
	monkey_ent.GetComponent<Collider>()->scale = vec3(0.5f, 2.0f, 0.5f);

	box1_ent.AddComponent<Collider>();
	box1_ent.AddComponent<Rigidbody>();
	box1_ent.GetComponent<Collider>()->scale = vec3(1.0f, 2.0f, 1.0f);

	currentScene.skybox_ent = new Entity();
	currentScene.skybox_ent->name = "Sky";
	skybox = mesh::loadModel("assets/models/sphere.glb");
	skybox.texture_path = "assets/skytest.jpg";
	currentScene.skybox_ent->transform.position = vec3(0.0f, 0.0f, 0.0f);
	currentScene.skybox_ent->transform.rotation = vec3(0.0f, 0.0f, 0.0f);
	currentScene.skybox_ent->transform.scale = vec3(200.0f, 200.0f, 200.0f);
	skybox.shader = 3;

	currentScene.skybox_ent->AddComponent<Mesh>(skybox);

	currentScene.entityList.push_back(&player.entity);
	currentScene.entityList.push_back(&box1_ent);
	currentScene.entityList.push_back(&globe_ent);
	currentScene.entityList.push_back(&plane_ent);
	currentScene.entityList.push_back(&monkey_ent);

	currentScene.entityList.push_back(&light_ent);
	currentScene.entityList.push_back(&light2);
	currentScene.lights.push_back(light_ent.GetComponent<Light>());
	currentScene.lights.push_back(light2.GetComponent<Light>());

	//currentScene.entityList.push_back(*&currentScene.skybox_ent);
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
				CheckCollision(physics_object->GetComponent<Collider>(), object->GetComponent<Collider>());
			}
		}
	}
	cam.move();
	player.move();
	light_ent.transform.position = player.entity.transform.position;
	currentScene.skybox_ent->transform.position = player.entity.transform.position;

	globe_ent.transform.rotation.x += Time::deltaTime * 5.0f;
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