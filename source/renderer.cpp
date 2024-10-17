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

using namespace glm;


unsigned int vertexShader;
unsigned int fragmentShader;

unsigned int shaderProgram;
unsigned int defaultShaderID;

unsigned int woodTexture, crateTexture;



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
		if (!object->HasComponent<Mesh>())
		{
			continue;
		}
		//setupMesh(*currentScene.skybox_ent->GetComponent<Mesh>());
		setupMesh(*object->GetComponent<Mesh>());
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


	for (Shader shader : shaders)
	{
		shader.use();

		shader.setMat4("projection", projection);
		shader.setMat4("view", view);


		for (Entity* object : currentScene.entityList)
		{
			if (!object->HasComponent<Mesh>())
			{
				continue;
			}
			Mesh* objMesh = object->GetComponent<Mesh>();
			shader.setBool("useTexture", objMesh->hasTexture);
			shader.setVec4("objColor", objMesh->color);

			if (shader.ID != objMesh->shader)
			{
				continue;
			}
			
			//if (shader.ID != 3 && objMesh->shader != 3) continue;
			shader.use();

			mat4 model = setupTransform(object->transform);
			shader.setMat4("model", model);
			
			// LIGHTS
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
			}

			if (shader.ID == 6)
			{
				shader.setVec3("material.ambient", objMesh->material.ambient);
				shader.setVec3("material.diffuse", objMesh->material.diffuse);
				shader.setVec3("material.specular", objMesh->material.specular); // specular lighting doesn't have full effect on this object's material
				shader.setFloat("material.shininess", objMesh->material.shininess);
			}

			/*unlitShader.use();
			if (object->HasComponent<Collider>()) Debug::DrawWireframeCube(vec3(0.0f), object->GetComponent<Collider>()->scale);
			shader.use();*/

			//SKYBOX
			if (object->GetID() == currentScene.skybox_ent->GetID()) glDepthMask(GL_FALSE);

			render(*object);
			
			glDepthMask(GL_TRUE);
		}
	}
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