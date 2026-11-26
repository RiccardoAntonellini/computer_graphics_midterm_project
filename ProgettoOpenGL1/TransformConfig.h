#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

/**
 * @namespace TransformConfig
 * @brief Funzioni helper per costruire matrici di trasformazione affine 2D in coordinate omogenee 4x4.
 */
namespace TransformConfig {

    /**
     * @brief Costruisce la composizione standard TRS: M = T(position) * R_z(angle) * S(scale).
     *
     * @param position Traslazione (x, y).
     * @param angle Angolo di rotazione attorno all'asse Z in radianti.
     * @param scale Fattori di scala (sx, sy); valori negativi producono una riflessione.
     * @return glm::mat4 Matrice di trasformazione 4x4.
     */
    inline glm::mat4 make(const glm::vec2& position, float angle, const glm::vec2& scale)
    {
        return glm::translate(glm::mat4(1.0f), glm::vec3(position, 0.0f)) *
            glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0f, 0.0f, 1.0f)) *
            glm::scale(glm::mat4(1.0f), glm::vec3(scale, 1.0f));
    }

    /**
     * @brief Costruisce una trasformazione TRS con fattore di scala uniforme su X e Y.
     * @param position Traslazione (x, y).
     * @param angle Angolo di rotazione attorno all'asse Z in radianti.
     * @param scale Fattore di scala uniforme.
     * @return glm::mat4 Matrice di trasformazione 4x4.
     */
    inline glm::mat4 makeUniform(const glm::vec2& position, float angle, float scale)
    {
        return make(position, angle, glm::vec2(scale));
    }

    /**
     * @brief Costruisce una matrice di traslazione pura nel piano XY.
     * @param x Spostamento lungo X.
     * @param y Spostamento lungo Y.
     * @return glm::mat4 Matrice di traslazione 4x4.
     */
    inline glm::mat4 at(float x, float y)
    {
        return glm::translate(glm::mat4(1.0f), glm::vec3(x, y, 0.0f));
    }

    /**
     * @brief Costruisce la trasformazione locale di un arto che ruota attorno al suo estremo superiore (spalla o anca).
     *
     * @param pivotX Coordinata X dell'articolazione nello spazio locale del Rig.
     * @param pivotY Coordinata Y dell'articolazione nello spazio locale del Rig.
     * @param angle Angolo di rotazione in radianti attorno all'articolazione.
     * @param length Lunghezza dell'arto.
     * @return glm::mat4 Matrice locale 4x4 dell'arto.
     */
    inline glm::mat4 limb(float pivotX, float pivotY, float angle, float length)
    {
        return at(pivotX, pivotY) *
            glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0f, 0.0f, 1.0f)) *
            at(0.0f, -length * 0.5f);
    }

} // namespace TransformConfig