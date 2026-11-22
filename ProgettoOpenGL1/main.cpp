#include "WindowUtils.h"
#include "FileUtils.h"
#include "Shader.h"
#include "Renderer.h"
#include "BackgroundRenderer.h"
#include "BulletRenderer.h"
#include "Scene.h"
#include "Game.h"
#include "Hud.h"
#include "Input.h"
#include "Config.h"
#include <algorithm>
#include <exception>
#include <iostream>
#include <string>

using namespace WorldConfig;

/**
 * @brief Entry point dell'applicazione: inizializza OpenGL, shader, scena e avvia il game loop.
 *
 * Architettura di rendering (3 programmi shader distinti per frame):
 *   1. `background`: shader procedurale full-screen in NDC (`background.vert/frag`).
 *   2. `renderer`: shader standard degli oggetti (`basic.vert/frag`) con proiezione ortografica,
 *      escludendo la Geometry dei bullets.
 *   3. `BulletRenderer`: shader animato dei bullets (`bullet.vert` + `basic.frag`).
 *   4. `hud`: overlay Dear ImGui.
 *
 * Gestione del tempo: `dt = clamp(now - previousTime, 0.0, 0.03)` garantisce moto indipendente
 * dal frame rate ed evita salti fisici (tunneling) in caso di lag o spostamento della finestra.
 */
int main()
{
    try {
        // L'ordine di dichiarazione è inverso a quello di distruzione (RAII):
        // `window` (contesto OpenGL) viene distrutto per ultimo.
        WindowUtils window(610, 700, "Donkey Kong");
        Hud hud(window.handle());

        const std::string basicFragment = FileUtils::readText("shaders/basic.frag");
        Shader shader(FileUtils::readText("shaders/basic.vert"), basicFragment);
        Renderer renderer(shader);
        renderer.setOrthoProjection(FIELD_HALF_WIDTH, HALF_HEIGHT);

        Shader backgroundShader(FileUtils::readText("shaders/background.vert"),
            FileUtils::readText("shaders/background.frag"));
        BackgroundRenderer background(backgroundShader);

        Shader bulletShader(FileUtils::readText("shaders/bullet.vert"), basicFragment);
        BulletRenderer bulletRenderer(bulletShader);

        Scene scene;
        Game game(scene);
        InputReader input;

        double previousTime = glfwGetTime();
        while (!window.shouldClose()) {
            glfwPollEvents();

            const double now = glfwGetTime();
            const float time = static_cast<float>(now);
            const float dt = std::clamp(static_cast<float>(now - previousTime), 0.0f, 0.03f);
            previousTime = now;

            const Controls controls = input.read(window.handle());
            if (controls.quit) window.close();
            if (controls.restart) game.reset();
            game.update(dt, time, controls);

            window.beginFrame();
            background.draw(time);
            renderer.drawScene(scene, game.bulletGeometry());
            bulletRenderer.draw(scene, game.bulletInstances(),
                renderer.projectionMatrix(), time);
            hud.draw(game.hudInfo());
            window.endFrame();
        }
    }
    catch (const std::exception& e) {
        std::cerr << "ERRORE: " << e.what() << '\n';
        return 1;
    }
    return 0;
}
