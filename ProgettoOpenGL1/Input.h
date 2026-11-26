#pragma once
#define GLFW_INCLUDE_NONE
#pragma once
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include "Controls.h"

/**
 * @class InputReader
 * @brief Traduce lo stato della tastiera GLFW in una struct Controls indipendente dalla libreria.
 *
 * Mantiene lo stato del frame precedente per i comandi discreti (salto, sparo, restart, debug)
 * in modo da rilevare la singola pressione --> pressedThisFrame = isPressedNow && !wasPressedLastFrame
 * evitando ripetizioni involontarie quando il tasto resta premuto per più frame.
 */
class InputReader {
public:
    /**
     * @brief Campiona la tastiera dalla finestra GLFW e aggiorna lo stato interno dei fronti.
     * @param window Finestra GLFW attiva.
     * @return Controls Snapshot dei comandi per il frame corrente.
     */
    Controls read(GLFWwindow* window);

private:
    bool m_wasJump = false;
    bool m_wasFire = false;
    bool m_wasRestart = false;
    bool m_wasLastLife = false;
};
