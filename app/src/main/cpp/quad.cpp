#include "quad.h"
#include "Fluid.h"

//#include <malloc.h>
#include <EGL/egl.h>
//#include <GLES3/gl3.h>
//#include <android/log.h>

static const GLfloat coffeeColor[] = {103.0f/255.0f, 67.0f/255.0f, 45.0f/255.0f, 1.0f};

static GLuint programObject;
static GLuint vertexBufferObject;
static GLuint* textureID;

static GLfloat vVertices[] = {  1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
                                1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
                                -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
                                -1.0f, 1.0f, 0.0f, 0.0f, 1.0f };

static GLubyte* milk_pixels;
static GLubyte* choco_pixels;

static GLint p_width, p_height;
static GLint dH = 10;

static Fluid* milk;
static Fluid* choco;
static GLfloat m_viscosity = 0.05;
static GLfloat m_diffRate = 0;
static GLfloat c_viscosity = 0.5;
static GLfloat c_diffRate = 0;

void setGridSize(int width, int height) {
    p_width = width / dH;
    p_height = height / dH;

    milk_pixels = (GLubyte*)malloc(4*p_width*p_height*sizeof(GLubyte));
    choco_pixels = (GLubyte*)malloc(4*p_width*p_height*sizeof(GLubyte));

    for (int i = 0; i < p_width; i++) {
        for (int j = 0; j < p_height; j++) {
            int k = 4 * (j*p_width + i);
            milk_pixels[k] = 255;
            milk_pixels[k + 1] = 252;
            milk_pixels[k + 2] = 247;
            milk_pixels[k + 3] = 0;

            choco_pixels[k] = 43;
            choco_pixels[k + 1] = 19;
            choco_pixels[k + 2] = 11;
            choco_pixels[k + 3] = 0;
        }
    }

    milk = new Fluid(m_viscosity, m_diffRate, p_width, p_height, dH);
    choco = new Fluid(c_viscosity, c_diffRate, p_width, p_height, dH);
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
    glClearColor(coffeeColor[0], coffeeColor[1], coffeeColor[2], coffeeColor[3]);
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
    textureID = (GLuint*)malloc(2*sizeof(GLuint));
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glGenTextures(2, textureID);

    glActiveTexture(GL_TEXTURE_2D);

    //glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureID[0]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, p_width, p_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, milk_pixels);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);


    //glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, textureID[1]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, p_width, p_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, choco_pixels);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);


    exit:
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return;
}

void on_surface_changed(int width, int height) {
    __android_log_print(ANDROID_LOG_DEBUG, "UPDATE", "woooooooaaaaaaaahhhhhhhh there!!!");

    glViewport (0, 0, width, height);
    eglGetCurrentContext();
}

void updateTextures() {
    for (int i = 0; i < p_width; i++){
        for (int j = 0; j < p_height; j++){
            int k = 4* (j*p_width + i);
            if (milk->densityAt(i,j) > 255)
                milk_pixels[k+3] = 255;
            else if (milk->densityAt(i,j) < 0)
                milk_pixels[k+3] = 0;
            else
                milk_pixels[k+3] = milk->densityAt(i,j);

            if (choco->densityAt(i,j) > 255)
                choco_pixels[k+3] = 255;
            else if (choco->densityAt(i,j) < 0)
                choco_pixels[k+3] = 0;
            else
                choco_pixels[k+3] = choco->densityAt(i,j);
            //__android_log_print(ANDROID_LOG_DEBUG, "milk_pixels", "[%d, %d] : %d", i, j, milk_pixels[k+3]);
        }
    }

    //__android_log_print(ANDROID_LOG_DEBUG, "milk_pixels", "_________________________________________________");

    glActiveTexture(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, textureID[0]);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, p_width, p_height, GL_RGBA, GL_UNSIGNED_BYTE, milk_pixels);

    glBindTexture(GL_TEXTURE_2D, textureID[1]);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, p_width, p_height, GL_RGBA, GL_UNSIGNED_BYTE, choco_pixels);

}

void update(long dt) {
    int i = (int)100/dH;
    int j = (int)100/dH;

    //__android_log_print(ANDROID_LOG_DEBUG, "UPDATE", "velocity was, %g", (double)milk->velocityAt(i, j));
    milk->updateVelocity(dt);
    //choco->updateVelocity(dt);
    //__android_log_print(ANDROID_LOG_DEBUG, "UPDATE", "velocity is, %g", (double)milk->velocityAt(i, j));

    //__android_log_print(ANDROID_LOG_DEBUG, "UPDATE", "density was, %g", (double)milk->densityAt(i, j));
    milk->updateDensity(dt);
    //choco->updateDensity(dt);
    //__android_log_print(ANDROID_LOG_DEBUG, "UPDATE", "density is, %g", (double)milk->densityAt(i, j));

    updateTextures();
    //__android_log_print(ANDROID_LOG_DEBUG, "UPDATE", "__________________________________________________");
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

    // Milk texture + draw
    glBindTexture(GL_TEXTURE_2D, textureID[0]);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    // Choco texture + draw
    glBindTexture(GL_TEXTURE_2D, textureID[1]);
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
    free(milk_pixels);
    free(choco_pixels);
    free(textureID);
    delete milk;
}

void addForce(float x0, float y0, float x, float y, float size) {
    //int i = (int)x0 / dH;
    //int j = (int)y0 / dH;
    int i = (int)(x*p_width);
    int j = (int)(y*p_height);
    if (i > p_width - 1)
        i = p_width - 1;

    //__android_log_print(ANDROID_LOG_DEBUG, "addF", "add %g - %g = %g", y, y0, (y - y0));
    milk->addForce(i, j, 4*p_width*dH*(x-x0), 4*p_height*dH*(y-y0), size);
    choco->addForce(i, j, 4*p_width*dH*(x-x0), 4*p_height*dH*(y-y0), size);
}

void addDensity(float x, float y, float amount, int mode, float size) {
    //int i = (int)x / dH;
    //int j = (int)y / dH;
    int i = (int)(x*p_width);
    int j = (int)(y*p_height);
    if (i > p_width - 1)
        i = p_width - 1;

    //__android_log_print(ANDROID_LOG_DEBUG, "addDensity", "mode %d", mode);

    if (mode == 1)
        milk->addDensity(i, j, 4*amount, size);
    else
        choco->addDensity(i, j, 4*amount, size);
}

void clearTextures() {
    for (int i = 0; i < p_width; i++) {
        for (int j = 0; j < p_height; j++) {
            int k = 4 * (j*p_width + i);
            milk_pixels[k + 3] = 0;
            choco_pixels[k + 3] = 0;
        }
    }
}

void resetSim() {
    clearTextures();
    milk->reset();
    choco->reset();
}
