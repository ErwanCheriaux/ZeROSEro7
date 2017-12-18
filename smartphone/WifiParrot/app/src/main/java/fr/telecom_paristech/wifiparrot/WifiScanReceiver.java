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
        if(!intent.getAction().equals(WifiManager.SCAN_RESULTS_AVAILABLE_ACTION)) {
            Log.println(Log.DEBUG, "Scan","Action Error");
            return;
        }
        Log.println(Log.DEBUG, "Scan","Finished");
        List<ScanResult> scanResultList = wifiManager.getScanResults();
        Log.println(Log.DEBUG, "Scan","" + scanResultList.size());
        for (int i = 0; i < scanResultList.size(); i++) {
            //Log.println(Log.DEBUG, "scanResult[" + i + "]","" + scanResultList.get(i));
        }
    }
}
