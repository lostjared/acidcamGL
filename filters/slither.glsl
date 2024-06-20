#version 330

in vec2 tc;
out vec4 color;
uniform sampler2D samp;
uniform float time_f;

void main(void) {
    float frequency = 10.0;
    float amplitude = 0.02;
    float speed = 2.0;
    float rowHeight = 1.0 / 4.0;

    vec2 snake;
    if (tc.y < rowHeight) {
        snake = tc + vec2(amplitude * sin(tc.y * frequency + time_f * speed), amplitude * sin(tc.x * frequency + time_f * speed));
    } else if (tc.y < 2.0 * rowHeight) {
        snake = tc + vec2(amplitude * sin((tc.y - rowHeight) * frequency + time_f * speed), amplitude * sin(tc.x * frequency + time_f * speed));
    } else if (tc.y < 3.0 * rowHeight) {
        snake = tc + vec2(amplitude * sin((tc.y - 2.0 * rowHeight) * frequency + time_f * speed), amplitude * sin(tc.x * frequency + time_f * speed));
    } else {
        snake = tc + vec2(amplitude * sin((tc.y - 3.0 * rowHeight) * frequency + time_f * speed), amplitude * sin(tc.x * frequency + time_f * speed));
    }
    
    vec4 texColor = texture(samp, snake);
    color = texColor;
}
