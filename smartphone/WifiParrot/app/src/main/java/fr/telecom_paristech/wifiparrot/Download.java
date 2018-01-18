package fr.telecom_paristech.wifiparrot;

import android.os.SystemClock;
import android.util.Log;

/* Inspired by https://stackoverflow.com/questions/38162775/really-simple-tcp-client
 */

public class Download extends Transfer
{
    protected Integer doInBackground(String... input)
    {
        mTcpClient.openSocket();
        while(true) {
            String msg = mTcpClient.receive();
            Log.i("TCP receive", msg);
            SystemClock.sleep(100);
        }
    }
}
