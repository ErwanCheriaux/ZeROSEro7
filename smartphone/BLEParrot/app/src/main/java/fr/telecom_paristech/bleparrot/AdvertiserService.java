package fr.telecom_paristech.bleparrot;

import android.app.Service;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.le.AdvertiseCallback;
import android.bluetooth.le.AdvertiseData;
import android.bluetooth.le.AdvertiseSettings;
import android.bluetooth.le.BluetoothLeAdvertiser;
import android.content.Intent;
import android.os.Binder;
import android.os.IBinder;
import android.util.Log;
import android.widget.Toast;

public class AdvertiserService extends Service {

    private final IBinder mBinder = new LocalBinder();
    private BluetoothAdapter adapter;
    private BluetoothLeAdvertiser advertiserInstance;
    private AdvertiseSettings settings;
    private AdvertiseData advertiseData;
    private AdvertiseCallback callback;

    public AdvertiserService() {
        adapter = BluetoothAdapter.getDefaultAdapter();
        advertiserInstance = adapter.getBluetoothLeAdvertiser();
        Log.i("AdvertiserService", "AdvertiserService Instance : " + advertiserInstance);

        settings = buildSettings();
        advertiseData = buildData();
        callback = new AdvertiserCallbackFeedback(this);

        Log.i("AdvertiserService", settings + " " + advertiseData + " " + callback);

        advertiserInstance.startAdvertising(settings,
                advertiseData,
                callback
        );
    }


    public void pause() {
        advertiserInstance.stopAdvertising(callback);
        Log.i("AdvertiserService", "BLE Advertising paused");
    }


    public void resume() {
        advertiserInstance.startAdvertising(settings,
                advertiseData,
                callback
        );
        Log.i("AdvertiserService", "BLE Advertising resumed");
    }

    private AdvertiseSettings buildSettings() {
        AdvertiseSettings.Builder settingsBuilder = new AdvertiseSettings.Builder();

        // Most consuming for best performances. The phone as great battery and advertisement won't last long.
        settingsBuilder.setAdvertiseMode(AdvertiseSettings.ADVERTISE_MODE_LOW_LATENCY);
        settingsBuilder.setTxPowerLevel(AdvertiseSettings.ADVERTISE_TX_POWER_HIGH);
        settingsBuilder.setConnectable(false);
        settingsBuilder.setTimeout(0);

        return settingsBuilder.build();
    }

    private AdvertiseData buildData() {
        AdvertiseData.Builder dataBuilder = new AdvertiseData.Builder();

        dataBuilder.setIncludeDeviceName(true);

        return dataBuilder.build();
    }

    @Override
    public void onDestroy() {
        pause();
        Log.i("AdvertiserService", "Service destroyed");
        super.onDestroy();
    }

    @Override
    public int onStartCommand(Intent intent, int flags, int startId) {
        Log.i("AdvertiserService", "Service started. Received start id " + startId + ": " + intent);
        return START_STICKY;    // Remain in the background when the app is paused
    }

    @Override
    public IBinder onBind(Intent intent) {
        return mBinder;
    }

    // Mandatory callback when starting advertiser
    private class AdvertiserCallbackFeedback extends AdvertiseCallback {

        private AdvertiserService parentAdvertiserService; // Context of the application to make Toasts

        AdvertiserCallbackFeedback(AdvertiserService parentAdvertiserService) {
            this.parentAdvertiserService = parentAdvertiserService;
        }

        @Override
        public void onStartSuccess(AdvertiseSettings settingsInEffect) {
            super.onStartSuccess(settingsInEffect);
            Toast.makeText(parentAdvertiserService.getApplicationContext(), "Now advertising through BLE", Toast.LENGTH_LONG);
        }

        @Override
        public void onStartFailure(int errorCode) {
            super.onStartFailure(errorCode);
            Toast.makeText(parentAdvertiserService.getApplicationContext(), "Failed to start BLE advertising", Toast.LENGTH_LONG);
        }
    }

    public class LocalBinder extends Binder {
        // Return this instance of LocalService so clients can call public methods
        AdvertiserService getService() {
            return AdvertiserService.this;
        }
    }

}
