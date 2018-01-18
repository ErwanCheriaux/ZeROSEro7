package fr.telecom_paristech.wifiparrot;

import android.os.AsyncTask;
import android.util.Log;

public abstract class Transfer extends AsyncTask<String, Void, Integer>
{
    protected TcpClient mTcpClient = new TcpClient();

    protected abstract Integer doInBackground(String... input);
}
