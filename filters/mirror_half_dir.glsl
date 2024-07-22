#version 330

in vec2 tc;
out vec4 color;
uniform sampler2D samp;
in vec2 iResolution_;
uniform float time_f;

void main(void)
{
    vec2 uv = tc;
    float duration = 2.0;
    float totalDuration = 2.0 * duration;
    float currentTime = mod(time_f, totalDuration);

    if (currentTime < duration) {
        if (uv.x > 0.5) {
            uv.x = 1.0 - uv.x;
        }
    } else {
        if (uv.x < 0.5) {
            uv.x = 1.0 - uv.x;
        }
    }
    
    color = texture(samp, uv);
}
