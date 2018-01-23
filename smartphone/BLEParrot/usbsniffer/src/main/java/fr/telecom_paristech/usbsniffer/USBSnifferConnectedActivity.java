package fr.telecom_paristech.usbsniffer;

import android.os.Bundle;
import android.text.method.ScrollingMovementMethod;
import android.util.Log;
import android.view.KeyEvent;
import android.view.View;
import android.widget.EditText;
import android.widget.TextView;

import blecommon.ConnectedActivity;

public class USBSnifferConnectedActivity extends ConnectedActivity {


    private TextView logWindow;

    @Override
    public void onNotificationReceived(String msg) {
        logWindow.append("<- " + msg + "\n");
        logWindow.scrollTo(0, 0);
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_usbsniffer_connected);

        logWindow = (TextView) findViewById(R.id.logWindow);



        logWindow.setMovementMethod(new ScrollingMovementMethod());
    }
}
