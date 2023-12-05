#version 330 core

layout (location = 0) in vec3 position;
//layout (location = 1) in vec3 color;
vec3 objectSpacePosition = position;

layout (location = 1) in vec3 normal;
//layout (location = 1) in vec3 color;
vec3 objectSpaceNormal = normal;

// Task 5: declare `out` variables for the world-space position and normal,
//         to be passed to the fragment shader
out vec4 worldSpacePosition;
out vec3 worldSpaceNormal;

//out vec3 col;

// Adding this in to try and port from lab

// Task 6: declare a uniform mat4 to store model matrix
uniform mat4 modelMatrix; // Has the CTM in it

// Task 7: declare uniform mat4's for the view and projection matrix
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

void main() {

    worldSpacePosition = modelMatrix * vec4(objectSpacePosition, 1.0f);

    //vec3 objNormal = objectSpaceNormal;
    worldSpaceNormal = normalize(inverse(transpose(mat3(modelMatrix)))*objectSpaceNormal);

    // Transform the vertex position to clip space and output to gl_Position
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(objectSpacePosition, 1.0f);
}



