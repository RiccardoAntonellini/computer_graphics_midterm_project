#include "Bullets.h"
#include <array>
#include <cmath>

using namespace BulletConfig;

/**
 * @brief Posiziona i 5 pickup alle quote calcolate tramite `Platform::yAt()`.
 */
Bullets::Bullets(const Level& level)
{
    for (const Spot& spot : SPOTS) {
        Bullet bullet;
        bullet.position = { spot.x, level.platforms()[spot.platform].yAt(spot.x) + spot.height };
        m_bullets.push_back(bullet);
    }
}

/**
 * @brief Rende nuovamente raccoglibili tutti i pickup.
 */
void Bullets::reset()
{
    for (Bullet& bullet : m_bullets) bullet.collected = false;
}

/**
 * @brief Calcola la posizione animata del pickup `index` al tempo `time`.
 *
 * Legge oraria del moto armonico verticale:
 *   y(t) = y0 + A * sin(omega * t + index * phaseStep)
 * Identica alla formula eseguita in `coin.vert` lato GPU, così hitbox e grafica coincidono.
 */
glm::vec2 Bullets::animatedPosition(std::size_t index, float time) const
{
    glm::vec2 position = m_bullets[index].position;
    position.y += BOB_AMPLITUDE * std::sin(BOB_FREQUENCY * time + static_cast<float>(index) * PHASE_STEP);
    return position;
}

/**
 * @brief Conta quanti pickup hanno `collected == true`.
 */
int Bullets::collectedCount() const
{
    int count = 0;
    for (const Bullet& bullet : m_bullets)
        if (bullet.collected) ++count;
    return count;
}

/**
 * @brief Controlla le collisioni AABB con i pickup non ancora raccolti e li segna come raccolti.
 */
int Bullets::collectTouched(const AABB& box, float time)
{
    int taken = 0;
    for (std::size_t i = 0; i < m_bullets.size(); ++i) {
        if (m_bullets[i].collected) continue;
        const AABB pickupBox{ animatedPosition(i, time), { RADIUS, RADIUS } };
        if (intersects(pickupBox, box)) {
            m_bullets[i].collected = true;
            ++taken;
        }
    }
    return taken;
}
