#include "DonkeyKong.h"
#include <cmath>

using namespace DonkeyKongConfig;
using namespace WorldConfig;

/**
 * @brief Posiziona Donkey Kong sulla trave `DK_PLATFORM` all'ascissa `X`.
 */
DonkeyKong::DonkeyKong(const Level& level)
{
    m_feet = { X, level.platforms()[DK_PLATFORM].yAt(X) };
}

/**
 * @brief Azzera il ciclo di lancio e il contatore dei colpi subiti.
 */
void DonkeyKong::reset()
{
    resetThrow();
    m_hits = 0;
}

/**
 * @brief Riavvia i timer di animazione e lancio senza modificare `m_hits`.
 */
void DonkeyKong::resetThrow()
{
    m_time = 0.0f;
    m_wait = 0.0f;
    m_throwTime = 0.0f;
    m_throwing = false;
}

/**
 * @brief Incrementa il numero di colpi subiti.
 */
void DonkeyKong::hit(int count)
{
    m_hits += count;
}

/**
 * @brief Restituisce la AABB di Donkey Kong centrata in `(m_feet.x, m_feet.y + HEIGHT/2)`.
 */
AABB DonkeyKong::box() const
{
    return { { m_feet.x, m_feet.y + HEIGHT * 0.5f }, { HALF_W, HEIGHT * 0.5f } };
}

/**
 * @brief Avanza la macchina a stati temporizzata (Attesa -> Lancio -> Attesa).
 *
 * Il rilascio del barile avviene esattamente nel frame in cui `m_throwTime` attraversa
 * la soglia `RELEASE_TIME` (controllo `before < RELEASE_TIME && m_throwTime >= RELEASE_TIME`),
 * garantendo un singolo spawn indipendentemente dal valore di `dt`.
 */
bool DonkeyKong::update(float dt)
{
    if (defeated()) return false;

    m_time += dt;
    bool release = false;

    if (m_throwing) {
        const float before = m_throwTime;
        m_throwTime += dt;

        if (before < RELEASE_TIME && m_throwTime >= RELEASE_TIME) release = true;

        if (m_throwTime >= THROW_DURATION) {
            m_throwing = false;
            m_throwTime = 0.0f;
            m_wait = 0.0f;
        }
    }
    else {
        m_wait += dt;
        if (m_wait >= WAIT_TIME) {
            m_throwing = true;
            m_throwTime = 0.0f;
        }
    }
    return release;
}

/**
 * @brief Calcola l'oscillazione sinusoidale a riposo: `sin(BEAT_SPEED * m_time)` in [-1, 1].
 */
float DonkeyKong::beat() const
{
    return std::sin(BEAT_SPEED * m_time);
}

/**
 * @brief Restituisce l'avanzamento normalizzato del gesto di lancio in [0, 1] (0 se a riposo).
 */
float DonkeyKong::throwProgress() const
{
    return m_throwing ? m_throwTime / THROW_DURATION : 0.0f;
}
