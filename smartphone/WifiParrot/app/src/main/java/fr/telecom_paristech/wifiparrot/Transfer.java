package fr.telecom_paristech.wifiparrot;

import android.content.Context;
import android.os.AsyncTask;

public abstract class Transfer extends AsyncTask<String, Void, Integer>
{
    protected static final int    BUFF_LEN = 1024;
    protected static final String START_SEQ = "[007]";
    protected abstract Integer doInBackground(String... input);

    protected Context context = null;

    Transfer(Context context)
    {
        this.context = context;
    }

    protected TcpClient mTcpClient = new TcpClient();
}
