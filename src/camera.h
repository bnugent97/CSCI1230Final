#pragma once

#include <glm/glm.hpp>

// A class representing a virtual camera.

// Ported over from the Ray Project

// Feel free to make your own design choices for Camera class, the functions below are all optional / for your convenience.
// You can either implement and use these getters, or make your own design.
// If you decide to make your own design, feel free to delete these as TAs won't rely on them to grade your assignments.

class Camera {

private:
    //glm::vec3 position;
    //glm::vec3 target;
    glm::vec3 up;
    float heightAngle;  // in radians
    float aspectRatio;  // width/height
    float focalLength;  // optional
    float aperture;
    float nearPlane;
    float farPlane;

public:
    // Default constructor
    glm::vec3 position;
    glm::vec3 target;
    Camera();
    // Returns the view matrix for the current camera settings.
    // You might also want to define another function that return the inverse of the view matrix.
    Camera(glm::vec3 pos, glm::vec3 tgt, glm::vec3 upVec, float fov, float aspect, float nearPlane, float farPlane);

    glm::mat4 getViewMatrix() const;

    glm::mat4 getProjectionMatrix() const;

    // Returns the aspect ratio of the camera.
    float getAspectRatio() const;

    // Returns the height angle of the camera in RADIANS.
    float getHeightAngle() const;

    // Returns the focal length of this camera.
    // This is for the depth of field extra-credit feature only;
    // You can ignore if you are not attempting to implement depth of field.
    float getFocalLength() const;

    // Returns the focal length of this camera.
    // This is for the depth of field extra-credit feature only;
    // You can ignore if you are not attempting to implement depth of field.
    float getAperture() const;
    const glm::vec3& getPosition() const;
    const glm::vec3& getUp() const;
    const glm::vec3& getTarget() const;
};
