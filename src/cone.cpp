#include "Cone.h"

void Cone::updateParams(int param1, int param2) {
    m_vertexData = std::vector<float>();
    m_param1 = param1;
    m_param2 = param2;
    setVertexData();
}


void Cone::setVertexData() {
    const float PI = 3.14159265358979323846f;
    const float radius = 0.5f;
    const float height = 1.0f;
    const float yOffset = height * 0.5f;  // This will be subtracted from y-coordinates to center the cone

    // Bottom cap
    for (int i = 0; i < m_param2; ++i) {
        float theta = 2.0f * PI * i / m_param2;
        float nextTheta = 2.0f * PI * (i + 1) / m_param2;

        // Bottom center point
        insertVec3(m_vertexData, glm::vec3(0, -yOffset, 0));  // Centered on origin
        insertVec3(m_vertexData, glm::vec3(0, -1, 0)); // Normal facing down

        // Bottom edge point 2 (counter-clockwise winding)
        insertVec3(m_vertexData, glm::vec3(radius * sin(nextTheta), -yOffset, radius * cos(nextTheta)));
        insertVec3(m_vertexData, glm::vec3(0, -1, 0)); // Normal facing down

        // Bottom edge point 1 (counter-clockwise winding)
        insertVec3(m_vertexData, glm::vec3(radius * sin(theta), -yOffset, radius * cos(theta)));
        insertVec3(m_vertexData, glm::vec3(0, -1, 0)); // Normal facing down


    }

    // Cone sides
    for (int i = 0; i < m_param1; ++i) {
        for (int j = 0; j < m_param2; ++j) {
            float theta = 2.0f * PI * i / m_param2;
            float nextTheta = 2.0f * PI * (i + 1) / m_param2;
            float stepHeight = height / m_param1;
            float currentHeight = stepHeight * j;
            float nextHeight = stepHeight * (j + 1);

            // Calculate the smaller radius at the current height
            float currentRadius = (height - currentHeight) / height * radius;
            float nextRadius = (height - nextHeight) / height * radius;

            // Base vertices
            glm::vec3 base1 = glm::vec3(currentRadius * sin(theta), currentHeight - yOffset, currentRadius * cos(theta));
            glm::vec3 base2 = glm::vec3(nextRadius * sin(theta), nextHeight - yOffset, nextRadius * cos(theta));

            // Next base vertices
            glm::vec3 base3 = glm::vec3(currentRadius * sin(nextTheta), currentHeight - yOffset, currentRadius * cos(nextTheta));
            glm::vec3 base4 = glm::vec3(nextRadius * sin(nextTheta), nextHeight - yOffset, nextRadius * cos(nextTheta));

            // Calculate normals
            glm::vec3 normal1 = glm::normalize(glm::vec3(sin(theta), radius / height, cos(theta)));
            glm::vec3 normal2 = glm::normalize(glm::vec3(sin(nextTheta), radius / height, cos(nextTheta)));

            // Two triangles for each quad
            insertVec3(m_vertexData, base1);
            insertVec3(m_vertexData, normal1);
            insertVec3(m_vertexData, base3);
            insertVec3(m_vertexData, normal2);
            insertVec3(m_vertexData, base2);
            insertVec3(m_vertexData, normal1);

            insertVec3(m_vertexData, base3);
            insertVec3(m_vertexData, normal2);
            insertVec3(m_vertexData, base4);
            insertVec3(m_vertexData, normal2);
            insertVec3(m_vertexData, base2);
            insertVec3(m_vertexData, normal1);
        }
    }
}


// Inserts a glm::vec3 into a vector of floats.
// This will come in handy if you want to take advantage of vectors to build your shape!
void Cone::insertVec3(std::vector<float> &data, glm::vec3 v) {
    data.push_back(v.x);
    data.push_back(v.y);
    data.push_back(v.z);
}
