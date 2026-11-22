#pragma once
#include "HudInfo.h"
#include <GLFW/glfw3.h>

/**
 * @class Hud
 * @brief Interfaccia grafica Dear ImGui per lo stato della partita (vite, proiettili, timer, martello, esito).
 *
 * Gestisce il ciclo di vita di ImGui secondo il pattern RAII: deve essere costruita DOPO
 * la creazione della finestra GLFW e distrutta PRIMA della sua chiusura.
 */
class Hud {
public:
    /**
     * @brief Inizializza Dear ImGui e i backend GLFW/OpenGL3.
     * @param window Puntatore alla finestra GLFW attiva.
     */
    explicit Hud(GLFWwindow* window);

    /**
     * @brief Chiude i backend e distrugge il contesto Dear ImGui.
     */
    ~Hud();

    Hud(const Hud&) = delete;
    Hud& operator=(const Hud&) = delete;

    /**
     * @brief Disegna l'HUD per il frame corrente a partire dallo snapshot `info`.
     * @param info Dati di gioco correnti forniti da `Game::hudInfo()`.
     */
    void draw(const HudInfo& info);
};
