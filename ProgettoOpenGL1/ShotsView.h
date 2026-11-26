#pragma once
#include "Scene.h"
#include "Shots.h"
#include <vector>

/**
 * @class ShotsView
 * @brief Rappresentazione grafica dei colpi sparati da Mario nella Scene.
 *
 * Crea una singola Geometry condivisa da tutte le istanze del pool,
 * aggiornando a ogni frame visibilità e traslazione.
 */
class ShotsView {
public:
    /**
     * @brief Registra la Geometry del disco e pre-alloca le istanze per il pool nella Scene.
     * @param scene Scena grafica condivisa.
     */
    explicit ShotsView(Scene& scene);

    /**
     * @brief Sincronizza visibilità e posizione delle istanze con lo stato logico di Shots.
     * @param scene Scena grafica da aggiornare.
     * @param shots Stato logico corrente dei colpi.
     */
    void update(Scene& scene, const Shots& shots);

private:
    std::vector<std::size_t> m_instances;
};
