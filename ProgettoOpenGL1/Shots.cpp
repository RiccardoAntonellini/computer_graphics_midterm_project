#include "Shots.h"
#include <cmath>

using namespace ShotConfig;
using namespace WorldConfig;

/**
 * @brief Pre-alloca il pool fisso di POOL_SIZE colpi (inizialmente tutti inattivi).
 */
Shots::Shots() : m_shots(POOL_SIZE) {}

/**
 * @brief Disattiva tutti i colpi nel pool.
 */
void Shots::reset()
{
    for (Shot& shot : m_shots) {
        shot.active = false;
    };
}

/**
 * @brief Attiva un colpo dal pool con moto rettilineo uniforme da origin verso target.
 *
 * Calcola il versore direzione dir = normalize(target - origin) (evitando la divisione
 * per zero se origine e bersaglio coincidono) e imposta velocity = dir * SPEED.
 */
bool Shots::fire(const glm::vec2& origin, const glm::vec2& target)
{
    const glm::vec2 direction = target - origin;
    if (glm::length(direction) < 0.001f) {
        return false;
    }

    for (Shot& shot : m_shots) {
        if (shot.active) continue;

        shot.active = true;
        shot.position = origin;
        shot.velocity = glm::normalize(direction) * SPEED;
        return true;
    }
    return false;
}

/**
 * @brief Integra la posizione (position += velocity * dt) e disattiva i colpi usciti dal campo.
 */
void Shots::update(float dt)
{
    for (Shot& shot : m_shots) {
        if (!shot.active) continue;

        shot.position += shot.velocity * dt;

        if (std::abs(shot.position.x) > FIELD_HALF_WIDTH + 5.0f || std::abs(shot.position.y) > HALF_HEIGHT + 5.0f) {
            shot.active = false;
        }
    }
}

/**
 * @brief Verifica le intersezioni AABB tra i colpi attivi e target, disattivando quelli a segno.
 */
int Shots::consumeHits(const AABB& target)
{
    int hits = 0;
    for (Shot& shot : m_shots) {
        if (!shot.active) continue;

        const AABB shotBox{ shot.position, { RADIUS, RADIUS } };
        if (intersects(shotBox, target)) {
            shot.active = false;
            ++hits;
        }
    }
    return hits;
}
