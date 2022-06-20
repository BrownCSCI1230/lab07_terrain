#ifndef TERRAINGENERATOR_H
#define TERRAINGENERATOR_H

#include <vector>

class TerrainGenerator
{
public:
    TerrainGenerator();
    ~TerrainGenerator();
    std::vector<float> generateTerrain();
};

#endif // TERRAINGENERATOR_H
