#include "PaulineView.h"
#include "ShapeGenerator.h"
#include "TransformConfig.h"
#include <cmath>

using namespace PaulineConfig;

/**
 * @brief Assembla il Rig di Pauline da rettangoli, triangolo (vestito) e disco (testa).
 */
PaulineView::PaulineView(Scene& scene)
{
    const std::size_t armGeo = scene.addGeometry(ShapeGenerator::rectangle(1.2f, ARM_LENGTH, SKIN));
    const std::size_t legGeo = scene.addGeometry(ShapeGenerator::rectangle(1.0f, 2.0f, SKIN));

    m_rig.addPart(scene, ShapeGenerator::rectangle(4.2f, 5.0f, HAIR), TransformConfig::at(0.0f, 9.0f));      // capelli
    m_rig.addPartFromGeometry(scene, legGeo, TransformConfig::at(-1.0f, 1.0f));
    m_rig.addPartFromGeometry(scene, legGeo, TransformConfig::at(1.0f, 1.0f));
    m_restArm = m_rig.addPartFromGeometry(scene, armGeo);
    m_rig.addPart(scene, ShapeGenerator::triangle(6.5f, 7.5f, PINK), TransformConfig::at(0.0f, 4.75f));      // vestito
    m_rig.addPart(scene, ShapeGenerator::disk(1.8f, SKIN, 24), TransformConfig::at(0.4f, 9.8f));             // testa
    m_waveArm = m_rig.addPartFromGeometry(scene, armGeo);
}

/**
 * @brief Anima il saluto del braccio (`2.6 + 0.4 * sin(WAVE_SPEED * time)`) e orienta Pauline a sinistra (scala X = -1).
 */
void PaulineView::update(Scene& scene, const Pauline& pauline, float time)
{
    const float waveAngle = 2.6f + 0.4f * std::sin(PaulineConfig::WAVE_SPEED * time);
    m_rig.setLocal(m_restArm, TransformConfig::limb(-1.2f, SHOULDER_Y, -0.15f, ARM_LENGTH));
    m_rig.setLocal(m_waveArm, TransformConfig::limb(1.2f, SHOULDER_Y, waveAngle, ARM_LENGTH));

    m_rig.update(scene, TransformConfig::make(pauline.feet(), 0.0f, { -1.0f, 1.0f }));
}
