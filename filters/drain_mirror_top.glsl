#version 330 core
out vec4 color;
in vec2 tc;

uniform sampler2D samp; // The texture sampler
uniform vec2 iResolution; // The resolution of the viewport
uniform float time_f; // Uniform variable for time, to animate the effect

void main(void)
{
    // Calculate loop duration and current time in the loop
    float loopDuration = 25.0; // Duration of each cycle in seconds
    float currentTime = mod(time_f, loopDuration);

    // Normalize texture coordinates to have the origin in the center and correct aspect ratio
    vec2 normCoord = (tc * 2.0 - 1.0) * vec2(iResolution.x / iResolution.y, 1.0);

    // Mirror the top half onto the bottom
    normCoord.y = abs(normCoord.y);

    // Calculate distance from the center and angle for polar coordinates
    float dist = length(normCoord);
    float angle = atan(normCoord.y, normCoord.x);

    // Apply spiraling effect adjusted for the loop
    float spiralSpeed = 5.0; // Control the speed of the spiraling
    float inwardSpeed = currentTime / loopDuration; // Adjust inward movement over the loop duration

    angle += (1.0 - smoothstep(0.0, 8.0, dist)) * currentTime * spiralSpeed; // Spiral effect
    dist *= 1.0 - inwardSpeed; // Contract towards the center as the loop progresses

    // Convert polar coordinates back to Cartesian coordinates
    vec2 spiralCoord = vec2(cos(angle), sin(angle)) * tan(dist);

    // Adjust coordinates back to [0,1] range
    spiralCoord = (spiralCoord / vec2(iResolution.x / iResolution.y, 1.0) + 1.0) / 2.0;

    // Sample the texture with the modified coordinates
    color = texture(samp, spiralCoord);
}
