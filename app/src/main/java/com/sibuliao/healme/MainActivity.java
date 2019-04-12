package com.sibuliao.healme;

import android.graphics.Point;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.view.View;
import android.view.ViewTreeObserver;

public class MainActivity extends AppCompatActivity {

    private FluidGLSurfaceView glSurfaceView;
    private int viewWidth;
    private int viewHeight;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        // Todo: check if supports OpenGLES 2.0/3.0

        /*
        if (true) {
            glSurfaceView = new FluidGLSurfaceView(getApplication());
            setContentView(glSurfaceView);

            glSurfaceView.setBoundSize(
                    getWindowManager().getDefaultDisplay().getWidth(),
                    getWindowManager().getDefaultDisplay().getHeight());

        }
        */

        setContentView(R.layout.activity_main);
        glSurfaceView = (FluidGLSurfaceView)findViewById(R.id.fluidGLSurfaceView);

        ViewTreeObserver viewTreeObserver = glSurfaceView.getViewTreeObserver();
        if (viewTreeObserver.isAlive()) {
            viewTreeObserver.addOnGlobalLayoutListener(new ViewTreeObserver.OnGlobalLayoutListener() {
                @Override
                public void onGlobalLayout() {
                    glSurfaceView.getViewTreeObserver().removeOnGlobalLayoutListener(this);
                    viewWidth = glSurfaceView.getWidth();
                    viewHeight = glSurfaceView.getHeight();
                    glSurfaceView.setBoundSize(viewWidth, viewHeight);
                    Log.d("size", "w " + viewWidth + ", h " + viewHeight);

                }
            });
        }
/*
        Point size = new Point();
        getWindowManager().getDefaultDisplay().getSize(size);
        Log.d("size", "~~~ w " + size.x + ", h " + size.y);
            2076x1080
*/
    }

    @Override
    protected void onPause() {
        super.onPause();
        glSurfaceView.onPause();
    }

    @Override
    protected void onResume() {
        super.onResume();
        glSurfaceView.onResume();
    }

    @Override
    protected void onDestroy() {
        if (isFinishing()){
            glSurfaceView.onDestroy();
        }
        super.onDestroy();
    }

    // Code for Sticky Immersive (fullscreen) mode taken from Android Development guides
    // https://developer.android.com/training/system-ui/immersive#EnableFullscreen

    @Override
    public void onWindowFocusChanged(boolean hasFocus) {
        super.onWindowFocusChanged(hasFocus);
        if (hasFocus){
            hideSystemUI();
        }
    }

    private void hideSystemUI() {
        View decorView = getWindow().getDecorView();
        decorView.setSystemUiVisibility(
                View.SYSTEM_UI_FLAG_IMMERSIVE_STICKY
                // Set the content to appear under the system bars so that
                // the content doesn't resize when the system bars hide and show
                | View.SYSTEM_UI_FLAG_LAYOUT_STABLE
                | View.SYSTEM_UI_FLAG_LAYOUT_HIDE_NAVIGATION
                | View.SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN
                // Hide the nav bar and status bar
                | View.SYSTEM_UI_FLAG_HIDE_NAVIGATION
                | View.SYSTEM_UI_FLAG_FULLSCREEN);
    }

    // Shows the system bars by removing all the flags
    // except for the ones that make the content appear under the system bars.
    private void showSystemUI() {
        View decorView = getWindow().getDecorView();
        decorView.setSystemUiVisibility(
                View.SYSTEM_UI_FLAG_LAYOUT_STABLE
                        | View.SYSTEM_UI_FLAG_LAYOUT_HIDE_NAVIGATION
                        | View.SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN);
    }
}
