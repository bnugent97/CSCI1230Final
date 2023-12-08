#version 330 core

// Task 16: Create a UV coordinate in variable
in vec2 fragUV;

// Task 8: Add a sampler2D uniform
uniform sampler2D u_texture;

// Task 29: Add a bool on whether or not to filter the texture
uniform bool daytime;

// Task 29: Add a bool on whether or not to filter the texture
uniform bool nighttime;

// Added for extra credit
uniform bool rainbowvomit;


out vec4 fragColor;

void main()
{
    fragColor = vec4(1);
    // Task 17: Set fragColor using the sampler2D at the UV coordinate
    fragColor = texture(u_texture, fragUV);

    // Task 33: Invert fragColor's r, g, and b color channels if your bool is true

    if (daytime) {
        fragColor.rgb = fragColor.rgb;
    }

    // Apply nighttime filter
    if (nighttime) {
        // Darken the color and add a blue tint
        float darknessFactor = 0.3; // Adjust this value to control darkness
        vec3 nightColor = vec3(0.1, 0.1, 0.3); // Blue tint for moonlight
        fragColor.rgb = mix(fragColor.rgb * darknessFactor, nightColor, 0.2);
    }

    // Apply color tint filter
    if (rainbowvomit) {
        fragColor.rgb = 1.0f - fragColor.rgb;
    }


}
