#version 330 core

in vec2 texCoords; // Texture coordinates from vertex shader
out vec4 fragColor; // Output color of the pixel

uniform sampler2D screenTexture; // The texture containing the rendered scene

// Function to calculate luminance from a color
float luminance(vec4 color) {
    return dot(color.rgb, vec3(0.299, 0.587, 0.114));
}

void main() {
    // Sample the texture at multiple points
    vec4 color = texture(screenTexture, texCoords);

    // Calculate the size of one texel
    vec2 texelSize = 1.0 / textureSize(screenTexture, 0);

    // Calculate luminance for center and neighboring pixels
    float lumCenter = luminance(color);
    float lumRight = luminance(texture(screenTexture, texCoords + vec2(texelSize.x, 0)));
    float lumLeft = luminance(texture(screenTexture, texCoords - vec2(texelSize.x, 0)));
    float lumUp = luminance(texture(screenTexture, texCoords + vec2(0, texelSize.y)));
    float lumDown = luminance(texture(screenTexture, texCoords - vec2(0, texelSize.y)));

    // Edge detection based on luminance difference
    float edgeThreshold = 0.01; // Adjust this value based on desired sensitivity
    if (abs(lumRight - lumLeft) > edgeThreshold || abs(lumUp - lumDown) > edgeThreshold) {
        // Apply anti-aliasing effect
        color = (color + texture(screenTexture, texCoords + vec2(texelSize.x, 0)) +
                       texture(screenTexture, texCoords - vec2(texelSize.x, 0)) +
                       texture(screenTexture, texCoords + vec2(0, texelSize.y)) +
                       texture(screenTexture, texCoords - vec2(0, texelSize.y))) / 5.0;
    }

    fragColor = color;
}
