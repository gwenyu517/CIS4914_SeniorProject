#include "quad.h"
#include "Fluid.h"

//#include <malloc.h>
#include <EGL/egl.h>
//#include <GLES3/gl3.h>
#include <android/log.h>


static GLuint programObject;
static GLuint vertexBufferObject;
static GLuint textureID;

static GLfloat vVertices[] = {  1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
                                1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
                                -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
                                -1.0f, 1.0f, 0.0f, 0.0f, 1.0f };

static GLubyte* pixels;
static GLint p_width, p_height;
static GLint dH = 4;

static Fluid* fluid;

void setGridSize(int width, int height) {
    p_width = width / dH;
    p_height = height / dH;

    pixels = (GLubyte*)malloc(4*p_width*p_height*sizeof(GLubyte));
    for (int i = 0; i < p_width; i++) {
        for (int j = 0; j < p_height; j++) {
            int k = 4 * (j*p_width + i);
            pixels[k] = 255;
            pixels[k + 1] = 255;
            pixels[k + 2] = 255;
            pixels[k + 3] = 0;
        }
    }

    fluid = new Fluid(p_width, p_height, dH);
}

GLuint LoadShader(GLenum type, const char *shaderSrc) {
    GLuint shader;
    GLint compiled;

    // Create the shader object
    shader = glCreateShader(type);

    if (shader == 0)
        return 0;

    // Load the shader source
    glShaderSource(shader, 1, &shaderSrc, NULL);

    // Compile the shader
    glCompileShader(shader);

    // Check the compile status
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);

    if (!compiled) {
        GLint infoLen = 0;

        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);

        if (infoLen > 1) {
            char *infoLog = (char*)malloc(sizeof(char) * infoLen);

            glGetShaderInfoLog(shader, infoLen, NULL, infoLog);
            // esLogMessage("Error compiling shader:\n%s\n", infoLog);

            free(infoLog);
        }
        glDeleteShader(shader);
        return 0;
    }
    return shader;
}

void on_surface_created() {
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Create vertex buffer object
    glGenBuffers(1, &vertexBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER, 4 * 5 * sizeof(GLfloat), vVertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), 0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(3*sizeof(float)));


    programObject = 0;

    char vShaderStr[] =
            "#version 300 es                          \n"
            "layout(location = 0) in vec3 vPosition;  \n"
            "layout(location = 1) in vec2 vTexCoord;  \n"
            "out vec2 texCoord;                       \n"
            "void main()                              \n"
            "{                                        \n"
            "   gl_Position = vec4(vPosition, 1.0);   \n"
            "   texCoord = vTexCoord;                 \n"
            "}                                        \n";

    char fShaderStr[] =
            "#version 300 es                              \n"
            "precision mediump float;                     \n"
            "in vec2 texCoord;                            \n"
            "uniform sampler2D s_texture;                 \n"
            "out vec4 fragColor;                          \n"
            "void main()                                  \n"
            "{                                            \n"
            "   fragColor = texture(s_texture, texCoord); \n"
            "}                                            \n";

    GLuint vertexShader;
    GLuint fragmentShader;
    // GLuint programObject;
    GLint linked;

    // Load the vertex/fragment shaders
    vertexShader = LoadShader ( GL_VERTEX_SHADER, vShaderStr );
    fragmentShader = LoadShader ( GL_FRAGMENT_SHADER, fShaderStr );

    // Create the program object
    programObject = glCreateProgram();

    if ( programObject == 0 )
    {
        goto exit;
        // return 0;
    }

    glAttachShader(programObject, vertexShader);
    glAttachShader(programObject, fragmentShader);

    // Link the program
    glLinkProgram(programObject);

    // Check the link status
    glGetProgramiv(programObject, GL_LINK_STATUS, &linked);

    if(!linked) {
        GLint infoLen = 0;

        glGetProgramiv(programObject, GL_INFO_LOG_LENGTH, &infoLen);

        if (infoLen > 1){
            char *infoLog = (char*)malloc(sizeof(char) * infoLen );

            glGetProgramInfoLog ( programObject, infoLen, NULL, infoLog );
            // esLogMessage ( "Error linking program:\n%s\n", infoLog );

            free ( infoLog );
        }
        glDeleteProgram(programObject);
        programObject = 0;
        return;
    }


    // Texture stuff
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, p_width, p_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);


    exit:
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return;
}

void on_surface_changed(int width, int height) {
    //glViewport (0, 0, width, height);
    //eglGetCurrentContext();
}

void updateTexture() {
    for (int i = 0; i < p_width; i++){
        for (int j = 0; j < p_height; j++){
            int k = 4* (j*p_width + i);

            pixels[k+3] = (GLubyte)fluid->densityAt(i,j);

        }
    }

    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, p_width, p_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

void update(long dt) {
    fluid->updateDensity(dt);
    updateTexture();
}

void drawFrame() {
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(programObject);

    // Vertex Buffer stuff
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), 0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(3*sizeof(float)));

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_ALPHA_BITS);

    // Texture stuff
    glBindTexture(GL_TEXTURE_2D, textureID);

    // DRAWWWWWWW
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);


    // Back it out bro
    glDisable(GL_BLEND);
    glDisable(GL_ALPHA_BITS);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void on_draw_frame(long dt) {
    update(dt);
    drawFrame();
}

void cleanup() {
    free(pixels);
    delete fluid;
}

void addDensity(float x, float y, float amount) {
    int i = (int)x / dH;
    int j = (int)y / dH;
    fluid->addDensity(i, j, amount);
}

