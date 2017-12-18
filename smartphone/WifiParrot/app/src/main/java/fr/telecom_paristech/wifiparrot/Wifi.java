package fr.telecom_paristech.wifiparrot;

import android.app.Service;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.net.wifi.WifiConfiguration;
import android.net.wifi.WifiInfo;
import android.net.wifi.WifiManager;
import android.os.Binder;
import android.os.IBinder;
import android.util.Log;

import java.util.List;

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
        wifiManager = (WifiManager) getSystemService(Context.WIFI_SERVICE);
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
        setWiFiStatus();
        if(!wifiManager.startScan())
            Log.println(Log.ERROR, "Scan", "startScan Error");
        Log.println(Log.INFO, "Scan", "Start");
    }

    public IBinder onBind(Intent intent)
    {
        return mBinder;
    }

    private void setWiFiStatus()
    {
        String textStatus = "";
        switch(wifiManager.getWifiState()){
            case WifiManager.WIFI_STATE_ENABLED:
                Log.d("State", "WIFI_STATE_ENABLED");
                break;
            case WifiManager.WIFI_STATE_DISABLED:
                Log.d("State", "WIFI_STATE_DISABLED");
                break;
            case WifiManager.WIFI_STATE_DISABLING:
                Log.d("State", "WIFI_STATE_DISABLING");
                break;
            case WifiManager.WIFI_STATE_ENABLING:
                Log.d("State", "WIFI_STATE_ENABLING");
                break;
            case WifiManager.WIFI_STATE_UNKNOWN:
                Log.d("State", "WIFI_STATE_UNKNOWN");
                break;
        }
        // Get WiFi status
        WifiInfo info = wifiManager.getConnectionInfo();
        textStatus += "\n\nWiFi Status: " + info.toString();
        // List available networks
        List<WifiConfiguration> configs = wifiManager.getConfiguredNetworks();
        for (WifiConfiguration config : configs)
            textStatus += "\n\n" + config.toString();
        Log.d("status", textStatus);
    }
}
