#version 330 core

out vec4 FragColor;

in vec3 TexCoords;

uniform samplerCube skybox;

void main()
{
    vec3 fogColor = vec3(0.5, 0.5, 0.5); // Greyish fog color, adjust as needed

    // Determine the vertical position in the skybox
    // TexCoords.y ranges from -1 (bottom) to 1 (top)
    float verticalPosition = TexCoords.y;

    // Adjust these values as needed to control the height and density of the fog
    float fogLowerLimit = -0.2; // Lower boundary of the fog (e.g., -0.2 is slightly below the horizon)
    float fogUpperLimit = 0.2;  // Upper boundary of the fog (e.g., 0.2 is slightly above the horizon)

    // Calculate fog factor based on vertical position
    // The fog factor will be 0 above fogUpperLimit and 1 below fogLowerLimit
    float fogFactor = smoothstep(fogLowerLimit, fogUpperLimit, 1.0 - verticalPosition) * .9;

    // Get the skybox color
    vec3 skyboxColor = texture(skybox, TexCoords).rgb;

    // Apply the fog effect
    vec3 finalColor = mix(skyboxColor, fogColor, fogFactor);

    FragColor = vec4(finalColor, 1.0);
}



//#version 330 core

//out vec4 FragColor;

//in vec3 TexCoords;

//uniform samplerCube skybox;

//void main()
//{
//    FragColor = texture(skybox, TexCoords);
//}

