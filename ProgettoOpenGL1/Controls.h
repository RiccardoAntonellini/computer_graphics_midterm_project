#pragma once

/**
 * @struct Controls
 * @brief Snapshot dello stato dei comandi utente per il frame corrente.
 *
 * Disaccoppia completamente la logica di gioco dalla libreria di input (GLFW):
 * - I campi di movimento continuo (left, right, up, down) sono true finché il tasto è tenuto premuto.
 * - I campi di azione discreta (jump, fire, restart, lastLife) sono true solo nel frame in cui vengono premuti.
 */
struct Controls {
    bool left = false;// Movimento continuo verso sinistra.
    bool right = false;// Movimento continuo verso destra.
    bool up = false;// Salita su scala.
    bool down = false;// Discesa da scala.
    bool jump = false;// Impulso di salto.
    bool fire = false;// Impulso di sparo.
    bool restart = false;// Riavvio partita.
    bool lastLife = false;// Comando di debug (tasto P) = porta le vite a 1.
    bool quit = false;// Richiesta di chiusura dell'applicazione.
};
