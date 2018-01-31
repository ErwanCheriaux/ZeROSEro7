package fr.telecom_paristech.wifiparrot;

import android.content.Context;
import android.util.Log;
import android.widget.Toast;

import java.util.ArrayList;

public class Scan extends Transfer
{
    CallbackInterface callback = null;
    ArrayList<String> filenames = null;

    Scan(Context context, CallbackInterface callback)
    {
        super(context);
        this.callback = callback;
        filenames = new ArrayList<>();
    }

    @Override
    protected Integer doInBackground(String... input)
    {
        int conn_res = mTcpClient.openSocket();
        if(conn_res != 0)
            return conn_res;
        mTcpClient.send(START_SEQ + "L\n");
        String data;
        Log.i("Download", "Download list");
        while((data = mTcpClient.receive_line()) != null) {
            String split[] = data.split(" ");
            if(split.length != 2)
                break;
            filenames.add(split[0] + " (" + split[1] + "B)");
            Log.i("Download", filenames.get(filenames.size() - 1));
        }
        mTcpClient.closeSocket();
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
        for(int i = 0; i < filenames.size(); i++)
            callback.addDownloadButton(filenames.get(i));
    }
}
