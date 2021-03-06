package com.example.neon;

import com.example.neon.R;

import android.app.Activity;
import android.os.Bundle;
import android.widget.TextView;

public class HelloNeon extends Activity {
	
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        TextView  tv = new TextView(this);
        tv.setText( stringFromJNI() );
        setContentView(tv);
    }
    
    /* A native method that is implemented by the
     * 'helloneon' native library, which is packaged
     * with this application.
     */
    public native String  stringFromJNI();

    /* this is used to load the 'helloneon' library on application
     * startup. The library has already been unpacked into
     * /data/data/com.example.neon/lib/libhelloneon.so at
     * installation time by the package manager.
     */
    static {
        System.loadLibrary("helloneon");
    }

}