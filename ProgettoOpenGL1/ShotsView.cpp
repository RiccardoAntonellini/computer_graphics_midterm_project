#include "ShotsView.h"
#include "ShapeGenerator.h"
#include "TransformConfig.h"

using namespace TransformConfig;

/**
 * @brief Crea una singola Geometry disco (raggio unitario) condivisa da tutte le istanze del pool.
 */
ShotsView::ShotsView(Scene& scene)
{
    const std::size_t geometry = scene.addGeometry(
        ShapeGenerator::disk(1.0f, { 1.0f, 0.90f, 0.30f }, 16));

    for (int i = 0; i < ShotConfig::POOL_SIZE; ++i)
        m_instances.push_back(scene.addInstance(geometry));
}

/**
 * @brief Aggiorna visibilità e matrice TRS (traslazione + scala `RADIUS`) di ogni colpo attivo.
 */
void ShotsView::update(Scene& scene, const Shots& shots)
{
    for (std::size_t i = 0; i < m_instances.size(); ++i) {
        const Shot& shot = shots.all()[i];
        scene.instance(m_instances[i]).visible = shot.active;
        if (shot.active)
            scene.instance(m_instances[i]).modelMatrix =
            makeUniform(shot.position, 0.0f, ShotConfig::RADIUS);
    }
}
