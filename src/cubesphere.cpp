#include <cmath>
#include "cubesphere.h"

// Constructor for the CubeSphere class 
CubeSphere::CubeSphere(float radius, unsigned int subs) : Radius(radius), Subdivisions(subs) {
    generateSphere();
}

// Setter functions
void CubeSphere::setRadius(float radius) {
    Radius = radius;
    generateSphere();
}

void CubeSphere::setSubdivisions(unsigned int subs) {
    Subdivisions = subs;
    generateSphere();
}

// Getter functions
const float* CubeSphere::getVertexData() const {
    return Vertices.data();
}

const unsigned int* CubeSphere::getIndexData() const{
    return Indices.data();
}

const size_t CubeSphere::getVertexDataSize() const {
    return Vertices.size() * sizeof(float);
}

const size_t CubeSphere::getIndexDataSize() const {
    return Indices.size() * sizeof(unsigned int);
}

const size_t CubeSphere::getIndexCount() const {
    return Indices.size();
}

const unsigned int CubeSphere::getSubdivisions() const {
    return Subdivisions;
}

const float CubeSphere::getRadius() const {
    return Radius;
}

void CubeSphere::buildVertices() {
    float n[3];
    float tmpV[3];

    // Sphere face for +X.
    std::vector<float> vertices;

    // Extract and Normalize each vertex's coordinates
    for(unsigned int face = 0; face < 6; ++face) {
        switch (face) {
            case 0: vertices = buildFaceVertices(Face::X, POS); break;
            case 1: vertices = buildFaceVertices(Face::X, NEG); break;
            case 2: vertices = buildFaceVertices(Face::Y, POS); break;
            case 3: vertices = buildFaceVertices(Face::Y, NEG); break;
            case 4: vertices = buildFaceVertices(Face::Z, POS); break;
            case 5: vertices = buildFaceVertices(Face::Z, NEG); break;
        }

        for(unsigned int i = 0; i < verticesPerFace; ++i) {
            tmpV[0] = vertices[3 * i];
            tmpV[1] = vertices[3 * i + 1];
            tmpV[2] = vertices[3 * i + 2];

            normalizeVectors(tmpV, n);
            scaleVectors(n, Radius);
            addVertices(n);
        }
    }
}

std::vector<float> CubeSphere::buildFaceVertices(Face face, float sign) {
    std::vector<float> vertices;

    // fixed, horizontal and vertical axes.
    int fixedAxis, hAxis, vAxis;

    // specify axis parameters based on the face being worked on.
    switch (face) {
        case Face::X : fixedAxis = 0; vAxis = 1; hAxis = 2; break;
        case Face::Y : fixedAxis = 1; vAxis = 2; hAxis = 0; break;
        case Face::Z : fixedAxis = 2; vAxis = 1; hAxis = 0; break;
    }

    for(unsigned int i = 0; i < verticesPerRow; ++i) {
        float v[3];

        // Specify the sign and value of the face 
        // which is being contructed.
        v[fixedAxis] = sign;

        // Vertex for the vertical edges.
        v[vAxis] = 1.0f - ((2.0f / Subdivisions) * i);

        for(unsigned int j = 0; j < verticesPerRow; ++j) {

            // Vertex for the horizontal edges.
            v[hAxis] = -1.0f + ((2.0f / Subdivisions) * j);

            vertices.push_back(v[0]);
            vertices.push_back(v[1]);
            vertices.push_back(v[2]);
        }
    }

    return vertices;
}

void CubeSphere::calculateIndices() {

    // top left, top right, bottom left, 
    // bottom right indices.
    unsigned int tl, tr, bl, br;

    // temp arrays to store indices
    unsigned int i1[3], i2[3];

    for(unsigned int face = 0; face < 6; ++face) {
        unsigned int faceIndex = face * verticesPerFace;

        for(unsigned int i = 0; i < Subdivisions; ++i) {
            for(unsigned int j = 0; j < Subdivisions; ++j) {
                tl = (i * verticesPerRow + j) + faceIndex;
                tr = tl + 1;
                bl = ((i + 1) * verticesPerRow + j) + faceIndex;
                br = bl + 1;

                // CCW configuration of vertices stored
                // as indices.
                i1[0] = tl; i1[1] = bl; i1[2] = br;
                i2[0] = tl; i2[1] = br; i2[2] = tr;

                // Push indices to the final array.
                addIndices(i1);
                addIndices(i2);
            }
        }
    }
}

// Add Normalized vertex data to the final vertices array.
void CubeSphere::addVertices(const float n[3]) {
    Vertices.push_back(n[0]);
    Vertices.push_back(n[1]);
    Vertices.push_back(n[2]);
}

// Add the final indices required to construct the sphere.
void CubeSphere::addIndices(const unsigned int i[3]) {
    Indices.push_back(i[0]);
    Indices.push_back(i[1]);
    Indices.push_back(i[2]);
}

float* CubeSphere::scaleVectors(float v[3], float radius) {
    v[0] *= radius;
    v[1] *= radius;
    v[2] *= radius;

    return v;
}

// Normalize vertices to form the sphere. 
void CubeSphere::normalizeVectors(const float v[3], float n[3]) {
    const float EPSILON = 0.000001f;

    float mag = sqrt(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]);

    if (mag > EPSILON) {
        float inverse = 1.0f / mag;
        n[0] = v[0] * inverse;
        n[1] = v[1] * inverse;
        n[2] = v[2] * inverse;
    }
}

// This function makes the final call 
// to generate the required vertices 
// and indices for the sphere.
void CubeSphere::generateSphere() {

    clearArrays();

    if (Subdivisions < 1) {
        Subdivisions = 1;
    }

    if (Radius < 0.0000001f) {
        Radius = 0.0000001f;
    }

    verticesPerRow = Subdivisions + 1;
    verticesPerFace = verticesPerRow * verticesPerRow;

    buildVertices();
    calculateIndices();
}

void CubeSphere::clearArrays() {
    Vertices.clear();
    Indices.clear();
}
