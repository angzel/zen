package com.example.zen_view_sample_android;

import android.os.Bundle;
import android.util.Log;

import zen.app.*;

public class MainActivity extends AppActivity {

    // Used to load the 'native-lib' library on application startup.
    static {
        Log.v("ZEN LIB", "looo lib");
        System.loadLibrary("native-lib");
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
    }

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    public native String stringFromJNI();
}
