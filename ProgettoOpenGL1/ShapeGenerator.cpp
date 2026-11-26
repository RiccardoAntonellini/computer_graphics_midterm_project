#include "ShapeGenerator.h"
#include <array>
#include <cmath>

namespace ShapeGenerator {

    /**
     * @brief Genera un rettangolo centrato in (0, 0) come 2 triangoli con orientamento antiorario.
     *
     * Semi-dimensioni: x = w / 2, y = h / 2.
     * Triangolo 1: (-x, -y), (+x, -y), (+x, +y)
     * Triangolo 2: (-x, -y), (+x, +y), (-x, +y)
     */
    MeshData rectangle(float w, float h, const glm::vec3& color)
    {
        const float x = w * 0.5f;
        const float y = h * 0.5f;
        const std::array<glm::vec2, 6> pts = {
            glm::vec2(-x, -y), { x, -y }, { x, y },
            glm::vec2(-x, -y), { x,  y }, { -x, y }
        };

        MeshData mesh;
        for (const auto& v : pts) {
            mesh.vertices.insert(mesh.vertices.end(), { v.x, v.y, color.r, color.g, color.b });
        }
            
        return mesh;
    }

    /**
     * @brief Genera un disco centrato in (0, 0) approssimato da `segments` settori triangolari.
     *
     * Per ogni settore i in [0, segments - 1], gli angoli agli estremi dell'arco sono:
     *   a = 2 * PI * i / segments
     *   b = 2 * PI * (i + 1) / segments
     * Il triangolo collega l'origine (0, 0) ai due punti sulla circonferenza in coordinate polari:
     *   (r * cos(a), r * sin(a)) e (r * cos(b), r * sin(b)).
     */
    MeshData disk(float r, const glm::vec3& color, int segments)
    {
        MeshData mesh;
        const float pi = 3.14159265358979323846f;
        for (int i = 0; i < segments; ++i) {
            const float a = 2.0f * pi * i / segments;
            const float b = 2.0f * pi * (i + 1) / segments;
            const std::array<glm::vec2, 3> tri = {
                glm::vec2(0.0f),
                { r * std::cos(a), r * std::sin(a) },
                { r * std::cos(b), r * std::sin(b) }
            };

            for (const auto& v : tri) {
                mesh.vertices.insert(mesh.vertices.end(), { v.x, v.y, color.r, color.g, color.b });
            }
        }
        return mesh;
    }

    /**
     * @brief Genera un triangolo isoscele centrato nell'origine con base in basso e vertice in alto.
     *
     * Vertici: (-base/2, -height/2), (+base/2, -height/2), (0, +height/2).
     */
    MeshData triangle(float base, float height, const glm::vec3& color)
    {
        const std::array<glm::vec2, 3> pts = {
            glm::vec2(-base * 0.5f, -height * 0.5f),
            { base * 0.5f, -height * 0.5f },
            { 0.0f, height * 0.5f }
        };

        MeshData mesh;
        for (const auto& v : pts) {
            mesh.vertices.insert(mesh.vertices.end(), { v.x, v.y, color.r, color.g, color.b });
        }
        return mesh;
    }

} // namespace ShapeGenerator
