#ifndef TERRAINGENERATOR_H
#define TERRAINGENERATOR_H

#include <vector>
#include <QVector3D>

class TerrainGenerator
{
public:
    TerrainGenerator();
    ~TerrainGenerator();
    std::vector<float> generateTerrain();
    int getResolution();

private:
    int m_resolution;
    QVector3D getPosition(int x, int y);
    QVector3D getNormal(int x, int y);
};

#endif // TERRAINGENERATOR_H
