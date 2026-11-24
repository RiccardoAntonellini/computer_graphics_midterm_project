#include "BulletRenderer.h"
#include "Bullets.h"
#include <glad/glad.h>

using namespace BulletConfig;

/**
 * @brief Renderizza le istanze dei bullets passando allo shader `bullet.vert` i parametri dell'oscillazione.
 *
 * Imposta una volta per frame le uniform condivise (projection, uTime, uAmplitude, uFrequency)
 * e, per ciascuna istanza visibile, aggiorna lo sfasamento uPhase = i * PHASE_STEP e la model.
 */
void BulletRenderer::draw(const Scene& scene, const std::vector<std::size_t>& instances,
    const glm::mat4& projection, float timeSeconds) const
{
    m_shader.use();
    m_shader.setMat4("projection", projection);
    m_shader.setFloat("uTime", timeSeconds);
    m_shader.setFloat("uAmplitude", BOB_AMPLITUDE);
    m_shader.setFloat("uFrequency", BOB_FREQUENCY);

    for (std::size_t i = 0; i < instances.size(); ++i) {
        const GeometryInstance& instance = scene.instances().at(instances[i]);
        if (!instance.visible) continue;

        m_shader.setFloat("uPhase", static_cast<float>(i) * PHASE_STEP);
        m_shader.setMat4("model", instance.modelMatrix);
        scene.geometry(instance.geometryIndex).draw();
    }
    glBindVertexArray(0);
}
