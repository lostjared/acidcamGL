#version 330

in vec2 tc;
out vec4 color;
uniform float time_f;
uniform sampler2D samp;
uniform sampler2D mat_samp;

vec4 xor_RGB(vec4 icolor, ivec4 isource) {
    ivec3 int_color;
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
    float speed = 4.0;
    float intensity = 0.3;
    float flameHeight = 0.4;
    float yPos = pow(tc.y, 2.5);
    float xOscillation = sin(tc.x * 25.0 + time_f * speed) * intensity;
    float yOscillation = sin((tc.y + tc.x) * 10.0 + time_f * speed) * intensity;
    float flameFactor = yPos + xOscillation + yOscillation;
    vec3 bottomColor = vec3(1.0, 0.85, 0.0);
    vec3 middleColor = vec3(1.0, 0.0, 0.0);
    vec3 topColor = vec3(0.2, 0.0, 0.0);
    vec3 fireColor;
    if (tc.y < flameHeight) {
        fireColor = mix(bottomColor, middleColor, tc.y / flameHeight);
    } else {
        fireColor = mix(middleColor, topColor, (tc.y - flameHeight) / (1.0 - flameHeight));
    }
    
    fireColor *= flameFactor;
if (flameFactor > 0.2) {
        color = vec4(fireColor, 1.0);
    } else {
        color = vec4(topColor, 1.0);
    }

    vec4 color_s = mix(texture(samp, tc), texture(mat_samp, tc), 0.5);
    color = xor_RGB(color, ivec4(color_s * 255));
}
