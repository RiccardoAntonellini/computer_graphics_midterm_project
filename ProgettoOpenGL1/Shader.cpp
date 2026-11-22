#include "Shader.h"
#include <glm/gtc/type_ptr.hpp>
#include <stdexcept>
#include <iostream>

namespace {
    /**
     * @brief Compila un singolo stadio shader GLSL (vertex o fragment) e verifica gli errori.
     * @throws std::runtime_error Se la compilazione fallisce (stampa l'info log su `std::cerr`).
     */
    GLuint compileShader(GLenum kind, const std::string& src)
    {
        GLuint id = glCreateShader(kind);
        const char* ptr = src.c_str();
        glShaderSource(id, 1, &ptr, nullptr);
        glCompileShader(id);

        GLint ok = 0;
        glGetShaderiv(id, GL_COMPILE_STATUS, &ok);
        if (!ok) {
            char log[4096] = {};
            glGetShaderInfoLog(id, sizeof(log), nullptr, log);
            std::cerr << log << '\n';
            glDeleteShader(id);
            throw std::runtime_error("Compilazione shader fallita");
        }
        return id;
    }
}

/**
 * @brief Compila vertex e fragment shader, li collega in un programma OpenGL e libera gli oggetti intermedi.
 */
Shader::Shader(const std::string& vs, const std::string& fs)
{
    GLuint v = compileShader(GL_VERTEX_SHADER, vs);
    GLuint f = compileShader(GL_FRAGMENT_SHADER, fs);

    m_program = glCreateProgram();
    glAttachShader(m_program, v);
    glAttachShader(m_program, f);
    glLinkProgram(m_program);
    glDeleteShader(v);
    glDeleteShader(f);

    GLint ok = 0;
    glGetProgramiv(m_program, GL_LINK_STATUS, &ok);
    if (!ok) {
        char log[4096] = {};
        glGetProgramInfoLog(m_program, sizeof(log), nullptr, log);
        std::cerr << log << '\n';
        glDeleteProgram(m_program);
        m_program = 0;
        throw std::runtime_error("Link shader fallito");
    }
}

/**
 * @brief Elimina il programma shader dalla GPU.
 */
Shader::~Shader()
{
    if (m_program) glDeleteProgram(m_program);
}

/**
 * @brief Attiva il programma shader nella pipeline OpenGL corrente.
 */
void Shader::use() const
{
    glUseProgram(m_program);
}

/**
 * @brief Carica una matrice 4x4 nella uniform `name` (column-major, senza trasposizione).
 */
void Shader::setMat4(const char* name, const glm::mat4& value) const
{
    glUniformMatrix4fv(glGetUniformLocation(m_program, name), 1, GL_FALSE, glm::value_ptr(value));
}

/**
 * @brief Carica uno scalare float nella uniform `name`.
 */
void Shader::setFloat(const char* name, float value) const
{
    glUniform1f(glGetUniformLocation(m_program, name), value);
}
