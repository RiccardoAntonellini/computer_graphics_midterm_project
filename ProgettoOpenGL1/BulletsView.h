#pragma once
#include "Bullets.h"
#include "Scene.h"
#include <vector>

/**
 * @class BulletsView
 * @brief Gestisce la rappresentazione grafica dei pickup nella Scene.
 *
 * Utilizza una singola Geometry condivisa (modellata per il proiettile/pickup) istanziata
 * 5 volte. Le istanze vengono renderizzate tramite `CoinRenderer` (con shader dedicato
 * `coin.vert`) anziché dal renderer standard della scena.
 */
class BulletsView {
public:
    /**
     * @brief Registra la geometria condivisa e crea nella Scene un'istanza per ogni pickup.
     * @param scene Scena grafica.
     * @param bullets Stato logico iniziale dei pickup.
     */
    BulletsView(Scene& scene, const Bullets& bullets);

    /**
     * @brief Aggiorna la visibilità delle istanze nella Scene in base allo stato `collected`.
     * @param scene Scena grafica da aggiornare.
     * @param bullets Stato logico corrente dei pickup.
     */
    void update(Scene& scene, const Bullets& bullets);

    /**
     * @brief Restituisce l'indice della geometria condivisa nella Scene.
     * @return std::size_t Indice della Geometry.
     */
    std::size_t geometryIndex() const { return m_geometry; }

    /**
     * @brief Restituisce gli indici delle istanze dei pickup nella Scene.
     * @return const std::vector<std::size_t>& Vettore degli indici di istanza (passato a CoinRenderer).
     */
    const std::vector<std::size_t>& instances() const { return m_instances; }

private:
    std::size_t m_geometry = 0;
    std::vector<std::size_t> m_instances;
};
