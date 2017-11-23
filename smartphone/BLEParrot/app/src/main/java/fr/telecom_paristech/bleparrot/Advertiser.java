package fr.telecom_paristech.bleparrot;

import android.app.Service;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.le.AdvertiseCallback;
import android.bluetooth.le.AdvertiseData;
import android.bluetooth.le.AdvertiseSettings;
import android.bluetooth.le.BluetoothLeAdvertiser;
import android.content.Intent;
import android.content.Context;
import android.os.IBinder;
import android.util.Log;
import android.widget.Toast;

public class Advertiser extends Service {

    private BluetoothAdapter adapter ;
    private BluetoothLeAdvertiser advertiserInstance ;

    private AdvertiseSettings settings ;
    private AdvertiseData advertiseData ;
    private AdvertiseCallback callback ;

    public Advertiser() {
        adapter = BluetoothAdapter.getDefaultAdapter() ;
        advertiserInstance = adapter.getBluetoothLeAdvertiser() ;
        Log.i("Advertiser","Advertiser Instance : " + advertiserInstance) ;

        settings = buildSettings() ;
        advertiseData = buildData() ;
        callback = new AdvertiserCallbackFeedback(this.getBaseContext()) ;

        advertiserInstance.startAdvertising (settings,
                advertiseData,
                callback
        ) ;
    }

    private AdvertiseSettings buildSettings() {
        AdvertiseSettings.Builder settingsBuilder = new AdvertiseSettings.Builder() ;

        // Most consuming for best performances. The phone as great battery and advertisement won't last long.
        settingsBuilder.setAdvertiseMode(AdvertiseSettings.ADVERTISE_MODE_LOW_LATENCY) ;
        settingsBuilder.setTxPowerLevel(AdvertiseSettings.ADVERTISE_TX_POWER_HIGH) ;
        settingsBuilder.setConnectable(false) ; // TODO Become connectable
        settingsBuilder.setTimeout(0) ;

        return settingsBuilder.build() ;
    }

    private AdvertiseData buildData() {
        AdvertiseData.Builder dataBuilder = new AdvertiseData.Builder() ;

        dataBuilder.setIncludeDeviceName(true) ;
        /*  TODO Add recognizable Code through UUID or Manufacturer or ...
            Then enable some king of secured authentication.
        */

        return dataBuilder.build() ;
    }

    private class AdvertiserCallbackFeedback extends AdvertiseCallback {

        private Context context ; // Context of the application to make Toasts

        AdvertiserCallbackFeedback(Context context) {
            this.context = context ;
        }

        @Override
        public void onStartSuccess(AdvertiseSettings settingsInEffect) {
            super.onStartSuccess(settingsInEffect);
            Toast.makeText(context, "Now advertising through BLE", Toast.LENGTH_LONG) ;
        }

        @Override
        public void onStartFailure(int errorCode) {
            super.onStartFailure(errorCode);
            Toast.makeText(context, "Failed to start BLE advertising", Toast.LENGTH_LONG) ;
        }
    }

    @Override
    public IBinder onBind(Intent intent) {
        // No data exchanged with this service.
        return null ;
    }
}
