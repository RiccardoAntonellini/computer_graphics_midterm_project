#pragma once
#include "Barrels.h"
#include "Scene.h"
#include <vector>

/**
 * @class BarrelsView
 * @brief Rappresentazione grafica dei barili rotolanti nella Scene.
 */
class BarrelsView {
public:
    /**
     * @brief Crea le geometrie condivise e pre-alloca nella Scene le istanze per l'intero pool.
     * @param scene Scena grafica in cui registrare geometrie e istanze.
     */
    explicit BarrelsView(Scene& scene);

    /**
     * @brief Sincronizza visibilità e matrici di trasformazione delle istanze con lo stato logico.
     *
     * @param scene Scena grafica da aggiornare.
     * @param barrels Stato logico corrente dei barili.
     */
    void update(Scene& scene, const Barrels& barrels);

private:
    /**
     * @struct Slot
     * @brief Raggruppa gli indici delle tre istanze nella Scene che compongono un singolo barile.
     */
    struct Slot {
        std::size_t body = 0;// Indice dell'istanza del disco principale.
        std::size_t stripeA = 0;// Indice dell'istanza della prima striscia.
        std::size_t stripeB = 0;// Indice dell'istanza della seconda striscia.
    };

    std::vector<Slot> m_slots;
};
