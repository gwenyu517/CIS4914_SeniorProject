package com.sibuliao.healme;

import android.graphics.Canvas;
import android.graphics.Path;
import android.graphics.Point;
import android.graphics.RectF;
import android.opengl.GLSurfaceView;
import android.content.Context;
import android.os.SystemClock;
import android.util.AttributeSet;
import android.util.Log;
import android.view.MotionEvent;
import android.widget.CompoundButton;
import android.widget.ToggleButton;

public class FluidGLSurfaceView extends GLSurfaceView {
    private final FluidRenderer renderer;
    private float x0;
    private float y0;
    private Path shapePath;

    private int mode;
    //private long currTime;
    //private long prevTime;
    //private long dt;

    public FluidGLSurfaceView(Context context){
        super(context);
        //setEGLConfigChooser(8, 8, 8, 8, 16, 0);
        setEGLContextClientVersion(3);

        renderer = new FluidRenderer(context);
        setRenderer(renderer);
    }

    public FluidGLSurfaceView(Context context, AttributeSet attrs){
        super(context, attrs);
        //setEGLConfigChooser(8, 8, 8, 8, 16, 0);
        setEGLContextClientVersion(3);

        renderer = new FluidRenderer(context);
        setRenderer(renderer);
    }

    public void setBoundSize(int width, int height){
        renderer.setBoundSize(width, height);
        //Log.d("size", "well .... w " + this.getWidth() + ", h " + this.getHeight());
    }

    public void onDestroy(){
        renderer.onDestroy();
    }

    @Override
    protected void dispatchDraw(Canvas canvas) {
        shapePath = new Path();
        shapePath.addRoundRect( new RectF(this.getLeft()+32, this.getTop()+32, this.getRight()-32, this.getBottom()-32),
                100.0f, 100.0f, Path.Direction.CW);
        canvas.clipPath(shapePath);
        super.dispatchDraw(canvas);
    }

    @Override
    public boolean onTouchEvent(MotionEvent e){
        final float x = (e.getX() - this.getLeft()) / this.getWidth();
        final float y = (this.getBottom() - e.getY()) / this.getHeight(); //this.getHeight() - e.getY();
        float size = e.getSize();
        //currTime = SystemClock.elapsedRealtime();

        Log.d("touch", "getSize = " + e.getSize());

        switch (e.getAction()) {
            case MotionEvent.ACTION_DOWN:
                //Log.d("size", "py " + y);
                //Log.d("size", "getBottom " + this.getBottom());
                x0 = x;
                y0 = y;
                Log.d("mode", "" + mode);
                if (mode > 0)
                    renderer.addDensity(x, y, 255, mode, size);

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
                float currX;
                if (x > this.getRight())
                    currX = (float)this.getRight() / this.getWidth();
                else
                    currX = x;

                if (mode == 0)
                    renderer.addForce(x0, y0, currX, y, 0);

                if (mode > 0)
                    renderer.addForce(x0, y0, currX, y, size);

                if (mode > 0)
                    renderer.addDensity(currX, y, 255, mode, size);

                x0 = currX;
                y0 = y;
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

    public void setMode(int m){
        mode = m;
    }

    public void clearCoffee() {
        renderer.clearCoffee();
    }

}