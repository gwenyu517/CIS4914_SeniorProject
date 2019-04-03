package com.sibuliao.healme;

import android.opengl.GLSurfaceView;
import android.content.Context;
import android.os.SystemClock;
import android.view.MotionEvent;

public class FluidGLSurfaceView extends GLSurfaceView {
    private final FluidRenderer renderer;
    private float x0;
    private float y0;
    //private long currTime;
    //private long prevTime;
    //private long dt;

    public FluidGLSurfaceView(Context context){
        super(context);
        setEGLConfigChooser(8, 8, 8, 8, 16, 0);
        setEGLContextClientVersion(3);

        renderer = new FluidRenderer(context);
        setRenderer(renderer);
    }

    public void setBoundSize(int width, int height){
        renderer.setBoundSize(width, height);
    }

    public void onDestroy(){
        renderer.onDestroy();
    }

    @Override
    public boolean onTouchEvent(MotionEvent e){
        final float x = e.getX();// - this.getLeft();
        final float y = this.getHeight() - e.getY();
        //currTime = SystemClock.elapsedRealtime();

        switch (e.getAction()) {
            case MotionEvent.ACTION_DOWN:
                x0 = x;
                y0 = y;
                queueEvent(new Runnable() {
                    @Override
                    public void run() {
                        renderer.addDensity(x, y, 225);
                    }
                });

            case MotionEvent.ACTION_MOVE:
                queueEvent(new Runnable() {
                    @Override
                    public void run() {
                        renderer.addDensity(x, y, 225);
                    }
                });
                x0 = x;
                y0 = y;
            case MotionEvent.ACTION_UP:

        }
        return true;
    }
}