#version 330
in vec2 tc;
out vec4 color;
uniform float time_f;
uniform sampler2D samp;
uniform sampler2D mat_samp;
uniform vec2 iResolution;
void main(void)
{
    vec2 normCoord = ((gl_FragCoord.xy / iResolution.xy) * 2.0 - 1.0) * vec2(iResolution.x / iResolution.y, 1.0);

    float distanceFromCenter = length(normCoord);
    float wave = sin(distanceFromCenter * 12.0 - time_f * 4.0);

    vec2 tcAdjusted = tc + (normCoord * 0.301 * wave);

    vec4 textureColor = texture(samp, tcAdjusted);
    vec4 mainColor = texture(mat_samp, tcAdjusted);
    
    color = mix(textureColor, mainColor, 0.5);
}


