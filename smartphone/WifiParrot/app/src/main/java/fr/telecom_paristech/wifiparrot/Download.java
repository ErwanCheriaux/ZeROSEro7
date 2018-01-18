package fr.telecom_paristech.wifiparrot;

import android.util.Log;

/* Inspired by https://stackoverflow.com/questions/38162775/really-simple-tcp-client
 */

public class Download extends Transfer
{
    protected Integer doInBackground(String... input)
    {
        //we create a TCPClient object
        mTcpClient = new TcpClient(new TcpClient.OnMessageReceived() {
            @Override
            //here the messageReceived method is implemented
            public void messageReceived(String message) {
                //this method calls the onProgressUpdate
                Log.i("Tcp receive",  message);
            }
        });
        mTcpClient.run();
        return 0;
    }
}
