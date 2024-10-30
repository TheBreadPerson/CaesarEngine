#include <editor.hpp>
#include <globals.hpp>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <light.hpp>
#include <scene.hpp>
#include <mesh.hpp>
#include <inputs.hpp>
#include <vector>
#include <string>

float color[3] = { 0, 0, 0 };
float intensity, radius;

int selectedItem = -1;
unsigned int entitySelected = 0;
Entity testEnt;
Entity* selectedEntity;
Light* activeLight;

Material material;
Mesh cubeMesh;
MeshRenderer m_renderer;

std::vector<const char*> components;
ImGuiWindowFlags immobile = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse;
ImGuiWindowFlags movable = ImGuiWindowFlags_NoResize;

void Editor::init()
{
	cubeMesh = graphics::loadModel("assets/models/Cube.glb");

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();

	// Setup Platform/Renderer backends
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 460");

	components.push_back("MeshRenderer");
	components.push_back("Light");
	components.push_back("Collider");
	components.push_back("Rigidbody");

	material.shader = 3;
	material.diffuse = vec3(1.0f);
	Entity* n = new Entity();
	m_renderer.mesh = &cubeMesh;
	n->AddComponent<MeshRenderer>(m_renderer);
	Instantiate(n);
}

Entity* CreateLight()
{
	Entity* lightEntity = new Entity();
	lightEntity->transform.position = player.entity.transform.position;
	lightEntity->AddComponent<Light>();
	lightEntity->GetComponent<Light>()->diffuse = vec3(1.0f);
	//currentScene.lights.push_back(lightEntity->GetComponent<Light>());
	Instantiate(lightEntity);
	return lightEntity;
}

void LightMenu()
{
	if (ImGui::Button("Create Light"))
	{
		std::cout << "Test button pressed" << std::endl;
		activeLight = CreateLight()->GetComponent<Light>();
	}
	if (ImGui::Button("Delete Light"))
	{
		// Remove activeLight from currentScene.lights

	}

	ImGui::ColorEdit3("Light Color", color);
	if (activeLight != nullptr) activeLight->diffuse = vec3(color[0], color[1], color[2]);

	ImGui::SliderFloat("Intensity", &intensity, 0.0f, 20.0f);
	if (activeLight != nullptr) activeLight->intensity = intensity;

}

void EntityCreateMenu()
{
	ImGui::Begin("Caesar Engine", nullptr, immobile);
	if (ImGui::Button("Create Entity"))
	{
		Entity* newEntity = new Entity();
		newEntity->transform.position = player.entity.transform.position;

		// Add the new entity to the scene
		Instantiate(newEntity);
		selectedEntity = newEntity;
		entitySelected = currentScene.entityList.size() - 1;
	}

	if (ImGui::Combo("Add Component", &selectedItem, components.data(), components.size()))
	{
		if (selectedItem == 0)
		{
			// Add MeshRenderer

			MeshRenderer newRenderer;

			newRenderer.mesh = &cubeMesh;
			newRenderer.material = material;
			selectedEntity->AddComponent<MeshRenderer>(newRenderer);
		}
		else if (selectedItem == 1)
		{
			// Add Light
			selectedEntity->AddComponent<Light>();
			currentScene.lights.push_back(selectedEntity->GetComponent<Light>());
		}
		else if (selectedItem == 2)
		{
			// Add Collider
			selectedEntity->AddComponent<Collider>();
			currentScene.entityList.back()->GetComponent<Collider>()->scale;
		}
		else if (selectedItem == 3)
		{
			// Add Rigidbody
			selectedEntity->AddComponent<Rigidbody>();
		}
	}

	ImGui::End();
}


void InspectorMenu()
{
	ImGui::Begin("Inspector", nullptr, immobile);
	ImVec2 windowSize = ImGui::GetWindowSize();
	ImGui::SetWindowPos(ImVec2(screen_width-windowSize.x, 0));

	if (!currentScene.entityList.empty())
	{
		if (Input::GetKeyDown(KeyCode::RIGHT))
		{
			entitySelected = (entitySelected + 1) % currentScene.entityList.size();
			std::cout << entitySelected << std::endl;
		}
		if (Input::GetKeyDown(KeyCode::LEFT))
		{
			if (entitySelected == 0) 
			{
				entitySelected = currentScene.entityList.size() - 1;
			}
			else 
			{
				entitySelected = (entitySelected - 1) % currentScene.entityList.size();
			}

			std::cout << entitySelected << std::endl;
		}
		selectedEntity = currentScene.entityList[entitySelected];

		// Display entity name as input field
		char name[256];
		strcpy(name, selectedEntity->name.c_str());
		if (ImGui::InputText("Name", name, 256))
		{
			selectedEntity->name = name;
		}
		ImGui::SameLine();
		
		auto it = std::find(currentScene.entityList.begin(), currentScene.entityList.end(), selectedEntity);
		int index = std::distance(currentScene.entityList.begin(), it);

		// Display the index
		ImGui::Text("ID: %d", index);

		ImGui::Text("Transform");
		ImGui::DragFloat3("Position", &selectedEntity->transform.position.x, 0.1f);
		ImGui::DragFloat3("Rotation", &selectedEntity->transform.rotation.x, 0.1f);
		ImGui::DragFloat3("Size", &selectedEntity->transform.scale.x, 0.1f);
		if (selectedEntity->HasComponent<MeshRenderer>())
		{
			MeshRenderer* meshRenderer = selectedEntity->GetComponent<MeshRenderer>();
			ImGui::Text("MeshRenderer");
			ImGui::Text("Material");
			//ImGui::InputText("Shader", (char*)meshRenderer->material.shader, 256);
			ImGui::ColorEdit3("Diffuse", &meshRenderer->material.diffuse.x);
			ImGui::ColorEdit3("Specular", &meshRenderer->material.specular.x);
			//ImGui::ColorEdit3("Ambient", meshRenderer->material.ambient);
			ImGui::SliderFloat("Shininess", &meshRenderer->material.shininess, 0.0f, 256.0f);
		}
		if (selectedEntity->HasComponent<Light>())
		{
			Light* light = selectedEntity->GetComponent<Light>();
			ImGui::Text("Light");
			ImGui::SameLine();
			if (ImGui::Button("Delete Light"))
			{
				currentScene.lights.erase(std::remove(currentScene.lights.begin(), currentScene.lights.end(), light), currentScene.lights.end());
				selectedEntity->RemoveComponent<Light>();
			}
			float lightColor[3] = { light->diffuse.r, light->diffuse.g, light->diffuse.b };
			ImGui::ColorEdit3("Light Color", lightColor);
			light->diffuse.r = lightColor[0];
			light->diffuse.g = lightColor[1];
			light->diffuse.b = lightColor[2];
			ImGui::SliderFloat("Intensity", &light->intensity, 0.0f, 20.0f);
			ImGui::SliderFloat("Radius", &light->radius, 0.0f, 100.0f);
		}

		if (selectedEntity->HasComponent<Collider>())
		{
			Collider* collider = selectedEntity->GetComponent<Collider>();
			ImGui::Text("Collider");
			ImGui::DragFloat3("Scale", &selectedEntity->GetComponent<Collider>()->scale.x, 0.1f);

			ImGui::DragFloat3("Offset", &selectedEntity->GetComponent<Collider>()->offset.x, 0.1f);
		}
	}
	ImGui::End();
}

void SceneMenu()
{
	ImGui::Begin("Scene");
	ImGui::Text("Count");
	ImGui::Text("Entities: %d", currentScene.entityList.size());
	ImGui::Text("Lights: %d", currentScene.lights.size());
	ImGui::Text("Lighting");
	ImGui::Text("Light");
	ImGui::ColorEdit3("Ambient Light", &currentScene.sceneLighting.ambient.x);
	ImGui::End();
}

void Editor::draw()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	
	EntityCreateMenu();
	InspectorMenu();
	SceneMenu();
}