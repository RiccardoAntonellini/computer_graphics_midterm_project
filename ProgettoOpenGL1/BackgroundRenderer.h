#pragma once
#include <glad/glad.h>
#include "Shader.h"

/**
 * @class BackgroundRenderer
 * @brief Gestisce il rendering dello sfondo a schermo intero tramite uno shader dedicato.
 *
 * Non fa parte della Scene: la geometria (un quad full-screen) è definita direttamente
 * in Normalized Device Coordinates (NDC), ovvero nel dominio [-1, 1] x [-1, 1].
 * Poiché i vertici sono già in Clip Space / NDC, il vertex shader non richiede alcuna
 * matrice di modello (Model) né di proiezione (Projection) per coprire l'intera viewport.
 */
    class BackgroundRenderer
{
public:
    /**
     * @brief Costruisce il renderer allocando VAO e VBO per il quad full-screen in NDC.
     * @param shader Riferimento allo shader program dedicato allo sfondo (deve restare valido
     *               per l'intera durata di vita dell'istanza).
     */
    explicit BackgroundRenderer(const Shader& shader);

    /**
     * @brief Rilascia le risorse OpenGL (VAO e VBO) allocate nel costruttore.
     */
    ~BackgroundRenderer();

    // Disabilita copia per garantire unicità di proprietà sulle risorse GPU (RAII).
    BackgroundRenderer(const BackgroundRenderer&) = delete;
    BackgroundRenderer& operator=(const BackgroundRenderer&) = delete;

    /**
     * @brief Disegna lo sfondo passando il tempo corrente allo shader per effetti procedurali.
     * @param timeSeconds Tempo trascorso dall'avvio dell'applicazione in secondi, passato come
     *                    uniform per animare gradienti o variazioni cromatiche lato GPU.
     */
    void draw(float timeSeconds) const;

private:
    const Shader& m_shader;
    GLuint m_vao = 0;
    GLuint m_vbo = 0;
};
