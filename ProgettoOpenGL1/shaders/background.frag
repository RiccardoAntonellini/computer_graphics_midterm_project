#version 330 core

/**
 * @brief Fragment shader procedurale in stile arcade/CRT animato da `uTime`.
 *
 * Combina 5 effetti puramente matematici nello spazio UV [0, 1]^2 senza uso di texture:
 *   1. Gradiente verticale pulsante.
 *   2. Campo stellare pixelato su griglia scorrevole.
 *   3. Barra di rinfresco CRT (campana gaussiana).
 *   4. Scanline orizzontali.
 *   5. Vignettatura radiale.
 */
in vec2 vUV;
out vec4 FragColor;
uniform float uTime;

/**
 * @brief Funzione hash 2D -> [0, 1) pseudo-casuale deterministica.
 *
 * Proietta la cella `p` su uno scalare tramite prodotto scalare con costanti grandi,
 * applica `sin()` e moltiplica per un fattore elevato; `fract()` estrae la sola parte
 * frazionaria, producendo valori ben distribuiti in [0, 1) costanti per la stessa cella.
 */
float hash(vec2 p)
{
    return fract(sin(dot(p, vec2(127.1, 311.7))) * 43758.5453);
}

void main()
{
    // 1) Gradiente verticale con interpolazione lineare: mix(a, b, t) = a*(1-t) + b*t.
    //    `pulse` rimappa sin(0.8 * uTime) da [-1, 1] a [0, 1].
    float pulse = 0.5 + 0.5 * sin(uTime * 0.8);
    vec3 bottom = vec3(0.00, 0.00, 0.05);
    vec3 top    = vec3(0.07 + 0.03 * pulse, 0.00, 0.13 + 0.04 * pulse);
    vec3 color  = mix(bottom, top, vUV.y);

    // 2) Stelle pixelate su griglia 48x56 scorrevole verso il basso (+uTime * 1.5 su Y).
    //    `cell = floor(scaled)` identifica univocamente la cella;
    //    `local = fract(scaled)` fornisce le coordinate locali in [0, 1)^2 dentro la cella.
    const vec2 GRID = vec2(48.0, 56.0);
    vec2 scaled = vec2(vUV.x * GRID.x, vUV.y * GRID.y + uTime * 1.5);
    vec2 cell   = floor(scaled);
    vec2 local  = fract(scaled);

    float h = hash(cell);
    if (h > 0.965) { // ~3.5% delle celle contiene una stella
        // Distanza di Chebyshev (norma L-infinito) dal centro cella (0.5, 0.5):
        // max(|dx|, |dy|) <= 0.22 definisce un quadrato centrato di lato 0.44.
        vec2 d = abs(local - 0.5);
        float square = step(max(d.x, d.y), 0.22);

        // Scintillio sinusoidale in [0, 1] con fase diversa per ogni stella (h * 60.0).
        float twinkle = 0.5 + 0.5 * sin(uTime * 3.0 + h * 60.0);

        // Selezione deterministica del colore (bianco, ciano o giallo) con hash sfalsato.
        float pick = hash(cell + 7.0);
        vec3 starColor = vec3(1.0);
        if (pick < 0.33)      starColor = vec3(0.4, 0.9, 1.0);
        else if (pick < 0.66) starColor = vec3(1.0, 0.9, 0.4);

        color += starColor * square * (0.25 + 0.75 * twinkle);
    }

    // 3) Barra di rinfresco CRT: `barY` scende periodicamente da 1 a 0.
    //    Profilo di intensità gaussiano: exp(-((y - barY) * 22)^2), massimo 1 al centro della barra.
    float barY = 1.0 - fract(uTime * 0.12);
    float bar = exp(-pow((vUV.y - barY) * 22.0, 2.0));
    color += vec3(0.10, 0.18, 0.40) * bar;

    // 4) Scanline: quantizza Y in 280 righe; `mod(row, 2.0)` alterna 0 e 1,
    //    moltiplicando la luminosità per 0.88 (righe pari) o 1.00 (righe dispari).
    float row = floor(vUV.y * 280.0);
    color *= 0.88 + 0.12 * mod(row, 2.0);

    // 5) Vignettatura radiale quadratica: attenua la luminosità in funzione di d^2 dal centro (0.5, 0.5).
    float dist = length(vUV - 0.5);
    color *= 1.0 - 0.6 * dist * dist;

    FragColor = vec4(color, 1.0);
}
