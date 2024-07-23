#version 330 core
out vec4 color;
in vec2 tc;

uniform sampler2D samp;
uniform float time_f;
uniform vec2 iResolution;

vec3 getRainbowColor(float position) {
    float r = sin(position + 0.0) * 0.5 + 0.5;
    float g = sin(position + 2.0) * 0.5 + 0.5;
    float b = sin(position + 4.0) * 0.5 + 0.5;
    return vec3(r, g, b);
}

float pingPong(float x, float length) {
    float modVal = mod(x, length * 2.0);
    return modVal <= length ? modVal : length * 2.0 - modVal;
}


void main(void) {
    float time_t = pingPong(time_f, 10.0) + 2.0;
    float wave = sin(tc.y * 10.0 + time_f) * 0.05;
    vec2 new_tc = vec2(tc.x + wave, tc.y);
    vec4 texColor = texture(samp, new_tc);
    
    float spiralPosX = tc.x * cos(time_f) - tc.y * sin(time_f);
    float spiralPosY = tc.x * sin(time_f) + tc.y * cos(time_f);
    
    float rainbowPos = sqrt(spiralPosX * spiralPosX + spiralPosY * spiralPosY) * 10.0 + time_f * 5.0;
    
    vec3 rainbowColor = getRainbowColor(sin(rainbowPos * time_t));
    
    color = sin(vec4(mix(texColor.rgb, rainbowColor, 0.5), texColor.a) * time_t);
}
