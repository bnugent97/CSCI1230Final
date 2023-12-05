#version 330 core

// Task 16: Create a UV coordinate in variable
in vec2 fragUV;

// Task 8: Add a sampler2D uniform
uniform sampler2D u_texture;

// Task 29: Add a bool on whether or not to filter the texture
uniform bool u_filterTexture;

// Task 29: Add a bool on whether or not to filter the texture
uniform bool u_filterBox;

// Added for extra credit
uniform bool u_filterTexture2;

// Task 29: Add a bool on whether or not to filter the texture
uniform bool u_filterBox2;

out vec4 fragColor;

void main()
{
    fragColor = vec4(1);
    // Task 17: Set fragColor using the sampler2D at the UV coordinate
    fragColor = texture(u_texture, fragUV);

    // Task 33: Invert fragColor's r, g, and b color channels if your bool is true

    if (u_filterTexture) {
        fragColor.rgb = 1.0 - fragColor.rgb;
    }

    // Apply filter
    if (u_filterBox) {
        vec2 texSize = textureSize(u_texture, 0); // Get the size of the texture
        vec4 sum = vec4(0.0);
        float blurSize = 1.0 / 25.0;

        for(int x = -2; x <= 2; x++) {
            for(int y = -2; y <= 2; y++) {
                vec2 offset = vec2(x, y) / texSize;
                sum += texture(u_texture, fragUV + offset);
            }
        }

        fragColor = sum * blurSize;
    }

    // Apply color tint filter
    if (u_filterTexture2) {
        float blueTintStrength = 0.2; // Adjust this for stronger/weaker tint
        fragColor.rgb += vec3(-blueTintStrength, -blueTintStrength, blueTintStrength);
        fragColor.rgb = clamp(fragColor.rgb, 0.0, 1.0); // Ensure values are within the valid range
    }

    // Apply filter
    if (u_filterBox2) {
        vec2 texSize = textureSize(u_texture, 0);

                // Sobel kernels for horizontal and vertical edge detection
                float kernelH[9] = float[](1, 0, -1, 2, 0, -2, 1, 0, -1);
                float kernelV[9] = float[](1, 2, 1, 0, 0, 0, -1, -2, -1);

                float edgeStrengthH = 0.0;
                float edgeStrengthV = 0.0;

                for(int i = -1; i <= 1; i++) {
                    for(int j = -1; j <= 1; j++) {
                        vec2 offset = vec2(i, j) / texSize;
                        vec4 sample = texture(u_texture, fragUV + offset);
                        edgeStrengthH += sample.r * kernelH[(i + 1) * 3 + (j + 1)];
                        edgeStrengthV += sample.r * kernelV[(i + 1) * 3 + (j + 1)];
                    }
                }

                // Combine horizontal and vertical strengths
                float edgeStrength = sqrt(edgeStrengthH * edgeStrengthH + edgeStrengthV * edgeStrengthV);

                fragColor = vec4(edgeStrength, edgeStrength, edgeStrength, 1.0);
    }

}
