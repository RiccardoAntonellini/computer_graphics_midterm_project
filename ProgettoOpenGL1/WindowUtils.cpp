#include "WindowUtils.h"
#include "Config.h"
#include <stdexcept>

using namespace WorldConfig;

/**
 * @brief Inizializza GLFW, crea la finestra OpenGL 3.3 Core, attiva V-Sync e carica GLAD.
 */
WindowUtils::WindowUtils(int width, int height, const char* title)
{
    if (!glfwInit()) throw std::runtime_error("Errore inizializzazione GLFW");

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    m_window = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if (!m_window) {
        glfwTerminate();
        throw std::runtime_error("Errore creazione finestra");
    }
    glfwMakeContextCurrent(m_window);
    glfwSwapInterval(1);// V-Sync attivo

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        glfwDestroyWindow(m_window);
        glfwTerminate();
        throw std::runtime_error("Errore inizializzazione GLAD");
    }

    glfwSetWindowAspectRatio(m_window,
        static_cast<int>(2.0f * FIELD_HALF_WIDTH),
        static_cast<int>(2.0f * HALF_HEIGHT));
}

/**
 * @brief Distrugge la finestra GLFW e termina la libreria.
 */
WindowUtils::~WindowUtils()
{
    glfwDestroyWindow(m_window);
    glfwTerminate();
}

/**
 * @brief Aggiorna la viewport OpenGL alle dimensioni correnti del framebuffer e pulisce lo schermo.
 */
void WindowUtils::beginFrame()
{
    int width = 0, height = 0;
    glfwGetFramebufferSize(m_window, &width, &height);
    glViewport(0, 0, width, height);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

/**
 * @brief Scambia il back buffer con il front buffer (double buffering).
 */
void WindowUtils::endFrame()
{
    glfwSwapBuffers(m_window);
}
