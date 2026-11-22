#version 330 core

/**
 * @brief Vertex shader dedicato ai pickup (proiettili) con oscillazione armonica lato GPU.
 *
 * Pipeline di trasformazione del vertice:
 *   1. World Space: `worldPosition = model * vec4(aPosition, 0.0, 1.0)` posiziona e scala il pickup.
 *   2. Animazione verticale in coordinate mondo:
 *        worldPosition.y += uAmplitude * sin(uFrequency * uTime + uPhase)
 *      Applicare lo spostamento DOPO la Model Matrix garantisce che `uAmplitude` sia in unità
 *      del mondo (indipendente dalla scala del pickup) e coincida con la hitbox CPU in `Bullets.cpp`.
 *   3. Clip Space: `gl_Position = projection * worldPosition`.
 */
layout(location = 0) in vec2 aPosition;
layout(location = 1) in vec3 aColor;

uniform mat4 projection;
uniform mat4 model;
uniform float uTime;
uniform float uAmplitude;
uniform float uFrequency;
uniform float uPhase;

out vec3 vColor;

void main()
{
    vec4 worldPosition = model * vec4(aPosition, 0.0, 1.0);
    worldPosition.y += uAmplitude * sin(uFrequency * uTime + uPhase);
    gl_Position = projection * worldPosition;
    vColor = aColor;
}
