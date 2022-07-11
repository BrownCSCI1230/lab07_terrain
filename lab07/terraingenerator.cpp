#include "terraingenerator.h"

#include <cmath>
#include "glm.hpp"

void addPointToVector(glm::vec3 point, std::vector<float>& vector) {
    vector.push_back(point.x);
    vector.push_back(point.y);
    vector.push_back(point.z);
}

glm::vec2 TerrainGenerator::randVec(int row, int col)
{
    std::hash<int> intHash;
    int index = intHash(row * 41 + col * 43) % m_lookupSize;
    return m_randVecLookup.at(index);
}

float TerrainGenerator::randVal(int row, int col)
{
    std::hash<int> intHash;
    int index = intHash(row * 11 + col * 7) % m_lookupSize;

    return (m_randVecLookup.at(index).x + 1)/2;
}

float interpolate(float A, float B, float x) {
    // Task X: implement your eased interpolation function below!

    const float pi = 3.14159265359;
    //auto ease = [](float x) -> float { return ((x * (x * 6.0 - 15.0) + 10.0) * x * x * x);};
    auto ease = [](float x) -> float { return x * x * (3 - 2 * x);};
    //auto ease = [](float x) -> float { return x;};
    //auto ease = [pi](float x) -> float {return sin(2 * pi * x) / 2.5 + x;};

    return A + ease(x) * (B - A);
}


float TerrainGenerator::computePerlin(float x, float y) {
    //Task 1: TODO description
    int intX = (int) x;
    int intY = (int) y;

    float A = glm::dot(randVec(intX, intY),
                       glm::vec2(x - intX,y - intY)) ;
    float B = glm::dot(randVec(intX + 1, intY),
                       glm::vec2(x - (intX + 1),y - intY));
    float C = glm::dot(randVec(intX + 1, intY + 1),
                       glm::vec2(x - (intX + 1),y - (intY + 1)));
    float D = glm::dot(randVec(intX, intY + 1),
                       glm::vec2(x - intX,y - (intY + 1)));

    return interpolate(interpolate(A,B,x - intX),
                       interpolate(D,C,x - intX),
                       y - intY);
}

float TerrainGenerator::computeValue(float x, float y) {
    //Task X: TODO description
    int intX = (int) x;
    int intY = (int) y;

    float A = randVal(intX, intY);
    float B = randVal(intX + 1, intY);
    float C = randVal(intX + 1, intY + 1);
    float D = randVal(intX, intY + 1);

    return interpolate(interpolate(A,B,x - intX),
                       interpolate(D,C,x - intX),
                       y - intY);
}

glm::vec3 TerrainGenerator::getPosition(int row, int col) {
    //normalize horizontal coordinates relative to unit square
    //makes scaling for sampling much more natural
    float x = 1.0 * row / m_resolution;
    float y = 1.0 * col / m_resolution;

    //Task X: TODO description
    float z = 0;

    float Pos2 = (computePerlin(x * 2, y * 2) / 2);
    float Pos4 = (computePerlin(x * 4, y * 4) / 4);
    float Pos8 = (computePerlin(x * 8, y * 8) / 8);
    float Pos16 = (computePerlin(x * 16, y * 16) / 16);
    float Pos32 = (computePerlin(x * 32, y * 32) / 32);
    float Pos64 = (computePerlin(x * 64, y * 64) / 64);
    float Pos128 = (computePerlin(x * 128, y * 128) / 128);

    //z = Pos2 + Pos4 + Pos8;
    //z = Pos2 + Pos4 + Pos8 + Pos16 + Pos32 + Pos8;
    //z = Pos32;
    z = Pos2 + Pos4 + (Pos8 + Pos16 + Pos32)* ((Pos2 + 1) / 4 + (Pos4 + 1) / 4);

    return glm::vec3(x,y,z);
}

glm::vec3 TerrainGenerator::getNormal(int row, int col) {
    //Task X: TODO description

    glm::vec3 normal = glm::vec3(0,0,0);
    int coordlookup[] = {
        -1,-1,
        0,-1,
        1,-1,
        1,0,
        1,1,
        0,1,
        -1,1,
        -1,0
    };
    glm::vec3 p = getPosition(row,col);
    for (int i = 0; i < 8; i++) {
        int rdisp1 = coordlookup[2*i];
        int cdisp1 = coordlookup[(2*i + 1) % 16];

        int rdisp2 = coordlookup[2*(i + 1) % 16];
        int cdisp2 = coordlookup[(2*(i + 1) + 1) % 16];
        glm::vec3 p1 = getPosition(row + rdisp1,col + cdisp1);
        glm::vec3 p2 = getPosition(row + rdisp2,col + cdisp2);
        normal = normal + glm::cross(p1 - p, p2 - p);
    }
    return glm::normalize(normal);
}


glm::vec3 TerrainGenerator::getColor(glm::vec3 normal, glm::vec3 position) {
    //Task X: TODO description

    float mix = glm::dot(normal, glm::vec3(0,0,1));
    float value = interpolate(0.2,1,mix);

    float heightValue = (position.z + 1)/2;
    float interpolant = 1 / (1 + pow(3,(heightValue - 0.5)*-10));

    //return glm::vec3(interpolant,interpolant,interpolant);

    return glm::vec3(value,value,value);
}



TerrainGenerator::TerrainGenerator()
{
    m_resolution = 250;
    m_lookupSize = 1024;
    m_wireshade = false;
    m_randVecLookup.reserve(m_lookupSize);

    std::srand(1);

    for(int i = 0; i < m_lookupSize; i++)
    {
        m_randVecLookup.push_back(glm::vec2(std::rand() * 2.0 / RAND_MAX - 1.0,std::rand() * 2.0 / RAND_MAX - 1.0));
    }
}

TerrainGenerator::~TerrainGenerator()
{
    m_randVecLookup.clear();
}

int TerrainGenerator::getResolution()
{
    return m_resolution;
}

std::vector<float> TerrainGenerator::generateTerrain() {
    std::vector<float> verts;
    verts.reserve(m_resolution * m_resolution * 6);

    for(int x = 0; x < m_resolution; x++) {
        for(int y = 0; y < m_resolution; y++) {
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

            //tris 1
            //x1y1z1
            //x2y1z2
            //x2y2z3
            addPointToVector(p1, verts);
            addPointToVector(n1, verts);
            addPointToVector(getColor(n1, p1), verts);

            addPointToVector(p2, verts);
            addPointToVector(n2, verts);
            addPointToVector(getColor(n2, p2), verts);

            addPointToVector(p3, verts);
            addPointToVector(n3, verts);
            addPointToVector(getColor(n3, p3), verts);

            //tris 2
            //x1y1z1
            //x2y2z3
            //x1y2z4
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

