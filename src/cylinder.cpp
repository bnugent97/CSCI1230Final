#include "Cylinder.h"

void Cylinder::updateParams(int param1, int param2) {
    m_vertexData = std::vector<float>();
    m_param1 = param1;
    m_param2 = param2;
    setVertexData();
}


void Cylinder::setVertexData() {
    const float PI = 3.14159265358979323846f;
    const float radius = 0.5f;
    const float halfHeight = 0.5f;

    // Top cap
    for (int i = 0; i < m_param2; ++i) {
        float theta = 2.0f * PI * i / m_param2;
        float nextTheta = 2.0f * PI * (i + 1) / m_param2;

        // Top center point
        insertVec3(m_vertexData, glm::vec3(0, -halfHeight, 0));
        insertVec3(m_vertexData, glm::vec3(0, -1, 0)); // Normal facing up

        // Top edge point 1 (counter-clockwise winding)
        insertVec3(m_vertexData, glm::vec3(radius * sin(nextTheta), -halfHeight, radius * cos(nextTheta)));
        insertVec3(m_vertexData, glm::vec3(0, -1, 0)); // Normal facing up

        // Top edge point 2 (counter-clockwise winding)
        insertVec3(m_vertexData, glm::vec3(radius * sin(theta), -halfHeight, radius * cos(theta)));
        insertVec3(m_vertexData, glm::vec3(0, -1, 0)); // Normal facing up
    }

    // Bottom cap
    for (int i = 0; i < m_param2; ++i) {
        float theta = 2.0f * PI * i / m_param2;
        float nextTheta = 2.0f * PI * (i + 1) / m_param2;

        // Bottom center point
        insertVec3(m_vertexData, glm::vec3(0, halfHeight, 0));
        insertVec3(m_vertexData, glm::vec3(0, 1, 0)); // Normal facing down

        // Bottom edge point 1 (counter-clockwise winding)
        insertVec3(m_vertexData, glm::vec3(radius * sin(theta), halfHeight, radius * cos(theta)));
        insertVec3(m_vertexData, glm::vec3(0, 1, 0)); // Normal facing down

        // Bottom edge point 2 (counter-clockwise winding)
        insertVec3(m_vertexData, glm::vec3(radius * sin(nextTheta), halfHeight, radius * cos(nextTheta)));
        insertVec3(m_vertexData, glm::vec3(0, 1, 0)); // Normal facing down
    }

    // Cylinder sides
    for (int i = 0; i < m_param1; ++i) {
        float y = halfHeight - (i * 2 * halfHeight / m_param1);
        float nextY = halfHeight - ((i + 1) * 2 * halfHeight / m_param1);

        for (int j = 0; j < m_param2; ++j) {
            float theta = 2.0f * PI * j / m_param2;
            float nextTheta = 2.0f * PI * (j + 1) / m_param2;

            // Quad vertices
            glm::vec3 bottomLeft = glm::vec3(radius * sin(theta), y, radius * cos(theta));
            glm::vec3 bottomRight = glm::vec3(radius * sin(nextTheta), y, radius * cos(nextTheta));
            glm::vec3 topRight = glm::vec3(radius * sin(nextTheta), nextY, radius * cos(nextTheta));
            glm::vec3 topLeft = glm::vec3(radius * sin(theta), nextY, radius * cos(theta));

            // Calculate normals for the new vertices
            glm::vec3 n1 = glm::normalize(glm::vec3(sin(theta), 0, cos(theta)));
            glm::vec3 n2 = glm::normalize(glm::vec3(sin(nextTheta), 0, cos(nextTheta)));

            // Triangle 1

            insertVec3(m_vertexData, topLeft);
            insertVec3(m_vertexData, n1);

            insertVec3(m_vertexData, bottomRight);
            insertVec3(m_vertexData, n2);

            insertVec3(m_vertexData, bottomLeft);
            insertVec3(m_vertexData, n1);

            // Triangle 2
            insertVec3(m_vertexData, topRight);
            insertVec3(m_vertexData, n2);

            insertVec3(m_vertexData, bottomRight);
            insertVec3(m_vertexData, n2);

            insertVec3(m_vertexData, topLeft);
            insertVec3(m_vertexData, n1);
        }
    }
}

// Inserts a glm::vec3 into a vector of floats.
// This will come in handy if you want to take advantage of vectors to build your shape!
void Cylinder::insertVec3(std::vector<float> &data, glm::vec3 v) {
    data.push_back(v.x);
    data.push_back(v.y);
    data.push_back(v.z);
}

