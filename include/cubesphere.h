#ifndef CUBESPHERE_H
#define CUBESPHERE_H

#include <vector>

#define NEG -1.0f
#define POS 1.0f

class CubeSphere {
public:
    CubeSphere(float radius, unsigned int subs);
    void setRadius(float radius);
    void setSubdivisions(unsigned int subs);
    const float* getVertexData() const;
    const size_t getVertexDataSize() const;
    const unsigned int* getIndexData() const;
    const size_t getIndexCount() const;
    const size_t getIndexDataSize() const;
    const unsigned int getSubdivisions() const;
    const float getRadius() const;

private:

    // Face enum
    typedef enum face {
        X = 0,
        Y = 1,
        Z = 2
    } Face;

    // Variables 
    float Radius;
    unsigned int Subdivisions;
    unsigned int verticesPerRow;
    unsigned int verticesPerFace;

    std::vector<float> Vertices;
    std::vector<unsigned int> Indices;

    // Functions
    void buildVertices();
    void calculateIndices();
    void normalizeVectors(const float v[3], float n[3]);
    float* scaleVectors(float v[3], float radius);
    void addVertices(const float n[3]);
    void addIndices(const unsigned int i[3]);
    void clearArrays();
    void generateSphere();
    std::vector<float> buildFaceVertices(Face face, float sign);
};

#endif