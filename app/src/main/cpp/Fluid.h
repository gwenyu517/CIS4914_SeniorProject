#ifndef HEALME_FLUID_H
#define HEALME_FLUID_H

#include <malloc.h>
#include <GLES3/gl3.h>
#include <android/log.h>


struct vectorField {
    GLfloat* x;
    GLfloat* y;
};

class Fluid {
private:
    GLfloat viscosity;
    GLfloat diffRate;

    int W;      // W = p_width + 2
    int H;      // H = p_height + 2
    int dH;

    vectorField* sForce;
    GLfloat* sDensity;

    vectorField* u0;
    vectorField* u1;

    GLfloat* s0;
    GLfloat* s1;

    void setupGrid();
    int index(int i, int j);

    void addSource(GLfloat* u, GLfloat* source);
    void diffuse(GLfloat* u0, GLfloat* u1, GLfloat v, long dt, int t);
    void advect(GLfloat *u0, GLfloat *u1, GLfloat *x, GLfloat *y, long dt, int t);
    void project(vectorField* u0, vectorField* u1);

    void setBoundary(GLfloat *f, int fieldType);
    void allocateMemory();
    void zeroFields();

public:
    Fluid(GLfloat viscosity, GLfloat diffRate, int width, int height, int dH);
    ~Fluid();

    void updateVelocity(long dt);
    void updateDensity(long dt);

    void addForce(int i, int j, GLfloat amountX, GLfloat amountY, GLfloat size);
    void addDensity(int i, int j, GLfloat amount, GLfloat size);
    void addGravity(GLfloat gx, GLfloat gy);
    void reset();

    GLfloat densityAt(int i, int j);

    // Todo: DELETE
    GLfloat velocityAt(int i, int j);
};


#endif //HEALME_FLUID_H
