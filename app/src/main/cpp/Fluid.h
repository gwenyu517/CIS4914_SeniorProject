#ifndef HEALME_FLUID_H
#define HEALME_FLUID_H

#include <malloc.h>
#include <GLES3/gl3.h>

struct vectorField {
    GLfloat* x;
    GLfloat* y;
};

class Fluid {
private:
    int W;      // W = p_width + 2
    int H;      // H = p_height + 2
    int dH;

    //vectorField* sForce;
    GLfloat* sDensity;
    GLfloat* s1;

    void setupGrid();
    int index(int i, int j);

    void addSource(GLfloat* u, GLfloat* source);

public:
    Fluid(int width, int height, int dH);
    ~Fluid();

    void updateDensity(long dt);
    void addDensity(int i, int j, GLfloat amount);
    //void addForce(int i, int j, GLfloat amountX, GLfloat amountY);
    GLfloat densityAt(int i, int j);
};


#endif //HEALME_FLUID_H
