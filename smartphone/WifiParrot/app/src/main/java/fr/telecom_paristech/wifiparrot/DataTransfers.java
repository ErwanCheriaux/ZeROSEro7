package fr.telecom_paristech.wifiparrot;

/**************
** Code From https://www.coderefer.com/android-upload-file-to-server/ **
***************/

import android.os.AsyncTask;
import android.util.Log;

import java.io.File;
import java.io.FileInputStream;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.net.ConnectException;
import java.net.HttpURLConnection;
import java.net.URL;

public class DataTransfers extends AsyncTask<String, Void, Integer>
{
    private String SERVER_URL = "http://setup.com/command/ls%20-v";

    protected Integer doInBackground(String... input)
    {
        String selectedFilePath = input[0];
        int serverResponseCode = 0;

        // Open file
        String lineEnd = "\r\n";
        String twoHyphens = "--";
        String boundary = "*****";
        int bytesRead,bytesAvailable,bufferSize;
        byte[] buffer;
        int maxBufferSize = 1 * 1024 * 1024;
        File selectedFile = new File(selectedFilePath);
        String[] parts = selectedFilePath.split("/");
        final String fileName = parts[parts.length-1];
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

        // Http
        try {
            // Http config
            URL url = new URL(SERVER_URL);
            HttpURLConnection connection = (HttpURLConnection) url.openConnection();

            // Http connection
            InputStream in = connection.getInputStream();
            Log.d("Server", "Server found.");

            // Receive Data
            InputStreamReader isw = new InputStreamReader(in);
            int data = isw.read();
            String res = "";
            while (data != -1) {
                char current = (char) data;
                data = isw.read();
                res += current;
            }
            Log.d("Data", res);

            //closing the input and output streams
            fileInputStream.close();

            // Http disconnection
            connection.disconnect();
        } catch (ConnectException e){
            Log.i("Server", "Server not found.\nPlease verify server is running.");
            return 1;
        } catch (Exception e) {
            e.printStackTrace();
            return 1;
        }

        return serverResponseCode;
    }
}
