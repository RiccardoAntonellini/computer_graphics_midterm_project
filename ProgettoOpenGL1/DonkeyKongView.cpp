#include "DonkeyKongView.h"
#include "ShapeGenerator.h"
#include "TransformConfig.h"
#include <cmath>

using namespace DonkeyKongConfig;
using namespace TransformConfig;

/**
 * @brief Assembla il Rig di Donkey Kong da rettangoli, cerchi e un triangolo.
 *
 * Riutilizza le stesse Geometry per le parti simmetriche.
 * L'ordine di aggiunta determina la stratificazione visiva.
 */
DonkeyKongView::DonkeyKongView(Scene& scene)
{
    const std::size_t armGeo = scene.addGeometry(ShapeGenerator::rectangle(ARM_WIDTH, ARM_LENGTH, BROWN));
    const std::size_t legGeo = scene.addGeometry(ShapeGenerator::rectangle(4.0f, 5.0f, BROWN));
    const std::size_t eyeGeo = scene.addGeometry(ShapeGenerator::disk(0.9f, WHITE, 16));
    const std::size_t pupilGeo = scene.addGeometry(ShapeGenerator::disk(0.45f, BLACK, 12));

    m_backArm = m_rig.addPartFromGeometry(scene, armGeo);
    m_rig.addPartFromGeometry(scene, legGeo, at(-3.5f, 2.5f));
    m_rig.addPartFromGeometry(scene, legGeo, at(3.5f, 2.5f));
    m_rig.addPart(scene, ShapeGenerator::rectangle(14.0f, 10.0f, BROWN), at(0.0f, 10.0f));// corpo
    m_rig.addPart(scene, ShapeGenerator::rectangle(8.0f, 6.0f, TAN), at(1.5f, 10.0f));// petto
    m_frontArm = m_rig.addPartFromGeometry(scene, armGeo);
    m_rig.addPart(scene, ShapeGenerator::triangle(5.0f, 3.0f, BROWN), at(1.0f, 23.5f));// ciuffo
    m_rig.addPart(scene, ShapeGenerator::disk(4.5f, BROWN, 32), at(2.0f, 18.5f));// testa
    m_rig.addPart(scene, ShapeGenerator::disk(2.8f, TAN, 24), at(3.6f, 17.6f));// muso
    m_rig.addPartFromGeometry(scene, eyeGeo, at(2.6f, 19.6f));
    m_rig.addPartFromGeometry(scene, eyeGeo, at(4.8f, 19.6f));
    m_rig.addPartFromGeometry(scene, pupilGeo, at(3.0f, 19.6f));
    m_rig.addPartFromGeometry(scene, pupilGeo, at(5.2f, 19.6f));
    m_rig.addPart(scene, ShapeGenerator::rectangle(3.2f, 1.2f, { 0.78f, 0.28f, 0.25f }), at(3.9f, 16.5f));// labbra
    m_rig.addPart(scene, ShapeGenerator::rectangle(2.8f, 0.28f, BLACK), at(3.9f, 16.5f));// bocca
}

/**
 * @brief Aggiorna visibilità, rotazione delle braccia attorno alle spalle e posizione globale del Rig.
 *
 * - Durante il lancio (throwProgress in (0, 1]): entrambe le braccia ruotano secondo la semionda
 *   2.5 * sin(PI * progress):  (0 -> 2.5 rad -> 0).
 * - A riposo: le braccia oscillano in opposizione di fase (+-0.35 * beat()).
 */
void DonkeyKongView::update(Scene& scene, const DonkeyKong& kong)
{
    m_rig.setVisible(scene, !kong.defeated());

    float frontAngle = 0.0f;
    float backAngle = 0.0f;

    if (kong.throwProgress() > 0.0f) {
        const float lift = 2.5f * std::sin(PI * kong.throwProgress());
        frontAngle = lift;
        backAngle = lift;
    }
    else {
        frontAngle = 0.35f * kong.beat();
        backAngle = -0.35f * kong.beat();
    }

    m_rig.setLocal(m_frontArm, limb(5.5f, SHOULDER_Y, frontAngle, ARM_LENGTH));
    m_rig.setLocal(m_backArm, limb(-5.5f, SHOULDER_Y, backAngle, ARM_LENGTH));

    m_rig.update(scene, at(kong.feet().x, kong.feet().y));
}