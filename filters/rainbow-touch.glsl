#version 330 core
out vec4 color;
in vec2 tc;

uniform sampler2D samp;
uniform float time_f;
uniform vec2 iResolution;

vec3 rainbow(vec2 uv, float offset) {
    float rainbowFactor = sin(offset + uv.x * 10.0) * 0.5 + 0.5;
    return vec3(
        sin(rainbowFactor * 3.0 + 0.0),
        sin(rainbowFactor * 3.0 + 2.0),
        sin(rainbowFactor * 3.0 + 4.0)
    );
}

void main(void) {
    vec4 texColor = texture(samp, tc);
    
    float phase = mod(time_f, 6.0);
    float t = fract(time_f / 2.0);
    vec2 uv = tc;
    vec3 rainbowColor;

    float angle = mix(0.0, 6.28318, t);


    uv += vec2(sin(angle + length(uv) * 10.0), cos(angle + length(uv) * 10.0)) * 0.1;
    
    
    rainbowColor = rainbow(uv, time_f);
    
    color = vec4(texColor.rgb + rainbowColor * 0.5, texColor.a);
}
