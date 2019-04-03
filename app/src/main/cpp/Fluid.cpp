#include "Fluid.h"

Fluid::Fluid(int width, int height, int dH) {
    this->W = width;
    this->H = height;
    this->dH = dH;

    setupGrid();
}

Fluid::~Fluid() {
    free(sDensity);
    free(s1);
}


void Fluid::updateDensity(long dt) {
    addSource(s1, sDensity);
}

void Fluid::addDensity(int i, int j, GLfloat amount) {
    sDensity[index(i+1,j+1)] += amount;
}

GLfloat Fluid::densityAt(int i, int j) {
    return s1[index(i+1,j+1)];
}





// ------ PRIVATE ------

void Fluid::setupGrid() {

    sDensity = (GLfloat*)malloc(W*H*sizeof(GLfloat));
    s1 = (GLfloat*)malloc(W*H*sizeof(GLfloat));

    for (int i = 0; i < W; i++) {
        for (int j = 0; j < H; j++) {
            s1[index(i,j)] = 0;
            sDensity[index(i,j)] = 0;
        }
    }
}

int Fluid::index(int i, int j) {
    return j*W + i;
}

void Fluid::addSource(GLfloat* u, GLfloat* source) {
    for (int i = 0; i < W; i++) {
        for (int j = 0; j < H; j++) {
            u[index(i,j)] += source[index(i,j)];
            source[index(i,j)] = 0;
        }
    }
}