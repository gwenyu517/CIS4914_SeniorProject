#version 300 es

precision highp float;
in vec2 texCoord;

uniform sampler2D u0;       // u0.x, u0.y, u0.z = s0
uniform sampler2D source;

out vec4 fragColor;

void main() {
    vec4 u0 = texture(u0, texCoord);
    vec4 source = texture(source, texCoord);
    fragColor = vec4(u0.x + source.x, u0.y + source.y, u0.z + source.z, 1.0);
}