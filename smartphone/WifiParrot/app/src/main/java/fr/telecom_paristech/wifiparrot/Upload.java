package fr.telecom_paristech.wifiparrot;

import android.content.Context;
import android.util.Log;
import android.widget.Toast;

import java.io.File;
import java.io.FileInputStream;

public class Upload extends Transfer
{
    Upload(Context context)
    {
        super(context);
    }

    @Override
    protected Integer doInBackground(String... input)
    {
        String selectedFilePath = input[0];
        String[] split = selectedFilePath.split("/");
        String filename = split[split.length - 1];

        // Open file
        File selectedFile = new File(selectedFilePath);
        if (!selectedFile.isFile()){
            Log.i("Data", "Source File Doesn't Exist: " + selectedFilePath);
            return 1;
        }
        FileInputStream fileInputStream;
        try{
            fileInputStream = new FileInputStream(selectedFile);
            byte[] data = new byte[BUFF_LEN];
            int dataLen;
            int conn_res = mTcpClient.openSocket();
            if(conn_res != 0)
                return conn_res;
            mTcpClient.send(START_SEQ + "U" + filename + "\n");
            while ((dataLen = fileInputStream.read(data)) != -1) {
                mTcpClient.send(data, dataLen);
                Thread.sleep(200);
            }
            fileInputStream.close();
            String response = mTcpClient.receive_line(0); // disable timeout
            if(response == null)
                return 1;
            if(response.compareTo("Success") != 0)
                return 1;
            mTcpClient.closeSocket();
        } catch (Exception e) {
            e.printStackTrace();
            return 1;
        }
        return 0;
    }

    @Override
    protected void onPostExecute(Integer result) {
        switch (result) {
            case 0: Toast.makeText(context, "Upload successful", Toast.LENGTH_SHORT).show(); break;
            case 2: Toast.makeText(context, "Connection timeout", Toast.LENGTH_SHORT).show(); break;
            default: Toast.makeText(context, "Upload error", Toast.LENGTH_SHORT).show();
        }
    }
}
