#pragma once
#include "Scene.h"
#include "MeshData.h"
#include <glm/glm.hpp>
#include <cstddef>
#include <vector>

/**
 * @class Rig
 * @brief Oggetto complesso assemblato da più parti geometriche tramite trasformazioni gerarchiche.
 *
 * Ogni parte è una GeometryInstance nella Scene dotata di una propria trasformazione locale
 * M_local (rispetto all'origine del personaggio, tipicamente i piedi).
 * A ogni frame, la matrice mondo di ciascuna parte è ottenuta per composizione matriciale:
 *   M_world = M_parent * M_local
 * 
 * Poiché GLM utilizza vettori colonna, un vertice locale v viene prima trasformato nello
 * spazio del personaggio (M_local * v) e poi portato nello spazio mondo (M_parent * ...).
 * I vertici nei VBO non vengono mai modificati; l'ordine di disegno coincide con l'ordine di aggiunta.
 */
class Rig {
public:
    /**
     * @brief Crea una nuova Geometry nella Scene e la aggiunge come parte del Rig.
     * @param scene Scena grafica condivisa.
     * @param mesh Dati geometrici della nuova parte.
     * @param local Matrice di trasformazione locale iniziale (default: identità).
     * @return std::size_t Indice della parte all'interno del Rig (usato con setLocal()).
     */
    std::size_t addPart(Scene& scene, const MeshData& mesh, const glm::mat4& local = glm::mat4(1.0f)) {
        return addPartFromGeometry(scene, scene.addGeometry(mesh), local);
    }

    /**
     * @brief Aggiunge una parte al Rig riutilizzando una Geometry già esistente nella Scene.
     * @param scene Scena grafica condivisa.
     * @param geometry Indice della Geometry condivisa (dati GPU condivisi, stato separato).
     * @param local Matrice di trasformazione locale iniziale (default: identità).
     * @return std::size_t Indice della parte all'interno del Rig.
     */
    std::size_t addPartFromGeometry(Scene& scene, std::size_t geometry, const glm::mat4& local = glm::mat4(1.0f)) {
        m_instances.push_back(scene.addInstance(geometry));
        m_local.push_back(local);
        return m_instances.size() - 1;
    }

    /**
     * @brief Aggiorna la trasformazione locale di una singola parte (es. rotazione di un arto).
     * @param part Indice della parte restituito da addPart() o addPartFromGeometry().
     * @param local Nuova matrice locale 4x4.
     */
    void setLocal(std::size_t part, const glm::mat4& local) { 
        m_local.at(part) = local; 
    }

    /**
     * @brief Propaga la trasformazione del padre a tutte le parti del Rig nella Scene.
     *
     * Calcola modelMatrix = parent * m_local[i] per ogni parte i.
     *
     * @param scene Scena grafica da aggiornare.
     * @param parent Matrice di trasformazione globale del personaggio (es. traslazione ai piedi + flip).
     */
    void update(Scene& scene, const glm::mat4& parent)
    {
        for (std::size_t i = 0; i < m_instances.size(); ++i) {
            scene.instance(m_instances[i]).modelMatrix = parent * m_local[i];
        }
    }

    /**
     * @brief Imposta la visibilità di tutte le parti del Rig in blocco (es. per il lampeggio).
     * @param scene Scena grafica da aggiornare.
     * @param visible True per mostrare il Rig, false per nasconderlo.
     */
    void setVisible(Scene& scene, bool visible)
    {
        for (std::size_t index : m_instances) {
            scene.instance(index).visible = visible;
        }
    }

private:
    std::vector<std::size_t> m_instances; // Indici delle istanze nella Scene.
    std::vector<glm::mat4> m_local;       // Matrici di trasformazione locale di ciascuna parte.
};
