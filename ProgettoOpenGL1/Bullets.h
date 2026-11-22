#pragma once
#include "Collision.h"
#include "Level.h"
#include <glm/glm.hpp>
#include <vector>

/**
 * @struct Bullet
 * @brief Stato logico di un singolo oggetto raccoglibile (pickup).
 */
struct Bullet {
    glm::vec2 position{ 0.0f }; ///< Posizione di riposo (centro) in coordinate mondo.
    bool collected = false;     ///< True se il pickup è già stato raccolto dal giocatore.
};

/**
 * @class Bullets
 * @brief Gestisce la logica dei pickup raccoglibili senza dipendenze dal layer grafico.
 */
class Bullets {
public:
    /**
     * @brief Inizializza i pickup posizionandoli secondo la configurazione del livello.
     * @param level Livello corrente.
     */
    explicit Bullets(const Level& level);

    /**
     * @brief Ripristina tutti i pickup allo stato non raccolto (`collected = false`).
     */
    void reset();

    /**
     * @brief Verifica l'intersezione tra la AABB data e i pickup non ancora raccolti.
     *
     * Calcola la posizione istantanea di ciascun pickup tramite `animatedPosition()` per
     * garantire coerenza tra la hitbox logica (CPU) e l'animazione visiva (GPU).
     *
     * @param box AABB del giocatore.
     * @param time Tempo corrente in secondi (usato per valutare l'oscillazione).
     * @return int Numero di pickup raccolti in questo frame.
     */
    int collectTouched(const AABB& box, float time);

    /**
     * @brief Restituisce il numero totale di pickup raccolti finora.
     * @return int Conteggio dei pickup con `collected == true`.
     */
    int collectedCount() const;

    /**
     * @brief Verifica se tutti i pickup del livello sono stati raccolti.
     * @return true se `collectedCount() == BulletConfig::COUNT`, false altrimenti.
     */
    bool allCollected() const { return collectedCount() == BulletConfig::COUNT; }

    /**
     * @brief Espone in sola lettura la lista dei pickup (usata da BulletsView).
     * @return const std::vector<Bullet>& Vettore dei pickup.
     */
    const std::vector<Bullet>& all() const { return m_bullets; }

private:
    std::vector<Bullet> m_bullets;

    /**
     * @brief Calcola la posizione oscillante istantanea del pickup i-esimo.
     *
     * Replica lato CPU la stessa legge oraria implementata nel vertex shader `coin.vert`
     * (tipicamente un moto armonico lungo Y sfasato in base all'indice), assicurando che
     * le collisioni corrispondano esattamente a ciò che viene renderizzato.
     *
     * @param index Indice del pickup nel vettore `m_bullets`.
     * @param time Tempo corrente in secondi.
     * @return glm::vec2 Posizione animata in coordinate mondo.
     */
    glm::vec2 animatedPosition(std::size_t index, float time) const;
};
