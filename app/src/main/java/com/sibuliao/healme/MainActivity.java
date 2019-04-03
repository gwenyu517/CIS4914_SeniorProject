package com.sibuliao.healme;

import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;

public class MainActivity extends AppCompatActivity {

    private FluidGLSurfaceView glSurfaceView;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        // Todo: check if supports OpenGLES 2.0/3.0

        if (true) {
            glSurfaceView = new FluidGLSurfaceView(getApplication());
            setContentView(glSurfaceView);

            glSurfaceView.setBoundSize(
                    getWindowManager().getDefaultDisplay().getWidth(),
                    getWindowManager().getDefaultDisplay().getHeight());

        }
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
}
