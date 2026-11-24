#pragma once
#include "Collision.h"
#include "Controls.h"
#include "Level.h"
#include <glm/glm.hpp>
#include <vector>

/**
 * @enum MarioState
 * @brief Stati di movimento del giocatore.
 */
enum class MarioState { Walking, Climbing };

/**
 * @class Mario
 * @brief Logica cinematica e stato del giocatore completamente disaccoppiata dal rendering.
 *
 * Il punto di riferimento spaziale m_feet rappresenta il centro orizzontale della base
 * (i piedi): questo semplifica sia il vincolo di appoggio sulle travi inclinate sia la
 * costruzione della Model Matrix del Rig in `MarioView`.
 */
class Mario {
public:
    /**
     * @brief Inizializza Mario nella posizione di partenza sulla trave inferiore.
     * @param level Livello corrente.
     */
    explicit Mario(const Level& level);

    /**
     * @brief Ripristina posizione, velocità, stato, martello e invulnerabilità ai valori iniziali.
     * @param level Livello corrente.
     */
    void reset(const Level& level);

    /**
     * @brief Aggiorna fisica, macchina a stati (camminata/salto/scala) e timer di Mario.
     * @param dt Delta time in secondi.
     * @param controls Input utente del frame corrente.
     * @param level Livello corrente (travi e scale).
     * @param solids AABB degli ostacoli solidi non attraversabili (pila di barili, DK).
     */
    void update(float dt, const Controls& controls, const Level& level,
        const std::vector<AABB>& solids);

    const glm::vec2& feet() const { return m_feet; }

    float facing() const { return m_facing; }// Direzione di sguardo: +1 destra, -1 sinistra.

    float swing() const { return m_swing; }// Oscillazione normalizzata degli arti in [-1, 1].

    bool climbing() const { return m_state == MarioState::Climbing; }

    /**
     * @brief Restituisce la bounding box di collisione di Mario in coordinate mondo.
     * @return AABB Rettangolo centrato in (feet.x, feet.y + MARIO_HEIGHT / 2).
     */
    AABB box() const;

    /**
     * @brief Attiva il power-up del martello impostando il timer a HAMMER_DURATION.
     */
    void startHammer();

    bool hammering() const { return m_hammerTime > 0.0f; }

    float hammerTimeLeft() const { return m_hammerTime; }

    /**
     * @brief Restituisce la hitbox offensiva del martello davanti a Mario (nel verso m_facing).
     * @return AABB Area entro cui i barili rotolanti vengono distrutti.
     */
    AABB hammerBox() const;

    /**
     * @brief Attiva la finestra temporanea di invulnerabilità con lampeggio.
     */
    void startInvulnerable();
    bool invulnerable() const { return m_invulnTime > 0.0f; }

    /**
     * @brief Determina se Mario deve essere visibile nel frame corrente durante il lampeggio.
     * @return true se visibile (sempre true quando non è invulnerabile), false nei frame "spenti".
     */
    bool blinkVisible() const;

private:
    glm::vec2 m_feet{ 0.0f };
    float m_velocityY = 0.0f;
    bool m_onGround = true;
    MarioState m_state = MarioState::Walking;
    int m_ladder = -1;// Indice della scala attualmente in uso (-1 se nessuna).
    float m_facing = 1.0f;// +1.0f destra, -1.0f sinistra.
    float m_phase = 0.0f;// Fase accumulata (rad) per l'oscillazione sinusoidale degli arti.
    float m_swing = 0.0f;// Valore sin(m_phase) in [-1, 1] esposto alla View.
    float m_hammerTime = 0.0f;// Secondi residui del power-up martello.
    float m_invulnTime = 0.0f;// Secondi residui di invulnerabilità.

    void tryStartClimb(const Controls& controls, const Level& level);
    void climb(float dt, const Controls& controls, const Level& level);
    void walk(float dt, const Controls& controls, const Level& level, const std::vector<AABB>& solids);
    void stopClimbing();
};
