#include "BulletsView.h"
#include "ShapeGenerator.h"
#include "TransformConfig.h"

using namespace BulletConfig;
using namespace TransformConfig;

/**
 * @brief Crea una singola Geometry composita e 5 istanze.
 *
 * Le matrici Model sono statiche (posizione di riposo + scala uniforme RADIUS):
 * l'oscillazione verticale viene applicata dinamicamente nel vertex shader bullet.vert.
 */
BulletsView::BulletsView(Scene& scene, const Bullets& bullets)
{
    MeshData mesh = ShapeGenerator::disk(1.0f, { 0.95f, 0.50f, 0.10f });
    const MeshData inner = ShapeGenerator::disk(0.55f, { 1.0f, 0.90f, 0.30f });
    mesh.vertices.insert(mesh.vertices.end(), inner.vertices.begin(), inner.vertices.end());
    m_geometry = scene.addGeometry(mesh);

    for (const Bullet& bullet : bullets.all()) {
        const std::size_t instance = scene.addInstance(m_geometry);
        scene.instance(instance).modelMatrix = makeUniform(bullet.position, 0.0f, RADIUS);
        m_instances.push_back(instance);
    }
}

/**
 * @brief Nasconde nella Scene i proiettili già raccolti.
 */
void BulletsView::update(Scene& scene, const Bullets& bullets)
{
    for (std::size_t i = 0; i < m_instances.size(); ++i)
        scene.instance(m_instances[i]).visible = !bullets.all()[i].collected;
}
