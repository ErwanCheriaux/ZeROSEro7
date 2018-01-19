package fr.telecom_paristech.wifiparrot;

import android.util.Log;

import java.io.File;
import java.io.FileInputStream;

public class Upload extends Transfer
{
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
            byte[] data = new byte[BUFF_LEN_UPLOAD];
            int dataLen;
            mTcpClient.openSocket();
            mTcpClient.send(START_SEQ + "U" + filename + "\n");
            while(true) {
                dataLen = fileInputStream.read(data, 0, BUFF_LEN_UPLOAD);
                String dataStr = new String(data);
                Log.i("dataStr", dataStr);
                mTcpClient.send(dataStr);
                if (dataLen == -1) {
                    break;
                }
            }
            mTcpClient.closeSocket();
        } catch (Exception e) {
            e.printStackTrace();
            return 1;
        }
        return 0;
    }
}
