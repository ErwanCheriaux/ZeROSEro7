package fr.telecom_paristech.wifiparrot;

import android.content.Context;
import android.os.Environment;
import android.util.Log;
import android.widget.Toast;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;

public class Download extends Transfer
{
    Download(Context context)
    {
        super(context);
    }

    @Override
    protected Integer doInBackground(String... input)
    {
        String filename = input[0];
        int conn_res = mTcpClient.openSocket();
        if(conn_res != 0)
            return conn_res;
        String path = Environment.getExternalStoragePublicDirectory(Environment.DIRECTORY_DOWNLOADS) + File.separator;
        File file = new File(path, filename);
        try {
            file.createNewFile();
            if(file.exists()) {
                FileOutputStream fo = new FileOutputStream(file);
                mTcpClient.send(START_SEQ + "D" + filename + "\n");
                // check server response
                String response = mTcpClient.receive_line();
                if(response == null) {
                    mTcpClient.closeSocket();
                    return 1;
                }
                Log.i("response", response);
                if(response.compareTo("No file") == 0) {
                    mTcpClient.closeSocket();
                    return 4;
                }
                if(response.compareTo("Success") != 0) {
                    mTcpClient.closeSocket();
                    return 1;
                }
                // get file content
                while(true) {
                    response = mTcpClient.receive();
                    if(response != null)
                        fo.write(response.getBytes());
                    else
                        break;
                }
                fo.close();
                Log.i("Download", "Success");
            } else {
                Log.e("Download", "Cannot create file");
                return 3;
            }
        } catch (IOException e) {
            e.printStackTrace();
            mTcpClient.closeSocket();
            return 1;
        }
        mTcpClient.closeSocket();
        return 0;
    }

    @Override
    protected void onPostExecute(Integer result) {
        switch (result) {
            case 0: Toast.makeText(context, "Download successful", Toast.LENGTH_SHORT).show(); break;
            case 2: Toast.makeText(context, "Connection timeout", Toast.LENGTH_SHORT).show(); break;
            case 3: Toast.makeText(context, "File cannot be created", Toast.LENGTH_SHORT).show(); break;
            case 4: Toast.makeText(context, "File do not exists\nPlease scan again", Toast.LENGTH_SHORT).show(); break;
            default: Toast.makeText(context, "Download error", Toast.LENGTH_SHORT).show();
        }
    }
}
