#version 300 es

precision highp float;
in vec2 texCoord;

uniform sampler2D u0;       // u0.x, u0.y, u0.z = s0;
uniform float dt;
uniform float v;


out vec4 fragColor;

void main() {


    // current texel position
    // vec2 currTexel = floor(texCoord * textureSize);
    float i = floor(gl_FragCoord.x * p_width / v_width);
    float j = floor(gl_FragCoord.y * p_height / v_height);

    // stuffs from that texel
    vec4 u0 = texelFetch(u0, gl_FragCoord.xy, 0);

    // posX0, posY0 in texel --> NOT curr texel
    // vec2 prevTexel = currTexel - dt*u0.xy
    float posX0 = i - dt*u0.x; // * dt*v_width/p_width;
    float posY0 = j - dt*u0.y; // * dt*v_heiht/p_height;

    //uhh......fix if you go with the vector style
    if (posX0 < 0.5) {
        posX0 = 0.5;
    }
    if (posX0 > p_width - 1.5){
        posX0 = p_width - 1.5;
    }
    if (posY0 < 0.5) {
        posY0 = 0.5;
    }
    if (posY0 > p_height - 1.5){
        posY0 = p_height - 1.5;
    }

    // in texel
    // vec2 texel0 = int(prevTexel);
    // vec2 texel1 = texel0 + 1;
    int c_posX0 = int(posX0);
    int c_posX1 = c_posX0 + 1;
    int c_posY0 = int(posY0);
    int c_posY1 = c_posY0 + 1;

    // in texel
    // vec2 weight = vec2(float(texel1) - prevTexel, prevTexel - float(texel0))
    float x0 = posX0 - c_posX0;
    float x1 = 1 - x0;
    float y0 = posY0 - c_posY0;
    float y1 = 1 - y0;

    //dH = viewportSize / textureSize
    //





}