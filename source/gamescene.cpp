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

MeshRenderer plane_renderer;
MeshRenderer box1_renderer;

Material defaultMaterial;
Material unlitMaterial;
Material lightMaterial;

Material grassMaterial;

Entity player_ent;
Entity box1_ent;
Entity globe_ent;
Entity plane_ent;
Entity monkey_ent;
Entity light_ent;
Entity light2;

Entity* testEntity;

GameScene::GameScene()
{

}
void GameScene::Start()
{
	Mesh CubeMesh = graphics::loadModel("assets/models/Cube.glb");
	Mesh SphereMesh = graphics::loadModel("assets/models/Sphere.glb");
	Mesh testMesh = graphics::loadModel("assets/models/Cube.glb");

	defaultMaterial.diffuse = vec3(1.0f);
	defaultMaterial.diffuseMap = graphics::LoadTexture("assets/container.png");
	defaultMaterial.specularMap = graphics::LoadTexture("assets/specular2.png");
	defaultMaterial.shader = 6;

	grassMaterial.diffuseMap = graphics::LoadTexture("assets/realgrass.jpg");
	grassMaterial.specularMap = graphics::LoadTexture("assets/grassspecular.png");
	grassMaterial.specular = vec3(2.0f);
	grassMaterial.shininess = 32.0f;
	grassMaterial.shader = 6;

	unlitMaterial.diffuse = vec3(1.0f);
	unlitMaterial.shader = 3;

	player.entity.transform.scale = vec3(1.0f, 1.0f, 1.0f);
	player.entity.AddComponent<Rigidbody>();
	player.entity.AddComponent<Collider>();
	player.entity.GetComponent<Collider>()->scale = vec3(1.0f, 1.0f, 1.0f);
	player.entity.name = "Player";

	//defaultPlane = graphics::loadModel("assets/models/Plane.glb");
	defaultPlane = CubeMesh;
	plane_ent.transform.position = vec3(0.0f, 0.0f, 0.0f);
	plane_ent.transform.scale = vec3(20.0f, 20.0f, 20.0f);
	plane_ent.name = "Floor";

	box1 = CubeMesh;
	box1_ent.transform.position = vec3(5.0f, 15.0f, 0.0f);
	box1_ent.transform.scale = vec3(1.0f, 1.0f, 1.0f);

	box1_ent.name = "Box";

	Mesh lightMesh = SphereMesh;
	light_ent.AddComponent<Light>();
	light_ent.GetComponent<Light>()->diffuse = vec3(1.0f);
	light_ent.transform.position = vec3(5.0f, 15.0f, 0.0f);
	light_ent.name = "Light";


	plane_renderer.mesh = &defaultPlane;
	
	plane_renderer.material = grassMaterial;
	box1_renderer.material = defaultMaterial;

	plane_ent.AddComponent<MeshRenderer>(plane_renderer);
	

	plane_ent.AddComponent<Collider>();
	plane_ent.GetComponent<Collider>()->scale = vec3(40.0f, 40.0f, 40.0f);

	box1_ent.AddComponent<Collider>();
	box1_ent.AddComponent<Rigidbody>();
	box1_ent.GetComponent<Collider>()->scale = vec3(1.0f, 2.0f, 1.0f);

	currentScene.skybox_ent = new Entity();
	currentScene.skybox_ent->name = "Sky";
	skybox = graphics::loadModel("assets/models/sphere.glb");

	Material skyboxMaterial = unlitMaterial;
	skyboxMaterial.diffuseMap = graphics::LoadTexture("assets/sky.jpg");

	currentScene.skybox_ent->AddComponent<MeshRenderer>();
	currentScene.skybox_ent->GetComponent<MeshRenderer>()->mesh = &skybox;
	currentScene.skybox_ent->GetComponent<MeshRenderer>()->material = skyboxMaterial;

	currentScene.skybox_ent->transform.position = vec3(0.0f, 0.0f, 0.0f);
	currentScene.skybox_ent->transform.rotation = vec3(0.0f, 0.0f, 0.0f);
	currentScene.skybox_ent->transform.scale = vec3(200.0f, 200.0f, 200.0f);

	currentScene.sceneLighting.ambient = vec3(0.1f);

	Instantiate(&player.entity);
	
	Instantiate(&plane_ent);
	box1_renderer.mesh = &box1;
	Instantiate(&box1_ent);
	box1_ent.AddComponent<MeshRenderer>(box1_renderer);
	Instantiate(&light_ent);
	currentScene.lights.push_back(light_ent.GetComponent<Light>());

	Instantiate(*&currentScene.skybox_ent);

	testEntity = new Entity();
	
	testEntity->AddComponent<MeshRenderer>();
	testEntity->GetComponent<MeshRenderer>()->mesh = &testMesh;
	testEntity->transform.position = vec3(0.0f, 15.0f, 0.0f);
	Instantiate(testEntity);
}

void GameScene::Update()
{
	
	if (!currentScene.entityList.empty())
	{
		for (Entity* object : currentScene.entityList)
		{
			if (!object->HasComponent<Collider>() || !object->GetComponent<Collider>()->enabled)
			{
				continue;
			}
			for (Entity* physics_object : currentScene.entityList)
			{
				if (!physics_object->HasComponent<Rigidbody>() || object == physics_object || !physics_object->GetComponent<Rigidbody>()->enabled)
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
		if (!physics_object->HasComponent<Rigidbody>() || !physics_object->GetComponent<Rigidbody>()->enabled)
		{
			continue;
		}
		PhysicsUpdate(*physics_object, deltaTime);
	}
}

void GameScene::OnDisable()
{

}