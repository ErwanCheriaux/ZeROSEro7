package fr.telecom_paristech.wifiparrot;

import android.util.Log;

public class Scan extends Transfer
{
    CallbackInterface callback = null;
    String[] filenames = null;

    Scan(CallbackInterface callback)
    {
        this.callback = callback;
    }

    @Override
    protected Integer doInBackground(String... input)
    {
        mTcpClient.openSocket();
        mTcpClient.send(START_SEQ + "L\n");
        String response = mTcpClient.receive_line();
        if(response != null) {
            filenames = response.split(" ");
            Log.i("Download", "Download list");
            for(int i = 0; i < filenames.length; i++)
                Log.i("Download", filenames[i]);
        }
        mTcpClient.closeSocket();
        if(response == null)
            return 1;
        return 0;
    }

    @Override
    protected void onPostExecute(Integer result) {
        callback.removeDownloadButtons();
        if(filenames == null)
            return;
        for(int i = 0; i < filenames.length; i++)
            callback.addDownloadButton(filenames[i]);
    }
}
