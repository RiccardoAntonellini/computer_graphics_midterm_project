#include "Mario.h"
#include "Config.h"
#include <algorithm>
#include <cmath>

using namespace WorldConfig;
using namespace HammerConfig;
using namespace MarioConfig;

/**
 * @brief Costruisce Mario inizializzandolo nella posizione di partenza.
 */
Mario::Mario(const Level& level)
{
    reset(level);
}

/**
 * @brief Ripristina posizione, velocità, stato di camminata e timer ai valori iniziali.
 */
void Mario::reset(const Level& level)
{
    m_feet = { MARIO_START_X, level.platforms()[0].yAt(MARIO_START_X) };
    m_velocityY = 0.0f;
    m_onGround = true;
    m_state = MarioState::Walking;
    m_ladder = -1;
    m_facing = 1.0f;
    m_phase = 0.0f;
    m_swing = 0.0f;
    m_hammerTime = 0.0f;
    m_invulnTime = 0.0f;
}

/**
 * @brief Restituisce la AABB di Mario centrata in (feet.x, feet.y + MARIO_HEIGHT / 2).
 */
AABB Mario::box() const
{
    return { { m_feet.x, m_feet.y + MARIO_HEIGHT * 0.5f }, { MARIO_WIDTH * 0.5f, MARIO_HEIGHT * 0.5f } };
}

void Mario::startHammer()
{
    m_hammerTime = HammerConfig::HAMMER_DURATION;
}

void Mario::startInvulnerable()
{
    m_invulnTime = HammerConfig::INVULNERABLE_TIME;
}

/**
 * @brief Calcola l'onda di lampeggio durante l'invulnerabilità.
 *
 * Usando il resto fmod(m_invulnTime, 2 * T) con semi-periodo T = BLINK_HALF_PERIOD,
 * alterna intervalli di durata T visibili (< T) e nascosti (>= T).
 */
bool Mario::blinkVisible() const
{
    if (!invulnerable()) return true;

    const float half = HammerConfig::BLINK_HALF_PERIOD;
    return std::fmod(m_invulnTime, 2.0f * half) < half;
}

/**
 * @brief Calcola la hitbox del martello davanti a Mario, proiettata nel verso m_facing.
 */
AABB Mario::hammerBox() const
{
    return { { m_feet.x + m_facing * HammerConfig::HIT_OFFSET_X, m_feet.y + HammerConfig::HIT_CENTER_Y },
             { HammerConfig::HIT_HALF_W, HammerConfig::HIT_HALF_H } };
}

/**
 * @brief Aggiorna i timer con dt ed esegue lo stato attivo (Walking o Climbing).
 */
void Mario::update(float dt, const Controls& controls, const Level& level,
    const std::vector<AABB>& solids)
{
    m_hammerTime = std::max(0.0f, m_hammerTime - dt);
    m_invulnTime = std::max(0.0f, m_invulnTime - dt);

    if (m_state == MarioState::Walking) {
        tryStartClimb(controls, level);
    }

    if (m_state == MarioState::Climbing) {
        climb(dt, controls, level);
    }
    else {
        walk(dt, controls, level, solids);
    }
}

/**
 * @brief Verifica se Mario può agganciarsi a una scala vicina (solo da terra, premendo Su o Giù).
 */
void Mario::tryStartClimb(const Controls& controls, const Level& level)
{
    if (!m_onGround) return;
    if (controls.up == controls.down) return;

    const std::vector<Ladder>& ladders = level.ladders();
    for (std::size_t i = 0; i < ladders.size(); ++i) {
        const Ladder& ladder = ladders[i];
        if (std::abs(m_feet.x - ladder.x) > LADDER_GRAB_RANGE) continue;

        const bool inRange = m_feet.y > ladder.yBottom - 1.5f &&
            m_feet.y < ladder.yTop + 1.5f;
        const bool canGoUp = controls.up && m_feet.y < ladder.yTop - 0.5f;
        const bool canGoDown = controls.down && m_feet.y > ladder.yBottom + 0.5f;

        if (inRange && (canGoUp || canGoDown)) {
            m_state = MarioState::Climbing;
            m_ladder = static_cast<int>(i);
            m_feet.x = ladder.x;
            m_feet.y = std::clamp(m_feet.y, ladder.yBottom, ladder.yTop);
            m_velocityY = 0.0f;
            return;
        }
    }
}

/**
 * @brief Gestisce il moto verticale lungo la scala attiva e l'uscita agli estremi.
 */
void Mario::climb(float dt, const Controls& controls, const Level& level)
{
    const Ladder& ladder = level.ladders()[m_ladder];
    const float direction = static_cast<float>(controls.up) - static_cast<float>(controls.down);

    m_feet.y += direction * CLIMB_SPEED * dt;

    if (direction != 0.0f) {
        m_phase += CLIMB_ANIM_SPEED * dt;
        m_swing = std::sin(m_phase);
    }

    if (m_feet.y >= ladder.yTop) {
        m_feet.y = ladder.yTop;
        stopClimbing();
    }
    else if (m_feet.y <= ladder.yBottom) {
        m_feet.y = ladder.yBottom;
        stopClimbing();
    }
}

/**
 * @brief Esce dallo stato di arrampicata riportando Mario a terra in stato Walking.
 */
void Mario::stopClimbing()
{
    m_state = MarioState::Walking;
    m_onGround = true;
    m_velocityY = 0.0f;
    m_ladder = -1;
}

/**
 * @brief Integra camminata orizzontale, ostacoli solidi, salto, gravità, soffitto e appoggio su travi.
 *
 * Integrazione verticale in volo:
 *   v_y(t + dt) = v_y(t) + GRAVITY * dt
 *   y(t + dt)   = y(t) + v_y(t + dt) * dt
 * L'oscillazione degli arti segue m_swing = sin(m_phase) con m_phase += WALK_ANIM_SPEED * dt.
 */
void Mario::walk(float dt, const Controls& controls, const Level& level,
    const std::vector<AABB>& solids)
{
    // Movimento orizzontale
    const float direction = static_cast<float>(controls.right) - static_cast<float>(controls.left);
    if (direction != 0.0f) m_facing = direction;

    const float previousX = m_feet.x;
    const float limit = FIELD_HALF_WIDTH - MARIO_WIDTH * 0.5f;
    m_feet.x = std::clamp(m_feet.x + direction * MOVE_SPEED * dt, -limit, limit);

    // Risoluzione ostacoli solidi
    for (const AABB& solid : solids) {
        if (intersects(box(), solid)) {
            m_feet.x = previousX;
            break;
        }
    }

    // Salto
    if (controls.jump && m_onGround) {
        m_velocityY = JUMP_SPEED;
        m_onGround = false;
    }

    // Movimento verticale: adesione a terra o caduta libera con gravità
    const float previousY = m_feet.y;
    float stick = 0.0f;
    if (m_onGround) {
        stick = WALK_STICK;
    }
    else {
        m_velocityY += GRAVITY * dt;
        m_feet.y += m_velocityY * dt;
    }

    // Collisione con il lato inferiore delle travi in salita
    if (!m_onGround && m_velocityY > 0.0f) {
        float ceilingY = 0.0f;
        if (level.hitsCeiling(m_feet.x, previousY + MARIO_HEIGHT,
            m_feet.y + MARIO_HEIGHT, ceilingY)) {
            m_feet.y = ceilingY - MARIO_HEIGHT;
            m_velocityY = 0.0f;
        }
    }

    // Appoggio sulla trave
    const bool canLand = m_onGround || m_velocityY <= 0.0f;
    float groundY = 0.0f;
    if (canLand && level.findGround(m_feet.x, previousY, m_feet.y, stick, groundY)) {
        m_feet.y = groundY;
        m_velocityY = 0.0f;
        m_onGround = true;
    }
    else {
        m_onGround = false;
    }

    // 5) Animazione degli arti
    if (m_onGround && direction != 0.0f) {
        m_phase += WALK_ANIM_SPEED * dt;
        m_swing = std::sin(m_phase);
    }
    else {
        m_swing = 0.0f;
    }
}
