#include "Hammer.h"
#include "AssetsUtils.h"
#include "Config.h"
#include "TransformConfig.h"
#include <cmath>

using namespace WorldConfig;
using namespace HammerConfig;
using namespace TransformConfig;

/**
 * @brief Carica la mesh Hermite hammer.obj, crea la sua istanza e la nasconde fino all'ultima vita.
 */
Hammer::Hammer(Scene& scene, const Level& level) : m_rng(std::random_device{}()) {
    const std::size_t geometry = scene.addGeometry(AssetsUtils::loadMesh("hammer.obj"));
    m_instance = scene.addInstance(geometry);
    respawn(level);
    hide();
}

/**
 * @brief Posiziona il martello in un punto casuale di una trave intermedia, lontano da bordi e scale.
 */
void Hammer::respawn(const Level& level)
{
    // Scelgo a caso una delle travi intermedie.
    const int numPlatforms = LAST_HAMMER_PLATFORM - FIRST_HAMMER_PLATFORM + 1;
    const int platformIndex = FIRST_HAMMER_PLATFORM + static_cast<int>(m_rng() % numPlatforms);
    const Platform& platform = level.platforms()[platformIndex];

    const float minX = platform.left.x + HAMMER_EDGE_MARGIN;
    const float maxX = platform.right.x - HAMMER_EDGE_MARGIN;

    // Prendo una X casuale in [minX, maxX]; se è troppo vicina a una scala, riprova (max 50 tentativi).
    float x = minX;
    for (int attempt = 0; attempt < 50; ++attempt) {
        const float t = static_cast<float>(m_rng()) / static_cast<float>(m_rng.max());
        x = minX + t * (maxX - minX);

        bool tooCloseToLadder = false;
        for (const Ladder& ladder : level.ladders()) {
            if (std::abs(ladder.x - x) < HAMMER_LADDER_MARGIN) {
                tooCloseToLadder = true;
                break;
            }
        }

        if (!tooCloseToLadder) break;
    }

    // Calcolo la quota Y sulla trave e compensa l'offset locale dell'impugnatura.
    m_position = { x, platform.yAt(x) + HAMMER_BOTTOM_LOCAL * HAMMER_SCALE };
    m_taken = false;
}

AABB Hammer::box() const
{
    return { m_position, { PICKUP_HALF_W, PICKUP_HALF_H } };
}

/**
 * @brief Aggiorna visibilità e oscillazione sinusoidale (0.12 * sin(3 * t)) del martello a terra.
 */
void Hammer::update(Scene& scene, float time)
{
    scene.instance(m_instance).visible = !m_taken;

    const float sway = 0.12f * std::sin(3.0f * time);
    scene.instance(m_instance).modelMatrix = makeUniform(m_position, sway, HAMMER_SCALE);
}
