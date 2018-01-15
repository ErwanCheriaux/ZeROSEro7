package fr.telecom_paristech.bleparrot;

import android.app.Service;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothGatt;
import android.bluetooth.BluetoothGattCallback;
import android.bluetooth.BluetoothGattCharacteristic;
import android.bluetooth.BluetoothGattService;
import android.bluetooth.BluetoothProfile;
import android.bluetooth.le.BluetoothLeScanner;
import android.bluetooth.le.ScanCallback;
import android.bluetooth.le.ScanResult;
import android.content.Intent;
import android.os.Binder;
import android.os.IBinder;
import android.support.v4.content.LocalBroadcastManager;
import android.util.Log;

import java.util.Arrays;
import java.util.List;
import java.util.UUID;

/*
 * Scans and initiates bonding connection with device.
 */

public class GAPService extends Service {

    public static final String DEVICE_CONNECTED_ACTION = "Device connected";
    public static final String DEVICE_DISCONNECTED_ACTION = "Device disconnected";
    public static final UUID UART_SERVICE_UUID = UUID.fromString("0000abcd-1212-efde-1523-785fef13d123") ;
    public static final UUID UART_CHARACTERISTIC_UUID = UUID.fromString("00001234-1212-efde-1523-785fef13d123") ;
    private final BluetoothLeScanner scannerInstance;
    private final ScanCallback scanCb;
    private final IBinder mBinder = new LocalBinder();
    private BluetoothAdapter adapter;
    private BluetoothGatt deviceGatt;

    BluetoothGattCharacteristic uart_chara;

    public GAPService() {
        adapter = BluetoothAdapter.getDefaultAdapter();
        scannerInstance = adapter.getBluetoothLeScanner();

        scanCb = new ScanCallback() {
            @Override
            public void onScanResult(int callbackType, ScanResult result) {
                BluetoothDevice device = result.getDevice();
                Log.i("GAPService", "BLE Advertisement detected from :" + device);

                // First connection, not bonded
                if (device != null && device.getName() != null && device.getName().equals("Connected shoe")) {
                    Log.i("GAPService", "Recognised device advertisement");
                    stopScan();

                    ConnectDevice(device);
                }

                // REVIEW could connect to previously bonded device
            }
        };

        startScan();
    }

    private String parseByteArray(byte[] value) {
        return new String(value) ;
    }

    public void send(String s) {
        uart_chara.setValue(s);
        deviceGatt.writeCharacteristic(uart_chara);
    }

    private void ConnectDevice(BluetoothDevice device) {
        BluetoothGattCallback mGattCallback = new BluetoothGattCallback() {
            @Override
            public void onConnectionStateChange(BluetoothGatt gatt, int oldState,
                                                int newState) {
                // TODO
                Log.i("GAPService", "Conn state change: " + newState);
                switch (newState) {
                    case BluetoothProfile.STATE_CONNECTED:
                        Log.i("GAPService", "Connected!");
                        deviceGatt.discoverServices();

                        Intent intent = new Intent(DEVICE_CONNECTED_ACTION);
                        LocalBroadcastManager.getInstance(GAPService.this).sendBroadcast(intent);
                        break;
                    case BluetoothProfile.STATE_CONNECTING:
                        Log.i("GAPService", "Connecting");
                        break;
                    case BluetoothProfile.STATE_DISCONNECTED:
                        Log.i("GAPService", "Disconnected");
                        Intent disco_intent = new Intent(DEVICE_DISCONNECTED_ACTION);
                        LocalBroadcastManager.getInstance(GAPService.this).sendBroadcast(disco_intent);
                        break;
                }

            }

            @Override
            // New services discovered
            public void onServicesDiscovered(BluetoothGatt gatt, int status) {
                // REVIEW could directly connect to know UUID
                Log.i("GAPService", "Service Discovered");
                for (BluetoothGattService service : deviceGatt.getServices()) {
                    Log.i("GAPService", "Service UUID: " + service.getUuid());
                    if(service.getUuid().equals(UART_SERVICE_UUID)) {
                        Log.i("GAPService", "Found UART service");
                        uart_chara = service.getCharacteristic(UART_CHARACTERISTIC_UUID);
                        deviceGatt.setCharacteristicNotification(uart_chara,true);
                        Log.i("GAPService", "UART characteristic is " + uart_chara.getUuid());
                    }
                }
            }

            @Override
            // Result of a characteristic read operation
            public void onCharacteristicRead(BluetoothGatt gatt,
                                             BluetoothGattCharacteristic chara,
                                             int status) {
                Log.i("GAPService","Characteristic Read " + chara.getService().getUuid() + ":" + chara.getUuid() + "=" + parseByteArray(chara.getValue()));
            }

            @Override
            public void onCharacteristicChanged(BluetoothGatt gatt, BluetoothGattCharacteristic characteristic) {
                super.onCharacteristicChanged(gatt, characteristic);
                deviceGatt.readRemoteRssi();
                Log.i("GAPService","Characteristic changed: " + characteristic.getUuid() + " = " + characteristic.getValue());
            }

            @Override
            public void onReadRemoteRssi(BluetoothGatt gatt, int rssi, int status) {
                super.onReadRemoteRssi(gatt, rssi, status);
                Log.i("GAPService","Connection RSSI: " + rssi);
            }

        };
        deviceGatt = device.connectGatt(this, false, mGattCallback);
    }

    private String parseName(String advString) {
        String[] split = advString.split("mDeviceName=");
        String beginning = split[1];
        split = beginning.split("]");
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
