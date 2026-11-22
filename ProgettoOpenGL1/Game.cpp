#include "Game.h"
#include "Config.h"

using namespace BulletConfig;
using namespace WorldConfig;
using namespace DonkeyKongConfig;
using namespace HammerConfig;

/**
 * @brief Costruisce livello, entità logiche e viste nell'ordine di rendering (painter's algorithm).
 */
Game::Game(Scene& scene)
    : m_scene(scene),
    m_level(scene),
    m_barrelStack(scene, m_level),
    m_hammer(scene, m_level),
    m_kong(m_level),
    m_kongView(scene),
    m_pauline(m_level),
    m_paulineView(scene),
    m_bullets(m_level),
    m_bulletsView(scene, m_bullets),
    m_barrels(),
    m_barrelsView(scene),
    m_mario(m_level),
    m_marioView(scene),
    m_shots(),
    m_shotsView(scene)
{}

/**
 * @brief Aggiorna la simulazione se la partita è in corso e sincronizza sempre le viste grafiche.
 */
void Game::update(float dt, float time, const Controls& controls)
{
    if (m_outcome == Outcome::Playing) {
        if (controls.lastLife) setLastLife();
        updatePlaying(dt, time, controls);
    }
    updateViews(time);
}

/**
 * @brief Raccoglie tutte le AABB solide non attraversabili da Mario (pila barili + DK se attivo).
 */
std::vector<AABB> Game::solidObstacles() const
{
    std::vector<AABB> solids = m_barrelStack.boxes();
    if (!m_kong.defeated()) solids.push_back(m_kong.box());
    return solids;
}

/**
 * @brief Rileva il contatto tra Mario e Donkey Kong.
 *
 * Poiché DK è un ostacolo solido, la fisica di Mario impedisce la sovrapposizione stretta
 * delle due AABB: si espande quindi la AABB di DK di 1 unità su entrambi gli assi per
 * rilevare il contatto radente.
 */
bool Game::marioTouchesKong() const
{
    if (m_kong.defeated()) return false;
    AABB contact = m_kong.box();
    contact.halfSize += glm::vec2(1.0f);
    return intersects(m_mario.box(), contact);
}

/**
 * @brief Esegue un passo completo della logica di gioco attiva.
 *
 * Sequenza:
 *   1. Movimento di Mario, aggiornamento di DK (con eventuale spawn barile) e moto dei barili.
 *   2. Raccolta martello/proiettili e aggiornamento del timer.
 *   3. Gestione sparo verso DK e consumo degli impatti.
 *   4. Risoluzione collisioni: prima il martello distrugge i barili, poi i barili residui o DK
 *      danneggiano Mario (se non invulnerabile).
 *   5. Controllo condizione di vittoria (DK sconfitto + contatto con Pauline).
 */
void Game::updatePlaying(float dt, float time, const Controls& controls)
{
    // 1) Movimento
    m_mario.update(dt, controls, m_level, solidObstacles());
    if (m_kong.update(dt)) m_barrels.spawn(m_level);
    m_barrels.update(dt, m_level);

    // 2) Raccolte e timer
    checkHammerPickup();
    m_bullets.collectTouched(m_mario.box(), time);
    updateTimer(dt);

    // 3) Sparo
    if (controls.fire) tryFire();
    m_shots.update(dt);
    m_kong.hit(m_shots.consumeHits(m_kong.box()));

    // 4) Collisioni barili e DK
    if (m_mario.hammering()) m_barrels.destroyHit(m_mario.hammerBox());
    if (!m_mario.invulnerable() && m_barrels.hits(m_mario.box())) loseLife();

    if (m_outcome == Outcome::Playing && !m_mario.invulnerable() && marioTouchesKong())
        hitByKong();

    // 5) Vittoria
    if (m_outcome == Outcome::Playing && m_kong.defeated() &&
        intersects(m_mario.box(), m_pauline.box()))
        m_outcome = Outcome::Won;
}

/**
 * @brief Propaga lo stato logico di tutte le entità alle rispettive viste nella Scene.
 */
void Game::updateViews(float time)
{
    m_bulletsView.update(m_scene, m_bullets);
    m_barrelsView.update(m_scene, m_barrels);
    m_kongView.update(m_scene, m_kong);
    m_paulineView.update(m_scene, m_pauline, time);
    m_hammer.update(m_scene, time);
    m_marioView.update(m_scene, m_mario);
    m_shotsView.update(m_scene, m_shots);
}

/**
 * @brief Attiva il power-up del martello se Mario lo tocca a piedi (non su scala e non già armato).
 */
void Game::checkHammerPickup()
{
    if (m_hammer.taken() || m_mario.hammering() || m_mario.climbing()) return;
    if (intersects(m_mario.box(), m_hammer.box())) {
        m_hammer.take();
        m_mario.startHammer();
    }
}

/**
 * @brief Decrementa il timer finché non sono stati raccolti tutti i proiettili; allo scadere toglie una vita.
 */
void Game::updateTimer(float dt)
{
    if (m_bullets.allCollected()) return;

    m_timeLeft -= dt;
    if (m_timeLeft <= 0.0f) {
        m_bullets.reset();
        m_timeLeft = RulesConfig::TIME_LIMIT;
        loseLife();
    }
}

/**
 * @brief Verifica se Mario si trova a piedi sulla trave più alta (`DK_PLATFORM`).
 */
bool Game::marioOnTop() const
{
    if (m_mario.climbing()) return false;
    const Platform& top = m_level.platforms()[DK_PLATFORM];
    const glm::vec2& feet = m_mario.feet();
    return top.coversX(feet.x) && feet.y >= top.yAt(feet.x) - 1.0f;
}

/**
 * @brief Spara un colpo mirato al centro di DK se Mario è armato, in cima e ha munizioni residue.
 */
void Game::tryFire()
{
    if (!m_bullets.allCollected() || m_kong.defeated()) return;
    if (m_shotsFired >= COUNT) return;
    if (!marioOnTop()) return;

    const glm::vec2 origin = m_mario.feet() + glm::vec2(0.0f, 6.0f);
    const glm::vec2 target = m_kong.box().center;
    if (m_shots.fire(origin, target)) ++m_shotsFired;
}

/**
 * @brief Gestisce la perdita di una vita da barile/timer: resetta Mario, barili e timer (o termina in sconfitta).
 */
void Game::loseLife()
{
    if (m_outcome != Outcome::Playing) return;

    --m_lives;
    if (m_lives <= 0) {
        m_outcome = Outcome::Lost;
        return;
    }

    m_mario.reset(m_level);
    m_mario.startInvulnerable();
    m_timeLeft = RulesConfig::TIME_LIMIT;
    m_barrels.reset();
    m_kong.resetThrow();

    if (m_lives == RulesConfig::HAMMER_LIVES) m_hammer.respawn(m_level);
}

/**
 * @brief Gestisce il danno da contatto con DK: toglie una vita e attiva l'invulnerabilità senza resettare la scena.
 */
void Game::hitByKong()
{
    if (m_outcome != Outcome::Playing) return;

    --m_lives;
    if (m_lives <= 0) {
        m_outcome = Outcome::Lost;
        return;
    }

    m_mario.startInvulnerable();

    if (m_lives == RulesConfig::HAMMER_LIVES) m_hammer.respawn(m_level);
}

/**
 * @brief Comando di debug (tasto P): imposta le vite a 1 e fa comparire il martello.
 */
void Game::setLastLife()
{
    if (m_lives <= RulesConfig::HAMMER_LIVES) return;
    m_lives = RulesConfig::HAMMER_LIVES;
    m_hammer.respawn(m_level);
}

/**
 * @brief Ripristina tutte le entità e le regole allo stato iniziale di nuova partita.
 */
void Game::reset()
{
    m_outcome = Outcome::Playing;
    m_lives = RulesConfig::START_LIVES;
    m_timeLeft = RulesConfig::TIME_LIMIT;
    m_shotsFired = 0;

    m_bullets.reset();
    m_mario.reset(m_level);
    m_kong.reset();
    m_barrels.reset();
    m_shots.reset();
    m_hammer.hide();
}

/**
 * @brief Compila la struct `HudInfo` con lo stato corrente della partita per il rendering ImGui.
 */
HudInfo Game::hudInfo() const
{
    HudInfo info;
    info.outcome = m_outcome;
    info.lives = m_lives;
    info.bullets = m_bullets.collectedCount();
    info.bulletTotal = COUNT;
    info.showTimer = !m_bullets.allCollected();
    info.timeLeft = m_timeLeft;
    info.armed = m_bullets.allCollected();
    info.ammo = COUNT - m_shotsFired;
    info.kongLife = MAX_HITS - m_kong.hits();
    info.hammerFraction = m_mario.hammerTimeLeft() / HAMMER_DURATION;
    return info;
}
