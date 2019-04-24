#include "Fluid.h"

static const int iter = 20;

Fluid::Fluid(GLfloat viscosity, GLfloat diffRate, int width, int height, int dH) {
    this->viscosity = viscosity;
    this->diffRate = diffRate;
    this->W = width+2;
    this->H = height+2;
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

void Fluid::updateVelocity(long dt) {
    int i = (int)100 / dH;
    int j = (int)100 / dH;
    //__android_log_print(ANDROID_LOG_DEBUG, "UPDATE", "-SOLVER velocity was, %g", u1->y[index(i+1,j+1)]);

    addSource(u1->x, sForce->x);
    addSource(u1->y, sForce->y);

    //__android_log_print(ANDROID_LOG_DEBUG, "UPDATE", "-SOLVER velocity added to, %g", u1->y[index(i+1,j+1)]);

    diffuse(u1->x, u0->x, viscosity, dt, 0);
    diffuse(u1->y, u0->y, viscosity, dt, 1);

    //__android_log_print(ANDROID_LOG_DEBUG, "UPDATE", "-SOLVER velocity diffuse to, %g", u0->y[index(i+1,j+1)]);


    project(u0, u1);
    //__android_log_print(ANDROID_LOG_DEBUG, "UPDATE", "-SOLVER velocity projected to, %g", u1->y[index(i+1,j+1)]);


    advect(u1->x, u0->x, u1->x, u1->y, dt, 0);
    advect(u1->y, u0->y, u1->x, u1->y, dt, 1);
    //__android_log_print(ANDROID_LOG_DEBUG, "UPDATE", "-SOLVER velocity advect to, %g", u0->y[index(i+1,j+1)]);


    project(u0, u1);
    //__android_log_print(ANDROID_LOG_DEBUG, "UPDATE", "-SOLVER velocity should now be, %g", u1->y[index(i+1,j+1)]);

}

void Fluid::updateDensity(long dt) {
    int i = (int)100 / dH;
    int j = (int)100 / dH;
    //__android_log_print(ANDROID_LOG_DEBUG, "UPDATE", "-SOLVER density was, %g", s1[index(i+1,j+1)]);

    addSource(s1, sDensity);
    //__android_log_print(ANDROID_LOG_DEBUG, "UPDATE", "-SOLVER density added to, %g", s1[index(i+1,j+1)]);

    diffuse(s1, s0, diffRate, dt, 0);
    //__android_log_print(ANDROID_LOG_DEBUG, "UPDATE", "-SOLVER density diffuse to, %g", s0[index(i+1,j+1)]);

    advect(s0, s1, u1->x, u1->y, dt, 0);
    //__android_log_print(ANDROID_LOG_DEBUG, "UPDATE", "-SOLVER density advect to, %g", s1[index(i+1,j+1)]);

}

void Fluid::addForce(int i, int j, GLfloat amountX, GLfloat amountY, GLfloat size) {
    //__android_log_print(ANDROID_LOG_DEBUG, "addF", "was %g", sForce->y[index(i+1,j+1)]);
    //__android_log_print(ANDROID_LOG_DEBUG, "addF", "add %g", amountY);

    //sForce->x[index(i+1,j+1)] += amountX * 0.1;
    //sForce->y[index(i+1,j+1)] += amountY * 0.1;
    //amountX = amountX*0.1;
    //amountY = amountY*0.1;

    int radius = (int)(size * 100) / 2;
    if (radius < 1){
        sForce->x[index(i+1, j+1)] += amountX;
        sForce->y[index(i+1, j+1)] += amountY;
        //radius = 1;
        return;
    }


    //__android_log_print(ANDROID_LOG_DEBUG, "addD", "welp! %d", radius);

    int pX, pY;

    for (int x = -radius; x <= radius; x++) {
        for (int y = -radius; y <= radius; y++) {
            pX = i + 1 + x;
            pY = j + 1 + y;
            if (pX < 1)
                pX = 1;
            else if (pX > W - 2)
                pX = W - 2;
            if (pY < 1)
                pY = 1;
            else if (pY > H - 2)
                pY = H - 2;

            //__android_log_print(ANDROID_LOG_DEBUG, "addD", "try pX*pX = %d, py*py = %d, radius*radius = %d", pX*pX, pY*pY, radius*radius);

            if (x*x + y*y <= 0.5*radius*radius){
                sForce->x[index(pX, pY)] += amountX;
                sForce->y[index(pX, pY)] += amountY;
            }
            else if (x*x + y*y <= radius*radius) {
                sForce->x[index(pX, pY)] += amountX / (x*x + y*y);
                sForce->y[index(pX, pY)] += amountY / (x*x + y*y);

            }

            //__android_log_print(ANDROID_LOG_DEBUG, "addF", "is %g", sDensity[index(pX,pY)]);
            //__android_log_print(ANDROID_LOG_DEBUG, "addF", "__________________________________");

        }
    }


    //__android_log_print(ANDROID_LOG_DEBUG, "addF", "is %g", sForce->y[index(i+1,j+1)]);
    //__android_log_print(ANDROID_LOG_DEBUG, "addF", "__________________________");

}

void Fluid::addDensity(int i, int j, GLfloat amount, GLfloat size) {
    //sDensity[index(i+1,j+1)] += amount;
    //__android_log_print(ANDROID_LOG_DEBUG, "addD", "size = %g", size);

    //amount = 255.0f + 1.0f / size;
    int radius = (int)(size * 100) / 2;
    if (radius < 1)
        radius = 1;

    //__android_log_print(ANDROID_LOG_DEBUG, "addD", "welp! %d", radius);

    int pX, pY;

    for (int x = -radius; x <= radius; x++) {
        for (int y = -radius; y <= radius; y++) {
            pX = i + 1 + x;
            pY = j + 1 + y;
            if (pX < 1)
                pX = 1;
            else if (pX > W - 2)
                pX = W - 2;
            if (pY < 1)
                pY = 1;
            else if (pY > H - 2)
                pY = H - 2;

            //__android_log_print(ANDROID_LOG_DEBUG, "addD", "try %g / (%d*%d + %d*%d) = %g", amount, x, x, y, y, amount / (x*x + y*y));
            //__android_log_print(ANDROID_LOG_DEBUG, "addD", "try pX*pX = %d, py*py = %d, radius*radius = %d", pX*pX, pY*pY, radius*radius);

            if (x*x + y*y <= 0.7*radius*radius)
                sDensity[index(pX, pY)] += amount;
            else if (x*x + y*y <= radius*radius)
                sDensity[index(pX, pY)] += amount / (x*x + y*y);

            //__android_log_print(ANDROID_LOG_DEBUG, "addD", "is %g", sDensity[index(pX,pY)]);
            //__android_log_print(ANDROID_LOG_DEBUG, "addD", "__________________________________");

        }
    }

}

void Fluid::addGravity(GLfloat gx, GLfloat gy) {
    for (int i = 0; i < W; i++) {
        for (int j = 0; j < H; j++) {
            sForce->x[index(i,j)] += gx;
            sForce->y[index(i,j)] += gy;
        }
    }
}


void Fluid::reset() {
    zeroFields();
}

GLfloat Fluid::densityAt(int i, int j) {
    return s1[index(i+1,j+1)];
}

GLfloat Fluid::velocityAt(int i, int j) {
    return u1->y[index(i+1,j+1)];
}



// ------ PRIVATE ------

void Fluid::setupGrid() {
    allocateMemory();
    zeroFields();
}

void Fluid::allocateMemory() {
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
}

void Fluid::zeroFields() {
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

void Fluid::diffuse(GLfloat* u0, GLfloat* u1, GLfloat v, long dt, int t) {
    GLfloat a = v*dt / (dH*dH);

    for (int m = 0; m < iter; m++){
        for (int i = 1; i < W - 1; i++) {
            for (int j = 1; j < H - 1; j++) {
                u1[index(i,j)] = (u0[index(i,j)] - a*(u1[index(i-1,j)] + u1[index(i+1,j)]
                                                      + u0[index(i,j-1)] + u0[index(i,j+1)])) / (1 + 4*a);

            }
        }
        setBoundary(u1, t);
    }
}

void Fluid::advect(GLfloat *u0, GLfloat *u1, GLfloat *x, GLfloat *y, long dt, int t) {
    GLfloat posX, posY;
    GLfloat posX0, posY0;
    GLint c_posX0, c_posY0;
    GLint c_posX1, c_posY1;
    GLfloat x0, x1;
    GLfloat y0, y1;

    for (int i = 1; i < W - 1; i++) {
        for (int j = 1; j < H - 1; j++) {
            posX0 = i - dt*x[index(i,j)]/dH;
            posY0 = j - dt*y[index(i,j)]/dH;

            if (posX0 < 0.5)
                posX0 = 0.5;
            if (posX0 > W - 1.5)
                posX0 = W - 1.5f;
            if (posY0 < 0.5)
                posY0 = 0.5;
            if (posY0 > H - 1.5)
                posY0 = H - 1.5f;

/*
            if (posX0 < 0)
                posX0 = 0;
            if (posX0 > W - 1)
                posX0 = W - 2;
            if (posY0 < 0)
                posY0 = 0;
            if (posY0 > H - 1)
                posY0 = H - 2;
*/
            c_posX0 = (int)posX0;
            c_posX1 = c_posX0 + 1;
            c_posY0 = (int)posY0;
            c_posY1 = c_posY0 + 1;

            x0 = posX0 - c_posX0;
            x1 = 1 - x0;
            y0 = posY0 - c_posY0;
            y1 = 1 - y0;

            u1[index(i,j)] = x1*(y1*u0[index(c_posX0,c_posY0)]+y0*u0[index(c_posX0,c_posY1)])
                    + x0*(y1*u0[index(c_posX1,c_posY0)]+y0*u0[index(c_posX1,c_posY1)]);
        }
    }

    setBoundary(u1, t);
}

void Fluid::project(vectorField *u0, vectorField *u1) {
    GLfloat* divU0 = (GLfloat*)malloc(W*H*sizeof(GLfloat));
    GLfloat* q = (GLfloat*)malloc(W*H*sizeof(GLfloat));

    for (int i = 1; i < W - 1; i++) {
        for (int j = 1; j < H - 1; j++) {
            divU0[index(i,j)] = 0.5f*dH*(u0->x[index(i+1,j)] - u0->x[index(i-1,j)]
                                         + u0->y[index(i,j+1)] - u0->y[index(i,j-1)]);
            q[index(i,j)] = 0;
        }
    }

    setBoundary(divU0, 2);
    setBoundary(q, 2);

    for (int m = 0; m < iter; m++) {
        for (int i = 1; i < W - 1; i++) {
            for (int j = 1; j < H - 1; j++) {
                q[index(i,j)] = (q[index(i-1,j)] + q[index(i+1,j)]
                                 + q[index(i,j-1)] + q[index(i,j+1)] - divU0[index(i,j)]) / 4;
            }
        }
        setBoundary(q, 2);
    }

    for (int i = 1; i < W - 1; i++) {
        for (int j = 1; j < H - 1; j++) {
            u1->x[index(i,j)] = u0->x[index(i,j)] - 0.5f*(q[index(i+1,j)] - q[index(i-1,j)])/dH;
            u1->y[index(i,j)] = u0->y[index(i,j)] - 0.5f*(q[index(i,j+1)] - q[index(i,j-1)])/dH;
        }
    }

    setBoundary(u1->x, 0);
    setBoundary(u1->y, 1);

    free(divU0);
    free(q);
}

void Fluid::setBoundary(GLfloat *f, int fieldType) {
    // vertical sides
    for (int j = 1; j < H - 2; j++) {
        switch(fieldType){
            case 0:     // x field
                f[index(0, j)] = -f[index(1,j)];
                f[index(W-1,j)] = -f[index(W-2,j)];
                break;
            case 1:     // y field
                f[index(0,j)] = f[index(1,j)];
                f[index(W-1,j)] = f[index(W-2,j)];
                break;
            case 2:     // scalar
                f[index(0,j)] = f[index(1,j)];
                f[index(W-1,j)] = f[index(W-2,j)];
        }
    }

    // horizontal sides
    for (int i = 1; i < W - 1; i++) {
        switch(fieldType){
            case 0:     // x field
                f[index(i,0)] = f[index(i,1)];
                f[index(i,H-1)] = f[index(i,H-2)];
                break;
            case 1:     // y field
                f[index(i,0)] = -f[index(i,1)];
                f[index(i,H-1)] = -f[index(i,H-2)];
                break;
            case 2:     // scalar
                f[index(i,0)] = f[index(i,1)];
                f[index(i,H-1)] = f[index(i,H-2)];
        }
    }

    f[index(0, 0)] = 0.5f * (f[index(0,1)] + f[index(1,0)]);
    f[index(W-1, 0)] = 0.5f * (f[index(W-1,1)] + f[index(W-2,0)]);
    f[index(W-1, H-1)] = 0.5f * (f[index(W-1,H-2)] + f[index(W-2,H-1)]);
    f[index(0, H-1)] = 0.5f * (f[index(0, H-2)] + f[index(1,H-1)]);

}