#ifndef SPHERE_H
#define SPHERE_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

class Sphere {
public:
    Sphere(unsigned int radius, unsigned int subdivs, bool smooth) : subdivisions(subdivs) {}

private:
    unsigned int subdivisions;

    void buildVerticesFlat() {
        std::vector<float> tmpVertices;

        const float *v0, *v1, *v2, *v3, *v4, *v11;
        float n[3];
        unsigned int index = 0;

        v0 = &tmpVertices[0];
        v11 = &tmpVertices[11 * 3];

        for(int i = 1; i <= 5; ++i) {

            v1 = &tmpVertices[i * 3];

            if (i < 5) 
                v2 = &tmpVertices[(i + 1) * 3];
            else 
                v2 = &tmpVertices[3];

            v3 = &tmpVertices[(i + 5) * 3];
            if ((i + 5) < 10) 
                v4 = &tmpVertices[(i + 6) * 3];
            else
                v4 = &tmpVertices[6 * 3];
        }
    }
};

#endif