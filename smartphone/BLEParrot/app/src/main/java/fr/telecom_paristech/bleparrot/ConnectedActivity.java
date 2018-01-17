package fr.telecom_paristech.bleparrot;

import android.content.BroadcastReceiver;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.ServiceConnection;
import android.os.Bundle;
import android.os.IBinder;
import android.support.v4.content.LocalBroadcastManager;
import android.support.v7.app.AppCompatActivity;
import android.text.method.ScrollingMovementMethod;
import android.util.Log;
import android.view.KeyEvent;
import android.view.View;
import android.widget.EditText;
import android.widget.TextView;
import android.widget.Toast;

public class ConnectedActivity extends AppCompatActivity {

    private Intent gapIntent;
    private GAPService gapService;

    private TextView logWindow;
    private EditText commandField;
    // Defines services connection callbacks
    private ServiceConnection mConnection = new ServiceConnection() {

        @Override
        public void onServiceConnected(ComponentName className,
                                       IBinder service) {
            if (className.getClassName().equals(GAPService.class.getName())) {
                GAPService.LocalBinder binder = (GAPService.LocalBinder) service;
                gapService = binder.getService();
            } else {
                throw new UnsupportedOperationException("Unsupported service connection");
            }
        }

        @Override // Never disconnected
        public void onServiceDisconnected(ComponentName arg0) {
        }

    };
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
    // Device disconnected callback
    private BroadcastReceiver notificationBroadcastReceiver = new BroadcastReceiver() {
        @Override
        public void onReceive(Context context, Intent intent) {
            if (intent.getAction().equals(GAPService.DEVICE_NOTIFICATION_ACTION)) {
                logWindow.append("<- " + intent.getStringExtra("Message") + "\n");
            }
        }
    };

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_connected);

        logWindow = (TextView) findViewById(R.id.logWindow);
        logWindow.setMovementMethod(new ScrollingMovementMethod());
        commandField = (EditText) findViewById(R.id.commandField);

        commandField.setOnKeyListener(new View.OnKeyListener() {
            @Override
            public boolean onKey(View v, int keyCode, KeyEvent event) {
                if (keyCode == KeyEvent.KEYCODE_ENTER && event.getAction() == KeyEvent.ACTION_DOWN) {
                    gapService.send(commandField.getText().toString());
                    logWindow.append("-> " + commandField.getText() + "\n");
                    Log.i("ConnectedActivity", "Sending " + commandField.getText());
                }
                return false;
            }
        });

        gapIntent = new Intent(this, GAPService.class);
        bindService(gapIntent, mConnection, Context.BIND_AUTO_CREATE);
        startService(gapIntent);

        LocalBroadcastManager.getInstance(this).registerReceiver(disconnectedBroadcastReceiver, new IntentFilter(GAPService.DEVICE_DISCONNECTED_ACTION));
        LocalBroadcastManager.getInstance(this).registerReceiver(notificationBroadcastReceiver, new IntentFilter(GAPService.DEVICE_NOTIFICATION_ACTION));
    }
}
