#ifndef TERRAINGENERATOR_H
#define TERRAINGENERATOR_H

#include <vector>
#include <QVector3D>

class TerrainGenerator
{
public:
    bool m_wireshade;

    TerrainGenerator();
    ~TerrainGenerator();
    std::vector<float> generateTerrain();
    int getResolution();

private:
    std::vector<QVector2D> m_randVecLookup;
    int m_resolution;
    int m_lookupSize;

    float randVal(int row, int col);
    QVector2D randVec(int row, int col);
    QVector3D getPosition(int x, int y);
    QVector3D getNormal(int x, int y);
    QVector3D getColor(QVector3D normal,QVector3D position);
    float computePerlin(float x, float y);
    float computeValue(float x, float y);
};

#endif // TERRAINGENERATOR_H
