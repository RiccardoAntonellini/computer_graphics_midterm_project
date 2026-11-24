#pragma once
#include "Collision.h"
#include "Level.h"
#include <glm/glm.hpp>

/**
 * @class Pauline
 * @brief Stato logico statico di Pauline sulla piattaforma superiore.
 *
 * La condizione di vittoria al contatto con Mario è verificata da Game tramite box().
 */
class Pauline {
public:
    /**
     * @brief Posiziona i piedi di Pauline sulla piattaforma PaulineConfig::PLATFORM all'ascissa X.
     * @param level Livello corrente.
     */
    explicit Pauline(const Level& level) : m_feet(PaulineConfig::X, level.platforms()[PaulineConfig::PLATFORM].yAt(PaulineConfig::X)) {}

    /**
     * @brief Restituisce le coordinate mondo dei piedi di Pauline (origine del Rig).
     * @return const glm::vec2& Coordinate (x, y).
     */
    const glm::vec2& feet() const { return m_feet; }

    /**
     * @brief Calcola la bounding box di Pauline in coordinate mondo.
     *
     * Centro: (m_feet.x, m_feet.y + HEIGHT / 2), semi-dimensioni: (HALF_W, HEIGHT / 2).
     *
     * @return AABB Area di collisione per il salvataggio.
     */
    AABB box() const
    {
        return { { m_feet.x, m_feet.y + PaulineConfig::HEIGHT * 0.5f }, { PaulineConfig::HALF_W, PaulineConfig::HEIGHT * 0.5f } };
    }

private:
    glm::vec2 m_feet;
};
