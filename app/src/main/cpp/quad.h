#ifndef HEALME_QUAD_H
#define HEALME_QUAD_H

#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>

void setAssetManger(AAssetManager* amgr);
void setGridSize(int width, int height);
void on_surface_created();
void on_surface_changed(int width, int height);
void on_draw_frame(long dt);
void cleanup();

void addForce(float x0, float y0, float amountX, float amountY, float size);
void addDensity(float x, float y, float amount, int mode, float size);
void addGravity(float gx, float gy);
void resetSim();

#endif //HEALME_QUAD_H
