#include <editor.hpp>
#include <globals.hpp>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <light.hpp>
#include <scene.hpp>
#include <mesh.hpp>

float color[3] = { 0, 0, 0 };
float intensity, radius;

#include <vector>
#include <string>

// ...

std::vector<const char*> components;
Mesh* cubeMesh;

void Editor::init()
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 460");

    components.push_back("MeshRenderer");
	components.push_back("Light");
	components.push_back("Collider");
	components.push_back("Rigidbody");

	cubeMesh = new Mesh(graphics::loadModel("assets/models/Cube.glb"));
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


Light* activeLight;

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

	ImGui::SliderFloat("Radius", &radius, 0.0f, 20.0f);
	if (activeLight != nullptr)
	{
		activeLight->linear = 1 / radius;
		activeLight->quadratic = 1 / (radius * radius);
	}
}

int selectedItem = -1;


void Editor::draw()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	ImGui::Begin("Caesar Engine");
	//LightMenu();
	if (ImGui::Button("Create Entity"))
	{
		Entity* newEntity = new Entity();
		newEntity->transform.position = player.entity.transform.position;
		Instantiate(newEntity);
	}

    if (ImGui::Combo("Add Component", &selectedItem, components.data(), components.size()))
	{
		if (selectedItem == 0)
		{
			// Add MeshRenderer
			MeshRenderer renderer;
			renderer.mesh = cubeMesh;
			renderer.material.shader = 3;
			currentScene.entityList.back()->AddComponent<MeshRenderer>(renderer);
		}
		else if (selectedItem == 1)
		{
			// Add Light
			currentScene.entityList.back()->AddComponent<Light>();
			currentScene.lights.push_back(currentScene.entityList.back()->GetComponent<Light>());
		}
		else if (selectedItem == 2)
		{
			// Add Collider
			currentScene.entityList.back()->AddComponent<Collider>();
			currentScene.entityList.back()->GetComponent<Collider>()->scale;
		}
		else if (selectedItem == 3)
		{
			// Add Rigidbody
			currentScene.entityList.back()->AddComponent<Rigidbody>();
		}
	}

	ImGui::End();

	ImGui::Begin("Inspector");
	if (!currentScene.entityList.empty())
	{
		Entity* selectedEntity = currentScene.entityList.back();
        // Display entity name as input field
		char name[256];
		strcpy(name, selectedEntity->name.c_str());
		if (ImGui::InputText("Name", name, 256))
		{
			selectedEntity->name = name;
		}
		ImGui::Text("Transform");
		ImGui::DragFloat3("Position", &selectedEntity->transform.position.x, 0.1f);
		ImGui::DragFloat3("Rotation", &selectedEntity->transform.rotation.x, 0.1f);
		ImGui::DragFloat3("Scale", &selectedEntity->transform.scale.x, 0.1f);
		if (selectedEntity->HasComponent<MeshRenderer>())
		{
			MeshRenderer* meshRenderer = selectedEntity->GetComponent<MeshRenderer>();
			ImGui::Text("MeshRenderer");
			ImGui::Text("Material");
			float diffuseColor[3] = {meshRenderer->material.diffuse.r, meshRenderer->material.diffuse.g, meshRenderer->material.diffuse.b};
			ImGui::ColorEdit3("Diffuse", diffuseColor);
			meshRenderer->material.diffuse.r = diffuseColor[0];
			meshRenderer->material.diffuse.g = diffuseColor[1];
			meshRenderer->material.diffuse.b = diffuseColor[2];
			/*ImGui::ColorEdit3("Specular", meshRenderer->material.specular);
			ImGui::ColorEdit3("Ambient", meshRenderer->material.ambient);*/
			ImGui::SliderFloat("Shininess", &meshRenderer->material.shininess, 0.0f, 256.0f);
		}
		if (selectedEntity->HasComponent<Light>())
		{
			Light* light = selectedEntity->GetComponent<Light>();
			ImGui::Text("Light");
			float lightColor[3] = { light->diffuse.r, light->diffuse.g, light->diffuse.b };
			ImGui::ColorEdit3("Diffuse", lightColor);
            light->diffuse.r = lightColor[0];
			light->diffuse.g = lightColor[1];
			light->diffuse.b = lightColor[2];
			ImGui::SliderFloat("Intensity", &light->intensity, 0.0f, 20.0f);
			ImGui::SliderFloat("Constant", &light->constant, 0.0f, 1.0f);
			ImGui::SliderFloat("Linear", &light->linear, 0.0f, 1.0f);
			ImGui::SliderFloat("Quadratic", &light->quadratic, 0.0f, 1.0f);
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