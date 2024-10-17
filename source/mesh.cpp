#include <mesh.hpp>
#include <globals.hpp>

#include <stb_image.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <shader.hpp>

#include <fastgltf/glm_element_traits.hpp>
#include <fastgltf/core.hpp>
#include <fastgltf/types.hpp>

#include <iostream>

using namespace glm;

Mesh::Mesh(const std::vector<Vertex>& verts, const std::vector<unsigned int>& inds) : vertices(verts), indices(inds)
{
	shader = 6;
}

fastgltf::Parser parser;

Mesh mesh::loadModel(std::filesystem::path path)
{
	Mesh gltfMesh;

	constexpr auto gltfOptions = fastgltf::Options::None;

	if (!std::filesystem::exists(path)) {
		std::cerr << "Failed to find " << path << '\n';
		return gltfMesh;
	}

	

	auto gltfFile = fastgltf::MappedGltfFile::FromPath(path);
	if (!bool(gltfFile)) {
		std::cerr << "Failed to open glTF file: " << fastgltf::getErrorMessage(gltfFile.error()) << '\n';
		return gltfMesh;
	}

	auto asset = parser.loadGltfBinary(gltfFile.get(), path.parent_path(), gltfOptions);
	if (asset.error() != fastgltf::Error::None) {
		std::cerr << "Failed to load glTF: " << fastgltf::getErrorMessage(asset.error()) << '\n';
		return gltfMesh;
	}
	std::cout << "Successfully loaded " << path << '\n';
	fastgltf::Asset gltf = std::move(asset.get());

	std::vector<unsigned int> indices;
	std::vector<Vertex> vertices;
	for (fastgltf::Mesh& mesh : gltf.meshes)
	{
		for (auto&& p : mesh.primitives)
		{
			size_t initial_vtx = vertices.size();

			// load indexes
			{
				fastgltf::Accessor& indexaccessor = gltf.accessors[p.indicesAccessor.value()];
				indices.reserve(indices.size() + indexaccessor.count);

				fastgltf::iterateAccessor<std::uint32_t>(gltf, indexaccessor,
					[&](std::uint32_t idx) {
						indices.push_back(idx + initial_vtx);
					});
			}

			// load vertex positions
			{
				fastgltf::Accessor& posAccessor = gltf.accessors[p.findAttribute("POSITION")->accessorIndex];
				vertices.resize(vertices.size() + posAccessor.count);

				fastgltf::iterateAccessorWithIndex<glm::vec3>(gltf, posAccessor,
					[&](glm::vec3 v, size_t index)
					{
						Vertex newvtx;
						newvtx.position = v;
						newvtx.normal = { 1, 0, 0 };
						newvtx.texCoords = { 0, 0 };
						newvtx.color = glm::vec4{ 1.f };
						vertices[initial_vtx + index] = newvtx;
					});
			}

			// load vertex normals
			auto normals = p.findAttribute("NORMAL");
			if (normals != p.attributes.end())
			{

				fastgltf::iterateAccessorWithIndex<glm::vec3>(gltf, gltf.accessors[(*normals).accessorIndex],
					[&](glm::vec3 v, size_t index)
					{
						vertices[initial_vtx + index].normal = v;
					});
			}

			// load UVs
			auto uv = p.findAttribute("TEXCOORD_0");
			if (uv != p.attributes.end())
			{

				fastgltf::iterateAccessorWithIndex<glm::vec2>(gltf, gltf.accessors[(*uv).accessorIndex],
					[&](glm::vec2 v, size_t index)
					{
						vertices[initial_vtx + index].texCoords = v;
					});
			}

			// load vertex colors
			auto colors = p.findAttribute("COLOR_0");
			if (colors != p.attributes.end())
			{
				fastgltf::iterateAccessorWithIndex<glm::vec4>(gltf, gltf.accessors[(*colors).accessorIndex],
					[&](glm::vec4 v, size_t index)
					{
						vertices[initial_vtx + index].color = v;
					});
			}
		}
	}

	gltfMesh.vertices = vertices;
	gltfMesh.indices = indices;

	return gltfMesh;
}

Mesh Mesh::createCube(glm::mat4 model)
{
	float size = 1.0f;
	float halfSize = size / 2.0f;

	// Define the vertices of the cube, including position, color, and texture coordinates
	std::vector<Vertex> vertices = {
		// Front face
		{{-halfSize, -halfSize,  halfSize}, {1.0f, 0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}},  // Bottom-left
		{{ halfSize, -halfSize,  halfSize}, {0.0f, 1.0f, 0.0f, 1.0f}, {1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}},  // Bottom-right
		{{ halfSize,  halfSize,  halfSize}, {0.0f, 0.0f, 1.0f, 1.0f}, {1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}},  // Top-right
		{{-halfSize,  halfSize,  halfSize}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}, {0.0f, 0.0f, 1.0f}},  // Top-left

		// Back face
		{{-halfSize, -halfSize, -halfSize}, {1.0f, 0.0f, 1.0f, 1.0f}, {0.0f, 0.0f}, {0.0f, 0.0f, -1.0f}},  // Bottom-left
		{{ halfSize, -halfSize, -halfSize}, {0.0f, 1.0f, 1.0f, 1.0f}, {1.0f, 0.0f}, {0.0f, 0.0f, -1.0f}},  // Bottom-right
		{{ halfSize,  halfSize, -halfSize}, {1.0f, 1.0f, 0.0f, 1.0f}, {1.0f, 1.0f}, {0.0f, 0.0f, -1.0f}},  // Top-right
		{{-halfSize,  halfSize, -halfSize}, {0.5f, 0.5f, 0.5f, 1.0f}, {0.0f, 1.0f}, {0.0f, 0.0f, -1.0f}},  // Top-left

		// Left face
		{{-halfSize, -halfSize, -halfSize}, {0.8f, 0.3f, 0.2f, 1.0f}, {0.0f, 0.0f}, {-1.0f, 0.0f, 0.0f}},  // Bottom-left
		{{-halfSize, -halfSize,  halfSize}, {0.2f, 0.8f, 0.3f, 1.0f}, {1.0f, 0.0f}, {-1.0f, 0.0f, 0.0f}},  // Bottom-right
		{{-halfSize,  halfSize,  halfSize}, {0.3f, 0.2f, 0.8f, 1.0f}, {1.0f, 1.0f}, {-1.0f, 0.0f, 0.0f}},  // Top-right
		{{-halfSize,  halfSize, -halfSize}, {0.8f, 0.5f, 0.5f, 1.0f}, {0.0f, 1.0f}, {-1.0f, 0.0f, 0.0f}},  // Top-left

		// Right face
		{{ halfSize, -halfSize, -halfSize}, {0.9f, 0.2f, 0.2f, 1.0f}, {0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}},  // Bottom-left
		{{ halfSize, -halfSize,  halfSize}, {0.2f, 0.9f, 0.2f, 1.0f}, {1.0f, 0.0f}, {1.0f, 0.0f, 0.0f}},  // Bottom-right
		{{ halfSize,  halfSize,  halfSize}, {0.2f, 0.2f, 0.9f, 1.0f}, {1.0f, 1.0f}, {1.0f, 0.0f, 0.0f}},  // Top-right
		{{ halfSize,  halfSize, -halfSize}, {0.9f, 0.9f, 0.2f, 1.0f}, {0.0f, 1.0f}, {1.0f, 0.0f, 0.0f}},  // Top-left

		// Top face
		{{-halfSize,  halfSize, -halfSize}, {0.2f, 0.2f, 0.7f, 1.0f}, {0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}},  // Bottom-left
		{{ halfSize,  halfSize, -halfSize}, {0.7f, 0.2f, 0.2f, 1.0f}, {1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}},  // Bottom-right
		{{ halfSize,  halfSize,  halfSize}, {0.2f, 0.7f, 0.2f, 1.0f}, {1.0f, 1.0f}, {0.0f, 1.0f, 0.0f}},  // Top-right
		{{-halfSize,  halfSize,  halfSize}, {0.7f, 0.7f, 0.2f, 1.0f}, {0.0f, 1.0f}, {0.0f, 1.0f, 0.0f}},  // Top-left

		// Bottom face
		{{-halfSize, -halfSize, -halfSize}, {0.2f, 0.7f, 0.7f, 1.0f}, {0.0f, 0.0f}, {0.0f, -1.0f, 0.0f}},  // Bottom-left
		{{ halfSize, -halfSize, -halfSize}, {0.7f, 0.2f, 0.7f, 1.0f}, {1.0f, 0.0f}, {0.0f, -1.0f, 0.0f}},  // Bottom-right
		{{ halfSize, -halfSize,  halfSize}, {0.7f, 0.7f, 0.2f, 1.0f}, {1.0f, 1.0f}, {0.0f, -1.0f, 0.0f}},  // Top-right
		{{-halfSize, -halfSize,  halfSize}, {0.2f, 0.7f, 0.2f, 1.0f}, {0.0f, 1.0f}, {0.0f, -1.0f, 0.0f}},  // Top-left
	};

	// Define the indices for each face
	std::vector<unsigned int> indices = {
		// Front face
		0, 1, 2, 2, 3, 0,
		// Back face
		4, 5, 6, 6, 7, 4,
		// Left face
		8, 9, 10, 10, 11, 8,
		// Right face
		12, 13, 14, 14, 15, 12,
		// Top face
		16, 17, 18, 18, 19, 16,
		// Bottom face
		20, 21, 22, 22, 23, 20
	};

	return Mesh(vertices, indices);
}
Mesh Mesh::createPlane(glm::mat4 model)
{
	float size = 1.0f;
	float halfSize = size / 2.0f;

	// Define the vertices of the plane, including position, color, and texture coordinates
	std::vector<Vertex> vertices = {
		// Plane (XY-plane facing +Z direction)
		{{-halfSize, 0.0f, -halfSize}, {1.0f, 0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}},  // Bottom-left
		{{ halfSize, 0.0f, -halfSize}, {0.0f, 1.0f, 0.0f, 1.0f}, {30.0f, 0.0f}, {0.0f, 1.0f, 0.0f}},  // Bottom-right
		{{ halfSize, 0.0f, halfSize},  {0.0f, 0.0f, 1.0f, 1.0f}, {30.0f, 30.0f}, {0.0f, 1.0f, 0.0f}},  // Top-right
		{{-halfSize, 0.0f, halfSize},  {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 30.0f}, {0.0f, 1.0f, 0.0f}}   // Top-left
	};

	// Define the indices for the plane
	std::vector<unsigned int> indices = {
		// Two triangles for the plane
		0, 1, 2, 2, 3, 0
	};

	return Mesh(vertices, indices);
}

void Mesh::SetTexture(std::string path)
{
	texture = Mesh::LoadTexture(path.c_str());
}

unsigned int Mesh::LoadTexture(const char* filepath)
{
	unsigned int texture;

	// texture 
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


	// load image, create texture and generate mipmaps
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
	// The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform; replace it with your own image path.
	unsigned char* data = stbi_load(filepath, &width, &height, &nrChannels, 0);
	if (data)
	{
		std::cout << "Successfully loaded texture: " << filepath << std::endl;
		glTexImage2D(GL_TEXTURE_2D, 0, nrChannels > 3 ? GL_RGBA : GL_RGB, width, height, 0, nrChannels > 3 ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture: " << filepath << std::endl;
		data = new unsigned char[1 * 1 * 4];
		data[0] = 255;
		data[1] = 0;
		data[2] = 255;
		data[3] = 255;
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	stbi_image_free(data);
	return texture;
}