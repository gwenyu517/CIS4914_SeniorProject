#version 300 es

precision mediump float;
in vec2 texCoord;
uniform sampler2D s_texture;

out vec4 fragColor;

void main() {
    fragColor = texture(s_texture, texCoord);
}