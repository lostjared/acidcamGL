#version 330
in vec2 tc;
out vec4 color;
uniform vec2 iResolution;
in vec2 iResolution_;
in float timeval;
uniform sampler2D samp;
in float alpha;

void main() {
    float time = timeval;
    vec2 resolution = iResolution_;
    vec2 uv = gl_FragCoord.xy / resolution.xy;
    uv = uv * 2.0 - 1.0;
    uv.x *= resolution.x / resolution.y;
    float angleRotation = time;
    float cosA = cos(angleRotation);
    float sinA = sin(angleRotation);
    mat2 rotationMat = mat2(cosA, -sinA, sinA, cosA);
    uv = rotationMat * uv;
    float radius = length(uv);
    float angle = atan(uv.y, uv.x);
    angle = mod(angle, 2.0 * 3.14159 / 5.0);
    float sharpness = 0.5;
    bool inStar = radius < (cos(sharpness) / cos(angle - sharpness));
    
    if (inStar) {
        color = texture(samp, tc);
        color = color * (alpha * 0.5);
    } else {
        color = texture(samp, tc);
    }
}
