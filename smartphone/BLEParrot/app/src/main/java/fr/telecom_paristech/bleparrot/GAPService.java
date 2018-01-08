package fr.telecom_paristech.bleparrot;

import android.app.Service;
import android.bluetooth.BluetoothAdapter;
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

    public static final String DEVICE_DETECTED_ACTION = "Device connected";

    public GAPService() {
        adapter = BluetoothAdapter.getDefaultAdapter();
        scannerInstance = adapter.getBluetoothLeScanner();

        scanCb = new ScanCallback() {
            @Override
            public void onScanResult(int callbackType, ScanResult result) {
                Log.i("GAPService", "BLE Advertisment detected from :" + parseName(result.toString()));
                if (parseName(result.toString()).equals("ZeROSEro7 Device")) {
                    Log.i("GAPService", "Recognised device advertisment");
                    Intent intent = new Intent(DEVICE_DETECTED_ACTION);
                    LocalBroadcastManager.getInstance(GAPService.this).sendBroadcast(intent);
                    // TODO Connect and bond
                }
            }
        };

        startScan();
    }

    private String parseName(String advString) {
        String[] split = advString.split("mDeviceName=") ;
        String beginning = split[1] ;
        split = beginning.split("]") ;
        return split[0];

    }

    public void startScan() {
        Log.i("GAPService", "BLE Scanning");
        scannerInstance.startScan(scanCb);
    }

    public void stopScan() {
        Log.i("GAPService", "BLE stopped Scanning");
        scannerInstance.stopScan(scanCb);
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
