#version 330 core

/**
 * @brief Fragment shader comune agli oggetti della scena e ai pickup.
 *
 * Riceve il colore `vColor` interpolato sui frammenti dal rasterizzatore e lo scrive
 * nel color buffer con opacità piena (alpha = 1.0).
 */
in vec3 vColor;
out vec4 FragColor;

void main()
{
    FragColor = vec4(vColor, 1.0);
}
