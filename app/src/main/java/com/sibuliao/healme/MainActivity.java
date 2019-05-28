package com.sibuliao.healme;

import android.content.res.AssetManager;
import android.os.Bundle;
import android.support.v4.app.FragmentManager;
import android.support.v4.app.FragmentTransaction;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.view.SurfaceHolder;
import android.view.View;
import android.view.ViewTreeObserver;
import android.widget.Button;
import android.widget.CompoundButton;
import android.widget.ToggleButton;

public class MainActivity extends AppCompatActivity implements SurfaceHolder.Callback {

    private FluidGLSurfaceView glSurfaceView;
    private int viewWidth;
    private int viewHeight;

    private ToggleButton milk;
    private ToggleButton choco;
    private ToggleButton stir;
//    private ToggleButton gravity;
    private Button clear;
    private Button help;
/*
    private SensorManager sensorManager;
    private Sensor gravity_sensor;
    private SensorEventListener gravityListener;
    private boolean gravityOn;
*/

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        AssetManager assetManager = getAssets();
        FluidLibJNIWrapper.passAssetManager(assetManager);

        // Todo: check if supports OpenGLES 2.0/3.0

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
            //2076x1080
*/

        createButtons();
        //setUpGravitySensor();
    }

    @Override
    protected void onPause() {
        super.onPause();
        //sensorManager.unregisterListener(gravityListener);

        glSurfaceView.onPause();
    }

    @Override
    protected void onResume() {
        super.onResume();
        //sensorManager.registerListener(gravityListener, gravity_sensor, SensorManager.SENSOR_DELAY_NORMAL);

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
        createMilkButton();
        createChocoButton();
        createStirButton();
        //createGravityButton();
        createClearButton();
        createHelpButton();

        milk.setChecked(true);
        //gravity.setChecked(false);
    }

    private void createMilkButton() {
        milk = (ToggleButton)findViewById(R.id.toggleButton_Milk);
        milk.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                if (isChecked){
                    stir.setChecked(false);
                    choco.setChecked(false);
                    glSurfaceView.setMode(2);
                } else {
                    glSurfaceView.setMode(0);
                }
            }
        });
    }

    private void createChocoButton() {
        choco = (ToggleButton)findViewById(R.id.toggleButton_Choco);
        choco.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                if (isChecked) {
                    milk.setChecked(false);
                    stir.setChecked(false);
                    glSurfaceView.setMode(3);
                } else {
                    glSurfaceView.setMode(0);
                }
            }
        });
    }

    private void createStirButton() {
        stir = (ToggleButton)findViewById(R.id.toggleButton_Stir);
        stir.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                if (isChecked) {
                    milk.setChecked(false);
                    choco.setChecked(false);
                    glSurfaceView.setMode(1);
                } else {
                    glSurfaceView.setMode(0);
                }
            }
        });
    }
/*
    private void createGravityButton() {
        gravity = (ToggleButton)findViewById(R.id.toggleButton_Gravity);
        gravity.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                if (isChecked) {
                    sensorManager.registerListener(gravityListener, gravity_sensor, SensorManager.SENSOR_DELAY_NORMAL);
                    gravityOn = true;
                }
                else {
                    gravityOn = false;
                }
            }
        });
    }
*/

    private void createClearButton() {
        clear = (Button)findViewById(R.id.button_clear);
        clear.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                glSurfaceView.clearCoffee();
            }
        });
    }

    private void createHelpButton() {
        help = (Button)findViewById(R.id.button_help);
        help.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                FragmentManager fragmentManager = getSupportFragmentManager();
                InstructionFragment newFragment = new InstructionFragment();

                FragmentTransaction transaction = fragmentManager.beginTransaction();
                transaction.setTransition(FragmentTransaction.TRANSIT_FRAGMENT_OPEN);
                transaction.add(android.R.id.content, newFragment).addToBackStack(null).commit();


                /*AlertDialog.Builder builder = new AlertDialog.Builder(MainActivity.this);
                LayoutInflater inflater = getLayoutInflater();
                builder.setView(inflater.inflate(R.layout.instructions, null));
                builder.setPositiveButton("wut", new DialogInterface.OnClickListener() {
                    @Override
                    public void onClick(DialogInterface dialog, int which) {

                    }
                });
                AlertDialog alert = builder.create();
                alert.show();*/
            }
        });
    }


/*
    private void setUpGravitySensor() {
        sensorManager = (SensorManager)getSystemService(SENSOR_SERVICE);
        if (sensorManager.getDefaultSensor(Sensor.TYPE_GRAVITY) != null) {
            gravity_sensor = sensorManager.getDefaultSensor(Sensor.TYPE_GRAVITY);
            gravityListener = new SensorEventListener() {
                @Override
                public void onSensorChanged(SensorEvent event) {
                    float gy = -event.values[0];
                    float gx = event.values[1];
                    Log.d("sensor", "paaaaaabo");

                    if (gravityOn)
                        glSurfaceView.addGravity(gx, gy);
                    else
                        sensorManager.unregisterListener(gravityListener);
                }

                @Override
                public void onAccuracyChanged(Sensor sensor, int accuracy) {

                }
            };
            gravityOn = true;
        } else {
            //gravity.setEnabled(false);
        }
    }
*/
}
