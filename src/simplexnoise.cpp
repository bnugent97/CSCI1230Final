#include "simplexnoise.h"
#include <glm/glm.hpp>
#include <algorithm>
#include <random>

// Hash lookup table as defined by Ken Perlin
static constexpr std::uint8_t ORIGINAL_PERMUTATION[256] = {
    151, 160, 137, 91,  90,  15,  131, 13,  201, 95,  96,  53,  194, 233, 7,
    225, 140, 36,  103, 30,  69,  142, 8,   99,  37,  240, 21,  10,  23,  190,
    6,   148, 247, 120, 234, 75,  0,   26,  197, 62,  94,  252, 219, 203, 117,
    35,  11,  32,  57,  177, 33,  88,  237, 149, 56,  87,  174, 20,  125, 136,
    171, 168, 68,  175, 74,  165, 71,  134, 139, 48,  27,  166, 77,  146, 158,
    231, 83,  111, 229, 122, 60,  211, 133, 230, 220, 105, 92,  41,  55,  46,
    245, 40,  244, 102, 143, 54,  65,  25,  63,  161, 1,   216, 80,  73,  209,
    76,  132, 187, 208, 89,  18,  169, 200, 196, 135, 130, 116, 188, 159, 86,
    164, 100, 109, 198, 173, 186, 3,   64,  52,  217, 226, 250, 124, 123, 5,
    202, 38,  147, 118, 126, 255, 82,  85,  212, 207, 206, 59,  227, 47,  16,
    58,  17,  182, 189, 28,  42,  223, 183, 170, 213, 119, 248, 152, 2,   44,
    154, 163, 70,  221, 153, 101, 155, 167, 43,  172, 9,   129, 22,  39,  253,
    19,  98,  108, 110, 79,  113, 224, 232, 178, 185, 112, 104, 218, 246, 97,
    228, 251, 34,  242, 193, 238, 210, 144, 12,  191, 179, 162, 241, 81,  51,
    145, 235, 249, 14,  239, 107, 49,  192, 214, 31,  181, 199, 106, 157, 184,
    84,  204, 176, 115, 121, 50,  45,  127, 4,   150, 254, 138, 236, 205, 93,
    222, 114, 67,  29,  24,  72,  243, 141, 128, 195, 78,  66,  215, 61, 156,
    180
};

// Pseudo random Gradients for 2D space
static constexpr glm::vec2 GRADIENT_2D[9] = {
    glm::vec2(1.0f, 1.0f), glm::vec2(-1.0f, 1.0f), glm::vec2(1.0f, -1.0f),
    glm::vec2(-1.0f, -1.0f), glm::vec2(1.0f, 0.0f), glm::vec2(-1.0f, 0.0f),
    glm::vec2(0.0f, 1.0f), glm::vec2(0.0f, -1.0f), glm::vec2(1.0f, 0.0f)
};

SimplexNoise::SimplexNoise() {
    std::copy(std::begin(ORIGINAL_PERMUTATION), std::end(ORIGINAL_PERMUTATION), std::begin(permutation));
}

inline unsigned short SimplexNoise::hash(const int &i) {
    return permutation[static_cast<unsigned int>(i) & 255];
}

float SimplexNoise::signedRawNoise(const glm::vec2 &pos) {
    glm::vec2 skewedPos = pos + glm::dot(pos, glm::vec2(SKEWING_FACTOR));
    glm::ivec2 cell = glm::floor(skewedPos);

    glm::vec2 unskewedPos = glm::vec2(cell) - glm::dot(glm::vec2(cell), glm::vec2(UNSKEWING_FACTOR));
    glm::vec2 posInCell = pos - unskewedPos;

    glm::ivec2 offset = (posInCell.x > posInCell.y) ? glm::ivec2(1, 0) : glm::ivec2(0, 1);

    glm::vec2 corners[3] = { posInCell, posInCell - glm::vec2(offset) + glm::vec2(UNSKEWING_FACTOR),
                            posInCell - glm::vec2(1.0f) + glm::vec2(2.0f * UNSKEWING_FACTOR) };

    float n[3];
    for (int i = 0; i < 3; ++i) {
        glm::ivec2 gridPoint = glm::ivec2(cell) + (i < 2 ? glm::ivec2(offset * i) : glm::ivec2(1));
        int gradientIndex = hash(gridPoint.x + hash(gridPoint.y)) % 9;
        n[i] = calculateCornerValue(corners[i], GRADIENT_2D[gradientIndex]);
    }

    return 70.0f * (n[0] + n[1] + n[2]);
}

float SimplexNoise::calculateCornerValue(const glm::vec2 &pos, const glm::vec2 &gradient) {
    float t = 0.5f - glm::dot(pos, pos);

    if (t < 0.0f) {
        return 0.0f;
    }

    return glm::pow(t, 4) * glm::dot(gradient, pos);
}

void SimplexNoise::randomizeSeed() {
    setSeed(static_cast<unsigned int>(time(nullptr)));
}

void SimplexNoise::setSeed(const unsigned int &seedNumber) {
    std::shuffle(std::begin(permutation), std::end(permutation), std::default_random_engine(seedNumber));
}

float SimplexNoise::signedFBM(const glm::vec2 &pos, const unsigned int &octaves, const float &lacunarity, const float &gain) {
    float sum = 0.0f, amplitude = 1.0f, maxValue = 0.0f, frequency = 1.0f;
    for (unsigned int i = 0; i < octaves; ++i) {
        sum += signedRawNoise(pos * frequency) * amplitude;
        maxValue += amplitude;
        amplitude *= gain;
        frequency *= lacunarity;
    }
    return sum / maxValue;
}







