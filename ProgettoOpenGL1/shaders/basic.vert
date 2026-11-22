#version 330 core

/**
 * @brief Vertex shader standard per gli oggetti 2D della scena.
 *
 * Trasforma ogni vertice locale `aPosition` (esteso in coordinate omogenee con z = 0, w = 1)
 * prima in World Space tramite `model` e poi in Clip Space tramite la matrice ortografica `projection`:
 *   gl_Position = projection * model * vec4(aPosition, 0.0, 1.0)
 * Passa il colore per vertice `aColor` al fragment shader (dove verrà interpolato dal rasterizzatore).
 */
layout(location = 0) in vec2 aPosition;
layout(location = 1) in vec3 aColor;

uniform mat4 projection;
uniform mat4 model;

out vec3 vColor;

void main()
{
    gl_Position = projection * model * vec4(aPosition, 0.0, 1.0);
    vColor = aColor;
}
