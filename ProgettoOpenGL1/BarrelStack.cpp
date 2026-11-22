#include "BarrelStack.h"
#include "AssetsUtils.h"
#include "TransformConfig.h"
#include "Config.h"

using namespace WorldConfig;

namespace {
    // Semi-larghezza locale ricavata da barrel.obj (x in [-0.17, +0.18]).
    constexpr float BARREL_HALF_WIDTH_LOCAL = 0.18f;
}

/**
 * @brief Carica la mesh Hermite `barrel.obj` una sola volta e crea 4 istanze impilate (2x2).
 *
 * Calcolo della quota di ciascun barile in coordinate mondo:
 *   y = platform.yAt(x) + (BARREL_BOTTOM_LOCAL * BARREL_SCALE) + row * (BARREL_HEIGHT_LOCAL * BARREL_SCALE)
 * Il termine `BARREL_BOTTOM_LOCAL * BARREL_SCALE` compensa il fatto che la base della mesh
 * in coordinate locali si trova a y = -0.363 sotto l'origine.
 */
BarrelStack::BarrelStack(Scene& scene, const Level& level)
{
    const std::size_t geometry = scene.addGeometry(AssetsUtils::loadMesh("barrel.obj"));
    const Platform& platform = level.platforms()[DK_PLATFORM];
    const float barrelHeight = BARREL_HEIGHT_LOCAL * BARREL_SCALE;
    const float centerAboveSurface = BARREL_BOTTOM_LOCAL * BARREL_SCALE;

    // 2 colonne x 2 righe nell'angolo sinistro della trave di DK.
    for (const float x : { -57.5f, -51.0f }) {
        for (int row = 0; row < 2; ++row) {
            const std::size_t instance = scene.addInstance(geometry);
            const glm::vec2 position(x, platform.yAt(x) + centerAboveSurface + row * barrelHeight);
            scene.instance(instance).modelMatrix = TransformConfig::makeUniform(position, 0.0f, BARREL_SCALE);
            m_instances.push_back(instance);

            // AABB solida allineata a centro e dimensioni scalate della mesh.
            m_boxes.push_back({ position,
                                { BARREL_HALF_WIDTH_LOCAL * BARREL_SCALE, barrelHeight * 0.5f } });
        }
    }
}
