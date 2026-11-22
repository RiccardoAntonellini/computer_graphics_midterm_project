#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

/**
 * @class WindowUtils
 * @brief Wrapper RAII per la finestra GLFW, il contesto OpenGL 3.3 Core e l'inizializzazione di GLAD.
 *
 * @note Va dichiarata PRIMA di qualsiasi oggetto che possiede risorse OpenGL (`Shader`, `Scene`,
 *       `BackgroundRenderer`, ecc.): in questo modo, grazie all'ordine inverso di distruzione in C++,
 *       il contesto OpenGL viene distrutto per ultimo dopo che tutti i VAO/VBO/shader sono già stati liberati.
 */
class WindowUtils {
public:
    /**
     * @brief Inizializza GLFW, crea la finestra con contesto OpenGL 3.3 Core e carica i puntatori GLAD.
     * @param width Larghezza iniziale della finestra in pixel.
     * @param height Altezza iniziale della finestra in pixel.
     * @param title Titolo della finestra.
     * @throws std::runtime_error Se l'inizializzazione di GLFW, la creazione della finestra o GLAD falliscono.
     */
    WindowUtils(int width, int height, const char* title);

    /**
     * @brief Distrugge la finestra GLFW e termina la libreria (`glfwTerminate`).
     */
    ~WindowUtils();

    WindowUtils(const WindowUtils&) = delete;
    WindowUtils& operator=(const WindowUtils&) = delete;

    /**
     * @brief Restituisce il puntatore nativo alla finestra GLFW (usato da InputReader e Hud).
     * @return GLFWwindow* Handle della finestra.
     */
    GLFWwindow* handle() const { return m_window; }

    /**
     * @brief Verifica se l'utente ha richiesto la chiusura della finestra.
     * @return true se la finestra deve chiudersi.
     */
    bool shouldClose() const { return glfwWindowShouldClose(m_window) != 0; }

    /**
     * @brief Imposta il flag di chiusura della finestra GLFW.
     */
    void close() { glfwSetWindowShouldClose(m_window, GLFW_TRUE); }

    /**
     * @brief Imposta la viewport sull'intero framebuffer e pulisce il color buffer.
     */
    void beginFrame();

    /**
     * @brief Scambia i buffer (double buffering) e processa gli eventi GLFW.
     */
    void endFrame();

private:
    GLFWwindow* m_window = nullptr;
};
