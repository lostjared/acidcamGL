#version 330

in vec2 tc;
out vec4 color;
uniform vec2 iResolution;
uniform float time_f;
uniform sampler2D samp;


float rand(vec2 co){
    return fract(sin(dot(co.xy, vec2(12.9898, 78.233))) * 43758.5453);
}

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

vec3 smoothBaseColor(float t) {
    vec3 colors[3] = vec3[](vec3(0.1, 0.1, 0.2), vec3(0.05, 0.05, 0.15), vec3(0.15, 0.15, 0.25));
    float index = fract(t * 0.1);
    float transition = sin(index * 3.14159265 * 2.0) * 0.5 + 0.5;
    int i = int(fract(t * 0.05) * 3.0);
    int next = (i + 1) % 3;
    return mix(colors[i], colors[next], transition);
}

void main(void) {
    float aspectRatio = iResolution.x / iResolution.y;

    vec2 adjustedTC = tc;
    adjustedTC.y /= aspectRatio;
    vec2 center = vec2(0.5, 0.5);
    vec2 toCenter = adjustedTC - center;
    float dist = length(toCenter) * aspectRatio;
    vec3 baseColor = smoothBaseColor(time_f);
    float wave1 = sin(tc.x * 3.0 + time_f * 1.5) * 0.5 + 0.5;
    float wave2 = sin(tc.y * 2.0 + time_f * 2.0 + cos(tc.x * 5.0)) * 0.5 + 0.5;
    float auroraFactor = wave1 * wave2;
    vec3 auroraColor = mix(vec3(0.0, 1.0, 0.0), vec3(0.0, 0.5, 1.0), wave2);
    vec3 finalColor = mix(baseColor, auroraColor, auroraFactor * 0.6);
    vec4 ocolor = texture(samp, tc);
    color = vec4(finalColor, 1.0);
    color *= time_f;
    color = mix(color, texture(samp, tc), 0.5);
    color = xor_RGB(color, ivec4(ocolor * 255));
}
