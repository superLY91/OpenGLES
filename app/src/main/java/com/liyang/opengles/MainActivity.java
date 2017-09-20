package com.liyang.opengles;

import android.content.Context;
import android.opengl.GLSurfaceView;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

public class MainActivity extends AppCompatActivity {

    MyGLSurface mView;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        mView = new MyGLSurface(this);
        setContentView(mView);
    }

    class MyGLSurface extends GLSurfaceView {
        MyGLRenderer mGLRenderer;

        public MyGLSurface(Context context) {
            super(context);
            Native.InitAssetManager(getAssets());
            setEGLContextClientVersion(2);
            mGLRenderer = new MyGLRenderer();
            setRenderer(mGLRenderer);
        }
    }

    class MyGLRenderer implements GLSurfaceView.Renderer {

        @Override
        public void onSurfaceCreated(GL10 gl, EGLConfig eglConfig) {
//            gl.glClearColor(0.1f, 0.4f, 0.6f, 1.0f);
            Native.InitOpenGL();
        }

        @Override
        public void onSurfaceChanged(GL10 gl, int width, int height) {
//            gl.glViewport(0, 0, width, height);
            Native.OnViewportChanged(width, height);
        }

        @Override
        public void onDrawFrame(GL10 gl) {
//            gl.glClear(GL10.GL_COLOR_BUFFER_BIT);
            Native.RenderOneFrame();
        }
    }
}



