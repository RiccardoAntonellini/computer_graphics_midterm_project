#include "Scene.h"
#include <stdexcept>

/**
 * @brief Crea una nuova Geometry (VAO/VBO) in GPU e restituisce il suo indice univoco.
 */
std::size_t Scene::addGeometry(const MeshData& mesh)
{
    m_geometries.push_back(std::make_unique<Geometry>(mesh));
    return m_geometries.size() - 1;
}

/**
 * @brief Aggiunge un'istanza visibile con Model Matrix identità associata a `geometryIndex`.
 * @throws std::out_of_range Se `geometryIndex` non corrisponde a nessuna Geometry registrata.
 */
std::size_t Scene::addInstance(std::size_t geometryIndex)
{
    if (geometryIndex >= m_geometries.size()) {
        throw std::out_of_range("geometryIndex non valido");
    }
    m_instances.push_back({ geometryIndex, glm::mat4(1.0f), true });
    return m_instances.size() - 1;
}

/**
 * @brief Accesso modificabile all'istanza `index` (con controllo dei limiti).
 */
GeometryInstance& Scene::instance(std::size_t index)
{
    return m_instances.at(index);
}

/**
 * @brief Accesso in sola lettura alla Geometry `index` (con controllo dei limiti).
 */
const Geometry& Scene::geometry(std::size_t index) const
{
    return *m_geometries.at(index);
}

/**
 * @brief Restituisce il vettore completo delle istanze nell'ordine di rendering.
 */
const std::vector<GeometryInstance>& Scene::instances() const
{
    return m_instances;
}

/**
 * @brief Svuota istanze e geometrie (invocando i distruttori RAII dei buffer GPU).
 */
void Scene::clear()
{
    m_instances.clear();
    m_geometries.clear();
}
