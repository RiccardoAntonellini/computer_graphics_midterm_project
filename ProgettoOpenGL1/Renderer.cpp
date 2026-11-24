#include "Renderer.h"
#include <glm/gtc/matrix_transform.hpp>

/**
 * @brief Imposta la matrice di proiezione ortografica 2D centrata nell'origine.
 *
 * Mappa il volume [-w, +w] x [-h, +h] x [-1, +1] in NDC [-1, +1]^3.
 */
void Renderer::setOrthoProjection(float w, float h)
{
    m_projection = glm::ortho(-w, w, -h, h, -1.0f, 1.0f);
}

/**
 * @brief Attiva lo shader standard, imposta projection e disegna tutte le istanze visibili non escluse.
 */
void Renderer::drawScene(const Scene& scene, std::size_t excludedGeometryIndex) const
{
    m_shader.use();
    m_shader.setMat4("projection", m_projection);
    for (const GeometryInstance& i : scene.instances()) {
        if (!i.visible || i.geometryIndex == excludedGeometryIndex) continue;

        m_shader.setMat4("model", i.modelMatrix);
        scene.geometry(i.geometryIndex).draw();
    }
    glBindVertexArray(0);
}
