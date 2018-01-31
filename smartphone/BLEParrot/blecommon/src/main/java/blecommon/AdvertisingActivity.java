package blecommon;

import android.Manifest;
import android.app.AlertDialog;
import android.bluetooth.BluetoothAdapter;
import android.content.BroadcastReceiver;
import android.content.ComponentName;
import android.content.Context;
import android.content.DialogInterface;
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

public abstract class AdvertisingActivity extends AppCompatActivity {

    public static final String APP_ID_EXTRA = "APP ID";

    private Intent advertisementIntent;
    private AdvertiserService advertiser;

    private Intent gapIntent;
    private GAPService gap;

    private boolean advertiserStarted = true;
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
                Log.i("AdvertisingActivity", "Device connected, switching activity");
                Toast.makeText(getApplicationContext(), "Device connected!", Toast.LENGTH_SHORT).show();
                advertiser.pause();
                gap.stopScan();
                getAdvertisingProgress().setVisibility(View.INVISIBLE);
                getPauseResumeButton().setText("Stopped");
                getAdvertisingTitle().setText("Device detected");
                onPhoneConnected();
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
                getAdvertisingProgress().setVisibility(View.VISIBLE);
                getPauseResumeButton().setText("Pause");
                getAdvertisingTitle().setText("Advertising in BLE");
            }
        }
    };

    public abstract ProgressBar getAdvertisingProgress();

    public abstract Button getPauseResumeButton();

    public abstract TextView getAdvertisingTitle();

    public abstract void onPhoneConnected();

    // Manufacturer Data in BLE advertisements used to differentiate apps and devices.
    public abstract byte[] getAppID();

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        // Allow location access, strangely required for accessing BLE scan results.
        // Permission is prompted once at first use of the application
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M && checkSelfPermission(Manifest.permission.ACCESS_COARSE_LOCATION) != PackageManager.PERMISSION_GRANTED) {
            requestPermissions(new String[]{Manifest.permission.ACCESS_COARSE_LOCATION}, 1);
        }

        super.onCreate(savedInstanceState);

        final BluetoothAdapter mBluetoothAdapter = BluetoothAdapter.getDefaultAdapter();
        if (!mBluetoothAdapter.isEnabled()) {
            AlertDialog.Builder builder = new AlertDialog.Builder(this);
            builder.setMessage("Le Bluetooth va être activé.")
                    .setPositiveButton("Ok", new DialogInterface.OnClickListener() {
                        public void onClick(DialogInterface dialog, int id) {
                            mBluetoothAdapter.enable();
                            IntentFilter filter = new IntentFilter(BluetoothAdapter.ACTION_STATE_CHANGED);
                            registerReceiver(new BroadcastReceiver() {
                                @Override
                                public void onReceive(Context context, Intent intent) {
                                    final String action = intent.getAction();
                                    if (action.equals(BluetoothAdapter.ACTION_STATE_CHANGED)) {
                                        final int state = intent.getIntExtra(BluetoothAdapter.EXTRA_STATE,
                                                BluetoothAdapter.ERROR);
                                        switch (state) {
                                            case BluetoothAdapter.STATE_ON:
                                                startBluetoothServices();
                                                break;
                                            default:
                                                break;
                                        }
                                    }
                                }
                            }, filter);
                        }
                    });
            builder.show();
        } else {
            startBluetoothServices();
        }
    }

    private void startBluetoothServices() {
        advertisementIntent = new Intent(this, AdvertiserService.class);
        advertisementIntent.putExtra(APP_ID_EXTRA, getAppID());
        bindService(advertisementIntent, mConnection, Context.BIND_AUTO_CREATE);
        startService(advertisementIntent);

        gapIntent = new Intent(this, GAPService.class);
        gapIntent.putExtra(APP_ID_EXTRA, getAppID());
        bindService(gapIntent, mConnection, Context.BIND_AUTO_CREATE);
        startService(gapIntent);

        LocalBroadcastManager.getInstance(this).registerReceiver(connectedBroadcastReceiver, new IntentFilter(GAPService.DEVICE_CONNECTED_ACTION));
        LocalBroadcastManager.getInstance(this).registerReceiver(disconnectedBroadcastReceiver, new IntentFilter(GAPService.DEVICE_DISCONNECTED_ACTION));
    }

    public void onPauseResumeButton(View v) {
        if (advertiserStarted) {
            advertiser.pause();
            gap.stopScan();
            getAdvertisingProgress().setVisibility(View.INVISIBLE);
            getPauseResumeButton().setText("Resume");
            getAdvertisingTitle().setText("Advertising Stopped");
        } else {
            advertiser.resume();
            gap.startScan();
            getAdvertisingProgress().setVisibility(View.VISIBLE);
            getPauseResumeButton().setText("Pause");
            getAdvertisingTitle().setText("Advertising in BLE");
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
        Log.i("AdvertisingActivity", "Activity paused");
        super.onPause();
    }

    @Override
    public void onResume() {
        Log.i("AdvertisingActivity", "Activity resumed");
        super.onResume();
    }

    @Override
    public void onDestroy() {
        Toast.makeText(getApplicationContext(), "BLE Advertisement stopped", Toast.LENGTH_LONG).show();
        Log.i("AdvertisingActivity", "Activity destroyed");
        stopService(advertisementIntent);
        stopService(gapIntent);
        unbindService(mConnection);
        super.onDestroy();
    }

}
