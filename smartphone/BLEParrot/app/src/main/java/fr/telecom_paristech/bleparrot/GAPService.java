package fr.telecom_paristech.bleparrot;

import android.app.Service;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.le.BluetoothLeScanner;
import android.bluetooth.le.ScanCallback;
import android.bluetooth.le.ScanResult;
import android.content.Intent;
import android.os.Binder;
import android.os.IBinder;
import android.support.v4.content.LocalBroadcastManager;
import android.util.Log;

/*
 * Scans and initiates bonding connection with device.
 */

public class GAPService extends Service {

    private final BluetoothLeScanner scannerInstance;
    private final ScanCallback scanCb;
    private final IBinder mBinder = new LocalBinder();
    private BluetoothAdapter adapter;

    public static final String DEVICE_CONECTED_ACTION = "Device connected";

    public GAPService() {
        adapter = BluetoothAdapter.getDefaultAdapter();
        scannerInstance = adapter.getBluetoothLeScanner();

        scanCb = new ScanCallback() {
            @Override
            public void onScanResult(int callbackType, ScanResult result) {
                BluetoothDevice device = result.getDevice();
                Log.i("GAPService", "BLE Advertisement detected from :" + device);

                // First connection, not bonded
                if (device != null && device.getName()!=null && device.getName().equals("Connected shoe")) {
                    Log.i("GAPService", "Recognised device advertisement");
                    stopScan();

                    BondWithDevice(device);

                    Intent intent = new Intent(DEVICE_CONECTED_ACTION);
                    LocalBroadcastManager.getInstance(GAPService.this).sendBroadcast(intent);
                }

                // TODO connect to bonded device
            }
        };

        startScan();
    }

    private void BondWithDevice(BluetoothDevice device) {
        //TODO
    }

    private String parseName(String advString) {
        String[] split = advString.split("mDeviceName=") ;
        String beginning = split[1] ;
        split = beginning.split("]") ;
        return split[0];

    }

    public void startScan() {
        scannerInstance.startScan(scanCb);
        Log.i("GAPService", "BLE Scanning");
    }

    public void stopScan() {
        scannerInstance.stopScan(scanCb);
        Log.i("GAPService", "BLE stopped Scanning");
    }

    @Override
    public void onDestroy() {
        stopScan();
        Log.i("GAPService", "Service destroyed");
        super.onDestroy();
    }

    @Override
    public int onStartCommand(Intent intent, int flags, int startId) {
        Log.i("GAPService", "Service started. Received start id " + startId + ": " + intent);
        return START_STICKY;    // Remain in the background when the app is paused. Has to be explicitly stopped
    }

    @Override
    public IBinder onBind(Intent intent) {
        return mBinder;
    }

    public class LocalBinder extends Binder {
        // Return this instance of LocalService so clients can call public methods
        GAPService getService() {
            return GAPService.this;
        }
    }
}
