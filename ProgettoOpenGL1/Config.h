#pragma once
#include <glm/glm.hpp>
#include <array>

/**
 * @namespace RulesConfig
 * @brief Parametri delle regole di gioco.
 */
namespace RulesConfig {
	constexpr int START_LIVES = 3;
	constexpr float TIME_LIMIT = 40.0f;// Tempo limite in secondi per raccogliere tutti i proiettili.
	constexpr int HAMMER_LIVES = 1;// Soglia di vite rimanenti alla quale compare il martello.
}

/**
 * @namespace WorldConfig
 * @brief Dimensioni del mondo di gioco, parametri costruttivi di travi/scale e misure OBJ.
 */
namespace WorldConfig {
	// Campo di gioco
	constexpr float HALF_HEIGHT = 70.0f;
	constexpr float FIELD_HALF_WIDTH = 61.0f;

	// Travi
	constexpr float PLATFORM_THICKNESS = 4.0f;
	constexpr float GIRDER_BAR_THICKNESS = 0.9f;// Spessore delle barre orizzontali superiore e inferiore.
	constexpr float AMPIEZZA_TRIANGOLI_TRAVI = 6.0f;// Larghezza di una singola cella a zig-zag del traliccio.
	constexpr float SPESSORE_DIAGONALI_TRAVI = 0.8f;// Spessore dei segmenti diagonali interni.
	constexpr float LAND_TOLERANCE = 1.0f;// Margine verticale per rilevare l'atterraggio sulla superficie.

	// Scale
	constexpr float LADDER_WIDTH = 6.0f;
	constexpr float LADDER_RAIL_WIDTH = 0.8f;
	constexpr float LADDER_RUNG_THICKNESS = 0.7f;
	constexpr float LADDER_RUNG_SPACING = 3.0f;

	// Indici delle travi definiti in LevelConfig::PLATFORMS
	constexpr int DK_PLATFORM = 4;
	constexpr int FIRST_HAMMER_PLATFORM = 1;
	constexpr int LAST_HAMMER_PLATFORM = 3;

	// Barile statico (mesh OBJ)
	constexpr float BARREL_SCALE = 16.0f;
	constexpr float BARREL_BOTTOM_LOCAL = 0.363f;
	constexpr float BARREL_HEIGHT_LOCAL = 0.738f;
	constexpr float BARREL_HALF_WIDTH_LOCAL = 0.18f;
}

/**
 * @namespace MarioConfig
 * @brief Costanti fisiche, cromatiche e anatomiche (Rig) del giocatore.
 */
namespace MarioConfig {
	// Fisica e controllo
	constexpr float MARIO_WIDTH = 6.0f;
	constexpr float MARIO_HEIGHT = 11.0f;
	constexpr float MARIO_START_X = -50.0f;
	constexpr float MOVE_SPEED = 30.0f;
	constexpr float JUMP_SPEED = 55.0f;
	constexpr float GRAVITY = -150.0f;
	constexpr float CLIMB_SPEED = 22.0f;
	constexpr float LADDER_GRAB_RANGE = 3.0f;// Distanza massima in X dal centro scala per iniziare a salire/scendere.
	constexpr float WALK_STICK = 1.0f;// Tolleranza di adesione alla trave in discesa.
	constexpr float WALK_ANIM_SPEED = 14.0f;// Pulsazione dell'oscillazione degli arti durante la camminata.
	constexpr float CLIMB_ANIM_SPEED = 10.0f;// Pulsazione dell'animazione di arrampicata.

	// Colori
	const glm::vec3 RED(0.90f, 0.15f, 0.12f);
	const glm::vec3 BLUE(0.15f, 0.30f, 0.85f);
	const glm::vec3 SKIN(1.00f, 0.78f, 0.60f);

	// Proporzioni delle parti del Rig
	constexpr float LEG_WIDTH = 2.0f;
	constexpr float LEG_LENGTH = 4.0f;
	constexpr float ARM_WIDTH = 1.8f;
	constexpr float ARM_LENGTH = 3.6f;
	constexpr float SHOULDER_Y = 7.2f;

	// Punto di aggancio del martello in mano a Mario
	constexpr float HAND_X = 2.5f;
	constexpr float HAND_Y = 6.0f;
	constexpr float HAMMER_HANDLE_LOCAL = 0.27f;// Offset locale dell'impugnatura in hammer.obj.
	constexpr float HALF_PI = 1.5707963f;
}

/**
 * @namespace PaulineConfig
 * @brief Costanti di posizionamento, animazione e grafica di Pauline.
 */
namespace PaulineConfig {
	constexpr int PLATFORM = 5;
	constexpr float X = 14.0f;
	constexpr float HALF_W = 2.5f;
	constexpr float HEIGHT = 11.0f;
	constexpr float WAVE_SPEED = 6.0f;// Pulsazione del movimento di saluto del braccio.

	// Colori e proporzioni
	const glm::vec3 PINK(0.95f, 0.45f, 0.75f);
	const glm::vec3 SKIN(1.00f, 0.78f, 0.60f);
	const glm::vec3 HAIR(0.45f, 0.22f, 0.10f);
	constexpr float ARM_LENGTH = 3.5f;
	constexpr float SHOULDER_Y = 7.5f;
}

/**
 * @namespace DonkeyKongConfig
 * @brief Costanti di logica, tempistiche di lancio e proporzioni del Rig di Donkey Kong.
 */
namespace DonkeyKongConfig {
	constexpr float X = -36.0f;// Posizione X sulla trave più alta.
	constexpr float WAIT_TIME = 2.4f;// Pausa in secondi tra due lanci consecutivi.
	constexpr float THROW_DURATION = 0.6f;// Durata complessiva dell'animazione di lancio.
	constexpr float RELEASE_TIME = 0.3f;// Istante interno al gesto in cui viene generato il barile.
	constexpr float BEAT_SPEED = 10.0f;// Pulsazione del gesto di battere le braccia a riposo.
	constexpr int MAX_HITS = 5;// Colpi necessari per sconfiggere Donkey Kong.
	constexpr float HALF_W = 8.0f;// Semi-larghezza della AABB di collisione.
	constexpr float HEIGHT = 23.0f;// Altezza della AABB di collisione.

	// Colori e proporzioni
	const glm::vec3 BROWN(0.55f, 0.30f, 0.12f);
	const glm::vec3 TAN(0.95f, 0.70f, 0.45f);
	const glm::vec3 WHITE(1.0f, 1.0f, 1.0f);
	const glm::vec3 BLACK(0.0f, 0.0f, 0.0f);
	constexpr float ARM_WIDTH = 3.0f;
	constexpr float ARM_LENGTH = 8.0f;
	constexpr float SHOULDER_Y = 14.0f;
	constexpr float PI = 3.14159265f;
}

/**
 * @namespace HammerConfig
 * @brief Parametri del martello (mesh Hermite OBJ), durata del power-up, hitbox e invulnerabilità.
 */
namespace HammerConfig {
	// Mesh OBJ
	constexpr float HAMMER_SCALE = 18.0f;
	constexpr float HAMMER_BOTTOM_LOCAL = 0.27f;
	constexpr float HAMMER_EDGE_MARGIN = 8.0f;// Distanza minima dai bordi della trave per lo spawn.
	constexpr float HAMMER_LADDER_MARGIN = 8.0f;// Distanza minima da una scala per lo spawn.

	// Animazione e durata power-up
	constexpr float HAMMER_DURATION = 5.0f;// Durata totale del power-up in secondi.
	constexpr float HAMMER_BLINK_TIME = 1.0f;// Ultimo intervallo in cui il martello lampeggia.
	constexpr float SWING_SPEED = 16.0f;// Pulsazione dell'oscillazione in mano.
	constexpr float HELD_SCALE = 14.0f;// Fattore di scala quando impugnato da Mario.

	// Hitbox di raccolta a terra
	constexpr float PICKUP_HALF_W = 3.0f;
	constexpr float PICKUP_HALF_H = 5.0f;

	// Hitbox attiva davanti a Mario
	constexpr float HIT_OFFSET_X = 7.0f;
	constexpr float HIT_CENTER_Y = 6.0f;
	constexpr float HIT_HALF_W = 4.0f;
	constexpr float HIT_HALF_H = 5.0f;

	// Finestra di invulnerabilità di Mario dopo aver subito un danno
	constexpr float INVULNERABLE_TIME = 1.7f;// Durata invulnerabilità.
	constexpr float BLINK_HALF_PERIOD = 0.1f;// Semi-periodo del lampeggio.
}

/**
 * @namespace BarrelConfig
 * @brief Costanti fisiche e di pooling dei barili rotolanti.
 */
namespace BarrelConfig {
	constexpr int POOL_SIZE = 8;// Numero massimo di barili attivi contemporaneamente.
	constexpr float RADIUS = 3.5f;// Raggio geometrico del barile.
	constexpr float SPEED = 25.0f;// Velocità orizzontale costante.
	constexpr float GRAVITY = -150.0f;// Accelerazione gravitazionale verticale.
	constexpr float STICK = 1.0f;// Tolleranza di adesione alla trave in discesa.
	constexpr float SPAWN_X = -24.0f;// Coordinata X di rilascio davanti alle mani di DK.
	constexpr float HITBOX_HALF = 3.0f;// Semi-lato AABB.

	//Colori e proporzioni
	const glm::vec3 BODY_COLOR(0.90f, 0.50f, 0.10f);
	const glm::vec3 STRIPE_COLOR(0.45f, 0.20f, 0.05f);
	constexpr float HALF_PI = 1.5707963f;
}

/**
 * @namespace BulletConfig
 * @brief Parametri dei 5 pickup (proiettili) e della loro oscillazione sinusoidale.
 */
namespace BulletConfig {
	constexpr int COUNT = 5;
	constexpr float RADIUS = 2.5f;
	constexpr float HEIGHT = 6.0f;// Altezza del centro di riposo sopra la superficie della trave.
	constexpr float BOB_AMPLITUDE = 1.5f;// Ampiezza A dell'oscillazione armonica verticale.
	constexpr float BOB_FREQUENCY = 3.0f;// Pulsazione omega del moto armonico.
	constexpr float PHASE_STEP = 0.8f;// Sfasamento tra un proiettile e il successivo.

	// Posizione di ciascun proiettile = (indice trave, ascissa x, quota sopra la superficie).
	struct Spot { int platform; float x; float height; };
	constexpr std::array<Spot, COUNT> SPOTS = { {
		{ 0,  48.0f, 19.0f },// Salto sulla trave 0.
		{ 1, -45.0f, 19.0f },// Salto vicino al bordo sinistro della trave 1.
		{ 2,  44.0f, 18.0f },// Salto sul bordo destro della trave 2.
		{ 1,  56.0f, 26.0f },// Raggiungibile cadendo dal bordo destro della trave 2 o camminando al limite.
		{ 3, -40.0f, 19.0f } // Salto sulla trave 3 a sinistra.
	} };
}

/**
 * @namespace ShotConfig
 * @brief Parametri dei colpi sparati da Mario verso Donkey Kong.
 */
namespace ShotConfig {
	constexpr int POOL_SIZE = 5;// Al massimo 5 colpi contemporanei.
	constexpr float SPEED = 80.0f;// Velocità di volo.
	constexpr float RADIUS = 1.3f;
}

/**
 * @namespace LevelConfig
 * @brief Geometria analitica delle piattaforme inclinate e delle scale del livello.
 */
namespace LevelConfig {
	/**
	 * @struct PlatformSpec
	 * @brief Segmento della superficie superiore di una trave definito dai due estremi (x0, y0) -> (x1, y1).
	 *
	 * - Travi pari (0, 2, 4): pendenza negativa (scendono verso destra).
	 * - Travi dispari (1, 3): pendenza positiva (salgono verso destra).
	 * - Trave 5: piattaforma orizzontale di Pauline.
	 */
	struct PlatformSpec { float x0, y0, x1, y1; };

	constexpr std::array<PlatformSpec, 6> PLATFORMS = { {
		{ -61.0f, -57.0f,  61.0f, -63.0f },
		{ -50.0f, -39.0f,  61.0f, -33.0f },
		{ -61.0f,  -9.0f,  50.0f, -15.0f },
		{ -50.0f,   9.0f,  61.0f,  15.0f },
		{ -61.0f,  39.0f,  50.0f,  33.0f },
		{  -6.0f,  54.0f,  24.0f,  54.0f }
	} };

	/**
	 * @struct LadderSpec
	 * @brief Specifica di una scala verticale che collega la trave lower alla trave lower + 1 all'ascissa x.
	 */
	struct LadderSpec { int lower; float x; };

	constexpr std::array<LadderSpec, 5> LADDERS = { {
		{ 0,  35.0f }, { 1, -30.0f }, { 2, 30.0f }, { 3, -24.0f }, { 4, 2.0f }
	} };
}
