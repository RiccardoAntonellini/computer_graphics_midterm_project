#version 330 core

/**
 * @brief Vertex shader dedicato allo sfondo full-screen.
 *
 * I vertici `aPos` arrivano già in Normalized Device Coordinates (NDC) nel dominio [-1, +1]^2,
 * quindi non servono matrici Model o Projection: `gl_Position` è assegnato direttamente.
 *
 * Trasformazione affine NDC -> UV:
 *   vUV = aPos * 0.5 + 0.5
 * Mappa l'intervallo [-1, +1] in [0, 1] su entrambi gli assi, fornendo al fragment shader
 * coordinate normalizzate comodissime per gradienti, griglie e vignettatura.
 */
layout(location = 0) in vec2 aPos;
out vec2 vUV;

void main()
{
    vUV = aPos * 0.5 + 0.5;
    gl_Position = vec4(aPos, 0.0, 1.0);
}
