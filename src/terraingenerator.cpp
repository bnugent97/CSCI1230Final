#include "terraingenerator.h"
#include <cmath>
#include "glm/glm.hpp"
#include <glm/gtc/random.hpp>>
#include <array>
#include <vector>
#include <algorithm> // For std::min
#include <limits>    // For std::numeric_limits

#include "settings.h"

// Constructor
TerrainGenerator::TerrainGenerator()
{
    m_wireshade = false; // TA SOLUTION
    //Define resolution of terrain generation
    m_resolutionX = 250;
    m_resolutionY = 5000;
//    m_resolutionX = 100;
//    m_resolutionY = 500;
    // Generate random vector lookup table
    m_lookupSize = 1024;
    m_randVecLookup.reserve(m_lookupSize);
    // Initialize random number generator
    //  float rand = glm::linearRand(0.f,1.f);
    //  std::srand(int(rand*2000));
    std::srand(1230);
    // Populate random vector lookup table
    for (int i = 0; i < m_lookupSize; i++)
    {
        m_randVecLookup.push_back(glm::vec2(std::rand() * 2.0 / RAND_MAX - 1.0,
                                            std::rand() * 2.0 / RAND_MAX - 1.0));
    }
}

// Destructor
TerrainGenerator::~TerrainGenerator()
{
    m_randVecLookup.clear();
}

// Helper for generateTerrain()
void addPointToVector(glm::vec3 point, std::vector<float>& vector) {
    vector.push_back(point.x);
    vector.push_back(point.y);
    vector.push_back(point.z);
}

// Generates the geometry of the output triangle mesh
std::vector<float> TerrainGenerator::generateTerrain() {
    std::vector<float> verts;
    verts.reserve(m_resolutionX * m_resolutionY * 6);
    for(int x = 0; x < m_resolutionX; x++) {
        for(int y = 0; y < m_resolutionY; y++) {
            int x1 = x;
            int y1 = y;
            int x2 = x + 1;
            int y2 = y + 1;
            glm::vec3 p1 = getPosition(x1,y1);
            glm::vec3 p2 = getPosition(x2,y1);
            glm::vec3 p3 = getPosition(x2,y2);
            glm::vec3 p4 = getPosition(x1,y2);
            glm::vec3 n1 = getNormal(x1,y1);
            glm::vec3 n2 = getNormal(x2,y1);
            glm::vec3 n3 = getNormal(x2,y2);
            glm::vec3 n4 = getNormal(x1,y2);
            // tris 1
            // x1y1z1
            // x2y1z2
            // x2y2z3
            addPointToVector(p1, verts);
            addPointToVector(n1, verts);
            addPointToVector(getColor(n1, p1), verts);
            addPointToVector(p2, verts);
            addPointToVector(n2, verts);
            addPointToVector(getColor(n2, p2), verts);
            addPointToVector(p3, verts);
            addPointToVector(n3, verts);
            addPointToVector(getColor(n3, p3), verts);
            // tris 2
            // x1y1z1
            // x2y2z3
            // x1y2z4
            addPointToVector(p1, verts);
            addPointToVector(n1, verts);
            addPointToVector(getColor(n1, p1), verts);
            addPointToVector(p3, verts);
            addPointToVector(n3, verts);
            addPointToVector(getColor(n3, p3), verts);
            addPointToVector(p4, verts);
            addPointToVector(n4, verts);
            addPointToVector(getColor(n4, p4), verts);
        }
    }
    return verts;
}
// Samples the (infinite) random vector grid at (row, col)
glm::vec2 TerrainGenerator::sampleRandomVector(int row, int col)
{
    std::hash<int> intHash;
    int index = intHash(row * 41 + col * 43) % m_lookupSize;
    return m_randVecLookup.at(index);
}

// Takes a grid coordinate (row, col), [0, m_resolution), which describes a vertex in a plane mesh
// Returns a normalized position (x, y, z); x and y in range from [0, 1), and z is obtained from getHeight()
glm::vec3 TerrainGenerator::getPosition(int row, int col) {
    float scale_x = 2.0; // Adjust this scale for the x-axis
    float scale_y = 50.0; // Adjust this scale for the y-axis
    //    float scale_x = 1.0; // Adjust this scale for the x-axis
    //    float scale_y = 5.0; // Adjust this scale for the y-axis


    // Adjust the normalization to scale around the center
    float halfResX = m_resolutionX / 2.0f;
    float halfResY = m_resolutionY / 2.0f;
    float x = ((row - halfResX) * scale_x + halfResX) / m_resolutionX;
    float y = ((col - halfResY) * scale_y + halfResY) / m_resolutionY;
    float z = getHeight(x, y);
    return glm::vec3(x, y, z);
}

// Helper for computePerlin() and, possibly, getColor()
float interpolate(float A, float B, float alpha) {
    // Cubic easing function: f(t) = 3t^2 - 2t^3
    float easedAlpha = (3 * (alpha*alpha)) - (2 * (alpha*alpha*alpha));
    // Perform cubic interpolation
    return A + (easedAlpha * (B - A));
}

float TerrainGenerator::getHeight(float x, float y) {
    // Calculate the offset for the "valley"
    float xOffset = 0.0f; // No horizontal offset
    float yOffset = 0.0f; // No vertical offset
    SimplexNoise noise;
    // Apply offsets to x and y
    x += xOffset;
    y += yOffset;
    // Set a central region (the "valley") where the terrain is flat
    float centralWidth = 0.1f; // Adjust this value to control the width of the valley
    float centralHeight = -0.1f; // Adjust this value to control the depth of the valley
    if (x >= 0.5f - centralWidth / 2.0f && x <= 0.5f + centralWidth / 2.0f) {
        return centralHeight;
    }
    // Calculate a smoothstep factor based on the distance from the center of the valley
    float smoothFactor = glm::smoothstep(0.0f, centralWidth / 0.25f, abs(x - 0.5f));



    //    float totalHeight = 0.0f;
    //    float amplitude = 0.50f;
    //    float frequency = 0.75f;

    float totalHeight = 0.25f;
    float amplitude = 0.30f;
    float frequency = 1.f;



    for (int octave = 0; octave < 4; ++octave) {
        totalHeight += amplitude * noise.signedRawNoise( x * frequency, y * frequency );
        //totalHeight += amplitude * computePerlin( x * frequency, y * frequency );
        amplitude *= 0.5f;
        frequency *= 2.0f;
    }
    float interpolatedHeight = glm::mix(centralHeight, centralHeight + totalHeight, smoothFactor);
    return interpolatedHeight;
}

// Computes the normal of a vertex by averaging neighbors
glm::vec3 TerrainGenerator::getNormal(int row, int col) {
    // Task 9: Compute the average normal for the given input indices
    // TA SOLUTION
    glm::vec3 normal = glm::vec3(0, 0, 0);
    std::vector<std::vector<int>> neighborOffsets = { // Counter-clockwise around the vertex
        {-1, -1},
        { 0, -1},
        { 1, -1},
        { 1,  0},
        { 1,  1},
        { 0,  1},
        {-1,  1},
        {-1,  0}
    };
    glm::vec3 V = getPosition(row,col);
    for (int i = 0; i < 8; ++i) {
        int n1RowOffset = neighborOffsets[i][0];
        int n1ColOffset = neighborOffsets[i][1];
        int n2RowOffset = neighborOffsets[(i + 1) % 8][0];
        int n2ColOffset = neighborOffsets[(i + 1) % 8][1];
        glm::vec3 n1 = getPosition(row + n1RowOffset, col + n1ColOffset);
        glm::vec3 n2 = getPosition(row + n2RowOffset, col + n2ColOffset);
        normal = normal + glm::cross(n1 - V, n2 - V);
    }
    return glm::normalize(normal);
}

// Computes color of vertex using normal and, optionally, position
glm::vec3 TerrainGenerator::getColor(glm::vec3 normal, glm::vec3 position) {
    // Adjust these thresholds according to your terrain's height range
    float snowHeight = 0.35f; // Height above which terrain is snow
    float dirtHeight = 0.2f; // Height above which terrain is dirt and below which it is snow
    float road = -0.1f;
    // Use the z component of position to determine the height
    float height = position.z;

    if (height > snowHeight) {
        return glm::vec3(1, 1, 1); // Snow (white)
    } else if (height > dirtHeight) {
        return glm::vec3(0.7, 0.48, 0.32); // Dirt (brown)
    } else if (height >= road && height < -0.099999994f ) {
        return glm::vec3(0.38,0.38,0.38); // road
    } else {
        return glm::vec3(0.2, 0.5, 0.2); // Grass (green)
    }


//    if (settings.extraCredit1) {

//        if (height > snowHeight) {
//            return glm::vec3(.54, .6, .72); // Snow (white)
//        } else if (height > dirtHeight) {
//            return glm::vec3(0.27, 0.188, 0.11); // Dirt (brown)
//        } else if (height >= road && height < -0.099999994f ) {
//            return glm::vec3(0.22,0.22,0.22); // road
//        } else {
//            return glm::vec3(0.13, 0.30, 0.09); // Grass (green)
//        }

//    }

//    else if (settings.extraCredit2) {

//        if (height > snowHeight) {
//            return glm::vec3(.54, .6, .72); // Snow (white)
//        } else if (height > dirtHeight) {
//            return glm::vec3(0.27, 0.188, 0.11); // Dirt (brown)
//        } else if (height >= road && height < -0.099999994f ) {
//            return glm::vec3(0.22,0.22,0.22); // road
//        } else {
//            return glm::vec3(0.13, 0.30, 0.09); // Grass (green)
//        }


//    }



//    else if (settings.extraCredit3) {

//        if (height > snowHeight) {
//            return glm::vec3(.54, .6, .72); // Snow (white)
//        } else if (height > dirtHeight) {
//            return glm::vec3(0.27, 0.188, 0.11); // Dirt (brown)
//        } else if (height >= road && height < -0.099999994f ) {
//            return glm::vec3(0.22,0.22,0.22); // road
//        } else {
//            return glm::vec3(0.13, 0.30, 0.09); // Grass (green)
//        }

//    }

//    if (settings.extraCredit1) {

//        if (height > snowHeight) {
//            return glm::vec3(1, 1, 1); // Snow (white)
//        } else if (height > dirtHeight) {
//            return glm::vec3(0.7, 0.48, 0.32); // Dirt (brown)
//        } else if (height >= road && height < -0.099999994f ) {
//            return glm::vec3(0.38,0.38,0.38); // road
//        } else {
//            return glm::vec3(0.2, 0.5, 0.2); // Grass (green)
//        }

//    }

//    else if (settings.extraCredit2) {

//        if (height > snowHeight) {
//            return glm::vec3(.54, .6, .72); // Snow (white)
//        } else if (height > dirtHeight) {
//            return glm::vec3(0.27, 0.188, 0.11); // Dirt (brown)
//        } else if (height >= road && height < -0.099999994f ) {
//            return glm::vec3(0.22,0.22,0.22); // road
//        } else {
//            return glm::vec3(0.13, 0.30, 0.09); // Grass (green)
//        }

//    }

//    else if (settings.extraCredit3) {

//        if (height > snowHeight) {
//            return glm::vec3(.54, .6, .72); // Snow (white)
//        } else if (height > dirtHeight) {
//            return glm::vec3(0.27, 0.188, 0.11); // Dirt (brown)
//        } else if (height >= road && height < -0.099999994f ) {
//            return glm::vec3(0.22,0.22,0.22); // road
//        } else {
//            return glm::vec3(0.13, 0.30, 0.09); // Grass (green)
//        }

//    }


}

// Computes the intensity of Perlin noise at some point
float TerrainGenerator::computePerlin(float x, float y) {
    // Task 1: get grid indices (as ints)
    int x0 = static_cast<int>(std::floor(x));  // x top-left grid point
    int y0 = static_cast<int>(std::floor(y));  // y top-left grid point
    int x1 = x0 + 1;  // x top-right grid point
    int y1 = y0 + 1;  // y bottom-left grid point
    // Task 2: compute offset vectors
    float offsetTLx = x - static_cast<float>(x0);
    float offsetTLy = y - static_cast<float>(y0);
    float offsetTRx = x - static_cast<float>(x1);
    float offsetTRy = y - static_cast<float>(y0);
    float offsetBLx = x - static_cast<float>(x0);
    float offsetBLy = y - static_cast<float>(y1);
    float offsetBRx = x - static_cast<float>(x1);
    float offsetBRy = y - static_cast<float>(y1);
    // Task 3: compute the dot product between the grid point direction vectors and its offset vectors
    float A = offsetTLx * sampleRandomVector(x0, y0).x + offsetTLy * sampleRandomVector(x0, y0).y;
    float B = offsetTRx * sampleRandomVector(x1, y0).x + offsetTRy * sampleRandomVector(x1, y0).y;
    float C = offsetBRx * sampleRandomVector(x1, y1).x + offsetBRy * sampleRandomVector(x1, y1).y;
    float D = offsetBLx * sampleRandomVector(x0, y1).x + offsetBLy * sampleRandomVector(x0, y1).y;
    // Task 5: Debug this line to properly use your interpolation function to produce the correct value
    return interpolate(interpolate(A, B, offsetTLx), interpolate(D, C, offsetTLx), offsetTLy);
}
