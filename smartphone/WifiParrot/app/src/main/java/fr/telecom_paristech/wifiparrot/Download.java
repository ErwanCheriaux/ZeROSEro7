package fr.telecom_paristech.wifiparrot;

import android.os.Environment;
import android.util.Log;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;

public class Download extends Transfer
{
    protected Integer doInBackground(String... input)
    {
        String filename = input[0];
        mTcpClient.openSocket();
        String path = Environment.getExternalStoragePublicDirectory(Environment.DIRECTORY_DOWNLOADS) + File.separator;
        File file = new File(path, filename);
        try {
            file.createNewFile();
            if(file.exists()) {
                FileOutputStream fo = new FileOutputStream(file);
                String response;
                mTcpClient.send(START_SEQ + "D" + filename + "\n");
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
            }
        } catch (IOException e) {
            e.printStackTrace();
            return 1;
        }
        mTcpClient.closeSocket();
        return 0;
    }
}
