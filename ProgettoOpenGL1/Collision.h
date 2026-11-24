#pragma once
#include <glm/glm.hpp>
#include <cmath>

/**
 * @struct AABB
 * @brief Axis-Aligned Bounding Box 2D in rappresentazione centro + semi-dimensioni.
 * 
 * Rispetto alla rappresentazione (min, max), questa formulazione rende immediato
 * il test di intersezione basato sulla distanza tra i centri.
 */
struct AABB {
    glm::vec2 center;   // Coordinate (x, y) del centro del rettangolo.
    glm::vec2 halfSize; // Semi-larghezza (x) e semi-altezza (y), sempre >= 0.
};

/**
 * @brief Verifica se due AABB si intersecano (sovrapposizione stretta).
 *
 * Applica il Separating Axis Theorem.
 * Il minore stretto (<) implica che due box tangenti lungo un bordo non sono in collisione.
 *
 * @param a Prima bounding box.
 * @param b Seconda bounding box.
 * @return true se le due AABB si sovrappongono su entrambi gli assi, false altrimenti.
 */
inline bool intersects(const AABB& a, const AABB& b)
{
    return std::abs(a.center.x - b.center.x) < a.halfSize.x + b.halfSize.x &&
        std::abs(a.center.y - b.center.y) < a.halfSize.y + b.halfSize.y;
}
