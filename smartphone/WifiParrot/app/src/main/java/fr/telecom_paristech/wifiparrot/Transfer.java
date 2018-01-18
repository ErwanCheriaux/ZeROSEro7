package fr.telecom_paristech.wifiparrot;

import android.os.AsyncTask;

public abstract class Transfer extends AsyncTask<String, Void, Integer>
{
    protected TcpClient mTcpClient;

    protected abstract Integer doInBackground(String... input);
}
