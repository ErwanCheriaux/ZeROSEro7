package fr.telecom_paristech.wifiparrot;

import android.os.SystemClock;
import android.util.Log;

public class Upload extends Transfer
{
    protected Integer doInBackground(String... input)
    {
        String message = "SMART2STM ok\r\n";
        //sends the message to the server
        if(mTcpClient != null) {
            while (true) {
                Log.i("Tcp", "Send: " + message);
                mTcpClient.sendMessage(message);
                SystemClock.sleep(1000);
            }
        }
        Log.e("Tcp", "mTcpClient == null");
        return 0;
    }
}
