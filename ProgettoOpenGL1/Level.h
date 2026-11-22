#pragma once
#include "Collision.h"
#include "Config.h"
#include "Scene.h"
#include <glm/glm.hpp>
#include <vector>

/**
 * @struct Platform
 * @brief Segmento della superficie calpestabile di una trave inclinata (con left.x < right.x).
 */
struct Platform {
    glm::vec2 left;  ///< Estremo sinistro della superficie superiore.
    glm::vec2 right; ///< Estremo destro della superficie superiore.

    /**
     * @brief Verifica se l'ascissa `x` ricade nella proiezione orizzontale della trave.
     * @param x Coordinata X da testare.
     * @return true se left.x <= x <= right.x.
     */
    bool coversX(float x) const { return x >= left.x && x <= right.x; }

    /**
     * @brief Calcola la quota Y della superficie all'ascissa `x` tramite interpolazione lineare (lerp).
     *
     * Parametro normalizzato lungo il segmento:
     *   t = (x - left.x) / (right.x - left.x)   con t in [0, 1] per x in [left.x, right.x]
     * Quota risultante:
     *   y(x) = left.y + t * (right.y - left.y)
     *
     * @param x Ascissa in coordinate mondo.
     * @return float Quota Y della superficie in `x`.
     */
    float yAt(float x) const {
        const float t = (x - left.x) / (right.x - left.x);
        return left.y + t * (right.y - left.y);
    }
};

/**
 * @struct Ladder
 * @brief Scala verticale che collega due piattaforme consecutive alla stessa ascissa `x`.
 */
struct Ladder {
    float x = 0.0f;       ///< Ascissa centrale della scala.
    float yBottom = 0.0f; ///< Quota Y sulla trave inferiore in `x`.
    float yTop = 0.0f;    ///< Quota Y sulla trave superiore in `x`.

    /**
     * @brief Costruisce la AABB della scala.
     *
     * Centro verticale: (yBottom + yTop) / 2.
     * Semi-altezza: (yTop - yBottom) / 2.
     * Semi-larghezza: LADDER_WIDTH / 2.
     *
     * @return AABB Bounding box della scala in coordinate mondo.
     */
    AABB box() const {
        return { {x, 0.5f * (yBottom + yTop)},
                 {WorldConfig::LADDER_WIDTH * 0.5f, 0.5f * (yTop - yBottom)} };
    }
};

/**
 * @class Level
 * @brief Gestisce la geometria analitica e la resa grafica di travi e scale del livello.
 *
 * Registra le proprie Geometry e istanze nella `Scene` nel costruttore.
 * @note Va costruito PRIMA delle entità dinamiche: in 2D senza depth test l'ordine di
 *       creazione delle istanze coincide con l'ordine di disegno (painter's algorithm).
 */
class Level {
public:
    /**
     * @brief Costruisce i dati analitici e le mesh procedurali di scale e travi nella Scene.
     * @param scene Scena grafica condivisa.
     */
    explicit Level(Scene& scene);

    const std::vector<Platform>& platforms() const { return m_platforms; }
    const std::vector<Ladder>& ladders() const { return m_ladders; }

    /**
     * @brief Determina se un'entità in caduta o in camminata atterra/aderisce a una trave in `x`.
     *
     * Implementa piattaforme "one-way" dall'alto: l'atterraggio è valido solo se al frame
     * precedente i piedi erano sopra la superficie (`previousFeetY`) e nel frame corrente
     * l'hanno raggiunta o superata (`feetY`). Il parametro `stick > 0` evita il distacco
     * durante la discesa lungo una trave inclinata. Tra più travi valide sceglie la più alta.
     *
     * @param x Ascissa corrente dell'entità.
     * @param previousFeetY Quota Y dei piedi al frame precedente.
     * @param feetY Quota Y candidata dei piedi al frame corrente.
     * @param stick Tolleranza verticale di adesione alla discesa (0 se in volo).
     * @param[out] groundY Quota Y della superficie trovata (valida solo se ritorna true).
     * @param[out] platformIndex Puntatore opzionale in cui scrivere l'indice della trave trovata.
     * @return true se è stata trovata una superficie di appoggio valida, false altrimenti.
     */
    bool findGround(float x, float previousFeetY, float feetY, float stick,
        float& groundY, int* platformIndex = nullptr) const;

    /**
     * @brief Verifica se la testa di un'entità in salita ha urtato il lato inferiore di una trave.
     *
     * Controlla l'attraversamento dal basso verso l'alto della quota inferiore
     * `yBottom = platform.yAt(x) - PLATFORM_THICKNESS`.
     *
     * @param x Ascissa corrente dell'entità.
     * @param previousHeadY Quota Y della testa al frame precedente.
     * @param headY Quota Y candidata della testa al frame corrente.
     * @param[out] ceilingY Quota del lato inferiore della trave colpita.
     * @return true se c'è stata collisione con il soffitto, false altrimenti.
     */
    bool hitsCeiling(float x, float previousHeadY, float headY, float& ceilingY) const;

private:
    std::vector<Platform> m_platforms;
    std::vector<Ladder> m_ladders;

    void buildLadders(Scene& scene);
    void buildPlatforms(Scene& scene);
};
