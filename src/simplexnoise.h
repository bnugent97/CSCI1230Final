//#ifndef SIMPLEXNOISE_H
//#define SIMPLEXNOISE_H

//#include <glm/glm.hpp>
//#include <random>
//#include <ctime>

//class SimplexNoise {
//public:
//    explicit SimplexNoise();

//    void randomizeSeed();
//    void setSeed(const unsigned int &seedNumber);
//    float signedFBM(const glm::vec2 &pos, const unsigned int &octaves, const float &lacunarity, const float &gain);
//    float signedRawNoise(const glm::vec2 &pos);
//    float unsignedFBM(const glm::vec2 &pos, const unsigned int &octaves, const float &lacunarity, const float &gain);
//    float unsignedRawNoise(const glm::vec2 &pos);

//private:
//    static constexpr float SKEWING_FACTOR = 0.366025f; // 0.5 * (sqrt(3.0f) - 1.0f)
//    static constexpr float UNSKEWING_FACTOR = 0.211325f; // (3.0f - sqrt(3.0f)) / 6.0f

//    std::uint8_t permutation[256];

//    float calculateCornerValue(const glm::vec2 &pos, const glm::vec2 &gradient);
//    int fastFloor(const float &x);
//    unsigned short hash(const int &i);
//};

//#endif // SIMPLEXNOISE_H





#ifndef SIMPLEXNOISE_H
#define SIMPLEXNOISE_H

#include <algorithm>
#include <cmath>
#include <ctime>
#include <random>

class SimplexNoise {
public:
    explicit SimplexNoise();

    void randomizeSeed();
    void setSeed(const unsigned int &seedNumber);
    double signedFBM(const double &xPos, const double &yPos, const unsigned int &octaves, const double &lacunarity, const double &gain);
    double signedRawNoise(const double &xPos, const double &yPos);
    double unsignedFBM(const double &xPos, const double &yPos, const unsigned int &octaves, const double &lacunarity, const double &gain);
    double unsignedRawNoise(const double &xPos, const double &yPos);

private:
    static constexpr double SKEWING_FACTOR = 0.366025; // 0.5 * ( std::sqrt( 3.0 ) - 1.0 )
    static constexpr double UNSKEWING_FACTOR = 0.211325; // ( 3.0 - std::sqrt( 3.0 )) / 6.0

    std::uint8_t permutation[256];

    double calculateCornerValue(const double &x, const double &y, const int &gradientIndex);
    double dot(std::pair< double, double > gradient2D, const double &x, const double &y);
    int fastFloor(const double &x);
    unsigned short hash(const int &i);
};

#endif // SIMPLEXNOISE_H
