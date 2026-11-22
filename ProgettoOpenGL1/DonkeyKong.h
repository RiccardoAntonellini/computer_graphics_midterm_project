#pragma once
#include "Collision.h"
#include "Level.h"
#include <glm/glm.hpp>

/**
 * @class DonkeyKong
 * @brief Logica comportamentale e stato di Donkey Kong (nessuna dipendenza grafica).
 *
 * Gestisce la macchina a stati temporizzata del lancio dei barili (attesa -> gesto di lancio
 * con rilascio a metà animazione -> attesa), l'oscillazione a riposo e il conteggio dei colpi subiti.
 */
class DonkeyKong {
public:
    /**
     * @brief Inizializza Donkey Kong posizionandolo sulla piattaforma designata del livello.
     * @param level Livello corrente da cui ricavare l'altezza della piattaforma `DK_PLATFORM`.
     */
    explicit DonkeyKong(const Level& level);

    /**
     * @brief Ripristina lo stato completo (azzera sia i colpi subiti sia il ciclo di lancio).
     */
    void reset();

    /**
     * @brief Riavvia il ciclo di lancio mantenendo invariato il conteggio dei colpi subiti.
     */
    void resetThrow();

    /**
     * @brief Avanza i timer di animazione e di lancio in modo indipendente dal frame rate.
     * @param dt Delta time in secondi trascorso dall'ultimo frame.
     * @return true nel singolo frame in cui DK raggiunge `RELEASE_TIME` e rilascia un barile.
     */
    bool update(float dt);

    /**
     * @brief Registra uno o più colpi subiti da Mario.
     * @param count Numero di colpi da sommare.
     */
    void hit(int count);

    /**
     * @brief Restituisce il numero di colpi subiti finora.
     * @return int Conteggio dei colpi.
     */
    int hits() const { return m_hits; }

    /**
     * @brief Verifica se Donkey Kong è stato sconfitto (`m_hits >= MAX_HITS`).
     * @return true se sconfitto, false altrimenti.
     */
    bool defeated() const { return m_hits >= DonkeyKongConfig::MAX_HITS; }

    /**
     * @brief Calcola la bounding box di collisione di Donkey Kong in coordinate mondo.
     * @return AABB Rettangolo di collisione centrato sopra `m_feet`.
     */
    AABB box() const;

    /**
     * @brief Restituisce la posizione dei piedi in coordinate mondo (origine del Rig).
     * @return const glm::vec2& Coordinate (x, y) dei piedi.
     */
    const glm::vec2& feet() const { return m_feet; }

    /**
     * @brief Restituisce il valore normalizzato dell'oscillazione delle braccia a riposo.
     * @return float Valore sinusoidale in [-1, 1] calcolato in funzione di `m_time`.
     */
    float beat() const;

    /**
     * @brief Restituisce l'avanzamento normalizzato del gesto di lancio.
     * @return float 0.0f se non sta lanciando, altrimenti valore in [0, 1] pari a `m_throwTime / THROW_DURATION`.
     */
    float throwProgress() const;

private:
    glm::vec2 m_feet{ 0.0f }; ///< Coordinate mondo del punto di appoggio.
    float m_time = 0.0f;      ///< Tempo accumulato per l'oscillazione periodica a riposo.
    float m_wait = 0.0f;      ///< Timer trascorso dall'ultimo lancio.
    float m_throwTime = 0.0f; ///< Timer interno al gesto di lancio corrente.
    bool m_throwing = false;  ///< True durante l'esecuzione del gesto di lancio.
    int m_hits = 0;           ///< Numero di colpi subiti.
};
