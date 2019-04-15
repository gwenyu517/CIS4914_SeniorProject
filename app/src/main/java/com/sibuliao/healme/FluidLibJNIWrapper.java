package com.sibuliao.healme;

import android.content.res.AssetManager;

public class FluidLibJNIWrapper {
    static {
        System.loadLibrary("fluid-lib");
    }

    public static native void passAssetManager(AssetManager java_asset_manager);
    public static native void setBoundSize(int width, int height);
    public static native void on_surface_created();
    public static native void on_surface_changed(int width, int height);
    public static native void on_draw_frame(long dt);
    public static native void on_destroy();

    public static native void addForce(float x0, float y0, float amountX, float amountY, float size);
    public static native void addDensity(float x, float y, float amount, int mode, float size);
    public static native void addGravity(float gx, float gy);
    public static native void clearQuad();
}
