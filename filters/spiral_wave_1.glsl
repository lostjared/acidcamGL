#version 330

in vec2 tc;
out vec4 color;
uniform sampler2D samp;
uniform float time_f;

float pingPong(float x, float length) {
    float modVal = mod(x, length * 2.0);
    return modVal <= length ? modVal : length * 2.0 - modVal;
}

void main(void) {
    float time_t = pingPong(time_f, 10.0);
    vec2 center = vec2(0.5, 0.5);
    float dist = sqrt((tc.x - center.x) * (tc.x - center.x) + (tc.y - center.y) * (tc.y - center.y));
    
    dist = sin(dist * time_t);
    
    float angle = atan(tc.y - center.y, tc.x - center.x);
      
     float wave = sin(dist * 10.0 + angle * 5.0 - time_f * 5.0);
      
      vec2 offset = tc + wave * 0.02 * vec2(cos(angle), sin(angle));
      vec4 texColor = texture(samp, offset);
      color = texColor;
}
