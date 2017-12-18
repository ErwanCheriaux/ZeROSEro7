package fr.telecom_paristech.wifiparrot;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.net.wifi.ScanResult;
import android.net.wifi.WifiManager;
import android.util.Log;

import java.util.List;

public class WifiScanReceiver extends BroadcastReceiver
{
    private WifiManager wifiManager;

    WifiScanReceiver(WifiManager wifiManager)
    {
        super();
        this.wifiManager = wifiManager;
    }

    @Override
    public void onReceive(Context context, Intent intent)
    {
        List<ScanResult> scanResultList = wifiManager.getScanResults();
        Log.println(Log.DEBUG, "Scan","Finished: " + scanResultList.size());
        for (int i = 0; i < scanResultList.size(); i++)
            Log.println(Log.DEBUG, "scanResult[" + i + "]","" + scanResultList.get(i));
    }
}
