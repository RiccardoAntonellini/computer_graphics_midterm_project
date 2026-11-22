#pragma once
#include "Pauline.h"
#include "Rig.h"
#include "Scene.h"

/**
 * @class PaulineView
 * @brief Rappresentazione grafica di Pauline costruita come Rig di primitive geometriche.
 *
 * Anima il braccio di saluto (`m_waveArm`) con una rotazione periodica in funzione del tempo.
 */
class PaulineView {
public:
    /**
     * @brief Costruisce il Rig di Pauline registrando tutte le sue parti nella Scene.
     * @param scene Scena grafica condivisa.
     */
    explicit PaulineView(Scene& scene);

    /**
     * @brief Aggiorna le Model Matrix del Rig e l'oscillazione del braccio di saluto.
     * @param scene Scena grafica da aggiornare.
     * @param pauline Stato logico di Pauline.
     * @param time Tempo assoluto in secondi per l'animazione periodica.
     */
    void update(Scene& scene, const Pauline& pauline, float time);

private:
    Rig m_rig;
    std::size_t m_waveArm = 0; ///< Indice nel Rig del braccio animato.
    std::size_t m_restArm = 0; ///< Indice nel Rig del braccio fermo.
};
