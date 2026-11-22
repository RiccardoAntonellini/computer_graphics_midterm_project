#include "Hud.h"
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <cmath>

namespace {
    // Finestre overlay non interattive (senza barra del titolo, bordi o input).
    constexpr ImGuiWindowFlags OVERLAY_FLAGS =
        ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoSavedSettings |
        ImGuiWindowFlags_AlwaysAutoResize;
}

/**
 * @brief Crea il contesto Dear ImGui e inizializza i backend GLFW e OpenGL 3.3.
 */
Hud::Hud(GLFWwindow* window)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");
}

/**
 * @brief Chiude i backend ImGui e distrugge il contesto.
 */
Hud::~Hud()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

/**
 * @brief Renderizza il pannello di stato in alto a destra e, a fine partita, il banner centrale.
 */
void Hud::draw(const HudInfo& info)
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    const ImVec2 screen = ImGui::GetIO().DisplaySize;

    // Pannello in alto a destra (pivot = (1, 0)): lascia libera la zona sinistra di DK.
    ImGui::SetNextWindowPos(ImVec2(screen.x - 10.0f, 10.0f), ImGuiCond_Always, ImVec2(1.0f, 0.0f));
    ImGui::Begin("Stato", nullptr, OVERLAY_FLAGS);

    if (info.showTimer && info.outcome == Outcome::Playing)
        ImGui::Text("Tempo: %d", static_cast<int>(std::ceil(info.timeLeft)));

    ImGui::Text("Vite: %d", info.lives);
    ImGui::Text("Proiettili: %d/%d", info.bullets, info.bulletTotal);
    if (info.armed) {
        ImGui::Text("Munizioni (F): %d", info.ammo);
        ImGui::Text("Colpi a Kong: %d", info.kongLife);
    }
    if (info.hammerFraction > 0.0f) {
        ImGui::Text("Martello");
        ImGui::ProgressBar(info.hammerFraction, ImVec2(120.0f, 0.0f), "");
    }
    ImGui::End();

    // Banner centrale di vittoria o sconfitta (pivot = (0.5, 0.5)).
    if (info.outcome != Outcome::Playing) {
        ImGui::SetNextWindowPos(ImVec2(screen.x * 0.5f, screen.y * 0.5f), ImGuiCond_Always, ImVec2(0.5f, 0.5f));
        ImGui::Begin("Esito", nullptr, OVERLAY_FLAGS);
        ImGui::SetWindowFontScale(2.0f);
        ImGui::Text("%s", info.outcome == Outcome::Won ? "HAI VINTO!" : "GAME OVER");
        ImGui::SetWindowFontScale(1.0f);
        ImGui::Text("Premi R per ricominciare");
        ImGui::End();
    }

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
