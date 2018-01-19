package fr.telecom_paristech.wifiparrot;

import android.app.Service;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.net.wifi.WifiManager;
import android.os.Binder;
import android.os.IBinder;
import android.util.Log;
import android.widget.TextView;

public class Wifi extends Service
{
    private WifiManager wifiManager;
    private final IBinder mBinder = new LocalBinder();
    private WifiScanReceiver wifiReceiver;

    public Wifi()
    {}

    @Override
    public void onCreate()
    {
        wifiManager = (WifiManager) getApplicationContext().getSystemService(Context.WIFI_SERVICE);
        wifiManager.setWifiEnabled(true);
        wifiReceiver = new WifiScanReceiver(wifiManager);
        registerReceiver(wifiReceiver, new IntentFilter(WifiManager.SCAN_RESULTS_AVAILABLE_ACTION));
    }


    public class LocalBinder extends Binder
    {
        Wifi getService() {
            // Return this instance of Wifi so clients can call public methods
            return Wifi.this;
        }
    }

    public void scan()
    {
        if(!wifiManager.startScan())
            Log.println(Log.ERROR, "Scan", "startScan Error");
        wifiReceiver.scan_started = true;
        Log.println(Log.INFO, "Scan", "Start");
    }

    public IBinder onBind(Intent intent)
    {
        return mBinder;
    }

    public void uploadFile(String filename)
    {
        new Upload().execute(filename);
    }

    public void downloadFile(String filename)
    {
        new Download().execute(filename);
    }

    public void getFiles(TextView response)
    {
        new Scan(response).execute();
    }
}
