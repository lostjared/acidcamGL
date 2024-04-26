#version 330 core
out vec4 color;
in vec2 tc;

uniform sampler2D samp; // Texture sampler
uniform float time_f; // Time factor to simulate dynamic 'pos'
uniform vec2 iResolution; // Image resolution

void main() {
    vec2 uv = tc; // Normalized texture coordinates
    ivec2 coords = ivec2(uv * iResolution); // Pixel coordinates

    vec4 origColor = texture(samp, uv); // Original color of the pixel
    float pos = mod(time_f, 7.0) + 1.0; // Simulate the 'pos' dynamic value

    int x = coords.x;
    int y = coords.y;
    vec3 newColor = origColor.rgb;

    if ((x % 2) == 0) {
        if ((y % 2) == 0) {
            newColor.r = (1.0 - pos * origColor.r);
            newColor.b = (float(x + y) * pos) / 255.0;
        } else {
            newColor.r = (pos * origColor.r - float(y)) / 255.0;
            newColor.b = (float(x - y) * pos) / 255.0;
        }
    } else {
        if ((y % 2) == 0) {
            newColor.r = (pos * origColor.r - float(x)) / 255.0;
            newColor.b = (float(x - y) * pos) / 255.0;
        } else {
            newColor.r = (pos * origColor.r - float(y)) / 255.0;
            newColor.b = (float(x + y) * pos) / 255.0;
        }
    }

    // Simulate color swapping and negative effect (invert)
    float temp = newColor.r;
    newColor.r = newColor.b;
    newColor.b = temp;
    vec3 finalColor = (sin(time_f) > 0.0) ? vec3(1.0) - newColor : newColor;

    color = sin(vec4(finalColor, 1.0) * time_f); // Set the color of the pixel with full opacity
}
