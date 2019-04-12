package com.sibuliao.healme;

import android.opengl.GLSurfaceView;
import android.content.Context;
import android.os.SystemClock;
import android.util.AttributeSet;
import android.util.Log;
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

    public FluidGLSurfaceView(Context context, AttributeSet attrs){
        super(context, attrs);
        setEGLConfigChooser(8, 8, 8, 8, 16, 0);
        setEGLContextClientVersion(3);

        renderer = new FluidRenderer(context);
        setRenderer(renderer);
    }

    public void setBoundSize(int width, int height){
        renderer.setBoundSize(width, height);
        Log.d("size", "well .... w " + this.getWidth() + ", h " + this.getHeight());

    }

    public void onDestroy(){
        renderer.onDestroy();
    }

    @Override
    public boolean onTouchEvent(MotionEvent e){
        final float x = e.getX() - this.getLeft();
        final float y = this.getBottom() - e.getY(); //this.getHeight() - e.getY();
        //currTime = SystemClock.elapsedRealtime();

        switch (e.getAction()) {
            case MotionEvent.ACTION_DOWN:
                Log.d("size", "py " + y);
                Log.d("size", "getBottom " + this.getBottom());
                x0 = x;
                y0 = y;
                renderer.addDensity(x/this.getWidth(), y/this.getHeight(), 255);
                /*
                queueEvent(new Runnable() {
                    @Override
                    public void run() {
                        renderer.addDensity(x, y, 255);
                        //renderer.addDensity(100, 100, 500);
                    }
                });
                */
            case MotionEvent.ACTION_MOVE:
                if (x > this.getRight()){
                    float maxX = this.getRight();
                    renderer.addDensity(maxX / this.getWidth(), y / this.getHeight(), 255);
                    renderer.addForce(x0 / this.getWidth(), y0 / this.getHeight(), maxX / this.getWidth(), y / this.getHeight());
                    x0 = maxX;
                    y0 = y;
                }
                else {
                    //renderer.addDensity(x, y, 255);
                    renderer.addDensity(x / this.getWidth(), y / this.getHeight(), 255);
                    renderer.addForce(x0 / this.getWidth(), y0 / this.getHeight(), x / this.getWidth(), y / this.getHeight());
                    x0 = x;
                    y0 = y;
                }
                /*
                queueEvent(new Runnable() {
                    @Override
                    public void run() {
                        renderer.addDensity(x, y, 500);
                        Log.d("addF", "MOVE " + x0 + " -> " + x + ", " + y0 + " -> " + y);

                        renderer.addForce(x0, y0, x, y);
                        x0 = x;
                        y0 = y;
                        //renderer.addDensity(100, 100, 500);
                        //renderer.addForce(100, 100, x, y);
                    }
                });
                */
            case MotionEvent.ACTION_UP:

        }
        return true;
    }
}