#version 330

in vec2 tc;
out vec4 color;
uniform sampler2D samp;
uniform float time_f;

void main(void) {
    float frequency = 10.0;
    float amplitude = 0.02;
    float speed = 2.0;
    float columnWidth = 1.0 / 4.0;

    vec2 snake;
    if (tc.x < columnWidth) {
        snake = tc + vec2(amplitude * sin(tc.y * frequency + time_f * speed), amplitude * sin(tc.x * frequency + time_f * speed));
    } else if (tc.x < 2.0 * columnWidth) {
        snake = tc + vec2(amplitude * sin(tc.y * frequency + time_f * speed), amplitude * sin((tc.x - columnWidth) * frequency + time_f * speed));
    } else if (tc.x < 3.0 * columnWidth) {
        snake = tc + vec2(amplitude * sin(tc.y * frequency + time_f * speed), amplitude * sin((tc.x - 2.0 * columnWidth) * frequency + time_f * speed));
    } else {
        snake = tc + vec2(amplitude * sin(tc.y * frequency + time_f * speed), amplitude * sin((tc.x - 3.0 * columnWidth) * frequency + time_f * speed));
    }
    
    vec4 texColor = texture(samp, snake);
    color = texColor;
}
