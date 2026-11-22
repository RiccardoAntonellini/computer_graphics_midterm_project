#include "MarioView.h"
#include "AssetsUtils.h"
#include "Config.h"
#include "ShapeGenerator.h"
#include "TransformConfig.h"
#include <glm/gtc/matrix_transform.hpp>
#include <cmath>

using namespace MarioConfig;
using namespace TransformConfig;

/**
 * @brief Assembla il Rig di Mario (rettangoli, cerchio, triangolo) e aggancia la mesh Hermite `hammer.obj`.
 */
MarioView::MarioView(Scene& scene)
{
    const std::size_t legGeo = scene.addGeometry(ShapeGenerator::rectangle(LEG_WIDTH, LEG_LENGTH, BLUE));
    const std::size_t armGeo = scene.addGeometry(ShapeGenerator::rectangle(ARM_WIDTH, ARM_LENGTH, RED));

    m_backLeg = m_rig.addPartFromGeometry(scene, legGeo);
    m_frontLeg = m_rig.addPartFromGeometry(scene, legGeo);
    m_rig.addPart(scene, ShapeGenerator::rectangle(4.6f, 3.6f, BLUE), at(0.0f, 5.8f));   // corpo
    m_rig.addPart(scene, ShapeGenerator::disk(2.0f, SKIN), at(0.3f, 8.7f));              // testa
    m_backArm = m_rig.addPartFromGeometry(scene, armGeo);
    m_frontArm = m_rig.addPartFromGeometry(scene, armGeo);
    m_rig.addPart(scene, ShapeGenerator::triangle(4.4f, 2.4f, RED), at(0.0f, 9.8f));     // cappello
    m_rig.addPart(scene, ShapeGenerator::rectangle(2.0f, 0.7f, RED), at(1.9f, 9.0f));    // visiera

    // Il martello è aggiunto per ultimo così viene disegnato sopra il corpo di Mario.
    m_heldHammer = m_rig.addPart(scene, AssetsUtils::loadMesh("hammer.obj"));
}

/**
 * @brief Calcola la trasformazione locale del martello impugnato da Mario.
 *
 * Composizione matriciale (applicata da destra a sinistra):
 *   M = T(HAND_X, HAND_Y) * R_z(angle) * S(HELD_SCALE) * T(0, +HAMMER_HANDLE_LOCAL)
 * 1. `T(0, +HAMMER_HANDLE_LOCAL)` sposta l'impugnatura (y = -0.27 in `hammer.obj`) sull'origine (0, 0).
 * 2. `S(HELD_SCALE)` porta la mesh alle dimensioni di gioco.
 * 3. `R_z(angle)` ruota il martello attorno all'impugnatura tra 0 (in alto) e -PI/2 (in avanti),
 *    con `t = 0.5 + 0.5 * sin(SWING_SPEED * timeLeft)` in [0, 1].
 * 4. `T(HAND_X, HAND_Y)` posiziona l'impugnatura nella mano di Mario.
 * Quando il martello è inattivo (o spento durante il lampeggio finale), una scala 0 lo nasconde.
 */
glm::mat4 MarioView::heldHammerMatrix(const Mario& mario) const
{
    const float timeLeft = mario.hammerTimeLeft();
    const bool blinkOff = timeLeft < HammerConfig::HAMMER_BLINK_TIME &&
        std::fmod(timeLeft, 0.2f) < 0.1f;
    if (!mario.hammering() || blinkOff)
        return glm::scale(glm::mat4(1.0f), glm::vec3(0.0f));

    const float t = 0.5f + 0.5f * std::sin(HammerConfig::SWING_SPEED * timeLeft);
    const float angle = -HALF_PI * t;

    return at(HAND_X, HAND_Y) *
        glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0f, 0.0f, 1.0f)) *
        glm::scale(glm::mat4(1.0f), glm::vec3(HammerConfig::HELD_SCALE, HammerConfig::HELD_SCALE, 1.0f)) *
        at(0.0f, HAMMER_HANDLE_LOCAL);
}

/**
 * @brief Aggiorna visibilità, rotazioni degli arti, martello e trasformazione padre del Rig.
 *
 * La matrice padre `T(feet) * S(facing, 1)` trasla il personaggio ai piedi e applica una
 * riflessione orizzontale (scala X = -1) quando Mario guarda verso sinistra.
 */
void MarioView::update(Scene& scene, const Mario& mario)
{
    m_rig.setVisible(scene, mario.blinkVisible());

    const float swing = mario.swing();

    // Gambe: oscillazione in opposizione di fase attorno alle anche (y = LEG_LENGTH).
    const float legAngle = 0.7f * swing;
    m_rig.setLocal(m_frontLeg, limb(0.9f, LEG_LENGTH, legAngle, LEG_LENGTH));
    m_rig.setLocal(m_backLeg, limb(-0.9f, LEG_LENGTH, -legAngle, LEG_LENGTH));

    // Braccia: alzate in arrampicata, altrimenti in opposizione di fase rispetto alle gambe.
    float frontArmAngle = -0.7f * swing;
    float backArmAngle = 0.7f * swing;
    if (mario.climbing()) {
        frontArmAngle = 2.8f + 0.4f * swing;
        backArmAngle = 2.8f - 0.4f * swing;
    }
    m_rig.setLocal(m_frontArm, limb(0.4f, SHOULDER_Y, frontArmAngle, ARM_LENGTH));
    m_rig.setLocal(m_backArm, limb(-0.4f, SHOULDER_Y, backArmAngle, ARM_LENGTH));

    m_rig.setLocal(m_heldHammer, heldHammerMatrix(mario));

    const glm::mat4 parent = make(mario.feet(), 0.0f, { mario.facing(), 1.0f });
    m_rig.update(scene, parent);
}
