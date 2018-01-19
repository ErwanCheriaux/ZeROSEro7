package fr.telecom_paristech.wifiparrot;

import android.os.AsyncTask;

public abstract class Transfer extends AsyncTask<String, Void, Integer>
{
    protected static final int    BUFF_LEN_UPLOAD = 512;
    protected static final String START_SEQ = "[007]";

    protected TcpClient mTcpClient = new TcpClient();

    protected abstract Integer doInBackground(String... input);
}
