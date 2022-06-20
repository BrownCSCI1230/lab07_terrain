#include "terraingenerator.h"

#include <QVector3D>
#include <cmath>

void addPointToVector(QVector3D point, std::vector<float>& vector) {
    vector.push_back(point.x());
    vector.push_back(point.y());
    vector.push_back(point.z());
}

QVector3D TerrainGenerator::getPosition(int x, int y) {
    return QVector3D(x,y,sin(20.0 / m_resolution * (sqrt(x*x + y*y))) / 40);
}

QVector3D TerrainGenerator::getNormal(int x, int y) {
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
    QVector3D p = getPosition(x,y);
    for (int i = 0; i < 8; i++) {
        int rdisp1 = coordlookup[2*i];
        int cdisp1 = coordlookup[(2*i + 1) % 16];

        int rdisp2 = coordlookup[2*(i + 1) % 16];
        int cdisp2 = coordlookup[(2*(i + 1) + 1) % 16];
        QVector3D p1 = getPosition(x + rdisp1,y + cdisp1);
        QVector3D p2 = getPosition(x + rdisp2,y + cdisp2);
        normal = normal + QVector3D::crossProduct(p1 - p, p2 - p);
    }
    return normal.normalized();
}



TerrainGenerator::TerrainGenerator()
{
    m_resolution = 100;
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

            addPointToVector(p2, verts);
            addPointToVector(n2, verts);

            addPointToVector(p3, verts);
            addPointToVector(n3, verts);

            //tris 2
            //x1y1z1
            //x2y2z3
            //x1y2z4
            addPointToVector(p1, verts);
            addPointToVector(n1, verts);

            addPointToVector(p3, verts);
            addPointToVector(n3, verts);

            addPointToVector(p4, verts);
            addPointToVector(n4, verts);
        }
    }
    return verts;
}

