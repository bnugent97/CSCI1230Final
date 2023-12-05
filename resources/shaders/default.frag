#version 330 core

in vec3 col;
out vec4 fragColor;


in vec4 worldSpacePosition;
in vec3 worldSpaceNormal;


// Task 12: declare relevant uniform(s) here, for ambient lighting

// Scene Ambient
uniform float k_a;

// Object Ambient
uniform vec4 o_a;

// Task 13: declare relevant uniform(s) here, for diffuse lighting
uniform float k_d;

// Object Ambient
uniform vec4 o_d;

// Task 14: declare relevant uniform(s) here, for specular lighting
uniform float k_s;

// Object Ambient
uniform vec4 o_s;

uniform float shininess;
uniform vec3 cameraPosition_worldspace;

uniform int lightCount;

struct Lights {
    int lightType;
    vec3 lightDirection;
    vec4 lightColor;
    vec4 lightPosition;
    vec3 function;
    float angle;
    float penumbra;
};

uniform Lights sceneLights[8];


void main() {
    //fragColor = vec4(abs(worldSpaceNormal), 1.0);
  // fragColor = vec4(1.0, 1.0, 1.0, 1.0);
    // Task 12: add ambient component to output color
    //fragColor = vec4(abs(worldSpaceNormal), 1.0);
    // Optionally set the output color to black to prepare for the next part of the lab
    fragColor = vec4(0.0f, 0.0f, 0.0f, 1.0f); // Uncomment this line if needed
    fragColor += k_a * o_a;
    float f_att;
    vec3 reflectDir;
    vec4 directionToLight;
    float distanceToLight;
    vec4 diffuse;
    vec4 specular;
    float angleBetween;
    float theta_inner;
    float theta_outer;

//     Task 13: add diffuse component to output color
//     Normalize?
//     DIRECTION TO LIGHT



        // KEEP IN MIND FOR PROJECT 6 THAT LIGHT TYPES ARE INCLUDED IN THE STRUCT TO USE THEN
        for (int i=0; i<lightCount; i++) {

             // Just doing directional light for this project
            if (sceneLights[i].lightType == 1) {

                // normalized in realtime.cpp
                vec3 lightDir = sceneLights[i].lightDirection;

                diffuse = k_d * o_d * sceneLights[i].lightColor * clamp(dot(normalize(worldSpaceNormal), lightDir), 0, 1);

                fragColor += diffuse;

                vec3 viewDir = normalize(vec3(cameraPosition_worldspace) - vec3(worldSpacePosition));

                reflectDir = normalize(reflect(-lightDir, normalize(worldSpaceNormal)));


                if (shininess == 0) {
                    specular = k_s * o_s * sceneLights[i].lightColor * 1;
                }
                else {
                    specular = k_s * o_s * sceneLights[i].lightColor * pow(clamp(dot(reflectDir, viewDir),0,1), shininess);
                }

                fragColor += specular;

            }
            // Just doing directional light for this project
            // type 2 is a point light
           else if (sceneLights[i].lightType == 2) {
                // Ensure the light position is also in world space
                // Vec 3 for direction
                directionToLight = normalize(sceneLights[i].lightPosition - worldSpacePosition);
                // Vec 4 for distance?
                distanceToLight = length(sceneLights[i].lightPosition - worldSpacePosition);

                f_att = min(1.0f, 1.0f / (sceneLights[i].function[0] + distanceToLight * sceneLights[i].function[1] + distanceToLight * distanceToLight * sceneLights[i].function[2]));

               // negative applied in realtime.cpp
//               vec3 lightDir = sceneLights[i].lightDirection;



               diffuse = k_d * o_d * sceneLights[i].lightColor * clamp(dot(normalize(worldSpaceNormal), vec3(directionToLight)), 0, 1) * f_att;

               fragColor += diffuse;

               vec3 viewDir = normalize(vec3(cameraPosition_worldspace) - vec3(worldSpacePosition));

               reflectDir = normalize(reflect(-vec3(directionToLight), normalize(worldSpaceNormal)));

               if (shininess == 0) {
                   specular = k_s * o_s * sceneLights[i].lightColor * 1 * f_att;
               }
               else {
                   specular = k_s * o_s * sceneLights[i].lightColor * pow(clamp(dot(reflectDir, viewDir),0,1), shininess) * f_att;
               }

               fragColor += specular;

           }

            else if (sceneLights[i].lightType == 3) {
                 // Ensure the light position is also in world space
                 // Vec 3 for direction
                 directionToLight = normalize(sceneLights[i].lightPosition - worldSpacePosition);
                 // Vec 4 for distance?
                 // Reversed for spot light for some reason
                 distanceToLight = length(worldSpacePosition - sceneLights[i].lightPosition);

                 f_att = min(1.0f, 1.0f / (sceneLights[i].function[0] + distanceToLight * sceneLights[i].function[1] + distanceToLight * distanceToLight * sceneLights[i].function[2]));

                // negative applied in realtime.cpp
                vec3 lightDir = sceneLights[i].lightDirection;

                angleBetween = acos(dot(vec3(directionToLight), lightDir));
                theta_inner = sceneLights[i].angle - sceneLights[i].penumbra;
                theta_outer = sceneLights[i].angle;

                if (angleBetween <= theta_inner) {
                    // Inside the inner cone, full intensity
                } else if (angleBetween <= theta_outer) {
                    // Between inner and outer cone, apply falloff
                    float x = angleBetween;
                    float falloff = -2 * pow((x - theta_inner) / (theta_outer - theta_inner), 3) + 3 * pow((x - theta_inner) / (theta_outer - theta_inner), 2);
                    f_att = f_att * (1.0f - falloff);
                } else {
                    // Outside the outer cone, no light
                    f_att = 0.0f;
                }



                diffuse = k_d * o_d * sceneLights[i].lightColor * clamp(dot(normalize(worldSpaceNormal), vec3(directionToLight)), 0, 1) * f_att;

                fragColor += diffuse;

                vec3 viewDir = normalize(vec3(cameraPosition_worldspace) - vec3(worldSpacePosition));

                reflectDir = normalize(reflect(-vec3(directionToLight), normalize(worldSpaceNormal)));

                if (shininess == 0) {
                    specular = k_s * o_s * sceneLights[i].lightColor * 1 * f_att;
                }
                else {
                    specular = k_s * o_s * sceneLights[i].lightColor * pow(clamp(dot(reflectDir, viewDir),0,1), shininess) * f_att;
                }

                fragColor += specular;

            }


        }

}
