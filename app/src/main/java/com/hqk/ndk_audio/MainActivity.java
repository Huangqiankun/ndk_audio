package com.hqk.ndk_audio;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.os.Environment;
import android.widget.TextView;

import java.io.File;

public class MainActivity extends AppCompatActivity {

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        // Example of a call to a native method
        TextView tv = findViewById(R.id.sample_text);
        tv.setText(stringFromJNI());


        File inputFile = new File(Environment.getExternalStorageDirectory(), "input.mp3");
        File outFile = new File(Environment.getExternalStorageDirectory(), "output.pcm");
        playMusic(inputFile.getAbsolutePath(), outFile.getAbsolutePath());
    }

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    public native String stringFromJNI();

    // input mp3 mp4 avi
    // output 原始数据
    public native void playMusic(String input, String output);
}
