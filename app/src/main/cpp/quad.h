#ifndef HEALME_QUAD_H
#define HEALME_QUAD_H

void setGridSize(int width, int height);
void on_surface_created();
void on_surface_changed(int width, int height);
void on_draw_frame(long dt);
void cleanup();

void addForce(float x0, float y0, float x, float y);
void addDensity(float x, float y, float amount, int mode);
void resetSim();

#endif //HEALME_QUAD_H
