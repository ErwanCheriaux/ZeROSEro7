package fr.telecom_paristech.wifiparrot;

import android.util.Log;

public class Remove extends Transfer
{
    String filename = null;
    CallbackInterface callback = null;

    Remove(String filename, CallbackInterface callback)
    {
        this.filename = filename;
        this.callback = callback;
    }

    @Override
    protected Integer doInBackground(String... input)
    {
        mTcpClient.openSocket();
        mTcpClient.send(START_SEQ + "R" + filename + "\n");
        mTcpClient.closeSocket();
        Log.i("File removed", filename);
        return 0;
    }

    @Override
    protected void onPostExecute(Integer result) {
        callback.removeDownloadButton(filename);
    }
}
