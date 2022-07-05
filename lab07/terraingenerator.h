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
    bool m_wireshade;

private:
    std::vector<QVector2D> m_randVecLookup;
    float randVal(int row, int col);
    QVector2D randVec(int row, int col);
    int m_resolution;
    QVector3D getPosition(int x, int y);
    QVector3D getNormal(int x, int y);
    QVector3D getColor(QVector3D normal,QVector3D position);
    float computePerlin(float x, float y);
    float computeValue(float x, float y);
};

#endif // TERRAINGENERATOR_H
