#include "Renderer/cubesphere.h"

// Default constructor: creates sphere with radius 1 and 16 subdivisions
CubeSphere::CubeSphere() : Radius(1.0f), Subdivisions(16) {
    generateSphere();
}

// Constructor with custom radius (default subdivisions 16)
CubeSphere::CubeSphere(float radius) : Radius(radius), Subdivisions(16) {
    generateSphere();
}

// Constructor with custom radius and subdivisions
CubeSphere::CubeSphere(float radius, unsigned int subs) : Radius(radius), Subdivisions(subs) {
    generateSphere();
}

// Set sphere radius and rebuild geometry
void CubeSphere::setRadius(float radius) {
    Radius = radius;
    generateSphere();
}

// Set subdivision level and rebuild geometry
void CubeSphere::setSubdivisions(unsigned int subs) {
    Subdivisions = subs;
    generateSphere();
}

// Return pointer to vertex buffer (positions)
const float* CubeSphere::getVertexData() const {
    return Vertices.data();
}

// Return pointer to index buffer
const unsigned int* CubeSphere::getIndexData() const{
    return Indices.data();
}

// Return vertex buffer size in bytes
const size_t CubeSphere::getVertexDataSize() const {
    return Vertices.size() * sizeof(float);
}

// Return index buffer size in bytes
const size_t CubeSphere::getIndexDataSize() const {
    return Indices.size() * sizeof(unsigned int);
}

// Return total number of indices
const size_t CubeSphere::getIndexCount() const {
    return Indices.size();
}

// Return current subdivision level
const unsigned int CubeSphere::getSubdivisions() const {
    return Subdivisions;
}

// Return current radius
const float CubeSphere::getRadius() const {
    return Radius;
}

// Build all vertex positions by projecting cube faces to a sphere
void CubeSphere::buildVertices() {
    float n[3];
    float tmpV[3];
    std::vector<float> vertices; // per-face temporary positions

    // Process each of the 6 cube faces
    for(unsigned int face = 0; face < 6; ++face) {
        switch (face) {
            case 0: vertices = buildFaceVertices(Face::X, POS); break;
            case 1: vertices = buildFaceVertices(Face::X, NEG); break;
            case 2: vertices = buildFaceVertices(Face::Y, POS); break;
            case 3: vertices = buildFaceVertices(Face::Y, NEG); break;
            case 4: vertices = buildFaceVertices(Face::Z, POS); break;
            case 5: vertices = buildFaceVertices(Face::Z, NEG); break;
        }

        // Normalize each face vertex and scale to sphere radius
        for(unsigned int i = 0; i < verticesPerFace; ++i) {
            tmpV[0] = vertices[3 * i];
            tmpV[1] = vertices[3 * i + 1];
            tmpV[2] = vertices[3 * i + 2];

            normalizeVectors(tmpV, n);  // direction (unit)
            scaleVectors(n, Radius);    // scale to radius
            addVertices(n);             // store position
        }
    }
}

// Generate grid vertices for a single cube face
std::vector<float> CubeSphere::buildFaceVertices(Face face, float sign) {
    std::vector<float> vertices;
    int fixedAxis, hAxis, vAxis;

    // Select axes for this face
    switch (face) {
        case Face::X : fixedAxis = 0; vAxis = 1; hAxis = 2; break;
        case Face::Y : fixedAxis = 1; vAxis = 2; hAxis = 0; break;
        case Face::Z : fixedAxis = 2; vAxis = 1; hAxis = 0; break;
    }

    // Iterate grid (rows = vertical, columns = horizontal)
    for(unsigned int i = 0; i < verticesPerRow; ++i) {
        float v[3];
        v[fixedAxis] = sign;                                          // face plane
        v[vAxis]     = 1.0f - ((2.0f / Subdivisions) * i);            // vertical position

        for(unsigned int j = 0; j < verticesPerRow; ++j) {
            v[hAxis] = -1.0f + ((2.0f / Subdivisions) * j);           // horizontal position
            vertices.push_back(v[0]);
            vertices.push_back(v[1]);
            vertices.push_back(v[2]);
        }
    }

    return vertices;
}

// Build triangle indices for all faces
void CubeSphere::calculateIndices() {
    unsigned int tl, tr, bl, br;    // quad corners
    unsigned int i1[3], i2[3];      // two triangles per quad

    // Iterate faces
    for(unsigned int face = 0; face < 6; ++face) {
        unsigned int faceIndex = face * verticesPerFace;

        // Iterate quads on face
        for(unsigned int i = 0; i < Subdivisions; ++i) {
            for(unsigned int j = 0; j < Subdivisions; ++j) {
                tl = (i * verticesPerRow + j) + faceIndex;
                tr = tl + 1;
                bl = ((i + 1) * verticesPerRow + j) + faceIndex;
                br = bl + 1;

                // Triangle 1 (CCW)
                i1[0] = tl; i1[1] = bl; i1[2] = br;
                // Triangle 2 (CCW)
                i2[0] = tl; i2[1] = br; i2[2] = tr;

                addIndices(i1);
                addIndices(i2);
            }
        }
    }
}

// Append one vertex position
void CubeSphere::addVertices(const float n[3]) {
    Vertices.push_back(n[0]);
    Vertices.push_back(n[1]);
    Vertices.push_back(n[2]);
}

// Append one triangle (3 indices)
void CubeSphere::addIndices(const unsigned int i[3]) {
    Indices.push_back(i[0]);
    Indices.push_back(i[1]);
    Indices.push_back(i[2]);
}

// Scale a 3D vector by radius
float* CubeSphere::scaleVectors(float v[3], float radius) {
    v[0] *= radius;
    v[1] *= radius;
    v[2] *= radius;
    return v;
}

// Normalize a 3D vector (safe check for near-zero magnitude)
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

// Regenerate all sphere data (vertices + indices)
void CubeSphere::generateSphere() {
    clearArrays();

    if (Subdivisions < 1) {
        Subdivisions = 1;
    }
    if (Radius < 0.0000001f) {
        Radius = 0.0000001f;
    }

    verticesPerRow  = Subdivisions + 1;
    verticesPerFace = verticesPerRow * verticesPerRow;

    buildVertices();
    calculateIndices();
}

// Clear vertex and index storage
void CubeSphere::clearArrays() {
    Vertices.clear();
    Indices.clear();
}
