#include "terraingenerator.h"

#include <QVector3D>
#include <cmath>

void addPointToVector(QVector3D point, std::vector<float>& vector) {
    vector.push_back(point.x());
    vector.push_back(point.y());
    vector.push_back(point.z());
}

QVector2D TerrainGenerator::randVec(int row, int col)
{
    return m_randVecLookup.at((row * (2 + m_resolution) + col) % ((2 + m_resolution) * (2 + m_resolution)));
}

float TerrainGenerator::randVal(int row, int col)
{
    return (m_randVecLookup.at((row * (2 + m_resolution) + col) % ((2 + m_resolution) * (2 + m_resolution))).x() + 1)/2;
}

float interpolate(float A, float B, float x) {
    // Task X: implement your eased interpolation function below!

    //auto ease = [](float x) -> float { return ((x * (x * 6.0 - 15.0) + 10.0) * x * x * x);};
    auto ease = [](float x) -> float { return x * x * (3 - 2 * x);};
    //auto ease = [](float x) -> float { return x;};

    return A + ease(x) * (B - A);
}


float TerrainGenerator::computePerlin(float x, float y) {
    //Task X: TODO description
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

QVector3D TerrainGenerator::getPosition(int row, int col) {
    float x = 1.0 * row / m_resolution;
    float y = 1.0 * col / m_resolution;

    //Task X: TODO description
    float z = 0;

    float Pos2 = (computePerlin(x * 2, y * 2) / 2);
    float Pos4 = (computePerlin(x * 4, y * 4) / 4);
    float Pos8 = (computePerlin(x * 8, y * 8) / 8);
    float Pos16 = (computePerlin(x * 16, y * 16) / 16);
    float Pos32 = (computePerlin(x * 32, y * 32) / 32);

    //z = Pos2 + Pos4 + Pos8 + Pos16 + Pos32 + Pos8;
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
    //Task X: TODO description

    float mix = QVector3D::dotProduct(normal, QVector3D(0,0,1));
    float value = interpolate(0.2,1,mix);

    return QVector3D(value,value,value);
}



TerrainGenerator::TerrainGenerator()
{
    m_resolution = 250;
    m_randVecLookup.reserve((m_resolution + 2) * (m_resolution + 2));

    std::srand(1);

    for(int r = 0; r < m_resolution + 2; r++)
    {
        for(int c = 0; c < m_resolution + 2; c++)
        {
            m_randVecLookup.push_back(QVector2D(std::rand() * 2.0 / RAND_MAX - 1.0,std::rand() * 2.0 / RAND_MAX - 1.0));
        }
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

