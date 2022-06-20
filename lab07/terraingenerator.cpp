#include "terraingenerator.h"

TerrainGenerator::TerrainGenerator()
{

}

TerrainGenerator::~TerrainGenerator()
{

}

std::vector<float> TerrainGenerator::generateTerrain() {
    std::vector<float> verts =
    { //     COORDINATES     /        NORMALS      //
        -0.5f, 1.0f,  0.5f,     0.83f, 0.70f, 0.44f,
        -0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,
         0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,
         //0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,
         //0.0f, 0.8f,  0.0f,     0.92f, 0.86f, 0.76f
        -0.5f, 1.0f,  0.5f,     0.83f, 0.70f, 0.44f,
         0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,
        -0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f
    };
    return verts;
}
