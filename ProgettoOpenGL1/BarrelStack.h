#pragma once
#include "Collision.h"
#include "Level.h"
#include "Scene.h"
#include <vector>

/**
 * @class BarrelStack
 * @brief Gestisce la pila statica di barili accanto a Donkey Kong.
 *
 * Dimostra il riutilizzo geometrico tramite istanziazione: una singola Geometry condivisa
 * viene istanziata quattro volte nella Scene con matrici di trasformazione (Model Matrix)
 * indipendenti. Funge anche da ostacolo solido esponendo le AABB di ciascun barile.
 */
class BarrelStack {
public:
    /**
     * @brief Crea la geometria condivisa, registra le 4 istanze nella Scene e calcola le loro AABB.
     * @param scene Scena grafica in cui registrare geometria e istanze.
     * @param level Livello corrente da cui ricavare la posizione della pila.
     */
    BarrelStack(Scene& scene, const Level& level);

    /**
     * @brief Restituisce le bounding box solide dei barili della pila per il controllo collisioni.
     * @return const std::vector<AABB>& Vettore delle AABB in coordinate mondo.
     */
    const std::vector<AABB>& boxes() const { return m_boxes; }

private:
    std::vector<std::size_t> m_instances; ///< Indici delle 4 istanze registrate nella Scene.
    std::vector<AABB> m_boxes;            ///< Bounding box statiche in coordinate mondo.
};
