#version 330 core

/**
 * @brief Fragment shader comune agli oggetti della scena e ai proiettili.
 */
in vec3 vColor;
out vec4 FragColor;

void main()
{
    FragColor = vec4(vColor, 1.0);
}
