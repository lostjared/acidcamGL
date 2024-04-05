#version 330
in vec2 tc;
out vec4 color;
uniform float time_f;
uniform sampler2D samp;
uniform vec2 iResolution;

void main(void) {
    vec2 normCoord = gl_FragCoord.xy / iResolution.xy;
float diagonalDistance = (normCoord.x + normCoord.y - 1.0) * sqrt(2.0);
    float antiDiagonalDistance = (normCoord.x - normCoord.y) * sqrt(2.0);
    float diagonalWave = sin((diagonalDistance + time_f) * 5.0); // Wave frequency and speed
    float antiDiagonalWave = cos((antiDiagonalDistance + time_f) * 5.0);
    float combinedWave = (diagonalWave + antiDiagonalWave) * 0.5;

    vec2 waveAdjusted = vec2(tc.x + combinedWave * 0.202, tc.y + combinedWave * 0.202);
    
    color = texture(samp, waveAdjusted);
}
