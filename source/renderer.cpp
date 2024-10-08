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

using namespace glm;


unsigned int vertexShader;
unsigned int fragmentShader;

unsigned int shaderProgram;
unsigned int defaultShaderID;

unsigned int woodTexture, crateTexture;



Shader defaultShader("assets/shaders/default.vert", "assets/shaders/default.frag");
Shader unlitShader("assets/shaders/unlit.vert", "assets/shaders/default.frag");
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
	defaultShaderID = unlitShader.ID;


	glClearColor(0.01f, 0.01f, 0.01f, 1.0f);
	glViewport(0, 0, screen_width, screen_height);
}



void Renderer::draw(GLFWwindow* window)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	float timeValue = glfwGetTime();

	mat4 projection = perspective(radians(cam.fov), ((float)screen_width / screen_height), 0.01f, 1000.0f);
	mat4 view = lookAt(cam.transform.position, cam.transform.position + cam.transform.forward, cam.transform.up);

	mat4 transform = mat4(1.0f);


	for (Shader shader : shaders)
	{
		shader.setMat4("transform", transform);
		shader.setMat4("projection", projection);
		shader.setMat4("view", view);
		shader.setFloat("timeValue", timeValue);


		for (Entity* object : currentScene.entityList)
		{
			if (!object->HasComponent<Mesh>())
			{
				continue;
			}
			Mesh* objMesh = object->GetComponent<Mesh>();
			if (shader.ID != objMesh->shader)
			{
				continue;
			}
			shader.use();

			glBegin(GL_LINES);
			glVertex2f(.25, 0.25);
			glVertex2f(.75, .75);
			glEnd();

			mat4 model = setupTransform(object->transform);
			shader.setMat4("model", model);
			//SKYBOX
			if (object->GetID() == currentScene.skybox_ent->GetID()) glDepthMask(GL_FALSE);

			render(*object);

			glDepthMask(GL_TRUE);
		}
	}

	glfwSwapBuffers(window);
}
