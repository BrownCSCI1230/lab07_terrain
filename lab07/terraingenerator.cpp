#include "terraingenerator.h"

#include <QVector3D>
#include <cmath>

void addPointToVector(QVector3D point, std::vector<float>& vector) {
    vector.push_back(point.x());
    vector.push_back(point.y());
    vector.push_back(point.z());
}


float interpolate(float A, float B, float x) {
    auto ease = [](float x) -> float { return ((x * (x * 6.0 - 15.0) + 10.0) * x * x * x);};
    //auto ease = [](float x) -> float { return x * x * (3 - 2 * x);};
    //auto ease = [](float x) -> float { return x;};

    return A + ease(x) * (B - A);
}

QVector2D randVec(int row, int col)
{
    double integralPart;
    float x = modf(sin(row * 127506.1f + col * 611046.7f) * 4375870.5453123f, &integralPart);
    float y = modf(sin(row * 9816.8f + col * 1687.4f) * 851064.379022f, &integralPart);

    return QVector2D(x,y);
}

float TerrainGenerator::computePerlin(float x, float y) {
    //Task
    int intX = (int) x;
    int intY = (int) y;

    float A = QVector2D::dotProduct(randVec(intX, intY),
                                        QVector2D(x - intX,y - intY)) ;
    float B = QVector2D::dotProduct(randVec(intX + 1, intY),
                                    QVector2D(x - (intX + 1),y - intY));
    float C = QVector2D::dotProduct(randVec(intX + 1, intY + 1),
                                    QVector2D(x - (intX + 1),y - (intY + 1)));
    float D = QVector2D::dotProduct(randVec(intX, intY + 1),
                                    QVector2D(x - intX,y - (intY + 1)));

    return interpolate(interpolate(A,B,x - intX),
                       interpolate(D,C,x - intX),
                       y - intY);
}

QVector3D TerrainGenerator::getPosition(int row, int col) {
    float x = 1.0 * row / m_resolution;
    float y = 1.0 * col / m_resolution;

    // Task X: compute the zPos using your compute Perlin Method
    float z = 0;

    float Pos2 = (computePerlin(x * 2, y * 2) / 2);
    float Pos4 = (computePerlin(x * 4, y * 4) / 4);
    float Pos8 = (computePerlin(x * 8, y * 8) / 8);
    float Pos16 = (computePerlin(x * 16, y * 16) / 16);
    float Pos32 = (computePerlin(x * 32, y * 32) / 32);

    z = Pos2 + Pos4 + (Pos8 + Pos16 + Pos32)* ((Pos2 + 1) / 4 + (Pos4 + 1) / 4);

    return QVector3D(x,y,z);
}

QVector3D TerrainGenerator::getNormal(int row, int col) {
    //Task X: TODO description

    QVector3D normal = QVector3D(0,0,0);
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
    QVector3D p = getPosition(row,col);
    for (int i = 0; i < 8; i++) {
        int rdisp1 = coordlookup[2*i];
        int cdisp1 = coordlookup[(2*i + 1) % 16];

        int rdisp2 = coordlookup[2*(i + 1) % 16];
        int cdisp2 = coordlookup[(2*(i + 1) + 1) % 16];
        QVector3D p1 = getPosition(row + rdisp1,col + cdisp1);
        QVector3D p2 = getPosition(row + rdisp2,col + cdisp2);
        normal = normal + QVector3D::crossProduct(p1 - p, p2 - p);
    }
    return normal.normalized();
}


QVector3D TerrainGenerator::getColor(QVector3D normal) {
    //Task X: using the normal, dot product, and your interpolation function,
    //        return the color of the terrain

    float mix = QVector3D::dotProduct(normal, QVector3D(0,0,1));
    float value = interpolate(0.3,1,mix);

    return QVector3D(value,value,value);
}



TerrainGenerator::TerrainGenerator()
{
    //Set resolution for terrain generation
    m_resolution = 250;
}

TerrainGenerator::~TerrainGenerator()
{

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

            QVector3D p1 = getPosition(x1,y1);
            QVector3D p2 = getPosition(x2,y1);
            QVector3D p3 = getPosition(x2,y2);
            QVector3D p4 = getPosition(x1,y2);

            QVector3D n1 = getNormal(x1,y1);
            QVector3D n2 = getNormal(x2,y1);
            QVector3D n3 = getNormal(x2,y2);
            QVector3D n4 = getNormal(x1,y2);

            //tris 1
            //x1y1z1
            //x2y1z2
            //x2y2z3
            addPointToVector(p1, verts);
            addPointToVector(n1, verts);
            addPointToVector(getColor(n1), verts);

            addPointToVector(p2, verts);
            addPointToVector(n2, verts);
            addPointToVector(getColor(n2), verts);

            addPointToVector(p3, verts);
            addPointToVector(n3, verts);
            addPointToVector(getColor(n3), verts);

            //tris 2
            //x1y1z1
            //x2y2z3
            //x1y2z4
            addPointToVector(p1, verts);
            addPointToVector(n1, verts);
            addPointToVector(getColor(n1), verts);

            addPointToVector(p3, verts);
            addPointToVector(n3, verts);
            addPointToVector(getColor(n3), verts);

            addPointToVector(p4, verts);
            addPointToVector(n4, verts);
            addPointToVector(getColor(n4), verts);
        }
    }
    return verts;
}

