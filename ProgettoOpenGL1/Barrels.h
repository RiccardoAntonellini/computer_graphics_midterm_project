#pragma once
#include "Collision.h"
#include "Config.h"
#include "Level.h"
#include <glm/glm.hpp>
#include <vector>

/**
 * @struct Barrel
 * @brief Stato cinematico e logico di un singolo barile rotolante.
 *
 * Il punto di riferimento spaziale scelto è feet (il punto di contatto inferiore del disco),
 * che semplifica il vincolo di appoggio sulle piattaforme del livello.
 */
struct Barrel {
    bool active = false;// Indica se il barile è attualmente in gioco (object pooling).
    glm::vec2 feet{ 0.0f };// Coordinate mondo (x, y) del punto più basso del disco.
    float velocityY = 0.0f;// Componente verticale della velocità (per gravità/caduta).
    float direction = 1.0f;// Verso del moto orizzontale: +1.0f (destra), -1.0f (sinistra).
    bool onGround = true;// True se il barile è attualmente appoggiato su una piattaforma.
    float angle = 0.0f;// Angolo di rotazione (in radianti) accumulato dal rotolamento, usato per la Model Matrix.

    /**
     * @brief Calcola il centro geometrico del barile a partire dal punto d'appoggio feet.
     *
     * Applica una traslazione verticale pura lungo l'asse Y.
     *
     * @return glm::vec2 Coordinate mondo del centro del disco.
     */
    glm::vec2 center() const { return { feet.x, feet.y + BarrelConfig::RADIUS }; }
};

/**
 * @class Barrels
 * @brief Gestisce la logica fisica e il ciclo di vita (pool) di tutti i barili rotolanti.
 *
 * Completamente indipendente dal layer grafico. Lo spawn è comandato esternamente
 * dalla logica di gioco (es. sincronizzato con l'animazione di Donkey Kong).
 */
class Barrels {
public:
    /**
     * @brief Inizializza il pool di barili (tutti inattivi).
     */
    Barrels();

    /**
     * @brief Disattiva tutti i barili e ripristina lo stato iniziale.
     */
    void reset();

    /**
     * @brief Attiva un barile inattivo dal pool posizionandolo al punto di spawn del livello.
     * @param level Riferimento al livello corrente (fornisce coordinate di spawn).
     */
    void spawn(const Level& level);

    /**
     * @brief Aggiorna la cinematica (moto, gravità, rotolamento) di tutti i barili attivi.
     * @param dt Delta time in secondi trascorso dall'ultimo frame (moto frame-rate indipendente).
     * @param level Livello corrente usato per verificare appoggi, pendenze e limiti.
     */
    void update(float dt, const Level& level);

    /**
     * @brief Verifica se almeno un barile attivo collide con una data bounding box.
     * @param box AABB dell'entità da testare (es. il giocatore).
     * @return true se esiste almeno un'intersezione, false altrimenti.
     */
    bool hits(const AABB& box) const;

    /**
     * @brief Disattiva tutti i barili attivi che intersecano una data bounding box.
     * @param box AABB dell'area di distruzione (es. hitbox del martello).
     */
    void destroyHit(const AABB& box);

    /**
     * @brief Espone in sola lettura il pool dei barili (usato dalla View per il rendering).
     * @return const std::vector<Barrel>& Vettore di tutti i barili nel pool.
     */
    const std::vector<Barrel>& all() const { return m_barrels; }

private:
    std::vector<Barrel> m_barrels;

    /**
     * @brief Esegue l'integrazione temporale e il controllo collisioni per un singolo barile.
     * @param barrel Riferimento al barile da aggiornare.
     * @param dt Delta time in secondi.
     * @param level Livello corrente.
     */
    void updateOne(Barrel& barrel, float dt, const Level& level);
};
