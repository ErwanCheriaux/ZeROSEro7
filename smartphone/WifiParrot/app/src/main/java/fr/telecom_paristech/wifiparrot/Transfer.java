package fr.telecom_paristech.wifiparrot;

import android.os.AsyncTask;

public abstract class Transfer extends AsyncTask<String, Void, Integer>
{
    protected String START_SEQ = "[007]";

    protected TcpClient mTcpClient = new TcpClient();

    protected abstract Integer doInBackground(String... input);
}
