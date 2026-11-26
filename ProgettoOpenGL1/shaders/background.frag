#version 330 core

/**
 * @brief Fragment shader procedurale in stile arcade animato da uTime.
 *
 * Gradiente verticale pulsante --> Campo stellare pixelato su griglia scorrevole --> Barra di rinfresco CRT (campana gaussiana) -->
 * --> Scanline orizzontali --> Vignettatura radiale agli angoli
 */
in vec2 vUV;
out vec4 FragColor;
uniform float uTime;

/**
 * @brief Funzione hash 2D -> [0, 1) pseudo-casuale deterministica.
 */
float hash(vec2 p)
{
    return fract(sin(dot(p, vec2(127.1, 311.7))) * 43758.5453);
}

void main()
{
    // Gradiente verticale con interpolazione lineare
    float pulse = 0.5 + 0.5 * sin(uTime * 0.8);
    vec3 bottom = vec3(0.00, 0.00, 0.05);
    vec3 top    = vec3(0.07 + 0.03 * pulse, 0.00, 0.13 + 0.04 * pulse);
    vec3 color  = mix(bottom, top, vUV.y);

    // Stelle pixelate su griglia 48x56 scorrevole verso il basso (+uTime * 1.5 su Y).
    const vec2 GRID = vec2(48.0, 56.0);
    vec2 scaled = vec2(vUV.x * GRID.x, vUV.y * GRID.y + uTime * 1.5);
    vec2 cell   = floor(scaled);
    vec2 local  = fract(scaled);

    float h = hash(cell);
    if (h > 0.965) {
        vec2 d = abs(local - 0.5);
        float square = step(max(d.x, d.y), 0.22);

        float twinkle = 0.5 + 0.5 * sin(uTime * 3.0 + h * 60.0);// Scintillio sinusoidale con fase diversa per ogni stella.

        float pick = hash(cell + 7.0);
        vec3 starColor = vec3(1.0);
        if (pick < 0.33)      starColor = vec3(0.4, 0.9, 1.0);
        else if (pick < 0.66) starColor = vec3(1.0, 0.9, 0.4);

        color += starColor * square * (0.25 + 0.75 * twinkle);
    }
    float barY = 1.0 - fract(uTime * 0.12);
    float bar = exp(-pow((vUV.y - barY) * 22.0, 2.0));
    color += vec3(0.10, 0.18, 0.40) * bar;

    // Scanline
    float row = floor(vUV.y * 280.0);
    color *= 0.88 + 0.12 * mod(row, 2.0);

    // Vignettatura radiale 
    float dist = length(vUV - 0.5);
    color *= 1.0 - 0.6 * dist * dist;

    FragColor = vec4(color, 1.0);
}
