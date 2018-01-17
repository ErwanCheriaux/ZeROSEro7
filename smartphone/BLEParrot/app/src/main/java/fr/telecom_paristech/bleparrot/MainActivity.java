package fr.telecom_paristech.bleparrot;

import android.Manifest;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothManager;
import android.bluetooth.BluetoothProfile;
import android.content.BroadcastReceiver;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.ServiceConnection;
import android.content.pm.PackageManager;
import android.os.Build;
import android.os.Bundle;
import android.os.IBinder;
import android.support.v4.content.LocalBroadcastManager;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.ProgressBar;
import android.widget.TextView;
import android.widget.Toast;

import java.util.List;

public class MainActivity extends AppCompatActivity {

    private Intent advertisementIntent;
    private AdvertiserService advertiser;
    private Button pauseResumeButton;
    private ProgressBar advertisingProgress;
    private TextView advertisingTitle;

    private Intent gapIntent;
    private GAPService gap;

    // Defines services connection callbacks
    private ServiceConnection mConnection = new ServiceConnection() {

        @Override
        public void onServiceConnected(ComponentName className,
                                       IBinder service) {
            if (className.getClassName().equals(AdvertiserService.class.getName())) {
                // We've bound to LocalService, cast the IBinder and get LocalService instance
                AdvertiserService.LocalBinder binder = (AdvertiserService.LocalBinder) service;
                advertiser = binder.getService();
            } else if (className.getClassName().equals(GAPService.class.getName())) {
                GAPService.LocalBinder binder = (GAPService.LocalBinder) service;
                gap = binder.getService();
            } else {
                throw new UnsupportedOperationException("Unsupported service connection");
            }
        }

        @Override // Never disconnected
        public void onServiceDisconnected(ComponentName arg0) {
        }

    };

    // Device connected callback
    private BroadcastReceiver connectedBroadcastReceiver = new BroadcastReceiver() {
        @Override
        public void onReceive(Context context, Intent intent) {
            if (intent.getAction().equals(GAPService.DEVICE_CONNECTED_ACTION)) {
                Toast.makeText(getApplicationContext(), "Device connected!", Toast.LENGTH_LONG).show();
                advertiser.pause();
                gap.stopScan();
                advertisingProgress.setVisibility(View.INVISIBLE);
                pauseResumeButton.setText("Stopped");
                advertisingTitle.setText("Device detected");
                startConnectedActiviy();
            }
        }
    };

    // Device disconnected callback
    private BroadcastReceiver disconnectedBroadcastReceiver = new BroadcastReceiver() {
        @Override
        public void onReceive(Context context, Intent intent) {
            if (intent.getAction().equals(GAPService.DEVICE_DISCONNECTED_ACTION)) {
                advertiser.resume();
                gap.startScan();
                advertisingProgress.setVisibility(View.VISIBLE);
                pauseResumeButton.setText("Pause");
                advertisingTitle.setText("Advertising in BLE");
            }
        }
    };

    private void startConnectedActiviy() {
        Intent intent = new Intent(this, ConnectedActivity.class);
        startActivity(intent);
    }

    private boolean advertiserStarted = true;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        // Allow location access, strangely required for accessing BLE scan results.
        // Permission is prompted once at first use of the application
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M && checkSelfPermission(Manifest.permission.ACCESS_COARSE_LOCATION) != PackageManager.PERMISSION_GRANTED) {
            requestPermissions(new String[]{Manifest.permission.ACCESS_COARSE_LOCATION}, 1);
        }
        // Allow File
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M && checkSelfPermission(Manifest.permission.WRITE_EXTERNAL_STORAGE) != PackageManager.PERMISSION_GRANTED) {
            requestPermissions(new String[]{Manifest.permission.WRITE_EXTERNAL_STORAGE}, 2);
        }

        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        LocalBroadcastManager.getInstance(this).registerReceiver(connectedBroadcastReceiver, new IntentFilter(GAPService.DEVICE_CONNECTED_ACTION));
        LocalBroadcastManager.getInstance(this).registerReceiver(disconnectedBroadcastReceiver, new IntentFilter(GAPService.DEVICE_DISCONNECTED_ACTION));

        pauseResumeButton = (Button) findViewById(R.id.pauseResumeButton);
        advertisingProgress = (ProgressBar) findViewById(R.id.advertisingProgress);
        advertisingTitle = (TextView) findViewById(R.id.advertisingTitle);

        advertisementIntent = new Intent(this, AdvertiserService.class);
        bindService(advertisementIntent, mConnection, Context.BIND_AUTO_CREATE);
        startService(advertisementIntent);

        gapIntent = new Intent(this, GAPService.class);
        bindService(gapIntent, mConnection, Context.BIND_AUTO_CREATE);
        startService(gapIntent);
    }

    public void onPauseResumeButton(View v) {
        if (advertiserStarted) {
            advertiser.pause();
            gap.stopScan();
            advertisingProgress.setVisibility(View.INVISIBLE);
            pauseResumeButton.setText("Resume");
            advertisingTitle.setText("Advertising Stopped");
        } else {
            advertiser.resume();
            gap.startScan();
            advertisingProgress.setVisibility(View.VISIBLE);
            pauseResumeButton.setText("Pause");
            advertisingTitle.setText("Advertising in BLE");
        }
        advertiserStarted = !advertiserStarted;
    }

    @Override
    public void onBackPressed() {
        moveTaskToBack(true);  // Doesn't stop the app
    }

    @Override
    public void onPause() {
        // Advertisement is running in the background
        Log.i("MainActivity", "Activity paused");
        super.onPause();
    }

    @Override
    public void onResume() {
        Log.i("MainActivity", "Activity resumed");
        super.onResume();
    }

    @Override
    public void onDestroy() {
        Toast.makeText(getApplicationContext(), "BLE Advertisement stopped", Toast.LENGTH_LONG).show();
        Log.i("MainActivity", "Activity destroyed");
        stopService(advertisementIntent);
        stopService(gapIntent);
        super.onDestroy();
    }

}
