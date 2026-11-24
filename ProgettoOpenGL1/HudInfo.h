#pragma once

/**
 * @enum Outcome
 * @brief Stato complessivo della partita.
 */
enum class Outcome { Playing, Won, Lost };

/**
 * @struct HudInfo
 * @brief Snapshot in sola lettura dei dati di gioco da mostrare nell'HUD ImGui.
 *
 * Viene popolata da Game::hudInfo() e consumata da Hud::draw() per evitare
 * dipendenze dirette tra l'interfaccia grafica e la logica di gioco.
 */
struct HudInfo {
    Outcome outcome = Outcome::Playing; // Esito corrente (in corso, vittoria, sconfitta).
    int lives = 0;                      // Vite rimanenti di Mario.
    int bullets = 0;                    // Numero di proiettili raccolti finora.
    int bulletTotal = 0;                // Numero totale di proiettili da raccogliere.
    bool showTimer = false;             // True se il conto alla rovescia deve essere visibile.
    float timeLeft = 0.0f;              // Secondi rimanenti prima dello scadere del tempo.
    bool armed = false;                 // True quando tutti i 5 proiettili sono stati raccolti.
    int ammo = 0;                       // Colpi residui ancora sparabili verso Donkey Kong.
    int kongLife = 0;                   // Colpi ancora necessari per sconfiggere Donkey Kong.
    float hammerFraction = 0.0f;        // Tempo residuo del martello normalizzato in [0, 1].
};
