#pragma once
#include "MeshData.h"
#include <glm/glm.hpp>

/**
 * @namespace ShapeGenerator
 * @brief Generatori procedurali di primitive geometriche 2D centrate nell'origine locale (0, 0).
 *
 * Tutte le funzioni restituiscono una `MeshData` triangolata (`GL_TRIANGLES`) con colore uniforme.
 */
namespace ShapeGenerator {

    /**
     * @brief Genera un rettangolo centrato in (0, 0) composto da 2 triangoli (6 vertici).
     *
     * Estensione locale: X in [-width/2, +width/2], Y in [-height/2, +height/2].
     *
     * @param width Larghezza del rettangolo.
     * @param height Altezza del rettangolo.
     * @param color Colore RGB uniforme.
     * @return MeshData Mesh triangolata del rettangolo.
     */
    MeshData rectangle(float width, float height, const glm::vec3& color);

    /**
     * @brief Genera un disco (cerchio pieno) centrato in (0, 0) approssimato da un poligono regolare.
     *
     * Suddivide l'angolo giro 2*PI in `segments` settori triangolari con vertice comune nell'origine
     * e punti sul bordo: (radius * cos(theta), radius * sin(theta)).
     *
     * @param radius Raggio del disco.
     * @param color Colore RGB uniforme.
     * @param segments Numero di triangoli sul perimetro (default: 40).
     * @return MeshData Mesh triangolata del disco.
     */
    MeshData disk(float radius, const glm::vec3& color, int segments = 40);

    /**
     * @brief Genera un triangolo isoscele centrato nell'origine: base in basso, vertice in alto.
     *
     * Vertici: (-base/2, -height/2), (+base/2, -height/2), (0, +height/2).
     *
     * @param base Lunghezza della base.
     * @param height Altezza del triangolo.
     * @param color Colore RGB uniforme.
     * @return MeshData Mesh del singolo triangolo.
     */
    MeshData triangle(float base, float height, const glm::vec3& color);

} // namespace ShapeGenerator