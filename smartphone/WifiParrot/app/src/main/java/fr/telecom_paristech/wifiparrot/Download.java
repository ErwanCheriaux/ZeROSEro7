package fr.telecom_paristech.wifiparrot;

import android.util.Log;

public class Download extends Transfer
{
    protected Integer doInBackground(String... input)
    {
        String filename = input[0];
        mTcpClient.openSocket();
        mTcpClient.send(START_SEQ + "D" + filename + "\n");
        mTcpClient.closeSocket();
        return 0;
    }
}
