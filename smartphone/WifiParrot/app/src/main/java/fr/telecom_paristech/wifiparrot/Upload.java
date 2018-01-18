package fr.telecom_paristech.wifiparrot;

import android.os.SystemClock;

public class Upload extends Transfer
{
    protected Integer doInBackground(String... input)
    {
        mTcpClient.openSocket();
        while(true) {
            mTcpClient.send("Smartphone to STM, do you copy?");
            SystemClock.sleep(1000);
        }
    }
}
