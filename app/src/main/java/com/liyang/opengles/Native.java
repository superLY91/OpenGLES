package com.liyang.opengles;

import android.content.res.AssetManager;

/**
 * Created by liyang on 2017/9/18.
 */

public class Native {
    static {
        System.loadLibrary("mygles");
    }

    public static native void InitAssetManager(AssetManager am);
    public static native void InitOpenGL();
    public static native void OnViewportChanged(float width, float height);
    public static native void RenderOneFrame();
}
