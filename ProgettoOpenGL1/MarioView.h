#pragma once
#include "Mario.h"
#include "Rig.h"
#include "Scene.h"

/**
 * @class MarioView
 * @brief Rappresentazione grafica di Mario: Rig assemblato da primitive + martello Hermite.
 */
class MarioView {
public:
    /**
     * @brief Costruisce il Rig di Mario e carica la mesh OBJ del martello impugnato.
     * @param scene Scena grafica condivisa.
     * @throws std::runtime_error Se hammer.obj non viene trovato o fallisce il caricamento.
     */
    explicit MarioView(Scene& scene);

    /**
     * @brief Aggiorna trasformazioni e visibilità del Rig e del martello impugnato.
     * @param scene Scena grafica da aggiornare.
     * @param mario Stato logico corrente del giocatore.
     */
    void update(Scene& scene, const Mario& mario);

private:
    Rig m_rig;
    std::size_t m_backLeg = 0;
    std::size_t m_frontLeg = 0;
    std::size_t m_backArm = 0;
    std::size_t m_frontArm = 0;
    std::size_t m_heldHammer = 0;// Indice dell'istanza del martello nella Scene.

    /**
     * @brief Compone la Model Matrix del martello in mano a Mario.
     *
     * @param mario Stato logico del giocatore.
     * @return glm::mat4 Matrice di trasformazione 4x4 in coordinate omogenee.
     */
    glm::mat4 heldHammerMatrix(const Mario& mario) const;
};
