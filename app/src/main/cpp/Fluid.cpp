#include "Fluid.h"

Fluid::Fluid(int width, int height, int dH) {
    this->W = width;
    this->H = height;
    this->dH = dH;

    setupGrid();
}

Fluid::~Fluid() {
    free(sForce->x);
    free(sForce->y);
    free(sForce);

    free(sDensity);

    free(u0->x);
    free(u0->y);
    free(u0);

    free(u1->x);
    free(u1->y);
    free(u1);

    free(s0);
    free(s1);
}

void Fluid::updateForce(long dt) {
    addSource(u1->x, sForce->x);
    addSource(u1->y, sForce->y);
}

void Fluid::updateDensity(long dt) {
    addSource(s1, sDensity);
}

void Fluid::addForce(int i, int j, GLfloat amountX, GLfloat amountY) {
    sForce->x[index(i+1,j+1)] += amountX;
    sForce->y[index(i+1,j+1)] += amountY;
}

void Fluid::addDensity(int i, int j, GLfloat amount) {
    sDensity[index(i+1,j+1)] += amount;
}

GLfloat Fluid::densityAt(int i, int j) {
    return s1[index(i+1,j+1)];
}





// ------ PRIVATE ------

void Fluid::setupGrid() {

    sForce = (vectorField*)malloc(sizeof(vectorField));
    sForce->x = (GLfloat*)malloc(W*H*sizeof(GLfloat));
    sForce->y = (GLfloat*)malloc(W*H*sizeof(GLfloat));

    sDensity = (GLfloat*)malloc(W*H*sizeof(GLfloat));

    u0 = (vectorField*)malloc(sizeof(vectorField));
    u0->x = (GLfloat*)malloc(W*H*sizeof(GLfloat));
    u0->y = (GLfloat*)malloc(W*H*sizeof(GLfloat));

    u1 = (vectorField*)malloc(sizeof(vectorField));
    u1->x = (GLfloat*)malloc(W*H*sizeof(GLfloat));
    u1->y = (GLfloat*)malloc(W*H*sizeof(GLfloat));

    s0 = (GLfloat*)malloc(W*H*sizeof(GLfloat));
    s1 = (GLfloat*)malloc(W*H*sizeof(GLfloat));

    for (int i = 0; i < W; i++) {
        for (int j = 0; j < H; j++) {
            sForce->x[index(i,j)] = 0;
            sForce->y[index(i,j)] = 0;

            sDensity[index(i,j)] = 0;

            u0->x[index(i,j)] = 0;
            u0->y[index(i,j)] = 0;

            u1->x[index(i,j)] = 0;
            u1->y[index(i,j)] = 0;

            s0[index(i,j)] = 0;
            s1[index(i,j)] = 0;
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