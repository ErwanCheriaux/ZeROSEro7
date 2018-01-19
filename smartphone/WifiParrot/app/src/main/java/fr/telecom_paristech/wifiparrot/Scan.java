package fr.telecom_paristech.wifiparrot;

import android.util.Log;
import android.widget.TextView;

public class Scan extends Transfer
{
    TextView response = null;
    String[] filenames = null;

    Scan(TextView response)
    {
        this.response = response;
    }

    @Override
    protected Integer doInBackground(String... input)
    {
        mTcpClient.openSocket();
        mTcpClient.send(START_SEQ + "L\n");
        String response = mTcpClient.receive();
        if(response != null) {
            filenames = response.split(" ");
            Log.i("Download", "Download list");
            for(int i = 0; i < filenames.length; i++)
                Log.i("Download", i + filenames[i]);
        }
        mTcpClient.closeSocket();
        if(response == null)
            return 1;
        return 0;
    }

    @Override
    protected void onPostExecute(Integer result) {
        StringBuffer text = new StringBuffer("");
        for(int i = 0; i < filenames.length; i++)
            text.append(filenames[i] + " ");
        response.setText(text.toString());
    }
}
