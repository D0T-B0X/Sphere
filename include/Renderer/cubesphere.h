#ifndef CUBESPHERE_H
#define CUBESPHERE_H

#include <vector>
#include <cmath>

#define NEG -1.0f          // Negative face direction
#define POS  1.0f          // Positive face direction

// Generates a sphere by subdividing and projecting cube faces
class CubeSphere {
public:
    CubeSphere();                               // Constructs with default radius/subdivisions
    CubeSphere(float radius);                   // Constructs with given radius
    CubeSphere(float radius, unsigned int subs);// Constructs with given radius and subdivisions

    void setRadius(float radius);               // Sets sphere radius and regenerates
    void setSubdivisions(unsigned int subs);    // Sets subdivision count and regenerates

    const float* getVertexData() const;         // Returns pointer to vertex array (positions)
    const size_t getVertexDataSize() const;     // Returns vertex data size in bytes
    const unsigned int* getIndexData() const;   // Returns pointer to index array
    const size_t getIndexCount() const;         // Returns number of indices
    const size_t getIndexDataSize() const;      // Returns index data size in bytes
    const unsigned int getSubdivisions() const; // Returns current subdivision count
    const float getRadius() const;              // Returns current radius

private:
    // Face axis identifiers
    typedef enum face {
        X = 0,
        Y = 1,
        Z = 2
    } Face;

    float Radius;                // Sphere radius
    unsigned int Subdivisions;   // Subdivision level per cube edge
    unsigned int verticesPerRow; // Vertices per row on one face
    unsigned int verticesPerFace;// Total vertices on one face

    std::vector<float> Vertices;           // Interleaved vertex positions (x,y,z)
    std::vector<unsigned int> Indices;     // Triangle indices

    void buildVertices();                  // Builds all face vertex positions
    void calculateIndices();               // Builds index list for faces
    void normalizeVectors(const float v[3], float n[3]); // Normalizes a 3D vector
    float* scaleVectors(float v[3], float radius);       // Scales a vector by radius
    void addVertices(const float n[3]);   // Appends one vertex position
    void addIndices(const unsigned int i[3]); // Appends one triangle indices
    void clearArrays();                   // Clears vertex and index storage
    void generateSphere();                // Regenerates full sphere data
    std::vector<float> buildFaceVertices(Face face, float sign); // Builds one face grid
};

#endif