package blecommon;

import android.app.Service;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothGatt;
import android.bluetooth.BluetoothGattCallback;
import android.bluetooth.BluetoothGattCharacteristic;
import android.bluetooth.BluetoothGattDescriptor;
import android.bluetooth.BluetoothGattService;
import android.bluetooth.BluetoothManager;
import android.bluetooth.BluetoothProfile;
import android.bluetooth.le.BluetoothLeScanner;
import android.bluetooth.le.ScanCallback;
import android.bluetooth.le.ScanResult;
import android.content.Context;
import android.content.Intent;
import android.os.Binder;
import android.os.IBinder;
import android.support.v4.content.LocalBroadcastManager;
import android.util.Log;

import java.util.Arrays;
import java.util.List;
import java.util.UUID;

import static blecommon.AdvertiserService.ADIDAS_MANUFACTURER_ID;

/*
 * Scans and initiates bonding connection with device.
 */

public class GAPService extends Service {

    public static final String DEVICE_CONNECTED_ACTION = "Device connected";
    public static final String DEVICE_DISCONNECTED_ACTION = "Device disconnected";
    public static final String DEVICE_NOTIFICATION_ACTION = "Device notification";
    public static final UUID UART_SERVICE_UUID = UUID.fromString("0000abcd-1212-efde-1523-785fef13d123");
    public static final UUID UART_CHARACTERISTIC_UUID = UUID.fromString("00001234-1212-efde-1523-785fef13d123");
    public static final UUID UART_DESCRIPTOR_UUID = UUID.fromString("00002902-0000-1000-8000-00805f9b34fb");
    public static final String BLE_DEVICE_NAME = "Connected shoe";
    public static final int MAX_MTU_NRF = 251;
    private final BluetoothLeScanner scannerInstance;
    private final ScanCallback scanCb;
    private final IBinder mBinder = new LocalBinder();
    private BluetoothGattCharacteristic bleUartChara;
    private BluetoothAdapter adapter;
    private BluetoothGatt deviceGatt;
    private int currentMTU = 20;

    private byte[] appId;

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
                    Log.i("GAPService", "Manufacturer specific data :" + Arrays.toString(result.getScanRecord().getManufacturerSpecificData(ADIDAS_MANUFACTURER_ID)));
                    Log.i("GAPService", "Expected :" + Arrays.toString(appId));
                    if (result.getScanRecord().getManufacturerSpecificData(ADIDAS_MANUFACTURER_ID) != null
                            && Arrays.equals(result.getScanRecord().getManufacturerSpecificData(ADIDAS_MANUFACTURER_ID), appId)) {
                        Log.i("GAPService", "Recognised device advertisement");
                        stopScan();

                        connectDevice(device);
                    } else {
                        Log.i("GAPService", "Recognised some shoe, but not our shoe.");
                    }
                }

                // REVIEW could connect to previously bonded device
            }
        };

        startScan();
    }

    public static String parseByteArray(byte[] value) {
        return new String(value);
    }

    // Copied from https://stackoverflow.com/questions/5513152/easy-way-to-concatenate-two-byte-arrays by Jonathan
    public static byte[] concat(byte[] a, byte[] b) {
        byte[] c = new byte[a.length + b.length];
        System.arraycopy(a, 0, c, 0, a.length);
        System.arraycopy(b, 0, c, a.length, b.length);
        return c;
    }

    // Called after application context is initialized
    @Override
    public void onCreate() {
        // Check if device is already connected
        BluetoothManager bluetoothManager = (BluetoothManager) getApplicationContext().getSystemService(Context.BLUETOOTH_SERVICE);
        List<BluetoothDevice> devices = bluetoothManager.getConnectedDevices(BluetoothProfile.GATT);
        for (BluetoothDevice device : devices) {
            if (device.getType() == BluetoothDevice.DEVICE_TYPE_LE) {
                if (device.getName().equals(BLE_DEVICE_NAME)) {
                    Log.i("GAPService", "Device advertisement detected");
                    connectDevice(device);
                }
            }
        }
    }

    public void send(String s) {
        Log.e("GAP Service", "Sending : " + s + "\n");
        bleUartChara.setValue(s);
        deviceGatt.writeCharacteristic(bleUartChara);
        if (s.length() > currentMTU) {
            Log.e("GAP Service", "Cannot send all the data");
        }
    }

    public void send(byte[] b) {
        Log.e("GAP Service", "Sending : " + Arrays.toString(b) + "\n");
        bleUartChara.setValue(b);
        deviceGatt.writeCharacteristic(bleUartChara);
        if (b.length > currentMTU) {
            Log.e("GAP Service", "Cannot send all the data");
        }
    }

    private void connectDevice(BluetoothDevice device) {
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
                        break;
                    case BluetoothProfile.STATE_CONNECTING:
                        Log.i("GAPService", "Connecting");
                        break;
                    case BluetoothProfile.STATE_DISCONNECTED:
                        Log.i("GAPService", "Disconnected");
                        Intent disconnectedIntent = new Intent(DEVICE_DISCONNECTED_ACTION);
                        LocalBroadcastManager.getInstance(GAPService.this).sendBroadcast(disconnectedIntent);
                        gatt.close();
                        break;
                    default:
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
                    if (service.getUuid().equals(UART_SERVICE_UUID)) {
                        Log.i("GAPService", "Found UART service");
                        bleUartChara = service.getCharacteristic(UART_CHARACTERISTIC_UUID);
                        deviceGatt.setCharacteristicNotification(bleUartChara, true);
                        BluetoothGattDescriptor descriptor = bleUartChara.getDescriptor(UART_DESCRIPTOR_UUID);
                        bleUartChara.setWriteType(BluetoothGattCharacteristic.WRITE_TYPE_NO_RESPONSE);
                        descriptor.setValue(BluetoothGattDescriptor.ENABLE_NOTIFICATION_VALUE);
                        deviceGatt.writeDescriptor(descriptor);
                        Log.i("GAPService", "UART characteristic is " + bleUartChara.getUuid());
                        Log.i("GAPService", "Chara descriptors :" + bleUartChara.getDescriptors().get(0).getUuid());
                    }
                }
            }

            @Override
            // Negociate MTU after notifications
            public void onDescriptorWrite(BluetoothGatt gatt,
                                          BluetoothGattDescriptor descriptor,
                                          int status) {
                deviceGatt.requestMtu(MAX_MTU_NRF);
                Log.i("GAPService", "MTU of 251 requested");

            }

            @Override
            // Result of a characteristic read operation
            public void onCharacteristicRead(BluetoothGatt gatt,
                                             BluetoothGattCharacteristic chara,
                                             int status) {
                Log.i("GAPService", "Characteristic Read " + chara.getService().getUuid() + ":" + chara.getUuid() + "=" + parseByteArray(chara.getValue()));
            }

            @Override
            public void onCharacteristicChanged(BluetoothGatt gatt, BluetoothGattCharacteristic characteristic) {
                super.onCharacteristicChanged(gatt, characteristic);
                Log.i("GAPService", "Characteristic changed: " + characteristic.getUuid() + " = " + Arrays.toString(characteristic.getValue()));
                Intent intent = new Intent(DEVICE_NOTIFICATION_ACTION);
                intent.putExtra("Message", characteristic.getValue());
                LocalBroadcastManager.getInstance(GAPService.this).sendBroadcast(intent);
            }

            @Override
            public void onMtuChanged(BluetoothGatt gatt, int mtu, int status) {
                super.onMtuChanged(gatt, mtu, status);
                currentMTU = mtu;

                Intent intent = new Intent(DEVICE_CONNECTED_ACTION);
                LocalBroadcastManager.getInstance(GAPService.this).sendBroadcast(intent);
            }

            @Override
            public void onReadRemoteRssi(BluetoothGatt gatt, int rssi, int status) {
                super.onReadRemoteRssi(gatt, rssi, status);
                Log.i("GAPService", "Connection RSSI: " + rssi);
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

    public void launchRSSITask() {
        deviceGatt.readRemoteRssi();
    }

    @Override
    public void onDestroy() {
        stopScan();
        Log.i("GAPService", "Service destroyed");
        deviceGatt.close();
        super.onDestroy();
    }

    @Override
    public int onStartCommand(Intent intent, int flags, int startId) {
        Log.i("GAPService", "Service started. Received start id " + startId + ": " + intent);
        return START_STICKY;    // Remain in the background when the app is paused. Has to be explicitly stopped
    }

    @Override
    public IBinder onBind(Intent intent) {
        appId = intent.getByteArrayExtra(AdvertisingActivity.APP_ID_EXTRA);

        return mBinder;
    }

    public class LocalBinder extends Binder {
        // Return this instance of LocalService so clients can call public methods
        GAPService getService() {
            return GAPService.this;
        }
    }
}
