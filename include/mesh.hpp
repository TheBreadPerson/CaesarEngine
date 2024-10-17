#pragma once
#include <glm/glm.hpp>
#include <vector>
#include <string>
#include "transform.hpp"
#include <filesystem>
#include <memory>


struct Vertex
{
	glm::vec3 position;
	glm::vec4 color;
	glm::vec2 texCoords;
	glm::vec3 normal;
};

class Mesh: public Component
{
public:
	unsigned int VBO, VAO, EBO, texture, shader;
	bool hasTexture = false;

	//Transform transform;
	std::string texture_path;
	glm::vec4 color;

	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;

	Mesh() : vertices(), indices(), VAO(0), VBO(0), EBO(0), texture(0), shader(0), color(1) {}

	Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices);


	/*void setupMesh();
	void draw();*/
	void SetTexture(std::string path);

	static Mesh createCube(glm::mat4 model);
	static Mesh createPlane(glm::mat4 model);
	static unsigned int LoadTexture(const char* filepath);
};

namespace mesh
{
	Mesh loadModel(std::filesystem::path path);
}