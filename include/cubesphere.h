#ifndef CUBESPHERE_H
#define CUBESPHERE_H

#include <vector>
#include <cmath>

class CubeSphere {

public:
    CubeSphere(float radius = 1.0f, int subdivision = 3, bool smooth = true);

private:
    float computeScaleForLength(const float v[3], int length);
    std::vector<float> buildPositiveUnitX(unsigned int pointsPerRow);
    void buildVerticesFlat();
    void buildVerticesSmooth();

    float radius;
    float subdivision;
    bool smooth;

    std::vector<float> Vertices;
    std::vector<float> Normals;
    std::vector<unsigned int> Indices;

    unsigned int vertexCountPerRow;
    unsigned int vertexCountPerFace;
};

#endif