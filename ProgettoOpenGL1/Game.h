#pragma once
#include "BarrelStack.h"
#include "Barrels.h"
#include "BarrelsView.h"
#include "Controls.h"
#include "DonkeyKong.h"
#include "DonkeyKongView.h"
#include "Hammer.h"
#include "HudInfo.h"
#include "Level.h"
#include "Mario.h"
#include "MarioView.h"
#include "Pauline.h"
#include "PaulineView.h"
#include "Bullets.h"
#include "BulletsView.h"
#include "Config.h"
#include "Scene.h"
#include "Shots.h"
#include "ShotsView.h"
#include <vector>

/**
 * @class Game
 * @brief Orchestratore centrale della partita: coordina logica, collisioni, regole e viste.
 *
 * Non possiede la Scene (la riceve per riferimento).
 * @note L'ordine di dichiarazione dei membri privati determina l'ordine di costruzione e,
 *       di conseguenza, l'ordine di registrazione e disegno nella Scene.
 */
class Game {
public:
    /**
     * @brief Inizializza il livello, tutte le entità logiche e le rispettive viste nella Scene.
     * @param scene Scena grafica condivisa.
     * @throws std::runtime_error Se il caricamento di una mesh OBJ da disco fallisce.
     */
    explicit Game(Scene& scene);

    /**
     * @brief Esegue un passo di simulazione della partita e sincronizza tutte le viste grafiche.
     * @param dt Delta time in secondi dall'ultimo frame.
     * @param time Tempo assoluto dall'avvio in secondi (usato per animazioni periodiche).
     * @param controls Stato dei comandi utente del frame corrente.
     */
    void update(float dt, float time, const Controls& controls);

    /**
     * @brief Riavvia completamente la partita (vite, timer, posizioni, entità e stato).
     */
    void reset();

    /**
     * @brief Costruisce lo snapshot dei dati da visualizzare nell'HUD ImGui.
     * @return HudInfo Struttura con vite, proiettili, tempo residuo, stato martello ed esito.
     */
    HudInfo hudInfo() const;

    /**
     * @brief Restituisce l'indice della Geometry condivisa dei pickup (per il rendering dedicato).
     * @return std::size_t Indice nella Scene.
     */
    std::size_t bulletGeometry() const { return m_bulletsView.geometryIndex(); }

    /**
     * @brief Restituisce gli indici delle istanze dei pickup da passare a BulletRenderer.
     * @return const std::vector<std::size_t>& Indici delle istanze nella Scene.
     */
    const std::vector<std::size_t>& bulletInstances() const { return m_bulletsView.instances(); }

private:
    Scene& m_scene;
    Level m_level;
    BarrelStack m_barrelStack;// Barili statici accanto a DK (ostacoli solidi).
    Hammer m_hammer;
    DonkeyKong m_kong;
    DonkeyKongView m_kongView;
    Pauline m_pauline;
    PaulineView m_paulineView;
    Bullets m_bullets;
    BulletsView m_bulletsView;
    Barrels m_barrels;// Logica dei barili rotolanti.
    BarrelsView m_barrelsView;// Grafica dei barili rotolanti.
    Mario m_mario;
    MarioView m_marioView;
    Shots m_shots;
    ShotsView m_shotsView;

    Outcome m_outcome = Outcome::Playing;
    int m_lives = RulesConfig::START_LIVES;
    float m_timeLeft = RulesConfig::TIME_LIMIT;
    int m_shotsFired = 0;

    void updatePlaying(float dt, float time, const Controls& controls);
    void updateViews(float time);
    void checkHammerPickup();
    void updateTimer(float dt);
    void tryFire();
    bool marioOnTop() const;
    void loseLife();
    void hitByKong();// Contatto con DK: toglie una vita senza resettare il livello.
    void setLastLife();// Funzione di debug (tasto P): imposta le vite a 1.
    std::vector<AABB> solidObstacles() const;// Restituisce tutte le AABB solide non attraversabili da Mario.
    bool marioTouchesKong() const;// Verifica il contatto tra Mario e DK ancora attivo.
};
