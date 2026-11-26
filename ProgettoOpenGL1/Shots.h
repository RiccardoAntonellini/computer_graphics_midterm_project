#pragma once
#include "Collision.h"
#include "Config.h"
#include <glm/glm.hpp>
#include <vector>

/**
 * @struct Shot
 * @brief Stato cinematico di un singolo colpo sparato da Mario.
 */
struct Shot {
    bool active = false;// True se il colpo è attualmente in volo.
    glm::vec2 position{ 0.0f };// Posizione corrente del centro in coordinate mondo.
    glm::vec2 velocity{ 0.0f };// Vettore velocità costante diretto verso il bersaglio.
};

/**
 * @class Shots
 * @brief Gestisce il pool e il moto rettilineo uniforme dei colpi sparati da Mario.
 */
class Shots {
public:
    /**
     * @brief Inizializza il pool di colpi.
     */
    Shots();

    /**
     * @brief Disattiva tutti i colpi in volo.
     */
    void reset();

    /**
     * @brief Attiva un colpo dal pool sparandolo da origin in direzione di target.
     *
     * La velocità è calcolata normalizzando il vettore direzione.
     *
     * @param origin Punto di partenza (es. posizione di Mario).
     * @param target Punto bersaglio (es. centro di Donkey Kong).
     * @return true se un colpo è stato attivato, false se il pool è pieno.
     */
    bool fire(const glm::vec2& origin, const glm::vec2& target);

    /**
     * @brief Integra la posizione di tutti i colpi attivi (position += velocity * dt) e disattiva quelli fuori campo.
     * @param dt Delta time in secondi.
     */
    void update(float dt);

    /**
     * @brief Disattiva i colpi attivi che intersecano target e restituisce il numero di impatti.
     * @param target AABB del bersaglio.
     * @return int Numero di colpi andati a segno in questo frame.
     */
    int consumeHits(const AABB& target);

    /**
     * @brief Espone in sola lettura il pool dei colpi (usato da ShotsView).
     * @return const std::vector<Shot>& Vettore dei colpi.
     */
    const std::vector<Shot>& all() const { return m_shots; }

private:
    std::vector<Shot> m_shots;
};
