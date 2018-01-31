package fr.telecom_paristech.wifiparrot;

import android.app.Service;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.net.wifi.WifiManager;
import android.os.Binder;
import android.os.IBinder;
import android.widget.ProgressBar;

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

    public IBinder onBind(Intent intent)
    {
        return mBinder;
    }

    public void uploadFile(Context context, String filename, ProgressBar progress)
    {
        new Upload(context, progress).execute(filename);
    }

    public void downloadFile(Context context, String filename, ProgressBar progress)
    {
        new Download(context, progress).execute(filename);
    }

    public void getFiles(Context context, CallbackInterface callback)
    {
        new Scan(context, callback).execute();
    }

    public void removeFile(Context context, String filename, CallbackInterface callback)
    {
        new Remove(context, filename, callback).execute();
    }
}
