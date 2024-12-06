#version 330 core

in vec2 tc;
out vec4 color;

uniform sampler2D samp;
uniform float time_f;
uniform vec2 iResolution;

const int SEGMENTS = 6;
float pingPong(float x, float length) {
    float modVal = mod(x, length * 2.0);
    return modVal <= length ? modVal : length * 2.0 - modVal;
}

vec4 adjustHue(vec4 c, float angle) {
    float U = cos(angle);
    float W = sin(angle);
    mat3 rotationMatrix = mat3(
        0.299,  0.587,  0.114,
        0.299,  0.587,  0.114,
        0.299,  0.587,  0.114
    ) + mat3(
        0.701, -0.587, -0.114,
       -0.299,  0.413, -0.114,
       -0.300, -0.588,  0.886
    ) * U
      + mat3(
         0.168,  0.330, -0.497,
        -0.328,  0.035,  0.292,
         1.250, -1.050, -0.203
    ) * W;
    return vec4(rotationMatrix * c.rgb, c.a);
}

void main() {
    vec2 uv = (tc - 0.5) * iResolution / min(iResolution.x, iResolution.y);
    float r = length(uv);
    float angle = atan(uv.y, uv.x);
    float swirlStrength = 2.5;
    float swirl = time_f * 0.5;
    angle += swirlStrength * sin(swirl + r * 4.0);
    float segmentAngle = 2.0 * 3.14159265359 / float(SEGMENTS);
    angle = mod(angle, segmentAngle);
    angle = abs(angle - segmentAngle * 0.5);
    vec2 kaleidoUV = vec2(cos(angle), sin(angle)) * r;
    float ripple = sin(r * 12.0 - pingPong(time_f, 10.0) * 10.0) * exp(-r * 4.0);
    kaleidoUV += ripple * 0.01;
    vec2 scale = vec2(1.0) / (iResolution / min(iResolution.x, iResolution.y));
    vec2 sampleTC = kaleidoUV * scale + 0.5;
    
    float offsetAmount = 0.003 * sin(time_f * 0.5);
    vec4 col = texture(samp, sampleTC);
    col += texture(samp, sampleTC + vec2(offsetAmount, 0.0));
    col += texture(samp, sampleTC + vec2(-offsetAmount, offsetAmount));
    col += texture(samp, sampleTC + vec2(offsetAmount * 0.5, -offsetAmount));
    col /= 4.0;
    
    float hueSpeed = 2.0;
    float hueShift = (time_f * hueSpeed + ripple * 2.0);
    
    color = adjustHue(col, hueShift);
    float saturationBoost = 1.5;
    float brightnessBoost = 1.1;
    vec3 hsv = color.rgb;
    float avg = (hsv.r + hsv.g + hsv.b) / 3.0;
    hsv = mix(vec3(avg), hsv, saturationBoost);
    hsv *= brightnessBoost;
    color.rgb = hsv;
    color = mix(clamp(color, 0.0, 1.0), texture(samp, tc), 0.5);
    color = sin(color * pingPong(time_f, 10.0) + 2.0);
}

