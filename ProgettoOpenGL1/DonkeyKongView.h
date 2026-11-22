#pragma once
#include "DonkeyKong.h"
#include "Rig.h"
#include "Scene.h"

/**
 * @class DonkeyKongView
 * @brief Rappresentazione grafica di Donkey Kong costruita come Rig di primitive geometriche.
 *
 * Assembla più primitive tramite trasformazioni gerarchiche/locali gestite da `Rig` e anima
 * le due braccia (`m_backArm`, `m_frontArm`) leggendo lo stato (`beat()`, `throwProgress()`)
 * da `DonkeyKong`.
 */
class DonkeyKongView {
public:
    /**
     * @brief Costruisce il Rig di Donkey Kong registrando tutte le parti nella Scene.
     * @param scene Scena grafica.
     */
    explicit DonkeyKongView(Scene& scene);

    /**
     * @brief Aggiorna le matrici di trasformazione delle parti del Rig in base allo stato di DK.
     * @param scene Scena grafica da aggiornare.
     * @param kong Stato logico corrente di Donkey Kong.
     */
    void update(Scene& scene, const DonkeyKong& kong);

private:
    Rig m_rig;
    std::size_t m_backArm = 0;  ///< Indice nel Rig del braccio posteriore.
    std::size_t m_frontArm = 0; ///< Indice nel Rig del braccio anteriore.
};
