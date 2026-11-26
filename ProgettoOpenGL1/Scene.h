#pragma once
#include "Geometry.h"
#include <glm/glm.hpp>
#include <memory>
#include <vector>
#include <cstddef>

/**
 * @struct GeometryInstance
 * @brief Stato di una singola istanza renderizzabile nella Scene.
 */
struct GeometryInstance {
    std::size_t geometryIndex = 0;// Indice della Geometry condivisa in `Scene`.
    glm::mat4 modelMatrix{ 1.0f };// Matrice di modello 4x4 in coordinate omogenee.
    bool visible = true;// Se false, l'istanza viene saltata dal renderer.
};

/**
 * @class Scene
 * @brief Contenitore centrale che separa i dati geometrici condivisi dalle istanze.
 *
 * Le istanze vengono disegnate nell'ordine di inserimento.
 */
class Scene {
public:
    /**
     * @brief Carica una nuova mesh in GPU creando una Geometry e ne restituisce l'indice.
     * @param mesh Dati geometrici lato CPU.
     * @return std::size_t Indice univoco della Geometry registrata.
     */
    std::size_t addGeometry(const MeshData& mesh);

    /**
     * @brief Crea una nuova istanza collegata a una Geometry esistente.
     * @param geometryIndex Indice della Geometry da referenziare.
     * @return std::size_t Indice univoco dell'istanza creata.
     */
    std::size_t addInstance(std::size_t geometryIndex);

    /**
     * @brief Accesso modificabile a un'istanza tramite indice.
     * @param index Indice dell'istanza.
     * @return GeometryInstance& Riferimento all'istanza.
     */
    GeometryInstance& instance(std::size_t index);

    /**
     * @brief Accesso in sola lettura a una Geometry tramite indice.
     * @param index Indice della Geometry.
     * @return const Geometry& Riferimento costante alla Geometry.
     */
    const Geometry& geometry(std::size_t index) const;

    /**
     * @brief Restituisce l'elenco completo delle istanze nell'ordine di disegno.
     * @return const std::vector<GeometryInstance>& Vettore delle istanze.
     */
    const std::vector<GeometryInstance>& instances() const;

    /**
     * @brief Svuota istanze e geometrie rilasciando tutte le risorse GPU possedute.
     */
    void clear();

private:
    std::vector<std::unique_ptr<Geometry>> m_geometries;
    std::vector<GeometryInstance> m_instances;
};
