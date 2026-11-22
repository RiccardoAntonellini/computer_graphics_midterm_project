#include "Input.h"

namespace {
    /**
     * @brief Restituisce true se il tasto GLFW specificato è attualmente premuto.
     */
    bool isDown(GLFWwindow* window, int key)
    {
        return glfwGetKey(window, key) == GLFW_PRESS;
    }
}

/**
 * @brief Campiona i tasti di movimento (continui) e i tasti di azione (fronte di salita).
 */
Controls InputReader::read(GLFWwindow* window)
{
    Controls c;
    c.left = isDown(window, GLFW_KEY_A) || isDown(window, GLFW_KEY_LEFT);
    c.right = isDown(window, GLFW_KEY_D) || isDown(window, GLFW_KEY_RIGHT);
    c.up = isDown(window, GLFW_KEY_W) || isDown(window, GLFW_KEY_UP);
    c.down = isDown(window, GLFW_KEY_S) || isDown(window, GLFW_KEY_DOWN);
    c.quit = isDown(window, GLFW_KEY_ESCAPE);

    // Rilevamento fronte di salita (edge-triggered): true solo al primo frame di pressione.
    const bool jumpNow = isDown(window, GLFW_KEY_SPACE);
    const bool fireNow = isDown(window, GLFW_KEY_F);
    const bool restartNow = isDown(window, GLFW_KEY_R);
    const bool lastLifeNow = isDown(window, GLFW_KEY_P);
    c.jump = jumpNow && !m_wasJump;
    c.fire = fireNow && !m_wasFire;
    c.restart = restartNow && !m_wasRestart;
    c.lastLife = lastLifeNow && !m_wasLastLife;

    m_wasJump = jumpNow;
    m_wasFire = fireNow;
    m_wasRestart = restartNow;
    m_wasLastLife = lastLifeNow;
    return c;
}
