#version 330 core
in vec4 vert;
in vec4 norm;
in vec3 color;
in vec3 lightDir;

uniform bool wireshade;

out  vec4 fragColor;

void main(void)
{
    // Calculate the intensity as before
    float intensity = dot(norm.xyz, lightDir);
    vec4 colore;

    // Check if the normal is pointing upwards, which could indicate a flat surface
    // This is just an example condition and might need to be adjusted
    if (abs(norm.y) >= 0.9999) {
        // Apply a grey color to the flat area
        colore = vec4(0.3, 0.3, 0.3, 1.0); // Grey color
    } else {
        // Existing color calculations based on intensity
        if (intensity > 0.95) {
            colore = vec4(1.0, 0.5, 0.5, 1.0);
        } else if (intensity > 0.5) {
            colore = vec4(0.6, 0.3, 0.3, 1.0);
        } else if (intensity > 0.25) {
            colore = vec4(0.4, 0.2, 0.2, 1.0);
        } else {
            colore = vec4(0.2, 0.1, 0.1, 1.0);
        }
    }

    fragColor = colore;
}
