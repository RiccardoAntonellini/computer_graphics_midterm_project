#include "Hammer.h"
#include "AssetsUtils.h"
#include "Config.h"
#include "TransformConfig.h"
#include <cmath>

using namespace WorldConfig;
using namespace HammerConfig;
using namespace TransformConfig;

/**
 * @brief Carica la mesh Hermite `hammer.obj`, crea la sua istanza e la nasconde fino all'ultima vita.
 */
Hammer::Hammer(Scene& scene, const Level& level)
    : m_rng(std::random_device{}())
{
    const std::size_t geometry = scene.addGeometry(AssetsUtils::loadMesh("hammer.obj"));
    m_instance = scene.addInstance(geometry);
    respawn(level);
    hide();
}

/**
 * @brief Sceglie una posizione casuale valida su una trave intermedia, lontana dai bordi e dalle scale.
 *
 * - Selezione trave: intero uniforme in [FIRST_HAMMER_PLATFORM, LAST_HAMMER_PLATFORM].
 * - Selezione ascissa x: normalizzazione lineare di `m_rng()` in [0, 1] mappata su [minX, maxX].
 * - Rejection sampling (fino a 50 tentativi) per evitare la sovrapposizione con le scale.
 * - Quota y: `platform.yAt(x) + HAMMER_BOTTOM_LOCAL * HAMMER_SCALE` per appoggiare l'impugnatura sulla trave.
 */
void Hammer::respawn(const Level& level)
{
    const int range = LAST_HAMMER_PLATFORM - FIRST_HAMMER_PLATFORM + 1;
    const int platformIndex = FIRST_HAMMER_PLATFORM + static_cast<int>(m_rng() % range);
    const Platform& platform = level.platforms()[platformIndex];

    const float minX = platform.left.x + HAMMER_EDGE_MARGIN;
    const float maxX = platform.right.x - HAMMER_EDGE_MARGIN;

    auto randomX = [&]() {
        const float u = static_cast<float>(m_rng()) / static_cast<float>(m_rng.max());
        return minX + u * (maxX - minX);
        };

    auto nearLadder = [&](float x) {
        for (const Ladder& ladder : level.ladders())
            if (std::abs(ladder.x - x) < HAMMER_LADDER_MARGIN) return true;
        return false;
        };

    float x = randomX();
    for (int attempt = 0; attempt < 50 && nearLadder(x); ++attempt)
        x = randomX();

    m_position = { x, platform.yAt(x) + HAMMER_BOTTOM_LOCAL * HAMMER_SCALE };
    m_taken = false;
}

/**
 * @brief Restituisce la AABB di raccolta centrata in `m_position`.
 */
AABB Hammer::box() const
{
    return { m_position, { PICKUP_HALF_W, PICKUP_HALF_H } };
}

/**
 * @brief Aggiorna visibilità e oscillazione sinusoidale (`0.12 * sin(3 * t)`) del martello a terra.
 */
void Hammer::update(Scene& scene, float time)
{
    scene.instance(m_instance).visible = !m_taken;

    const float sway = 0.12f * std::sin(3.0f * time);
    scene.instance(m_instance).modelMatrix = makeUniform(m_position, sway, HAMMER_SCALE);
}
