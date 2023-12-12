#version 330 core
in vec4 vert;
in vec4 norm;
in vec3 color;
in vec3 lightDir;

uniform bool wireshade;
uniform float farPlane; // The far plane of your camera frustum

out vec4 fragColor;

void main(void)
{
    // Normalize the depth value to be between 0 and 1
    float normalizedDepth = gl_FragCoord.y / farPlane;

    // Fog parameters
    vec3 fogColor = vec3(0.5, 0.5, 0.5); // Greyish fog color, adjust as needed
    float fogScale = 1.0f;
    float fogStart = 1.0f * fogScale; // Start of the fog effect, adjust as needed
    float fogEnd = 4.0f * fogScale; // End of the fog effect, adjust as needed

    // Calculate fog factor (0 = no fog, 1 = full fog)
    float fogFactor = smoothstep(fogStart, fogEnd, normalizedDepth) * .95;

    // Calculate object color with lighting
    vec3 objColor = color;
    float lightIntensity = clamp(dot(norm.xyz, lightDir), 0, 1);
    vec3 litColor = (lightIntensity * 0.7 + 0.3) * objColor;

    // Final color with fog effect
    vec3 finalColor = mix(litColor, fogColor, fogFactor);

    if (wireshade) {
        fragColor = vec4(color, 1);
    } else {
        fragColor = vec4(finalColor, 1.0);
    }
}


//#version 330 core
//in vec4 vert;
//in vec4 norm;
//in vec3 color;
//in vec3 lightDir;

//uniform bool wireshade;
//out  vec4 fragColor;

//void main(void)
//{


//    if (wireshade) {
//        fragColor = vec4(color,1);
//    } else {
//        vec3 objColor = color;
//        fragColor = vec4((clamp(dot(norm.xyz, lightDir), 0, 1) * 0.7 +  0.3) * objColor, 1.0);
//    }



////    // Calculate the intensity as before
////    float intensity = dot(norm.xyz, lightDir);
////    vec4 colore;

////    // Check if the normal is pointing upwards, which could indicate a flat surface
////    // This is just an example condition and might need to be adjusted
////    if (abs(norm.y) >= 0.9999) {
////        // Apply a grey color to the flat area
////        colore = vec4(0.3, 0.3, 0.3, 1.0); // Grey color
////    } else {
////        // Existing color calculations based on intensity
////        if (intensity > 0.95) {
////            colore = vec4(1.0, 0.5, 0.5, 1.0);
////        } else if (intensity > 0.5) {
////            colore = vec4(0.6, 0.3, 0.3, 1.0);
////        } else if (intensity > 0.25) {
////            colore = vec4(0.4, 0.2, 0.2, 1.0);
////        } else {
////            colore = vec4(0.2, 0.1, 0.1, 1.0);
////        }
////    }

////    fragColor = color;
//}
