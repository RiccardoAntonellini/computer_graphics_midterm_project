#pragma once
#include "Collision.h"
#include "Level.h"
#include <glm/glm.hpp>
#include <vector>

/**
 * @struct Bullet
 * @brief Stato logico di un singolo oggetto raccoglibile (proiettile).
 */
struct Bullet {
    glm::vec2 position{ 0.0f };// Posizione di riposo (centro) in coordinate mondo.
    bool collected = false;// True se il pickup è già stato raccolto dal giocatore.
};

/**
 * @class Bullets
 * @brief Gestisce la logica dei proiettili raccoglibili senza dipendenze dal layer grafico.
 */
class Bullets {
public:
    /**
     * @brief Inizializza i proiettili posizionandoli secondo la configurazione del livello.
     * @param level Livello corrente.
     */
    explicit Bullets(const Level& level);

    /**
     * @brief Ripristina tutti i proiettili allo stato non raccolto (collected = false).
     */
    void reset();

    /**
     * @brief Verifica l'intersezione tra la AABB data e i proiettili non ancora raccolti.
     *
     * Calcola la posizione istantanea di ciascun proiettile tramite animatedPosition() per
     * garantire coerenza tra la hitbox logica (CPU) e l'animazione visiva (GPU).
     *
     * @param box AABB del giocatore.
     * @param time Tempo corrente in secondi (usato per valutare l'oscillazione).
     * @return int Numero di proiettili raccolti in questo frame.
     */
    int collectTouched(const AABB& box, float time);

    /**
     * @brief Restituisce il numero totale di proiettili raccolti finora.
     * @return int Conteggio dei proiettili con collected == true.
     */
    int collectedCount() const;

    /**
     * @brief Verifica se tutti i proiettili del livello sono stati raccolti.
     * @return true se collectedCount() == BulletConfig::COUNT, false altrimenti.
     */
    bool allCollected() const { return collectedCount() == BulletConfig::COUNT; }

    /**
     * @brief Espone in sola lettura la lista dei proiettili (usata da BulletsView).
     * @return const std::vector<Bullet>& Vettore dei proiettili.
     */
    const std::vector<Bullet>& all() const { return m_bullets; }

private:
    std::vector<Bullet> m_bullets;

    /**
     * @brief Calcola la posizione oscillante istantanea del proiettile i-esimo.
     *
     * Replica lato CPU la stessa legge oraria implementata nel vertex shader `bullet.vert`
     * (tipicamente un moto armonico lungo Y sfasato in base all'indice), assicurando che
     * le collisioni corrispondano esattamente a ciò che viene renderizzato.
     *
     * @param index Indice del proiettile nel vettore m_bullets.
     * @param time Tempo corrente in secondi.
     * @return glm::vec2 Posizione animata in coordinate mondo.
     */
    glm::vec2 animatedPosition(std::size_t index, float time) const;
};
