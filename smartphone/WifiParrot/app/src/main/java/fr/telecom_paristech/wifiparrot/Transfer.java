package fr.telecom_paristech.wifiparrot;

import android.os.AsyncTask;
import android.util.Log;

import java.io.DataOutputStream;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.net.ConnectException;
import java.net.HttpURLConnection;
import java.net.URL;

public abstract class Transfer extends AsyncTask<String, Void, Integer>
{
    protected final int MAX_DATA_LEN_HTTP = 2560;

    /* Send a GET request to a url and return server response
    ** url:    url to connect
    ** return: server response
     */
    protected String GET(String url)
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
    protected int POST(String url, String[] header, String msg)
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

    public static int byte2uint8(byte b)
    {
        return 0x00 << 24 | b & 0xff;
    }

    /* Do not handle long files (> 1kB)
     */
    protected String encode(byte[] data, int dataLen)
    {
        int b, c, d, e, f;
        String g = "";
        String h = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
        int k = dataLen % 3;
        int l = dataLen - k;
        for (int m = 0; l > m; m += 3) {
            f = byte2uint8(data[m]) << 16 | byte2uint8(data[m + 1]) << 8 | byte2uint8(data[m + 2]);
            b = (16515072 & f) >> 18;
            c = (258048 & f) >> 12;
            d = (4032 & f) >> 6;
            e = 63 & f;
            g += "" + h.charAt(b) + h.charAt(c) + h.charAt(d) + h.charAt(e);
        }
        if(1 == k) {
            f = byte2uint8(data[l]);
            b = (252 & f) >> 2;
            c = (3 & f) << 4;
            g += "" + h.charAt(b) + h.charAt(c) + "==";
            return g;
        }
        f = byte2uint8(data[l]) << 8 | byte2uint8(data[l + 1]);
        b = (64512 & f) >> 10;
        c = (1008 & f) >> 4;
        d = (15 & f) << 2;
        g += "" + h.charAt(b) + h.charAt(c) + h.charAt(d) + "=";
        return g;
    }

    protected abstract Integer doInBackground(String... input);
}
