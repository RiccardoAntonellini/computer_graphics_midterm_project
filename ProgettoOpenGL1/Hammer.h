#pragma once
#include "Collision.h"
#include "Level.h"
#include "Scene.h"
#include <glm/glm.hpp>
#include <random>

/**
 * @class Hammer
 * @brief Gestisce il power-up martello (mesh modellata con curve cubiche di Hermite da OBJ) a terra.
 *
 * All'inizio della partita il martello è nascosto (m_taken = true): compare in una posizione
 * casuale valida su una trave intermedia solo quando Game invoca respawn().
 * Una volta raccolto da Mario scompare da terra e viene renderizzato in mano da MarioView.
 */
class Hammer {
public:
    /**
     * @brief Carica la mesh Hermite del martello da OBJ e registra la sua istanza nella Scene.
     * @param scene Scena grafica.
     * @param level Livello corrente.
     * @throws std::runtime_error Se il file OBJ del martello non viene trovato o fallisce il parsing.
     */
    Hammer(Scene& scene, const Level& level);

    /**
     * @brief Posiziona il martello in una coordinata casuale valida su una trave intermedia e lo rende raccoglibile.
     * @param level Livello corrente (usato per evitare bordi e scale).
     */
    void respawn(const Level& level);

    /**
     * @brief Nasconde il martello a terra (usato al reset o quando non disponibile).
     */
    void hide() { m_taken = true; }

    /**
     * @brief Aggiorna visibilità e trasformazione dell'istanza nella Scene.
     * @param scene Scena grafica.
     * @param time Tempo corrente in secondi.
     */
    void update(Scene& scene, float time);

    /**
     * @brief Segna il martello come raccolto dal giocatore.
     */
    void take() { m_taken = true; }

    /**
     * @brief Indica se il martello è già stato raccolto (o è nascosto).
     * @return true se non è presente a terra, false se è raccoglibile.
     */
    bool taken() const { return m_taken; }

    /**
     * @brief Restituisce la bounding box di raccolta del martello in coordinate mondo.
     * @return AABB Area di intersezione per il pickup.
     */
    AABB box() const;

private:
    std::mt19937 m_rng;// Generatore pseudo-casuale Mersenne Twister per lo spawn.
    std::size_t m_instance = 0;// Indice dell'istanza del martello nella Scene.
    glm::vec2 m_position{ 0.0f };// Posizione corrente in coordinate mondo.
    bool m_taken = true;// True se non è a terra (nascosto o già raccolto).
};
