package fr.telecom_paristech.bleparrot;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.support.v4.content.LocalBroadcastManager;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.Toast;

public class ConnectedActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_connected);
        LocalBroadcastManager.getInstance(this).registerReceiver(disconnectedBroadcastReceiver, new IntentFilter(GAPService.DEVICE_DISCONNECTED_ACTION));
    }

    // Device disconnected callback
    private BroadcastReceiver disconnectedBroadcastReceiver = new BroadcastReceiver() {
        @Override
        public void onReceive(Context context, Intent intent) {
            if (intent.getAction().equals(GAPService.DEVICE_DISCONNECTED_ACTION)) {
                Toast.makeText(getApplicationContext(), "Lost connection !", Toast.LENGTH_LONG).show();
                finish();
            }
        }
    };
}
