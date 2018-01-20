package fr.telecom_paristech.wifiparrot;

import android.content.Context;
import android.util.Log;
import android.widget.Toast;

public class Remove extends Transfer
{
    String filename = null;
    CallbackInterface callback = null;

    Remove(Context context, String filename, CallbackInterface callback)
    {
        super(context);
        this.filename = filename;
        this.callback = callback;
    }

    @Override
    protected Integer doInBackground(String... input)
    {
        int conn_res = mTcpClient.openSocket();
        if(conn_res != 0)
            return conn_res;
        mTcpClient.send(START_SEQ + "R" + filename + "\n");
        mTcpClient.closeSocket();
        Log.i("File removed", filename);
        return 0;
    }

    @Override
    protected void onPostExecute(Integer result) {
        switch (result) {
            case 0: Toast.makeText(context, filename + " was removed", Toast.LENGTH_SHORT).show(); break;
            case 2: Toast.makeText(context, "Connection timeout", Toast.LENGTH_SHORT).show(); return;
            default: Toast.makeText(context, "Remove error", Toast.LENGTH_SHORT).show(); return;
        }
        callback.removeDownloadButton(filename);

    }
}
