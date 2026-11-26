#pragma once
#include "Bullets.h"
#include "Scene.h"
#include <vector>

/**
 * @class BulletsView
 * @brief Gestisce la rappresentazione grafica dei proiettili nella Scene.
 */
class BulletsView {
public:
    /**
     * @brief Registra la geometria condivisa e crea nella Scene un'istanza per ogni proiettile.
     * @param scene Scena grafica.
     * @param bullets Stato logico iniziale dei proiettili.
     */
    BulletsView(Scene& scene, const Bullets& bullets);

    /**
     * @brief Aggiorna la visibilità delle istanze nella Scene in base allo stato collected.
     * @param scene Scena grafica da aggiornare.
     * @param bullets Stato logico corrente dei proiettili.
     */
    void update(Scene& scene, const Bullets& bullets);

    /**
     * @brief Restituisce l'indice della geometria condivisa nella Scene.
     * @return std::size_t Indice della Geometry.
     */
    std::size_t geometryIndex() const { return m_geometry; }

    /**
     * @brief Restituisce gli indici delle istanze dei proiettili nella Scene.
     * @return const std::vector<std::size_t>& Vettore degli indici di istanza.
     */
    const std::vector<std::size_t>& instances() const { return m_instances; }

private:
    std::size_t m_geometry = 0;
    std::vector<std::size_t> m_instances;
};
