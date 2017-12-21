package fr.telecom_paristech.wifiparrot;

/**************
** Code From https://www.coderefer.com/android-upload-file-to-server/ **
***************/

import android.os.AsyncTask;
import android.util.Log;

import java.io.DataOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.net.ConnectException;
import java.net.HttpURLConnection;
import java.net.URL;

public class DataTransfers extends AsyncTask<String, Void, Integer>
{
    /* Send a GET request to a url and return server response
    ** url:    url to connect
    ** return: server response
     */
    private String GET(String url)
    {
        String res = "";
        try {
            // Http config
            URL request_url = new URL(url);
            HttpURLConnection connection = (HttpURLConnection) request_url.openConnection();

            // Http connection
            InputStream in = connection.getInputStream();
            Log.d("Server", "Server found.");

            // Receive Data
            InputStreamReader isw = new InputStreamReader(in);
            int data = isw.read();
            while (data != -1) {
                char current = (char) data;
                data = isw.read();
                res += current;
            }
            connection.disconnect();
        } catch (ConnectException e){
            return "Server not found.\nPlease verify server is running.";
        } catch (Exception e) {
            e.printStackTrace();
            return "Error";
        }
        return res;
    }

    /* Send a POST request to a url and return server response
    ** url:    url to connect
    ** header: header parameters
    ** msg:    content to send
    ** return: server response
     */
    private int POST(String url, String[] header, String msg)
    {
        try {
            // Http config
            URL request_url = new URL(url);
            HttpURLConnection connection = (HttpURLConnection) request_url.openConnection();
            connection.setDoInput(true);
            connection.setDoOutput(true);
            connection.setRequestMethod("POST");
            //String dosData = "";
            for(int i = 0; i < header.length; i += 2)
                connection.setRequestProperty(header[i], header[i + 1]);

            // Http connection
            Log.d("Server", "Server found.");
            DataOutputStream dos = new DataOutputStream(connection.getOutputStream());
            dos.writeBytes(msg);
            dos.flush();
            dos.close();
            connection.connect();

            // Response code
            int response = connection.getResponseCode();
            connection.disconnect();
            return response;
        } catch (ConnectException e){
            Log.i("Error", "Server not found.\nPlease verify server is running.");
        } catch (Exception e) {
            e.printStackTrace();
        }
        return 1;
    }

    private void uploadRequests()
    {
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
        String msg = "{\"flags\":0,\"command\":\"fcr -o \\\"test.txt\\\" 13\"}";
        Log.d("Response code", "" + POST("http://setup.com/command", args, msg));
        // POST request to fill the created file
        msg = "{\"command\":\"write 0 13\",\"flags\":4,\"data\":\"aGVsbG8gd29ybGQKCg==\"}";
        Log.d("Response code", "" + POST("http://setup.com/command", args, msg));
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

        uploadRequests();

        return serverResponseCode;
    }
}
