#include <iostream>
#include <vector>
#include "cubesphere.h"

CubeSphere::CubeSphere(float radius, int sub, bool smooth) : radius(radius), subdivision(sub), smooth(smooth) {
    if (subdivision < 1) {
        subdivision = 1;
    }

    vertexCountPerRow = (unsigned int)subdivision + 1;
    vertexCountPerFace = vertexCountPerRow * vertexCountPerRow;

    if (smooth) {
        buildVerticesSmooth();
    } else {
        buildVerticesFlat();
    }
}

void CubeSphere::buildVerticesFlat() {
    std::vector<float> unitVertices = buildPositiveUnitX(vertexCountPerRow);

    unsigned int k = 0, k1, k2, i1, i2;
    float v1[3], v2[3], v3[3], v4[4];

    for(unsigned int i = 0; i < vertexCountPerRow - 1; ++i) {
        k1 = i * vertexCountPerRow;
        k2 = k1 + vertexCountPerRow;

        for(unsigned int j = 0; j < vertexCountPerRow - 1; ++j, ++k1, ++k2) {
            i1 = k1 * 3;
            i2 = k2 * 3;

            v1[0] = unitVertices[i1];
            v1[1] = unitVertices[i1 + 1];
            v1[2] = unitVertices[i1 + 2];
            v2[0] = unitVertices[i2];
            v2[1] = unitVertices[i2 + 1];
            v2[2] = unitVertices[i2 + 2];
            v3[0] = unitVertices[i1 + 3];
            v3[1] = unitVertices[i1 + 4];
            v3[2] = unitVertices[i1 + 5];
            v4[0] = unitVertices[i2 + 3];
            v4[1] = unitVertices[i2 + 4];
            v4[2] = unitVertices[i2 + 5];
        }

    }

}

// Generates the face vertices for the +X face. 
std::vector<float> CubeSphere::buildPositiveUnitX(unsigned int pointsPerRow) {
    
    // constant to convert angles from degrees to radians.
    const float DEG2RAD = acos(-1.0f) / 180.0f;

    std::vector<float> vertices; 
    float n1[3];
    float n2[3];
    float v[3];

    // Latitudnal calculation of angles based on subdivions
    for(unsigned int i = 0; i < pointsPerRow; ++i) {
        int a2 = DEG2RAD * (45.0f - 90.0f * i / (pointsPerRow - 1));
        n1[0] = -sin(a2);
        n1[1] = cos(a2);
        n1[2] = 0;

        // Longitudnal calculation of angles based on subdivisions
        for(unsigned int j = 0; j < pointsPerRow; ++j) {
            int a1 = DEG2RAD * (-45.0f - 90.0f * i / (pointsPerRow - 1));
            n2[0] = -sin(a1);
            n2[1] = 0;
            n2[2] = -cos(a1);
        }

        // Cross product of the latitude and longitude vectors.
        v[0] = n1[1] * n2[2] - n1[2] * n2[1];
        v[1] = n1[2] * n2[0] - n1[0] * n2[2];
        v[2] = n1[0] * n2[1] - n1[1] * n2[0];

        // Scale vectors for length
        float scale = CubeSphere::computeScaleForLength(v, 1);
        v[0] *= scale;
        v[1] *= scale;
        v[2] *= scale;

        vertices.push_back(v[0]);
        vertices.push_back(v[1]);
        vertices.push_back(v[2]);
    }  

    return vertices;
}

float CubeSphere::computeScaleForLength(const float v[3], int length) {
    return length / sqrt(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]);
}
