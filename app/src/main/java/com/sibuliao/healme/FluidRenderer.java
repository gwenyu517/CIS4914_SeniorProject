package com.sibuliao.healme;

import android.content.Context;
import android.opengl.GLSurfaceView;
import android.os.SystemClock;
import android.util.Log;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

public class FluidRenderer implements GLSurfaceView.Renderer {
    private final Context context;
    private long currTime;
    private long prevTime;
    //private long dt;

    public FluidRenderer(Context context){
        this.context = context;
    }

    @Override
    public void onSurfaceCreated(GL10 gl, EGLConfig config){
        FluidLibJNIWrapper.on_surface_created();
        prevTime = SystemClock.elapsedRealtime();
    }

    @Override
    public void onSurfaceChanged(GL10 gl, int width, int height){
        FluidLibJNIWrapper.on_surface_changed(width, height);
    }

    @Override
    public void onDrawFrame(GL10 gl){
        currTime = SystemClock.elapsedRealtime();
        FluidLibJNIWrapper.on_draw_frame(currTime - prevTime);
        prevTime = currTime;
    }

    public void setBoundSize(int width, int height){
        FluidLibJNIWrapper.setBoundSize(width, height);
    }

    public void onDestroy(){
        FluidLibJNIWrapper.on_destroy();
    }

    public void addForce(float x0, float y0, float amountX, float amountY, float size) {
        FluidLibJNIWrapper.addForce(x0, y0, amountX, amountY, size);
    }

    public void addDensity(float x, float y, float amount, int mode, float size) {
        FluidLibJNIWrapper.addDensity(x, y, amount, mode, size);
    }

    public void addGravity(float gx, float gy) {
        FluidLibJNIWrapper.addGravity(gx, gy);
    }

    public void clearCoffee() {
        FluidLibJNIWrapper.clearQuad();
    }
}

