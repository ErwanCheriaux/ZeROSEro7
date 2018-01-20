package fr.telecom_paristech.wifiparrot;

import android.content.Context;
import android.util.Log;
import android.widget.Toast;

public class Scan extends Transfer
{
    CallbackInterface callback = null;
    String[] filenames = null;

    Scan(Context context, CallbackInterface callback)
    {
        super(context);
        this.callback = callback;
    }

    @Override
    protected Integer doInBackground(String... input)
    {
        int conn_res = mTcpClient.openSocket();
        if(conn_res != 0)
            return conn_res;
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
    protected void onPostExecute(Integer result)
    {
        callback.removeDownloadButtons();
        switch (result) {
            case 0: Toast.makeText(context, "Scan finished", Toast.LENGTH_SHORT).show(); break;
            case 2: Toast.makeText(context, "Connection timeout", Toast.LENGTH_SHORT).show(); return;
            default: Toast.makeText(context, "Scan error", Toast.LENGTH_SHORT).show(); return;
        }
        if(result != 0) {
            Toast.makeText(context, "Scan error", Toast.LENGTH_SHORT).show();
            return;
        }
        if(filenames == null) {
            Toast.makeText(context, "No file available", Toast.LENGTH_SHORT).show();
            return;
        }
        for(int i = 0; i < filenames.length; i++)
            callback.addDownloadButton(filenames[i]);
    }
}
