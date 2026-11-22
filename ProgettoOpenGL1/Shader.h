#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <string>

/**
 * @class Shader
 * @brief Wrapper RAII per un programma shader OpenGL (vertex + fragment shader GLSL).
 *
 * Compila, linka e gestisce il ciclo di vita di `m_program`, offrendo metodi tipizzati
 * per attivare il programma (`glUseProgram`) e impostare variabili uniform.
 */
class Shader {
public:
    /**
     * @brief Compila i sorgenti GLSL di vertex e fragment shader e linka il programma OpenGL.
     * @param vertexSource Codice sorgente del vertex shader.
     * @param fragmentSource Codice sorgente del fragment shader.
     * @throws std::runtime_error Se la compilazione o il linking falliscono.
     */
    Shader(const std::string& vertexSource, const std::string& fragmentSource);

    /**
     * @brief Rilascia il programma shader dalla GPU (`glDeleteProgram`).
     */
    ~Shader();

    Shader(const Shader&) = delete;
    Shader& operator=(const Shader&) = delete;

    /**
     * @brief Attiva questo programma shader nella pipeline OpenGL corrente (`glUseProgram`).
     */
    void use() const;

    /**
     * @brief Imposta una uniform di tipo `mat4` (es. `uProjection`, `uModel`).
     * @param uniformName Nome della variabile uniform nello shader GLSL.
     * @param value Matrice 4x4 da caricare in GPU.
     */
    void setMat4(const char* uniformName, const glm::mat4& value) const;

    /**
     * @brief Imposta una uniform di tipo `float` (es. `uTime`).
     * @param uniformName Nome della variabile uniform nello shader GLSL.
     * @param value Valore scalare da caricare in GPU.
     */
    void setFloat(const char* uniformName, float value) const;

private:
    GLuint m_program = 0;
};
