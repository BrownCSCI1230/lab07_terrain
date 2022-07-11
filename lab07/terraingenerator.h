#ifndef TERRAINGENERATOR_H
#define TERRAINGENERATOR_H

#include <vector>
#include "glm.hpp"

class TerrainGenerator
{
public:
    bool m_wireshade;

    TerrainGenerator();
    ~TerrainGenerator();
    std::vector<float> generateTerrain();
    int getResolution();

private:
    std::vector<glm::vec2> m_randVecLookup;
    int m_resolution;
    int m_lookupSize;

    glm::vec2 randVec(int row, int col);
    glm::vec3 getPosition(int x, int y);
    glm::vec3 getNormal(int x, int y);
    glm::vec3 getColor(glm::vec3 normal,glm::vec3 position);
    float computePerlin(float x, float y);
};

#endif // TERRAINGENERATOR_H
