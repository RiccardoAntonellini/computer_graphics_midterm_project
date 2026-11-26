#include "Level.h"
#include "ShapeGenerator.h"
#include "TransformConfig.h"
#include "Config.h"
#include <glm/gtc/matrix_transform.hpp>
#include <algorithm>
#include <array>
#include <cmath>

using namespace WorldConfig;
using namespace TransformConfig;
using namespace LevelConfig;

/**
 * @brief Inizializza i dati analitici di travi e scale e costruisce le loro istanze nella Scene.
 *
 * Le scale vengono create prima delle travi così, nell'ordine di disegno 2D, le travi
 * coprono pulitamente gli estremi superiori e inferiori dei montanti.
 */
Level::Level(Scene& scene)
{
    for (const PlatformSpec& s : PLATFORMS)
        m_platforms.push_back({ {s.x0, s.y0}, {s.x1, s.y1} });

    for (const LadderSpec& s : LADDERS) {
        Ladder ladder;
        ladder.x = s.x;
        ladder.yBottom = m_platforms[s.lower].yAt(s.x);
        ladder.yTop = m_platforms[s.lower + 1].yAt(s.x);
        m_ladders.push_back(ladder);
    }

    buildLadders(scene);
    buildPlatforms(scene);
}

/**
 * @brief Costruisce tutte le scale condividendo una singola Geometry (rettangolo 1x1).
 *
 * Ogni scala è composta da 2 montanti verticali (traslati di +-railOffset) e rungs/pioli
 * distribuiti uniformemente tra `yBottom` e `yTop`.
 */
void Level::buildLadders(Scene& scene)
{
    const std::size_t geo = scene.addGeometry(
        ShapeGenerator::rectangle(1.0f, 1.0f, { 0.30f, 0.85f, 0.95f }));

    for (const Ladder& ladder : m_ladders) {
        const float height = ladder.yTop - ladder.yBottom;
        const float midY = 0.5f * (ladder.yBottom + ladder.yTop);
        const float railOffset = 0.5f * (LADDER_WIDTH - LADDER_RAIL_WIDTH);

        for (const float side : { -1.0f, 1.0f }) {
            const std::size_t rail = scene.addInstance(geo);
            scene.instance(rail).modelMatrix = make({ ladder.x + side * railOffset, midY }, 0.0f, { LADDER_RAIL_WIDTH, height });
        }

        const int rungs = std::max(1, static_cast<int>(height / LADDER_RUNG_SPACING));
        for (int i = 0; i < rungs; ++i) {
            const float y = ladder.yBottom + (static_cast<float>(i) + 0.5f) * height / rungs;
            const std::size_t rung = scene.addInstance(geo);
            scene.instance(rung).modelMatrix = make({ ladder.x, y }, 0.0f, { LADDER_WIDTH, LADDER_RUNG_THICKNESS });
        }
    }
}

/**
 * @brief Costruisce le 6 travi inclinate a traliccio condividendo solo 2 Geometry unitarie (1x1).
 *
 * Costruzione matematica tramite cambio di sistema di riferimento:
 * 1. Vettore direzione della trave.
 * 
 * 2. Matrice del riferimento locale della trave --> in questo spazio locale la trave è orizzontale da x = 0 a x = L, 
 * con la superficie superiore a y = 0.
 * 
 * 3. Ogni cella del traliccio contiene due diagonali lunghe  inclinate.
 * 
 * 4. Ogni pezzo (diagonali, barra superiore, barra inferiore) è portato in World Space.
 */
void Level::buildPlatforms(Scene& scene)
{
    const std::size_t bodyGeo = scene.addGeometry(ShapeGenerator::rectangle(1.0f, 1.0f, { 0.86f, 0.20f, 0.42f }));
    const std::size_t topGeo = scene.addGeometry(ShapeGenerator::rectangle(1.0f, 1.0f, { 0.98f, 0.45f, 0.60f }));

    for (const Platform& p : m_platforms) {
        const glm::vec2 d = p.right - p.left;
        const float length = glm::length(d);
        const float angle = std::atan2(d.y, d.x);

        const glm::mat4 girder =
            glm::translate(glm::mat4(1.0f), glm::vec3(p.left, 0.0f)) *
            glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0f, 0.0f, 1.0f));

        // Quote locali (y < 0 indica lo spessore sotto la superficie calpestabile).
        const float innerTop = -GIRDER_BAR_THICKNESS;
        const float innerBottom = -PLATFORM_THICKNESS + GIRDER_BAR_THICKNESS;
        const float innerHeight = innerTop - innerBottom;
        const float innerMid = 0.5f * (innerTop + innerBottom);

        // Suddivisione in celle a zig-zag e geometria delle diagonali (Pitagora + atan2).
        const int cells = std::max(1, static_cast<int>(std::round(length / AMPIEZZA_TRIANGOLI_TRAVI)));
        const float cellWidth = length / static_cast<float>(cells);
        const float halfCell = 0.5f * cellWidth;
        const float diagLength = std::sqrt(halfCell * halfCell + innerHeight * innerHeight);
        const float diagAngle = std::atan2(innerHeight, halfCell);

        // Diagonali prima delle barre così le barre coprono gli estremi sporgenti.
        for (int i = 0; i < cells; ++i) {
            const float x0 = static_cast<float>(i) * cellWidth;

            const std::size_t up = scene.addInstance(bodyGeo);
            scene.instance(up).modelMatrix = girder * make({ x0 + 0.5f * halfCell, innerMid }, diagAngle,
                { diagLength, SPESSORE_DIAGONALI_TRAVI });

            const std::size_t down = scene.addInstance(bodyGeo);
            scene.instance(down).modelMatrix = girder * make({ x0 + 1.5f * halfCell, innerMid }, -diagAngle,
                { diagLength, SPESSORE_DIAGONALI_TRAVI });
        }

        // Barra superiore (superficie calpestabile).
        const std::size_t top = scene.addInstance(topGeo);
        scene.instance(top).modelMatrix = girder * make({ 0.5f * length, -0.5f * GIRDER_BAR_THICKNESS }, 0.0f,
            { length, GIRDER_BAR_THICKNESS });

        // Barra inferiore.
        const std::size_t bottom = scene.addInstance(bodyGeo);
        scene.instance(bottom).modelMatrix = girder * make({ 0.5f * length, -PLATFORM_THICKNESS + 0.5f * GIRDER_BAR_THICKNESS }, 0.0f,
            { length, GIRDER_BAR_THICKNESS });
    }
}

/**
 * @brief Cerca la trave più alta su cui l'entità può appoggiarsi all'ascissa x.
 *
 * Condizioni di appoggio valido rispetto alla quota y = p.yAt(x):
 *   - previousFeetY >= y - LAND_TOLERANCE (al frame precedente era sopra o sulla trave)
 *   - feetY <= y + stick (al frame corrente l'ha raggiunta o superata; stick mantiene l'adesione in discesa)
 */
bool Level::findGround(float x, float previousFeetY, float feetY, float stick, float& groundY, int* platformIndex) const
{
    bool found = false;
    float best = 0.0f;
    for (std::size_t i = 0; i < m_platforms.size(); ++i) {
        const Platform& p = m_platforms[i];
        if (!p.coversX(x)) continue;

        const float y = p.yAt(x);
        const bool wasAbove = previousFeetY >= y - LAND_TOLERANCE;
        const bool nowReached = feetY <= y + stick;

        if (wasAbove && nowReached && (!found || y > best)) {
            found = true;
            best = y;

            if (platformIndex) {
                *platformIndex = static_cast<int>(i);
            }
        }
    }
    if (found) {
        groundY = best;
    }

    return found;
}

/**
 * @brief Verifica se la testa ha attraversato dal basso verso l'alto il lato inferiore di una trave.
 *
 * Lato inferiore della trave: underside = p.yAt(x) - PLATFORM_THICKNESS.
 */
bool Level::hitsCeiling(float x, float previousHeadY, float headY, float& ceilingY) const
{
    for (const Platform& p : m_platforms) {
        if (!p.coversX(x)) continue;
        const float underside = p.yAt(x) - PLATFORM_THICKNESS;

        if (previousHeadY <= underside && headY > underside) {
            ceilingY = underside;
            return true;
        }
    }
    return false;
}
