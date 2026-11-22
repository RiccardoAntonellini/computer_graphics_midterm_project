#pragma once
#include "MeshData.h"
#include <glad/glad.h>

/**
 * @class Geometry
 * @brief Incapsula i buffer OpenGL (VAO e VBO) di una singola mesh immutabile in GPU.
 *
 * Implementa la separazione tra dati geometrici condivisi e istanze: una singola `Geometry`
 * carica i vertici una volta sola in memoria video e può essere referenziata da più istanze
 * nella `Scene`, ciascuna con la propria Model Matrix.
 * Gestisce le risorse GPU tramite RAII (copia disabilitata).
 */
class Geometry {
public:
    /**
     * @brief Crea VAO e VBO in OpenGL e carica i vertici definiti in `mesh`.
     * @param mesh Dati geometrici lato CPU (vertici, colori/attributi e tipo di primitiva).
     */
    explicit Geometry(const MeshData& mesh);

    /**
     * @brief Rilascia VAO e VBO dalla memoria GPU.
     */
    ~Geometry();

    Geometry(const Geometry&) = delete;
    Geometry& operator=(const Geometry&) = delete;

    /**
     * @brief Esegue il bind del VAO e lancia la draw call OpenGL (`glDrawArrays`).
     */
    void draw() const;

private:
    GLuint m_vao = 0;
    GLuint m_vbo = 0;
    GLsizei m_vertexCount = 0;
    GLenum m_primitive = GL_TRIANGLES;
};
