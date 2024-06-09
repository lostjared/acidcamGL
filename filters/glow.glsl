#version 330

in vec2 tc;
out vec4 color;
uniform sampler2D samp;
uniform float time_f;
uniform vec2 iResolution;

vec4 xor_RGB(vec4 icolor, vec4 source) {
    ivec3 int_color;
    ivec4 isource = ivec4(source * 255);
    for(int i = 0; i < 3; ++i) {
        int_color[i] = int(255 * icolor[i]);
        int_color[i] = int_color[i]^isource[i];
        if(int_color[i] > 255)
            int_color[i] = int_color[i]%255;
        icolor[i] = float(int_color[i])/255;
    }
    return icolor;
}


void main(void) {
    vec2 uv = gl_FragCoord.xy / iResolution.xy;
    uv = uv * 2.0 - 1.0;
    uv.x *= iResolution.x / iResolution.y;
    
    float dist = sin(length(uv) * time_f);
    
    float glowFrequency = 15.0;
    float glowSpeed = 10.0;

    float glow = sin(dist * glowFrequency - time_f * glowSpeed) * 0.5 + 0.5;

    float expansion = abs(sin(time_f * 0.5));
    float beam = smoothstep(0.0, 1.0, expansion - dist);

    vec3 beamColor = vec3(0.0, 0.8, 1.0);
    vec3 glowColor = vec3(1.0, 1.0, 1.0);

    vec3 finalColor = mix(beamColor, glowColor, glow) * beam;
    
    color = vec4(finalColor, 1.0);
    color = xor_RGB(color, texture(samp, tc));
}
