#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

/**
 * @namespace TransformConfig
 * @brief Funzioni helper per costruire matrici di trasformazione affine 2D in coordinate omogenee 4x4.
 *
 * In OpenGL/GLM i vertici sono vettori colonna: in un prodotto matriciale `A * B * v` le
 * trasformazioni si applicano da destra verso sinistra (prima `B`, poi `A`).
 */
namespace TransformConfig {

    /**
     * @brief Costruisce la composizione standard TRS: M = T(position) * R_z(angle) * S(scale).
     *
     * Ordine di applicazione sul vertice locale:
     *   1. Scala (S) lungo X e Y.
     *   2. Rotazione (R) in senso antiorario attorno all'asse Z di `angle` radianti.
     *   3. Traslazione (T) alla posizione finale nel mondo.
     *
     * @param position Traslazione (x, y).
     * @param angle Angolo di rotazione attorno all'asse Z in radianti.
     * @param scale Fattori di scala (sx, sy); valori negativi producono una riflessione (mirroring).
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
     * La mesh dell'arto è un rettangolo centrato in (0, 0) di altezza `length`. Per farlo ruotare
     * attorno all'estremo superiore anziché attorno al proprio centro, si compone:
     *   M = T(pivotX, pivotY) * R_z(angle) * T(0, -length / 2)
     *
     * Effetto passo-passo sul vertice (da destra a sinistra):
     *   1. T(0, -length/2): sposta il rettangolo in basso in modo che il suo bordo superiore coincida con l'origine (0, 0).
     *   2. R_z(angle): ruota l'arto attorno all'origine (che ora è l'articolazione).
     *      Con angle > 0 (antiorario) l'arto pendente si sposta verso +X.
     *   3. T(pivotX, pivotY): posiziona l'articolazione nella sede corretta del corpo (spalla o anca).
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