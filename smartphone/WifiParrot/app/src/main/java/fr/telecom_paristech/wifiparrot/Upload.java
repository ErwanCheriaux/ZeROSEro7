package fr.telecom_paristech.wifiparrot;

import android.util.Log;
import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;

public class Upload extends Transfer
{
    private void uploadRequests(FileInputStream fileInputStream, String filename)
    {
        byte[] data = new byte[MAX_DATA_LEN_HTTP];
        int dataLen;
        // POST request to create a file
        String[] args = {
                "Host", "setup.com",
                "Connection", "keep-alive",
                "Accept", "application/json",
                "Origin", "http://setup.com",
                "Content-Type", "application/json",
                "Referer", "http://setup.com/",
                "Accept-Encoding", "gzip, deflate"
        };
        int count = 0;
        while(true) {
            try {
                dataLen = fileInputStream.read(data, 0, MAX_DATA_LEN_HTTP);
            } catch (IOException e) {
                e.printStackTrace();
                return;
            }
            if(dataLen == -1)
                break;
            // Create a new file
            String msg = "{\"flags\":0,\"command\":\"fcr -o \\\"" + filename + "_" + count + "\\\" " + dataLen + "\"}";
            Log.d("Request", msg);
            Log.d("Response code", "" + POST("http://setup.com/command", args, msg));
            // POST request to fill the created file
            msg = "{\"command\":\"write 0 " + dataLen + "\",\"flags\":4,\"data\":\"" + encode(data, dataLen) + "\"}";
            Log.i("Request", msg);
            Log.d("Response code", "" + POST("http://setup.com/command", args, msg));
            count++;
        }
    }

    protected Integer doInBackground(String... input)
    {
        String selectedFilePath = input[0];
        int serverResponseCode = 0;

        // Open file
        File selectedFile = new File(selectedFilePath);
        String[] parts = selectedFilePath.split("/");
        if (!selectedFile.isFile()){
            Log.i("Data", "Source File Doesn't Exist: " + selectedFilePath);
            return 1;
        }
        FileInputStream fileInputStream;
        try{
            fileInputStream = new FileInputStream(selectedFile);
        } catch (Exception e) {
            e.printStackTrace();
            return 1;
        }
        Log.d("Data", "Source File Found: " + selectedFilePath);

        uploadRequests(fileInputStream, parts[parts.length - 1]);

        return serverResponseCode;
    }
}
