package fr.telecom_paristech.wifiparrot;

/* Inspired by https://stackoverflow.com/questions/38162775/really-simple-tcp-client
 */

import android.util.Log;

public class Download extends Transfer
{
    protected Integer doInBackground(String... input)
    {
        mTcpClient.openSocket();
        mTcpClient.send(START_SEQ + "L\n");
        String response = mTcpClient.receive();
        Log.i("Download list", response);
        mTcpClient.send(START_SEQ + "DFile_wanted.txt\n");
        mTcpClient.closeSocket();
        return 0;
    }
}
