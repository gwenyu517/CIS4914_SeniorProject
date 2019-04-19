#version 300 es

precision mediump float;
in vec2 texCoord;
uniform sampler2D s_texture;

out vec4 fragColor;

void main() {
    fragColor = texture(s_texture, texCoord);

    //fragColor = texelFetch(s_texture, ivec2(texCoord.x/8,texCoord.y/8), 0);
    //fragColor = texelFetch(s_texture, ivec2(gl_FragCoord.xy), 0);
    /*if (gl_FragCoord.x > 800.5) {
        fragColor = vec4(1.0, 0.0, 0.0, 1.0);
    }
    else {
        fragColor = vec4(0.0, 1.0, 0.0, 1.0);
    }*/
}