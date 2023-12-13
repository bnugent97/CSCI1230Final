#ifndef SIMPLEXNOISE_H
#define SIMPLEXNOISE_H

#include <glm/glm.hpp>
#include <random>
#include <ctime>

class SimplexNoise {
public:
    explicit SimplexNoise();

    void randomizeSeed();
    void setSeed(const unsigned int &seedNumber);
    float signedFBM(const glm::vec2 &pos, const unsigned int &octaves, const float &lacunarity, const float &gain);
    float signedRawNoise(const glm::vec2 &pos);
    float unsignedFBM(const glm::vec2 &pos, const unsigned int &octaves, const float &lacunarity, const float &gain);
    float unsignedRawNoise(const glm::vec2 &pos);

private:
    static constexpr float SKEWING_FACTOR = 0.366025f; // 0.5 * (sqrt(3.0f) - 1.0f)
    static constexpr float UNSKEWING_FACTOR = 0.211325f; // (3.0f - sqrt(3.0f)) / 6.0f

    std::uint8_t permutation[256];

    float calculateCornerValue(const glm::vec2 &pos, const glm::vec2 &gradient);
    int fastFloor(const float &x);
    unsigned short hash(const int &i);
};

#endif // SIMPLEXNOISE_H






