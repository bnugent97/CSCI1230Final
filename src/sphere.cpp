#include "sphere.h"
#include "glm/ext/scalar_constants.hpp"

// Helper function to convert spherical to cartesian coordinates
glm::vec3 sphericalToCartesian(float radius, float phi, float theta) {
    return glm::vec3(radius * glm::sin(phi) * glm::sin(theta),
                     radius * glm::cos(phi),
                     radius * glm::sin(phi) * glm::cos(theta));

}

// Inserts a glm::vec3 into a vector of floats.
void pushVec3(std::vector<float> &data, const glm::vec3 &vec) {
    data.push_back(vec.x);
    data.push_back(vec.y);
    data.push_back(vec.z);
}

void Sphere::updateParams(int param1, int param2) {
    m_vertexData.clear();  // It's better to use clear() to empty the vector
    m_param1 = param1;
    m_param2 = param2;
    setVertexData();
}

void Sphere::makeTile(const glm::vec3 topLeft, const glm::vec3 topRight,
                      const glm::vec3 bottomLeft, const glm::vec3 bottomRight) {
    // Normal vectors are simply the normalized positions of vertices on a sphere
    glm::vec3 normal;

    // First triangle
    normal = glm::normalize(topLeft);
    pushVec3(m_vertexData, topLeft);
    pushVec3(m_vertexData, normal);

    normal = glm::normalize(bottomLeft);
    pushVec3(m_vertexData, bottomLeft);
    pushVec3(m_vertexData, normal);

    normal = glm::normalize(bottomRight);
    pushVec3(m_vertexData, bottomRight);
    pushVec3(m_vertexData, normal);

    // Second triangle

    normal = glm::normalize(topRight);
    pushVec3(m_vertexData, topRight);
    pushVec3(m_vertexData, normal);

    normal = glm::normalize(topLeft);
    pushVec3(m_vertexData, topLeft);
    pushVec3(m_vertexData, normal);

    normal = glm::normalize(bottomRight);
    pushVec3(m_vertexData, bottomRight);
    pushVec3(m_vertexData, normal);
}

void Sphere::makeWedge(float currentTheta, float nextTheta) {
    float phiStep = glm::pi<float>() / m_param1;

    for (int i = 0; i < m_param1; ++i) {
        float phi = i * phiStep;
        float nextPhi = (i + 1) * phiStep;

        glm::vec3 topLeft = sphericalToCartesian(m_radius, phi, currentTheta);
        glm::vec3 topRight = sphericalToCartesian(m_radius, phi, nextTheta);
        glm::vec3 bottomLeft = sphericalToCartesian(m_radius, nextPhi, currentTheta);
        glm::vec3 bottomRight = sphericalToCartesian(m_radius, nextPhi, nextTheta);

        makeTile(topLeft, topRight, bottomLeft, bottomRight);
    }
}

void Sphere::makeSphere() {
    float thetaStep = 2 * glm::pi<float>() / m_param2;

    for (int i = 0; i < m_param2; ++i) {
        float currentTheta = i * thetaStep;
        float nextTheta = (i + 1) * thetaStep;
        makeWedge(currentTheta, nextTheta);
    }
}

void Sphere::setVertexData() {
    makeSphere();
}



//#include "Sphere.h"
//#include "glm/ext/scalar_constants.hpp"

//// Make sure that the normals are pointing outwards, not inwards

//void Sphere::updateParams(int param1, int param2) {
//    m_vertexData = std::vector<float>();
//    m_param1 = param1;
//    m_param2 = param2;
//    setVertexData();
//}

//void Sphere::makeTile(glm::vec3 topLeft,
//                      glm::vec3 topRight,
//                      glm::vec3 bottomLeft,
//                      glm::vec3 bottomRight) {
//    // Calculate the normals for the vertices (sphere normals)
//    glm::vec3 topLeftNormal = glm::normalize(topLeft);
//    glm::vec3 topRightNormal = glm::normalize(topRight);
//    glm::vec3 bottomLeftNormal = glm::normalize(bottomLeft);
//    glm::vec3 bottomRightNormal = glm::normalize(bottomRight);

//    // First triangle - counter-clockwise winding order
//    insertVec3(m_vertexData, topLeft);
//    insertVec3(m_vertexData, topLeftNormal);

//    insertVec3(m_vertexData, bottomRight);
//    insertVec3(m_vertexData, bottomRightNormal);

//    insertVec3(m_vertexData, bottomLeft);
//    insertVec3(m_vertexData, bottomLeftNormal);

//    // Second triangle - counter-clockwise winding order
//    insertVec3(m_vertexData, topLeft);
//    insertVec3(m_vertexData, topLeftNormal);

//    insertVec3(m_vertexData, topRight);
//    insertVec3(m_vertexData, topRightNormal);

//    insertVec3(m_vertexData, bottomRight);
//    insertVec3(m_vertexData, bottomRightNormal);
//}

//void Sphere::makeWedge(float currentTheta, float nextTheta) {
//    // Task 6: create a single wedge of the sphere using the
//    //         makeTile() function you implemented in Task 5
//    // Note: think about how param 1 comes into play here!

//    // Calculate the number of divisions in latitude
//    int divisionsLatitude = m_param1;

//    // Calculate the angle increment in latitude (phi)
//    float phiStep = glm::radians(180.f / divisionsLatitude);

//    for (int i = 0; i < divisionsLatitude; ++i) {
//        float phi = i * phiStep;
//        float nextPhi = (i + 1) * phiStep;

//        // Calculate the vertices for this latitude division
//        glm::vec3 topLeft = glm::vec3(m_radius * sin(phi) * cos(currentTheta),
//                                      m_radius * cos(phi),
//                                      m_radius * sin(phi) * sin(currentTheta));

//        glm::vec3 topRight = glm::vec3(m_radius * sin(phi) * cos(nextTheta),
//                                       m_radius * cos(phi),
//                                       m_radius * sin(phi) * sin(nextTheta));

//        glm::vec3 bottomLeft = glm::vec3(m_radius * sin(nextPhi) * cos(currentTheta),
//                                         m_radius * cos(nextPhi),
//                                         m_radius * sin(nextPhi) * sin(currentTheta));

//        glm::vec3 bottomRight = glm::vec3(m_radius * sin(nextPhi) * cos(nextTheta),
//                                          m_radius * cos(nextPhi),
//                                          m_radius * sin(nextPhi) * sin(nextTheta));

//        // Create the tile for this latitude division
//        makeTile(topLeft, topRight, bottomLeft, bottomRight);

//        // Update currentTheta for the next iteration
//        currentTheta = nextTheta;
//        nextTheta += glm::radians(360.f / m_param2); // Increment theta
//    }

//}

//void Sphere::makeSphere() {
//    // Task 7: create a full sphere using the makeWedge() function you
//    //         implemented in Task 6
//    // Note: think about how param 2 comes into play here!

//    float thetaStep = glm::radians(360.f / m_param2);

//    for (int i = 0; i < m_param2; ++i) {
//        float currentTheta = i * thetaStep;
//        float nextTheta = (i + 1) * thetaStep;
//        makeWedge(currentTheta, nextTheta);
//    }

//}

//void Sphere::setVertexData() {
//    // Uncomment these lines to make a wedge for Task 6, then comment them out for Task 7:

//    //     float thetaStep = glm::radians(360.f / m_param2);
//    //     float currentTheta = 0 * thetaStep;
//    //     float nextTheta = 1 * thetaStep;
//    //     makeWedge(currentTheta, nextTheta);

//    // Uncomment these lines to make sphere for Task 7:

//    makeSphere();
//}

//// Inserts a glm::vec3 into a vector of floats.
//// This will come in handy if you want to take advantage of vectors to build your shape!
//void Sphere::insertVec3(std::vector<float> &data, glm::vec3 v) {
//    data.push_back(v.x);
//    data.push_back(v.y);
//    data.push_back(v.z);
//}


//#include "Sphere.h"
//#include "glm/ext/scalar_constants.hpp"




//// Make sure that the normals are pointing outwards, not inwards

//void Sphere::updateParams(int param1, int param2) {
//    m_vertexData = std::vector<float>();
//    m_param1 = param1;
//    m_param2 = param2;
//    setVertexData();
//}

//void Sphere::makeTile(glm::vec3 topLeft,
//                      glm::vec3 topRight,
//                      glm::vec3 bottomLeft,
//                      glm::vec3 bottomRight) {
//    // Calculate the normals for the vertices (sphere normals)
//    glm::vec3 topLeftNormal = glm::normalize(topLeft);
//    glm::vec3 topRightNormal = glm::normalize(topRight);
//    glm::vec3 bottomLeftNormal = glm::normalize(bottomLeft);
//    glm::vec3 bottomRightNormal = glm::normalize(bottomRight);

//    // First triangle - counter-clockwise winding order
//    insertVec3(m_vertexData, topLeft);
//    insertVec3(m_vertexData, topLeftNormal);

//    insertVec3(m_vertexData, bottomRight);
//    insertVec3(m_vertexData, bottomRightNormal);

//    insertVec3(m_vertexData, bottomLeft);
//    insertVec3(m_vertexData, bottomLeftNormal);

//    // Second triangle - counter-clockwise winding order
//    insertVec3(m_vertexData, topLeft);
//    insertVec3(m_vertexData, topLeftNormal);

//    insertVec3(m_vertexData, topRight);
//    insertVec3(m_vertexData, topRightNormal);

//    insertVec3(m_vertexData, bottomRight);
//    insertVec3(m_vertexData, bottomRightNormal);
//}

//void Sphere::makeWedge(float currentTheta, float nextTheta) {
//    // Task 6: create a single wedge of the sphere using the
//    //         makeTile() function you implemented in Task 5
//    // Note: think about how param 1 comes into play here!

//    // Calculate the number of divisions in latitude
//    int divisionsLatitude = m_param1;

//    // Calculate the angle increment in latitude (phi)
//    float phiStep = glm::radians(180.f / divisionsLatitude);

//    for (int i = 0; i < divisionsLatitude; ++i) {
//        float phi = i * phiStep;
//        float nextPhi = (i + 1) * phiStep;

//        // Calculate the vertices for this latitude division
//        glm::vec3 topLeft = glm::vec3(m_radius * sin(phi) * cos(currentTheta),
//                                      m_radius * cos(phi),
//                                      m_radius * sin(phi) * sin(currentTheta));

//        glm::vec3 topRight = glm::vec3(m_radius * sin(phi) * cos(nextTheta),
//                                       m_radius * cos(phi),
//                                       m_radius * sin(phi) * sin(nextTheta));

//        glm::vec3 bottomLeft = glm::vec3(m_radius * sin(nextPhi) * cos(currentTheta),
//                                         m_radius * cos(nextPhi),
//                                         m_radius * sin(nextPhi) * sin(currentTheta));

//        glm::vec3 bottomRight = glm::vec3(m_radius * sin(nextPhi) * cos(nextTheta),
//                                          m_radius * cos(nextPhi),
//                                          m_radius * sin(nextPhi) * sin(nextTheta));

//        // Create the tile for this latitude division
//        makeTile(topLeft, topRight, bottomLeft, bottomRight);

//        // Update currentTheta for the next iteration
//        currentTheta = nextTheta;
//        nextTheta += glm::radians(360.f / m_param2); // Increment theta
//    }

//}

//void Sphere::makeSphere() {
//    // Task 7: create a full sphere using the makeWedge() function you
//    //         implemented in Task 6
//    // Note: think about how param 2 comes into play here!

//    float thetaStep = glm::radians(360.f / m_param2);

//    for (int i = 0; i < m_param2; ++i) {
//        float currentTheta = i * thetaStep;
//        float nextTheta = (i + 1) * thetaStep;
//        makeWedge(currentTheta, nextTheta);
//    }

//}

//void Sphere::setVertexData() {
//    // Uncomment these lines to make a wedge for Task 6, then comment them out for Task 7:

//    //     float thetaStep = glm::radians(360.f / m_param2);
//    //     float currentTheta = 0 * thetaStep;
//    //     float nextTheta = 1 * thetaStep;
//    //     makeWedge(currentTheta, nextTheta);

//    // Uncomment these lines to make sphere for Task 7:

//    makeSphere();
//}

//// Inserts a glm::vec3 into a vector of floats.
//// This will come in handy if you want to take advantage of vectors to build your shape!
//void Sphere::insertVec3(std::vector<float> &data, glm::vec3 v) {
//    data.push_back(v.x);
//    data.push_back(v.y);
//    data.push_back(v.z);
//}
