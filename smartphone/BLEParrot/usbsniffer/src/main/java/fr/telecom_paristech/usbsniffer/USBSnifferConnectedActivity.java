package fr.telecom_paristech.usbsniffer;

import android.os.Bundle;

import blecommon.ConnectedActivity;

public class USBSnifferConnectedActivity extends ConnectedActivity {

    @Override
    public void onNotificationReceived(String msg) {
        // TODO
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_usbsniffer_connected);
    }
}
