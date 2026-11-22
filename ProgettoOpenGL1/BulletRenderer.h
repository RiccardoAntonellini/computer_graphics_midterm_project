#pragma once
#include "Scene.h"
#include "Shader.h"
#include <glm/glm.hpp>
#include <vector>

/**
 * @class BulletRenderer
 * @brief Renderer specializzato per disegnare istanze animate tramite lo shader `bullet.vert`.
 *
 * Non possiede né geometrie né istanze (che restano di proprietà della Scene): si limita
 * ad attivare il proprio shader program, impostare le uniform necessarie (projection,
 * model, timeSeconds) e invocare le draw call per il sottoinsieme di istanze richiesto.
 */
class BulletRenderer {
public:
    /**
     * @brief Costruisce il renderer associandolo allo shader dedicato ai pickup.
     * @param shader Riferimento allo shader program animato (deve restare valido per tutta la vita del renderer).
     */
    explicit BulletRenderer(const Shader& shader) : m_shader(shader) {}

    /**
     * @brief Renderizza le istanze specificate applicando proiezione e animazione temporale.
     *
     * Nella pipeline grafica ogni vertice locale viene trasformato in Clip Space tramite:
     *   v_clip = P * M * v_local
     * con uno spostamento verticale aggiuntivo calcolato nel vertex shader in funzione di timeSeconds.
     *
     * @param scene Scena contenente le geometrie e le istanze.
     * @param instances Indici delle istanze da disegnare.
     * @param projection Matrice di proiezione ortografica 4x4 (World Space -> Clip Space).
     * @param timeSeconds Tempo corrente in secondi passato come uniform allo shader.
     */
    void draw(const Scene& scene, const std::vector<std::size_t>& instances,
        const glm::mat4& projection, float timeSeconds) const;

private:
    const Shader& m_shader;
};
