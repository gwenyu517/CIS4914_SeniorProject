package com.stableFluids.cafelatte;

import android.graphics.Canvas;
import android.graphics.Path;
import android.graphics.RectF;
import android.opengl.GLSurfaceView;
import android.content.Context;
import android.util.AttributeSet;
import android.util.Log;
import android.view.MotionEvent;

public class FluidGLSurfaceView extends GLSurfaceView {
    private final FluidRenderer renderer;
    private float x0;
    private float y0;
    private Path shapePath;
    private int mode = 2;
    //private long currTime;
    //private long prevTime;
    //private long dt;

    private float gX, gY;

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
    }

    public void onDestroy(){
        renderer.onDestroy();
    }

    @Override
    protected void dispatchDraw(Canvas canvas) {
        shapePath = new Path();
        shapePath.addRoundRect( new RectF(0, 0, this.getWidth(), this.getHeight()),
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
        //final int historySize = e.getHistorySize();
        //final int pointerCount = e.getPointerCount();

        Log.d("touch", "getSize = " + e.getSize());

        switch (e.getAction()) {
            case MotionEvent.ACTION_DOWN:
                x0 = x;
                y0 = y;

                if (mode == 2) //milk
                    renderer.addDensity(x, y, 255, mode, 4*size);
                else if (mode == 3) // choco
                    renderer.addDensity(x, y, 255, mode, size);

                break;

            case MotionEvent.ACTION_MOVE:
                if (x > 1.0)
                    break;
                else if (x < 0.0)
                    break;

                switch(mode){
                    case 0:         //none
                        break;
                    case 1:         //stir
                        if (size < 0.03)
                            renderer.addForce(x0, y0, 2*size*(x - x0), 2*size*(y - y0), 0.25f*size);
                        else
                            renderer.addForce(x0, y0, 2*size*(x - x0), 2*size*(y - y0), 0.5f*size);
                        break;
                    case 2:         //milk
                        if (size > 0.05)
                            renderer.addForce(x0, y0, size*(x - x0), size*(y - y0), 2*size);
                        else if (size < 0.03)
                            renderer.addForce(x0, y0, size*(x - x0), size*(y - y0), 0.5f*size);
                        else
                            renderer.addForce(x0, y0, size*(x - x0), size*(y - y0), size);


                        renderer.addDensity(x, y, 0.8f*255, mode, size);
                        break;
                    case 3:         //choco
                        renderer.addForce(x0, y0, 0.01f*(x - x0), 0.01f*(y - y0), size);
                        renderer.addDensity(x, y, 255, mode, size);
                        break;
                }

                x0 = x;
                y0 = y;
                break;
                /*
                float currX;
                float currY;
                for (int h = 0; h < historySize; h++) {
                    for (int p = 0; p < pointerCount; p++) {
                        currX = (e.getHistoricalX(p, h) - this.getLeft()) / this.getWidth();
                        currY = (this.getBottom() - e.getHistoricalY(p, h)) / this.getHeight();

                        if (currX > this.getRight())
                            currX = (float)this.getRight() / this.getWidth();

                        switch(mode){
                            case 0:         //none
                                break;
                            case 1:         //stir
                                renderer.addForce(x0, y0, 0.1f*(currX - x0), 0.1f*(currY - y0), size);
                                break;
                            case 2:         //milk
                                renderer.addForce(x0, y0, (currX - x0), (currY - y0), 2*size);
                                renderer.addDensity(currX, currY, 255, mode, size);
                                break;
                            case 3:         //choco
                                renderer.addForce(x0, y0, 0.01f*(currX - x0), 0.01f*(currY - y0), size);
                                renderer.addDensity(currX, currY, 255, mode, size);
                                break;
                        }

                        x0 = currX;
                        y0 = currY;
                    }
                }

                for (int p = 0; p < pointerCount; p++) {
                    currX = (e.getX() - this.getLeft()) / this.getWidth();
                    currY = (this.getBottom() - e.getY()) / this.getHeight();


                    if (currX > this.getRight())
                        currX = (float)this.getRight() / this.getWidth();

                    switch(mode){
                        case 0:         //none
                            break;
                        case 1:         //stir
                            renderer.addForce(x0, y0, 0.1f*(currX - x0), 0.1f*(currY - y0), size);
                            break;
                        case 2:         //milk
                            renderer.addForce(x0, y0, (currX - x0), (currY - y0), 2*size);
                            renderer.addDensity(currX, currY, 255, mode, size);
                            break;
                        case 3:         //choco
                            renderer.addForce(x0, y0, 0.01f*(currX - x0), 0.01f*(currY - y0), size);
                            renderer.addDensity(currX, currY, 255, mode, size);
                            break;
                    }

                    x0 = currX;
                    y0 = currY;
                }
                */

                    }
        return true;
    }

    public void addGravity(float gx, float gy) {
        gX = 0.01f*gx;
        gY = 0.01f*gy;
        //renderer.addGravity(gx, gy);
    }

    public void setMode(int m){
        mode = m;
    }

    public void clearCoffee() {
        renderer.clearCoffee();
    }

}