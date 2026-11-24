#pragma once
#include "Barrels.h"
#include "Scene.h"
#include <vector>

/**
 * @class BarrelsView
 * @brief Rappresentazione grafica dei barili rotolanti nella Scene.
 *
 * Ogni barile del pool è un oggetto composto assemblato da 3 istanze (un disco centrale
 * e due strisce incrociate). Le geometrie di base sono caricate in GPU una sola volta
 * nel costruttore; a ogni frame vengono aggiornate esclusivamente le Model Matrix
 * (componendo traslazione e rotazione attorno al centro) senza riscrivere i vertici.
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
     * Per ogni barile attivo aggiorna la Model Matrix (traslazione nel centro e rotazione di
     * `barrel.angle` per le strisce, rendendo visibile il rotolamento); nasconde i barili inattivi.
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
        std::size_t body = 0;    // Indice dell'istanza del disco principale.
        std::size_t stripeA = 0; // Indice dell'istanza della prima striscia (ruota con il barile).
        std::size_t stripeB = 0; // Indice dell'istanza della seconda striscia (ortogonale alla prima).
    };

    std::vector<Slot> m_slots;
};
