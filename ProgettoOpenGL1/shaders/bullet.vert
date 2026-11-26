#version 330 core

/**
 * @brief Vertex shader dedicato ai proiettili con oscillazione armonica lato GPU.
 *
 * - World Space --> worldPosition = model * vec4(aPosition, 0.0, 1.0) posiziona e scala il proiettile.

 * - Animazione verticale in coordinate mondo --> worldPosition.y += uAmplitude * sin(uFrequency * uTime + uPhase)

 * - Clip Space --> gl_Position = projection * worldPosition.
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
