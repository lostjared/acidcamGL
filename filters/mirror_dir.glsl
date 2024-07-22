#version 330
in vec2 tc;
out vec4 color;
uniform sampler2D samp;
in vec2 iResolution_;
uniform float time_f;

void main(void)
{
    vec2 mirrored_tc = tc;
    
    int direction = int(mod(time_f, 4.0));
    
    if (direction == 0) {
        mirrored_tc = tc;
    } else if (direction == 1) {
        mirrored_tc.x = 1.0 - tc.x;
    } else if (direction == 2) {
        mirrored_tc.y = 1.0 - tc.y;
    } else if (direction == 3) {
        mirrored_tc = 1.0 - tc;
    }

    color = texture(samp, mirrored_tc);
}

