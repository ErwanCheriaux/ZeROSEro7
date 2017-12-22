package fr.telecom_paristech.wifiparrot;

/**************
** Code From https://www.coderefer.com/android-upload-file-to-server/ **
***************/

import android.os.AsyncTask;
import android.util.Log;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.DataOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.io.PrintWriter;
import java.net.ConnectException;
import java.net.HttpURLConnection;
import java.net.InetAddress;
import java.net.Socket;
import java.net.URL;

public class DataTransfers extends AsyncTask<String, Void, Integer>
{
    private PrintWriter tcpSendBuffer;
    private boolean tcpRun = false;
    private BufferedReader tcpReceiveBuffer;
    private String tcpServerMessage;
    private OnMessageReceived tcpMessageListener = null;

    public interface OnMessageReceived {
        public void messageReceived(String message);
    }

    public void TCPStart()
    {
        try {
            InetAddress ip = InetAddress.getByName("10.10.10.1");
            Log.i("TCP Client", "C: Connecting...");
            Socket socket = new Socket(ip, 20);
            try {
                //sends the message to the server
                tcpSendBuffer = new PrintWriter(socket.getOutputStream());
                //receives the message which the server sends back
                tcpReceiveBuffer = new BufferedReader(new InputStreamReader(socket.getInputStream()));
                int charsRead;
                char[] buffer = new char[1024];
                //in this while the client listens for the messages sent by the server
                while (tcpRun) {
                    charsRead = tcpReceiveBuffer.read(buffer);
                    tcpServerMessage = new String(buffer).substring(0, charsRead);
                    if (tcpServerMessage != null && tcpMessageListener != null)
                        //call the method messageReceived from MyActivity class
                        tcpMessageListener.messageReceived(tcpServerMessage);
                    tcpServerMessage = null;
                }
                Log.i("RESPONSE FROM SERVER", "S: Received Message: '" + tcpServerMessage + "'");
            } catch (Exception e) {
                Log.e("TCP", "S: Error", e);
            } finally {
                //the socket must be closed. It is not possible to reconnect to this socket
                // after it is closed, which means a new socket instance has to be created.
                socket.close();
            }
        } catch (Exception e) {
            Log.e("TCP", "C: Error", e);
        }
    }

    /* Close the TCP connection and release the members
     */
    public void TCPStop()
    {
        tcpRun = false;
        if (tcpSendBuffer != null) {
            tcpSendBuffer.flush();
            tcpSendBuffer.close();
        }
        tcpMessageListener = null;
        tcpReceiveBuffer = null;
        tcpSendBuffer = null;
        tcpServerMessage = null;
    }

    /* Send a TCP request to a url and return server response
    ** msg:    content to send
    ** return: server response
     */
    private String TCPRequest(String msg)
    {
        if (tcpSendBuffer != null && !tcpSendBuffer.checkError()) {
            tcpSendBuffer.println(msg);
            tcpSendBuffer.flush();
            Log.i("TCP", "Message sent.");
        }
        return "";
    }

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

    /* Do not handle long files (> 41kB)
     */
    private String encode(String in)
    {
        int[] a = new int[in.length()];
        for(int i = 0; i < in.length(); i++)
            a[i] = in.charAt(i);
        int b, c, d, e, f;
        String g = "";
        String h = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
        int j = a.length;
        int k = j % 3;
        int l = j - k;
        for (int m = 0; l > m; m += 3) {
            f = a[m] << 16 | a[m + 1] << 8 | a[m + 2];
            b = (16515072 & f) >> 18;
            c = (258048 & f) >> 12;
            d = (4032 & f) >> 6;
            e = 63 & f;
            g += "" + h.charAt(b) + h.charAt(c) + h.charAt(d) + h.charAt(e);
        }
        if(1 == k) {
            f = a[l];
            b = (252 & f) >> 2;
            c = (3 & f) << 4;
            g += "" + h.charAt(b) + h.charAt(c) + "==";
            return g;
        }
        f = a[l] << 8 | a[l + 1];
        b = (64512 & f) >> 10;
        c = (1008 & f) >> 4;
        d = (15 & f) << 2;
        g += "" + h.charAt(b) + h.charAt(c) + h.charAt(d) + "=";
        return g;
    }

    private void uploadRequests(String data)
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
        String msg = "{\"flags\":0,\"command\":\"fcr -o \\\"test.txt\\\" " + data.length() + "\"}";
        Log.d("Request", msg);
        Log.d("Response code", "" + POST("http://setup.com/command", args, msg));
        // POST request to fill the created file
        msg = "{\"command\":\"write 0 " + data.length() + "\",\"flags\":4,\"data\":\"" + encode(data) + "\"}";
        Log.d("Request", msg);
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

        TCPStart();
        try {
            InputStreamReader isr = new InputStreamReader(fileInputStream, "UTF-8");
            BufferedReader bufferedReader = new BufferedReader(isr);
            String line;
            String data = "";
            while((line = bufferedReader.readLine()) != null) {
                data = data.concat(line).concat("\n");
            }
            String msg = "fcr \"test.txt\" 10\n" + data + "\r\n";
            Log.i("Data", msg);
            TCPRequest(msg);
            //uploadRequests(data);
        } catch (IOException e) {
            e.printStackTrace();
        }
        TCPStop();
        return serverResponseCode;
    }
}
