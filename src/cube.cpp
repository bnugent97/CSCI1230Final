#include "cube.h"

void Cube::updateParams(int param1) {
    m_vertexData = std::vector<float>();
    m_param1 = param1;
    setVertexData();
}

void Cube::makeTile(glm::vec3 topLeft,
                    glm::vec3 topRight,
                    glm::vec3 bottomLeft,
                    glm::vec3 bottomRight) {
    // Task 2: create a tile (i.e. 2 triangles) based on 4 given points.

    // Calculate the normal of the tile

    // Calculate the normal of the triangle
    glm::vec3 edge1 = bottomLeft - topLeft;
    glm::vec3 edge2 = bottomRight - topLeft;
    glm::vec3 normal = glm::normalize(glm::cross(edge1, edge2));


    // First triangle
    insertVec3(m_vertexData, topLeft);
    insertVec3(m_vertexData, normal);

    insertVec3(m_vertexData, bottomLeft);
    insertVec3(m_vertexData, normal);

    insertVec3(m_vertexData, bottomRight);
    insertVec3(m_vertexData, normal);

    // Second triangle
    insertVec3(m_vertexData, topLeft);
    insertVec3(m_vertexData, normal);

    insertVec3(m_vertexData, bottomRight);
    insertVec3(m_vertexData, normal);

    insertVec3(m_vertexData, topRight);
    insertVec3(m_vertexData, normal);

}

void Cube::makeFace(glm::vec3 topLeft,
                    glm::vec3 topRight,
                    glm::vec3 bottomLeft,
                    glm::vec3 bottomRight) {
    // Task 3: create a single side of the cube out of the 4


    // Calculate the number of divisions
    int divisions = std::max(1, m_param1); // Ensure at least one division

    // Calculate the size of each tessellated square
    glm::vec3 horizontalStep = (topRight - topLeft) / static_cast<float>(divisions);
    glm::vec3 verticalStep = (bottomLeft - topLeft) / static_cast<float>(divisions);

    // Loop through each tessellated square
    for (int i = 0; i < divisions; ++i) {
        for (int j = 0; j < divisions; ++j) {
            // Calculate the four corners of the tessellated square
            glm::vec3 localTopLeft = topLeft + horizontalStep * static_cast<float>(j) + verticalStep * static_cast<float>(i);
            glm::vec3 localTopRight = localTopLeft + horizontalStep;
            glm::vec3 localBottomLeft = localTopLeft + verticalStep;
            glm::vec3 localBottomRight = localTopRight + verticalStep;

            // Create the two triangles for this tessellated square
            makeTile(localTopLeft, localTopRight, localBottomLeft, localBottomRight);
        }
    }
}

void Cube::setVertexData() {
    // Uncomment these lines for Task 2, then comment them out for Task 3:

    //     makeTile(glm::vec3(-0.5f,  0.5f, 0.5f),
    //              glm::vec3( 0.5f,  0.5f, 0.5f),
    //              glm::vec3(-0.5f, -0.5f, 0.5f),
    //              glm::vec3( 0.5f, -0.5f, 0.5f));

    // Uncomment these lines for Task 3:

    //     makeFace(glm::vec3(-0.5f,  0.5f, 0.5f),
    //              glm::vec3( 0.5f,  0.5f, 0.5f),
    //              glm::vec3(-0.5f, -0.5f, 0.5f),
    //              glm::vec3( 0.5f, -0.5f, 0.5f));

    // Task 4: Use the makeFace() function to make all 6 sides of the cube

    // Front face
    makeFace(glm::vec3(-0.5f,  0.5f, 0.5f),
             glm::vec3( 0.5f,  0.5f, 0.5f),
             glm::vec3(-0.5f, -0.5f, 0.5f),
             glm::vec3( 0.5f, -0.5f, 0.5f));

    // Back face (vertices specified in reverse order)
    makeFace(glm::vec3( 0.5f,  0.5f, -0.5f),
             glm::vec3(-0.5f,  0.5f, -0.5f),
             glm::vec3( 0.5f, -0.5f, -0.5f),
             glm::vec3(-0.5f, -0.5f, -0.5f));

    // Left face (vertices specified counter-clockwise)
    makeFace(glm::vec3(-0.5f,  0.5f, -0.5f),
             glm::vec3(-0.5f,  0.5f,  0.5f),
             glm::vec3(-0.5f, -0.5f, -0.5f),
             glm::vec3(-0.5f, -0.5f,  0.5f));

    // Right face (vertices specified counter-clockwise)
    makeFace(glm::vec3( 0.5f,  0.5f,  0.5f),
             glm::vec3( 0.5f,  0.5f, -0.5f),
             glm::vec3( 0.5f, -0.5f,  0.5f),
             glm::vec3( 0.5f, -0.5f, -0.5f));

    // Top face (vertices specified counter-clockwise)
    makeFace(glm::vec3(-0.5f,  0.5f, -0.5f),
             glm::vec3( 0.5f,  0.5f, -0.5f),
             glm::vec3(-0.5f,  0.5f,  0.5f),
             glm::vec3( 0.5f,  0.5f,  0.5f));

    // Bottom face (vertices specified counter-clockwise)
    makeFace(glm::vec3(-0.5f, -0.5f,  0.5f),
             glm::vec3( 0.5f, -0.5f,  0.5f),
             glm::vec3(-0.5f, -0.5f, -0.5f),
             glm::vec3( 0.5f, -0.5f, -0.5f));



}

// Inserts a glm::vec3 into a vector of floats.
// This will come in handy if you want to take advantage of vectors to build your shape!
void Cube::insertVec3(std::vector<float> &data, glm::vec3 v) {
    data.push_back(v.x);
    data.push_back(v.y);
    data.push_back(v.z);
}
