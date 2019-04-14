package com.sibuliao.healme;

import android.graphics.Point;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.view.SurfaceHolder;
import android.view.View;
import android.view.ViewTreeObserver;
import android.widget.CompoundButton;
import android.widget.ToggleButton;

public class MainActivity extends AppCompatActivity implements SurfaceHolder.Callback {

    private FluidGLSurfaceView glSurfaceView;
    private int viewWidth;
    private int viewHeight;

    private ToggleButton milk;
    private ToggleButton choco;
    private ToggleButton stir;



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

        glSurfaceView.setZOrderMediaOverlay(false);
        SurfaceHolder holder = glSurfaceView.getHolder();
        holder.addCallback(this);

/*
        Point size = new Point();
        getWindowManager().getDefaultDisplay().getSize(size);
        Log.d("size", "~~~ w " + size.x + ", h " + size.y);
            2076x1080
*/

        createButtons();
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

    @Override
    public void surfaceCreated(SurfaceHolder holder){

    }

    @Override
    public void surfaceChanged(SurfaceHolder holder, int format, int width, int height) {

    }

    @Override
    public void surfaceDestroyed(SurfaceHolder holder) {

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


    private void createButtons() {
        milk = (ToggleButton)findViewById(R.id.toggleButtonMilk);
        choco = (ToggleButton)findViewById(R.id.toggleButtonChoco);
        stir = (ToggleButton)findViewById(R.id.toggleButtonStir);


        milk.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                if (isChecked){
                    choco.setChecked(false);
                    stir.setChecked(false);
                    glSurfaceView.setMode(1);
                } else {
                    stir.setChecked(true);
                }
            }
        });

        choco.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                if (isChecked) {
                    milk.setChecked(false);
                    stir.setChecked(false);
                    glSurfaceView.setMode(2);
                } else {
                    stir.setChecked(true);
                }
            }
        });

        stir.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                if (isChecked) {
                    milk.setChecked(false);
                    choco.setChecked(false);
                    glSurfaceView.setMode(0);
                } else {
                    glSurfaceView.setMode(-1);
                }
            }
        });


        milk.setChecked(true);
    }
}
