#pragma once
#include <vector>
#include <glad/glad.h>

/**
 * @struct MeshData
 * @brief Contenitore lato CPU dei dati di una mesh 2D con attributi interleaved.
 *
 * Layout di ogni vertice nel vettore vertices (5 float per vertice, stride = 5 * sizeof(float)):
 *   [ x, y, r, g, b ]
 * - (x, y): posizione in coordinate locali (attribute location 0).
 * - (r, g, b): colore RGB normalizzato in [0, 1] (attribute location 1).
 */
struct MeshData {
    std::vector<float> vertices;// Buffer interleaved di vertici non indicizzati.
    GLenum primitive = GL_TRIANGLES;// Modalità di assemblaggio primitive OpenGL (es. GL_TRIANGLES, GL_TRIANGLE_STRIP).
};
