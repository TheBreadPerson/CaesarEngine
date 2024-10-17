// Include necessary headers
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <debug.hpp>

void Debug::DrawWireframeCube(const glm::vec3& position, const glm::vec3& scale)
{
    // Enable wireframe mode
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // Disable face culling to make sure all faces are drawn
    glDisable(GL_CULL_FACE);

    // Enable depth testing
    glEnable(GL_DEPTH_TEST);

    // Set line width to make the wireframe more visible
    glLineWidth(2.0f);

    // Define the 8 vertices of the cube
    float halfWidth = scale.x / 2.0f;
    float halfHeight = scale.y / 2.0f;
    float halfDepth = scale.z / 2.0f;

    glm::vec3 vertices[8] = {
        { position.x - halfWidth, position.y - halfHeight, position.z + halfDepth },
        { position.x + halfWidth, position.y - halfHeight, position.z + halfDepth },
        { position.x + halfWidth, position.y + halfHeight, position.z + halfDepth },
        { position.x - halfWidth, position.y + halfHeight, position.z + halfDepth },
        { position.x - halfWidth, position.y - halfHeight, position.z - halfDepth },
        { position.x + halfWidth, position.y - halfHeight, position.z - halfDepth },
        { position.x + halfWidth, position.y + halfHeight, position.z - halfDepth },
        { position.x - halfWidth, position.y + halfHeight, position.z - halfDepth }
    };

    // Define the indices for the lines of the cube
    unsigned int indices[] = {
        // Front face
        0, 1, 1, 2, 2, 3, 3, 0,
        // Back face
        4, 5, 5, 6, 6, 7, 7, 4,
        // Connect edges
        0, 4, 1, 5, 2, 6, 3, 7
    };

    // Create and bind the vertex array object (VAO)
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // Create and bind the vertex buffer object (VBO)
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Create and bind the element buffer object (EBO)
    unsigned int EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Define the vertex attributes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);


    // Draw the cube
    glDrawElements(GL_LINES, sizeof(indices) / sizeof(unsigned int), GL_UNSIGNED_INT, 0);

    // Unbind the VAO
    glBindVertexArray(0);

    // Clean up the VBO, VAO, and EBO
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteVertexArrays(1, &VAO);

    // Revert to solid fill mode
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    //glEnable(GL_CULL_FACE);
}
