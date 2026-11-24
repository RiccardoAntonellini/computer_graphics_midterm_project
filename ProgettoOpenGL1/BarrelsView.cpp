#include "BarrelsView.h"
#include "ShapeGenerator.h"
#include "TransformConfig.h"
#include "Config.h"

using namespace BarrelConfig;

/**
 * @brief Crea le 2 geometrie unitarie e istanzia i 3 componenti per ogni barile del pool.
 */
BarrelsView::BarrelsView(Scene& scene)
{
    // Geometrie di raggio unitario (1.0): la scala uniforme (= RADIUS) le porta alla dimensione mondo.
    const std::size_t bodyGeo = scene.addGeometry(ShapeGenerator::disk(1.0f, BODY_COLOR));
    const std::size_t stripeGeo = scene.addGeometry(ShapeGenerator::rectangle(1.8f, 0.25f, STRIPE_COLOR));

    for (int i = 0; i < BarrelConfig::POOL_SIZE; ++i) {
        Slot slot;
        slot.body = scene.addInstance(bodyGeo);
        slot.stripeA = scene.addInstance(stripeGeo);
        slot.stripeB = scene.addInstance(stripeGeo);
        m_slots.push_back(slot);
    }
}

/**
 * @brief Aggiorna visibilità e matrici TRS di disco e strisce incrociate per ogni barile attivo.
 *
 * stripeB è ruotata di barrel.angle + PI/2 rispetto a stripeA per formare una croce
 * che ruota solidalmente con il barile, rendendo visibile il rotolamento.
 */
void BarrelsView::update(Scene& scene, const Barrels& barrels)
{
    for (std::size_t i = 0; i < m_slots.size(); ++i) {
        const Barrel& barrel = barrels.all()[i];
        const Slot& slot = m_slots[i];

        scene.instance(slot.body).visible = barrel.active;
        scene.instance(slot.stripeA).visible = barrel.active;
        scene.instance(slot.stripeB).visible = barrel.active;
        if (!barrel.active) continue;

        const glm::vec2 center = barrel.center();
        scene.instance(slot.body).modelMatrix = TransformConfig::makeUniform(center, barrel.angle, BarrelConfig::RADIUS);
        scene.instance(slot.stripeA).modelMatrix = TransformConfig::makeUniform(center, barrel.angle, BarrelConfig::RADIUS);
        scene.instance(slot.stripeB).modelMatrix = TransformConfig::makeUniform(center, barrel.angle + HALF_PI, BarrelConfig::RADIUS);
    }
}
