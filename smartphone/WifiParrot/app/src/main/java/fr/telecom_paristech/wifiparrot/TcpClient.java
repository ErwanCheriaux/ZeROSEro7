package fr.telecom_paristech.wifiparrot;

import android.util.Log;
import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.io.PrintWriter;
import java.net.InetAddress;
import java.net.Socket;
import java.net.SocketTimeoutException;

/* Inspired by https://stackoverflow.com/questions/38162775/really-simple-tcp-client
 */

public class TcpClient
{
    public static final String SERVER_IP = "10.10.10.1"; //server IP address
    public static final int SERVER_PORT = 3000;
    private static final int TIMEOUT = 4000;
    // used to send messages
    private PrintWriter mBufferOut;
    // used to read messages from the server
    private BufferedReader mBufferIn;
    private Socket socket;

    public TcpClient() {

    }

    public void send(String message) {
        if (mBufferOut != null && !mBufferOut.checkError()) {
            mBufferOut.println(message);
            mBufferOut.flush();
            Log.i("Tcp send", message);
        } else
            Log.e("Tcp send", "mBufferOut == null OR mBufferOut.checkError()");
    }

    public String receive() {
        try {
            return mBufferIn.readLine();
        } catch(SocketTimeoutException e) {
            Log.i("RCP receive", "Timeout");
        } catch (IOException e) {
            e.printStackTrace();
        }
        return null;
    }

    public void openSocket()
    {
        try {
            //here you must put your computer's IP address.
            InetAddress serverAddr = InetAddress.getByName(SERVER_IP);
            Log.i("TCP Client", "C: Connecting...");
            //create a socket to make the connection with the server
            socket = new Socket(serverAddr, SERVER_PORT);
            socket.setSoTimeout(TIMEOUT);
            try {
                //sends the message to the server
                mBufferOut = new PrintWriter(new BufferedWriter(new OutputStreamWriter(socket.getOutputStream())), true);
                //receives the message which the server sends back
                mBufferIn = new BufferedReader(new InputStreamReader(socket.getInputStream()));
            } catch (Exception e) {
                Log.e("TCP", "S: Error", e);
            }
        } catch (Exception e) {
            Log.e("TCP", "Error", e);
        }
    }

    public void closeSocket()
    {
        try {
            socket.close();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public void stopClient()
    {
        if (mBufferOut != null) {
            mBufferOut.flush();
            mBufferOut.close();
        }
        mBufferIn = null;
        mBufferOut = null;
    }
}
