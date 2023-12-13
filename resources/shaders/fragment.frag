#version 330 core


// UNCOMMENT THE BELOW SECTION TO RENDER MOUNTAIN SCENE

in vec4 vert;
in vec4 norm;
in vec3 color;
in vec3 lightDir;

uniform bool wireshade;
uniform float farPlane;
uniform vec2 viewportSize;
uniform float fogSize;
uniform bool rainbowVomit;

out vec4 fragColor;

void main(void)
{
    // Normalize the depth value to be between 0 and 1
    float normalizedDepth = gl_FragCoord.y / farPlane;

    // Convert gl_FragCoord to normalized device coordinates (NDC)
    vec2 ndc = (gl_FragCoord.xy / viewportSize) * 2.0 - 1.0;

    // Define trapezoid boundaries in NDC
    float trapezoidBase = fogSize;  // Base width of the trapezoid
    float trapezoidTop = fogSize * 0.5;  // Top width of the trapezoid (smaller for inverted trapezoid)
    float trapezoidHeight = fogSize * 2.0;  // Height of the trapezoid

    // Calculate width at the current y position
    float currentWidth = mix(trapezoidTop, trapezoidBase, (ndc.y + 1.0) / 2.0);
    vec2 trapezoidMin = vec2(-currentWidth, -trapezoidHeight);
    vec2 trapezoidMax = vec2(currentWidth, trapezoidHeight);

    // Calculate distance to the nearest edge of the trapezoid
    float distX = max(max(trapezoidMin.x - ndc.x, ndc.x - trapezoidMax.x), 0.0);
    float distY = max(max(trapezoidMin.y - ndc.y, ndc.y - trapezoidMax.y), 0.0);
    float distToEdge = sqrt(distX * distX + distY * distY);

    // Smooth transition parameters
    float edgeSoftness = 0.5; // Adjust for softer/harder edges
    float edgeStart = 0.0;
    float edgeEnd = edgeSoftness;

    // Calculate smooth transition factor for edges
    float edgeFactor = smoothstep(edgeStart, edgeEnd, distToEdge);

    // Invert edgeFactor for fog effect inside the trapezoid
    edgeFactor = 1.0 - edgeFactor;

    // Fog parameters
    vec3 fogColor = vec3(0.5, 0.5, 0.5); // Greyish fog color, adjust as needed

    if (rainbowVomit) {
           fogColor = vec3(0.0, 0.2, 0.0); // Greyish fog color, adjust as needed
        }

    float fogScale = 1.0f;
    float fogStart = 1.0f * fogScale; // Start of the fog effect, adjust as needed
    float fogEnd = 4.0f * fogScale; // End of the fog effect, adjust as needed

    // Calculate fog factor (0 = no fog, 1 = full fog)
    float fogFactor = smoothstep(fogStart, fogEnd, normalizedDepth) * 0;
    if (fogSize > 0){
        fogFactor = smoothstep(fogStart, fogEnd, normalizedDepth) * .95;
        if (rainbowVomit) {
               fogFactor = smoothstep(fogStart, fogEnd, normalizedDepth) * .85;
            }
    }

    // Apply edge smoothness to fog factor
    fogFactor *= edgeFactor;

    // Calculate object color with lighting
    vec3 objColor = color;
    float lightIntensity = clamp(dot(norm.xyz, lightDir), 0, 1);
    vec3 litColor = (lightIntensity * 0.7 + 0.3) * objColor;

    // Final color with fog effect inside the trapezoid
    vec3 finalColor = mix(litColor, fogColor, fogFactor);

    if (wireshade) {
        fragColor = vec4(color, 1);
    } else if (fogSize == 0){
        fragColor = vec4(finalColor, 1.0);
    }
}


// UNCOMMENT BELOW FOR CANYON (CEL SHADING) SCENE




//#version 330 core
//in vec4 vert;
//in vec4 norm;
//in vec3 color;
//in vec3 lightDir;

//uniform bool wireshade;
//uniform float farPlane;
//uniform vec2 viewportSize;
//uniform float fogSize;
//uniform bool rainbowVomit;

//out vec4 fragColor;

//void main(void)
//{
//    // Calculate the intensity as before
//    float intensity = dot(norm.xyz, lightDir);
//    vec4 colore;




//    // Apply a grey color to the flat area


//    // Check if the normal is pointing upwards, which could indicate a flat surface
//    // This is just an example condition and might need to be adjusted
//    if (abs(norm.y) >= 0.9999) {

//        colore = vec4(0.3, 0.3, 0.3, 1.0); // Grey color

//        if (rainbowVomit) {
//           colore = vec4(.13, .28, .87, 1.0); // Grey color
//        }



//    } else {
//        // Existing color calculations based on intensity
//        if (intensity > 0.95) {
//            colore = vec4(1.0, 0.5, 0.5, 1.0);
//        } else if (intensity > 0.5) {
//            colore = vec4(0.6, 0.3, 0.3, 1.0);
//        } else if (intensity > 0.25) {
//            colore = vec4(0.4, 0.2, 0.2, 1.0);
//        } else {
//            colore = vec4(0.2, 0.1, 0.1, 1.0);
//        }
//    }

//    // Normalize the depth value to be between 0 and 1
//    float normalizedDepth = gl_FragCoord.y / farPlane;

//    // Convert gl_FragCoord to normalized device coordinates (NDC)
//    vec2 ndc = (gl_FragCoord.xy / viewportSize) * 2.0 - 1.0;

//    // Define trapezoid boundaries in NDC
//    float trapezoidBase = fogSize;  // Base width of the trapezoid
//    float trapezoidTop = fogSize * 0.5;  // Top width of the trapezoid (smaller for inverted trapezoid)
//    float trapezoidHeight = fogSize * 2.0;  // Height of the trapezoid

//    // Calculate width at the current y position
//    float currentWidth = mix(trapezoidTop, trapezoidBase, (ndc.y + 1.0) / 2.0);
//    vec2 trapezoidMin = vec2(-currentWidth, -trapezoidHeight);
//    vec2 trapezoidMax = vec2(currentWidth, trapezoidHeight);

//    // Calculate distance to the nearest edge of the trapezoid
//    float distX = max(max(trapezoidMin.x - ndc.x, ndc.x - trapezoidMax.x), 0.0);
//    float distY = max(max(trapezoidMin.y - ndc.y, ndc.y - trapezoidMax.y), 0.0);
//    float distToEdge = sqrt(distX * distX + distY * distY);

//    // Smooth transition parameters
//    float edgeSoftness = 0.5; // Adjust for softer/harder edges
//    float edgeStart = 0.0;
//    float edgeEnd = edgeSoftness;

//    // Calculate smooth transition factor for edges
//    float edgeFactor = smoothstep(edgeStart, edgeEnd, distToEdge);

//    // Invert edgeFactor for fog effect inside the trapezoid
//    edgeFactor = 1.0 - edgeFactor;

//    // Fog parameters
//    vec3 fogColor = vec3(0.5, 0.5, 0.5); // Greyish fog color, adjust as needed

//    if (rainbowVomit) {
//       fogColor = vec3(0.0, 0.2, 0.0); // Greyish fog color, adjust as needed
//    }


//    //vec3 fogColor = vec3(0.0, 0.2, 0.0); // Greyish fog color, adjust as needed
//    float fogScale = 1.0; // Adjust as needed
//    float fogStart = 1.0 * fogScale; // Start of the fog effect, adjust as needed
//    float fogEnd = 4.0 * fogScale; // End of the fog effect, adjust as needed

//    // Calculate fog factor (0 = no fog, 1 = full fog)
//    float fogFactor = smoothstep(fogStart, fogEnd, normalizedDepth) * 0;
//    if (fogSize > 0){


//        fogFactor = smoothstep(fogStart, fogEnd, normalizedDepth) * .95;


//        if (rainbowVomit) {
//           fogFactor = smoothstep(fogStart, fogEnd, normalizedDepth) * .825;
//        }


//    }

//    // Apply edge smoothness to fog factor
//    fogFactor *= edgeFactor;

//    // Combine fog with the existing color
//    vec3 finalColor = mix(colore.rgb, fogColor, fogFactor);

//    if (wireshade) {
//        fragColor = colore ;
//    } else {
//        fragColor = vec4(finalColor, 1.0);
//    }
//}
