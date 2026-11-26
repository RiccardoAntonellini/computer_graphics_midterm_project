#include "Barrels.h"
#include "Config.h"
#include <cmath>

using namespace BarrelConfig;

/**
 * @brief Pre-alloca il pool fisso di POOL_SIZE barili (tutti inizialmente inattivi).
 */
Barrels::Barrels() : m_barrels(POOL_SIZE) {}

/**
 * @brief Disattiva tutti i barili nel pool.
 */
void Barrels::reset()
{
    for (Barrel& barrel : m_barrels) barrel.active = false;
}

/**
 * @brief Attiva il primo slot libero del pool posizionandolo sulla trave di DK.
 *
 * Se tutti gli slot sono occupati lo spawn viene ignorato.
 */
void Barrels::spawn(const Level& level)
{
    for (Barrel& barrel : m_barrels) {
        if (barrel.active) continue;

        const Platform& platform = level.platforms()[WorldConfig::DK_PLATFORM];
        barrel.active = true;
        barrel.feet = { SPAWN_X, platform.yAt(SPAWN_X) };
        barrel.velocityY = 0.0f;
        barrel.direction = 1.0f;
        barrel.onGround = true;
        barrel.angle = 0.0f;
        return;
    }
}

/**
 * @brief Aggiorna tutti i barili attivi nel pool.
 */
void Barrels::update(float dt, const Level& level)
{
    for (Barrel& barrel : m_barrels) {
        if (barrel.active) {
            updateOne(barrel, dt, level);
        }
    }
}

/**
 * @brief Integra il moto di un singolo barile per un passo temporale dt.
 */
void Barrels::updateOne(Barrel& barrel, float dt, const Level& level)
{
    const float limit = WorldConfig::FIELD_HALF_WIDTH - RADIUS;
    const float previousY = barrel.feet.y;

    // Moto orizzontale e rotolamento puro
    const float step = barrel.direction * SPEED * dt;
    barrel.feet.x += step;
    barrel.angle -= step / RADIUS;

    // Collisione con i muri laterali del campo di gioco
    if (std::abs(barrel.feet.x) > limit) {
        if (barrel.onGround) {
            barrel.active = false;
            return;
        }
        barrel.feet.x = (barrel.feet.x > 0.0f) ? limit : -limit;
        barrel.direction = -barrel.direction;
    }

    // Moto verticale
    float stick = 0.0f;
    if (barrel.onGround) {
        stick = STICK;
    }
    else {
        barrel.velocityY += GRAVITY * dt;
        barrel.feet.y += barrel.velocityY * dt;
    }

    // Ricerca della superficie di appoggio
    const bool wasFalling = !barrel.onGround;
    float groundY = 0.0f;
    int platformIndex = -1;
    if (level.findGround(barrel.feet.x, previousY, barrel.feet.y, stick, groundY, &platformIndex)) {
        barrel.feet.y = groundY;
        barrel.velocityY = 0.0f;
        barrel.onGround = true;

        if (wasFalling) {
            // All'atterraggio rotola nel verso in cui la trave scende
            const Platform& platform = level.platforms()[platformIndex];
            barrel.direction = (platform.right.y < platform.left.y) ? 1.0f : -1.0f;
        }
    }
    else {
        barrel.onGround = false;
    }

    // Sicurezza = disattiva il barile se esce dal bordo inferiore
    if (barrel.feet.y < -WorldConfig::HALF_HEIGHT) {
        barrel.active = false;
    }
}

/**
 * @brief Verifica se almeno un barile attivo interseca box (usando HITBOX_HALF < RADIUS).
 */
bool Barrels::hits(const AABB& box) const
{
    for (const Barrel& barrel : m_barrels) {
        if (!barrel.active) continue;

        const AABB barrelBox{ barrel.center(), { HITBOX_HALF, HITBOX_HALF } };
        if (intersects(barrelBox, box)) return true;
    }
    return false;
}

/**
 * @brief Disattiva i barili attivi che intersecano box (usa l'intero RADIUS per favorire il giocatore).
 */
void Barrels::destroyHit(const AABB& box)
{
    for (Barrel& barrel : m_barrels) {
        if (!barrel.active) continue;

        const AABB barrelBox{ barrel.center(), { RADIUS, RADIUS } };
        if (intersects(barrelBox, box)) {
            barrel.active = false;
        }
    }
}
