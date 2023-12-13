#version 330 core

out vec4 FragColor;

in vec3 TexCoords;

uniform samplerCube skybox;

uniform float shapeParameter2;
uniform bool rainbowVomit;


void main()
{
    vec3 skyboxColor = texture(skybox, TexCoords).rgb;
    if (shapeParameter2 > 0.0) {
        // Existing fog effect code
        vec3 fogColor = vec3(0.5, 0.5, 0.5);
        if (rainbowVomit) {
            fogColor = vec3(0.0, 0.2, 0.0);
        }
        float verticalPosition = TexCoords.y;
        float fogLowerLimit = -0.2;
        float fogUpperLimit = 0.2;
        float fogFactor = smoothstep(fogLowerLimit, fogUpperLimit, 1.0 - verticalPosition) * .95;
        if (rainbowVomit) {
            fogFactor = smoothstep(fogLowerLimit, fogUpperLimit, 1.0 - verticalPosition) * .825;
        }
        vec3 finalColor = mix(skyboxColor, fogColor, fogFactor);
        FragColor = vec4(finalColor, 1.0);
    } else {
        // Render without fog
        FragColor = vec4(skyboxColor, 1.0);
    }
}



//#version 330 core

//out vec4 FragColor;

//in vec3 TexCoords;

//uniform samplerCube skybox;

//void main()
//{
//    FragColor = texture(skybox, TexCoords);
//}

