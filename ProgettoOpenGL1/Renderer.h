#pragma once
#include "Scene.h"
#include "Shader.h"
#include <glm/glm.hpp>
#include <limits>

/**
 * @class Renderer
 * @brief Gestisce il rendering degli oggetti della Scene tramite proiezione ortografica 2D.
 *
 * Attiva lo shader degli oggetti, carica la matrice di proiezione P e, per ogni istanza
 * visibile della scena, imposta la rispettiva Model Matrix M e invoca la draw call della
 * Geometry associata.
 */
class Renderer {
public:
    /**
     * @brief Costruisce il renderer associandolo allo shader standard degli oggetti.
     * @param shader Riferimento allo shader program (deve restare valido per tutta la vita del renderer).
     */
    explicit Renderer(const Shader& shader) : m_shader(shader) {}

    /**
     * @brief Costruisce la matrice di proiezione ortografica 2D centrata nell'origine.
     *
     * Mappa linearmente il rettangolo del mondo [-halfWidth, +halfWidth] x [-halfHeight, +halfHeight]
     * nello spazio normalizzato NDC [-1, +1] x [-1, +1] senza distorsione prospettica:
     *   P = glm::ortho(-halfWidth, halfWidth, -halfHeight, halfHeight, -1.0f, 1.0f)
     *
     * @param halfWidth Semi-larghezza visibile in coordinate mondo.
     * @param halfHeight Semi-altezza visibile in coordinate mondo.
     */
    void setOrthoProjection(float halfWidth, float halfHeight);

    /**
     * @brief Disegna tutte le istanze visibili della scena, escludendo opzionalmente una Geometry.
     *
     * Il parametro `excludedGeometryIndex` permette di saltare le istanze renderizzate da
     * uno shader dedicato (es. i pickup disegnati da `CoinRenderer`).
     *
     * @param scene Scena contenente geometrie e istanze.
     * @param excludedGeometryIndex Indice della Geometry da saltare (default: nessuna esclusione).
     */
    void drawScene(const Scene& scene, std::size_t excludedGeometryIndex =
        std::numeric_limits<std::size_t>::max()) const;

    /**
     * @brief Restituisce la matrice di proiezione ortografica corrente.
     * @return const glm::mat4& Matrice 4x4 di proiezione.
     */
    const glm::mat4& projectionMatrix() const { return m_projection; }

private:
    const Shader& m_shader;
    glm::mat4 m_projection{ 1.0f };
};
