package fr.telecom_paristech.usbsniffer;

import android.content.Context;
import android.os.Bundle;
import android.os.Environment;
import android.text.method.ScrollingMovementMethod;
import android.widget.TextView;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;

import blecommon.ConnectedActivity;

public class USBSnifferConnectedActivity extends ConnectedActivity {

    public static final String FILENAME = "usb_sniffer_dump.txt";
    private TextView logWindow;
    private FileOutputStream dumpFileStream;

    @Override
    public void onNotificationReceived(String msg) {
        logWindow.append("<- " + msg + "\n");
        logWindow.scrollTo(0, 0);

        dumpInFile(msg);
    }

    private void dumpInFile(String s) {
        try {
            dumpFileStream.write(s.getBytes());
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_usbsniffer_connected);

        logWindow = (TextView) findViewById(R.id.logWindow);
        logWindow.setMovementMethod(new ScrollingMovementMethod());
    }

    @Override
    protected void onRestart() {
        super.onRestart();

        String path = Environment.getExternalStoragePublicDirectory(Environment.DIRECTORY_DOWNLOADS) + File.separator;
        File dumpFile = new File(path, FILENAME);
        if (dumpFile.exists()) {
            dumpFile.delete();
        }
        try {
            dumpFile.createNewFile();
            dumpFileStream = openFileOutput(FILENAME, Context.MODE_PRIVATE);
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    @Override
    public void onStop() {

        try {
            dumpFileStream.close();
        } catch (IOException e) {
            e.printStackTrace();
        }
        super.onStop();
    }
}
