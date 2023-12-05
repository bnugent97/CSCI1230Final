#include <stdexcept>
#include "camera.h"
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera() {
    // Set default values
    position = glm::vec3(0.0f, 0.0f, 0.0f);
    target = glm::vec3(0.0f, 0.0f, -1.0f);
    up = glm::vec3(0.0f, 1.0f, 0.0f);
    heightAngle = glm::radians(45.0f);  // example value
    aspectRatio = 1.0f;  // example value
    nearPlane = 0.1f;    // example value
    farPlane = 100.0f;   // example value
    // Initialize other members as needed
}

Camera::Camera(glm::vec3 pos, glm::vec3 tgt, glm::vec3 upVec, float fov, float aspect, float nearPlane, float farPlane)
    : position(pos), target(tgt), up(upVec), heightAngle(fov), aspectRatio(aspect), nearPlane(nearPlane), farPlane(farPlane) {
    // Initialize other variables as necessary
    aperture = 0.0f; // default aperture (could be adjusted based on requirements)
    focalLength = getFocalLength(); // compute and store the focal length

    // Normalize the forward vector
    glm::vec3 forward = glm::normalize(target - position);

    // Compute the right vector
    glm::vec3 right = glm::normalize(glm::cross(up, forward));

    // Recompute the up vector
    // I do not believe is is any longer necessary to normalize this with OpenGL
    //    up = glm::normalize(glm::cross(forward, right));
}

glm::mat4 Camera::getViewMatrix() const {
    // Compute w(arrow), v(arrow), and u(arrow) basis vectors for the camera
    glm::vec3 w = glm::normalize(-(target - position)); // Negative normalized look vector
    glm::vec3 u = glm::normalize(glm::cross(up, w));  // Cross product of up and w vectors
    glm::vec3 v = glm::cross(w, u);   // Cross product of w and u vectors

    // Construct the rotation matrix R

    glm::mat4 R(
        u.x, v.x, w.x, 0.0f,
        u.y, v.y, w.y, 0.0f,
        u.z, v.z, w.z, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
        );


    glm::mat4 T(
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        -position.x, -position.y, -position.z, 1.0f
        );

    // Compose R and T to obtain V
    glm::mat4 V = R * T;

    return V;
}

// Implementing the Projection Matrix
glm::mat4 Camera::getProjectionMatrix() const {
    // Matrix 1:

    glm::mat4 mapToOpenGL = glm::mat4(0.0f);
    mapToOpenGL[0][0] = 1.0f;
    mapToOpenGL[1][1] = 1.0f;
    mapToOpenGL[2][2] = -2.0f;
    mapToOpenGL[3][2] = -1.0f;
    mapToOpenGL[3][3] = 1.0f;

    float c = -nearPlane/farPlane;
    glm::mat4 unhingeMatrix = glm::mat4(0.0f);
    unhingeMatrix[0][0] = 1.0f;
    unhingeMatrix[1][1] = 1.0f;
    unhingeMatrix[2][2] = 1.0f / (1.0f + c);
    unhingeMatrix[2][3] = -1.0f;
    unhingeMatrix[3][2] = -c / (1.0f + c);


    float thetaH = heightAngle;
    float thetaW = aspectRatio * heightAngle;

    glm::mat4 scaleMatrix = glm::mat4(0.0f);
    // DO I need to convert to radians here?
    scaleMatrix[0][0] = 1.0f / (farPlane * tan(thetaW / 2.0f));
    scaleMatrix[1][1] = 1.0f / (farPlane * tan(thetaH/2));
    scaleMatrix[2][2] = 1.0f / farPlane;
    scaleMatrix[3][3] = 1.0f;

    return mapToOpenGL * unhingeMatrix * scaleMatrix;
}

float Camera::getAspectRatio() const {
    return aspectRatio;
}

float Camera::getHeightAngle() const {
    return heightAngle;
}

float Camera::getFocalLength() const {
    // For basic pinhole model, we can derive the focal length from the height angle (FOV).
    // f = 0.5 / tan(FOV / 2)
    return 0.5f / tan(heightAngle * 0.5f);
}

float Camera::getAperture() const {
    // This is for depth-of-field. If not implemented, just return a default value.
    return aperture;
}

const glm::vec3& Camera::getPosition() const {
    return position;
}

const glm::vec3&  Camera::getUp() const {
    return up;
}

const glm::vec3&  Camera::getTarget() const {
    return target;
}

// Excluded the Generate Ray function as it is not necessary for this project

