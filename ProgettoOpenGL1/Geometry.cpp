#include "Geometry.h"

/**
 * @brief Carica i vertici interleaved in un VBO e configura i due attributi nel VAO.
 *
 * Layout di ogni vertice (5 float = 20 byte):
 *   - location 0: posizione 2D (x, y), 2 float, offset = 0
 *   - location 1: colore RGB (r, g, b), 3 float, offset = 2 * sizeof(float)
 * Numero di vertici: mesh.vertices.size() / 5.
 */
Geometry::Geometry(const MeshData& mesh)
{
    m_vertexCount = static_cast<GLsizei>(mesh.vertices.size() / 5);
    m_primitive = mesh.primitive;

    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_vbo);
    glBindVertexArray(m_vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, mesh.vertices.size() * sizeof(float), mesh.vertices.data(), GL_STATIC_DRAW);

    // Attributo 0: posizione (x, y)
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    // Attributo 1: colore (r, g, b)
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), reinterpret_cast<void*>(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}

/**
 * @brief Libera VBO e VAO dalla memoria GPU.
 */
Geometry::~Geometry()
{
    if (m_vbo) glDeleteBuffers(1, &m_vbo);
    if (m_vao) glDeleteVertexArrays(1, &m_vao);
}

/**
 * @brief Esegue il bind del VAO e lancia glDrawArrays con la primitiva memorizzata.
 */
void Geometry::draw() const
{
    glBindVertexArray(m_vao);
    glDrawArrays(m_primitive, 0, m_vertexCount);
}
