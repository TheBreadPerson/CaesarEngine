#include <glad/glad.h>
#include <iostream>
#include <GLFW/glfw3.h>
#include <memory>
#include "renderer.hpp"
#include "inputs.hpp"
#include "camera.hpp"
#include "globals.hpp"
#include "scene.hpp"
#include "gamescene.hpp"
#include "shader.hpp"
#include "entity.hpp"
#include <light.hpp>
#include <debug.hpp>
#include <stb_image.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>

using namespace glm;


unsigned int vertexShader;
unsigned int fragmentShader;

unsigned int shaderProgram;
unsigned int defaultShaderID;

unsigned int woodTexture, crateTexture;

int textureUnits[16] = { 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15 };

Shader defaultShader("assets/shaders/default.vert", "assets/shaders/default.frag");
Shader unlitShader("assets/shaders/unlit.vert", "assets/shaders/unlit.frag");
std::vector<Shader> shaders;



vec3 cubePositions[] =
{
	vec3(0.0f,  0.0f,  0.0f),
	vec3(2.0f,  2.0f, 0.0f),
	vec3(-2.0f, -2.0f, 0.0f),
	vec3(2.0f, -2.0f, 0.0f),
	vec3(-2.0f, 2.0f, 0.0f)
};


void Renderer::init()
{
	//glEnable(GL_FRAMEBUFFER_SRGB);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_COLOR);
	glEnable(GL_DEPTH_TEST);

	cam.transform.position = vec3(0.0f, 0.0f, 0.0f);


	for (Entity* object : currentScene.entityList)
	{
		if (!object->HasComponent<MeshRenderer>())
		{
			continue;
		}
		//setupMesh(*currentScene.skybox_ent->GetComponent<Mesh>());
		setupMesh(*object->GetComponent<MeshRenderer>()->mesh);
	}

	unlitShader.compile();
	defaultShader.compile();

	shaders.push_back(defaultShader);
	shaders.push_back(unlitShader);

	unlitShader.use();
	//defaultShaderID = unlitShader.ID;


	glClearColor(0.01f, 0.01f, 0.01f, 1.0f);
	glViewport(0, 0, screen_width, screen_height);
}

void Renderer::draw(GLFWwindow* window)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (Input::GetKeyDown(KeyCode::R))
	{
		reloadShaders();
	}

	float timeValue = static_cast<float>(glfwGetTime());

	mat4 projection = perspective(radians(cam.fov), ((float)screen_width / screen_height), 0.01f, 1000.0f);
	mat4 view = lookAt(cam.transform.position, cam.transform.position + cam.transform.forward, cam.transform.up);

	// IMGUI

	editor.draw();

	for (Shader shader : shaders)
	{
		shader.use();

		shader.setMat4("projection", projection);
		shader.setMat4("view", view);

		for (Entity* object : currentScene.entityList)
		{
			if (object->name == "bello")
			{
				std::cout << object->GetComponent<MeshRenderer>()->mesh << std::endl;
			}
			if (!object->HasComponent<MeshRenderer>())
			{
				continue;
			}
			MeshRenderer* objMesh = object->GetComponent<MeshRenderer>();
			//shader.setBool("useTexture", objMesh->hasTexture);
			shader.setVec4("objColor", vec4(object->GetComponent<MeshRenderer>()->material.diffuse, 1.0f));
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, objMesh->material.diffuseMap);
			shader.setInt("texture2d", 0);

			bool useTexture = (objMesh->material.diffuseMap != 0);
			shader.setBool("useTexture", useTexture);

			// Replace 3 with MeshRenderer material shader ID
			if (shader.ID != objMesh->material.shader)
			{
				continue;
			}

			shader.use();

			mat4 model = setupTransform(object->transform);
			shader.setMat4("model", model);

			// LIGHTS
			if (shader.ID == 6)
			{
				for (int i = 0; i < currentScene.lights.size(); i++)
				{
					std::string lightName = "pointLights[" + std::to_string(i) + "]";
					shader.setVec3(lightName + ".position", currentScene.lights[i]->entity->transform.position);
					shader.setVec3(lightName + ".ambient", currentScene.lights[i]->ambient);
					shader.setVec3(lightName + ".diffuse", currentScene.lights[i]->diffuse);
					shader.setVec3(lightName + ".specular", currentScene.lights[i]->specular);
					shader.setFloat(lightName + ".constant", currentScene.lights[i]->constant);
					shader.setFloat(lightName + ".linear", currentScene.lights[i]->linear);
					shader.setFloat(lightName + ".quadratic", currentScene.lights[i]->quadratic);
					shader.setFloat(lightName + ".intensity", currentScene.lights[i]->intensity);
				}
			}

			// MATERIAL HERE
			if (shader.ID == 6) {
				// Activate and bind the emission map
				glActiveTexture(GL_TEXTURE0); // Activate texture unit 0
				glBindTexture(GL_TEXTURE_2D, objMesh->material.emissionMap); // Bind emission map
				shader.setInt("material.emissionMap", 0); // Set the sampler uniform to texture unit 0
				shader.setVec3("material.emission", objMesh->material.emission);

				// Activate and bind the diffuse map
				glActiveTexture(GL_TEXTURE1); // Activate texture unit 1
				glBindTexture(GL_TEXTURE_2D, objMesh->material.diffuseMap); // Bind diffuse map
				shader.setInt("material.diffuseMap", 1); // Set the sampler uniform to texture unit 1
				shader.setVec3("material.diffuse", objMesh->material.diffuse);

				// Activate and bind the specular map
				glActiveTexture(GL_TEXTURE2); // Activate texture unit 2
				glBindTexture(GL_TEXTURE_2D, objMesh->material.specularMap); // Bind specular map
				shader.setInt("material.specularMap", 2); // Set the sampler uniform to texture unit 2
				shader.setVec3("material.specular", objMesh->material.specular);

				// Set shininess value
				shader.setFloat("material.shininess", objMesh->material.shininess);

				shader.setVec3("viewPos", cam.transform.position);
			}


			//SKYBOX
			if (object->GetID() == currentScene.skybox_ent->GetID()) glDepthMask(GL_FALSE);

			render(*object);

			glDepthMask(GL_TRUE);
		}
	}
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	glfwSwapBuffers(window);
}

void Renderer::reloadShaders()
{
	defaultShader.reload();
}

void Renderer::cleanup()
{
	shaders.clear();
}