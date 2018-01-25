package fr.telecom_paristech.wifiparrot;

import android.util.Log;

import java.io.BufferedOutputStream;
import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.net.InetAddress;
import java.net.InetSocketAddress;
import java.net.Socket;
import java.net.SocketException;
import java.net.SocketTimeoutException;

/* Inspired by https://stackoverflow.com/questions/38162775/really-simple-tcp-client
 */

public class TcpClient
{
    private static final String SERVER_IP = "10.10.10.1"; //server IP address
    private static final int SERVER_PORT = 3000;
    private static final int TIMEOUT = 4000;
    private static final int BUFF_LEN_DOWNLOAD = 1024;
    // used to send messages
    private BufferedOutputStream mBufferOut;
    // used to read messages from the server
    private BufferedReader mBufferIn;
    private Socket socket;

    public TcpClient()
    {}

    public void send(byte[] message, int len) {
        try {
            if (mBufferOut != null) {
                mBufferOut.write(message, 0, len);
                mBufferOut.flush();
            } else
                Log.e("Tcp send", "mBufferOut == null OR mBufferOut.checkError()");
            Log.i("Tcp send", "End of sending");
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public void send(String message) {
        try {
            if (mBufferOut != null) {
                mBufferOut.write(message.getBytes());
                mBufferOut.flush();
            } else
                Log.e("Tcp send", "mBufferOut == null OR mBufferOut.checkError()");
            Log.i("Tcp send", "End of sending");
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public String receive_line(int timeout)
    {
        try {
            socket.setSoTimeout(timeout);
            return receive_line();
        } catch (SocketException e) {
            e.printStackTrace();
        }
        return null;
    }

    public String receive_line()
    {
        try {
            return mBufferIn.readLine();
        } catch(SocketTimeoutException e) {
            Log.i("TCP receive", "Timeout");
        } catch (SocketException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        }
        return null;
    }

    public String receive()
    {
        try {
            char[] buffer = new char[BUFF_LEN_DOWNLOAD];
            int nb_char = mBufferIn.read(buffer, 0, BUFF_LEN_DOWNLOAD);
            if(nb_char == -1) {
                Log.i("Download buffer", "EOF reached");
                return null;
            }
            return String.valueOf(buffer).replace("\0", "");
        } catch(SocketTimeoutException e) {
            Log.i("TCP receive", "Timeout");
        } catch (IOException e) {
            e.printStackTrace();
        }
        return null;
    }

    public int openSocket()
    {
        try {
            //here you must put your computer's IP address.
            InetAddress serverAddr = InetAddress.getByName(SERVER_IP);
            Log.i("TCP Client", "C: Connecting...");
            //create a socket to make the connection with the server
            socket = new Socket();
            socket.connect(new InetSocketAddress(serverAddr, SERVER_PORT), 1000);
            socket.setSoTimeout(TIMEOUT);
            try {
                //sends the message to the server
                mBufferOut = new BufferedOutputStream(socket.getOutputStream());
                //receives the message which the server sends back
                mBufferIn = new BufferedReader(new InputStreamReader(socket.getInputStream()));
            } catch (Exception e) {
                return 1;
            }
        } catch (SocketTimeoutException e) {
            return 2;
        } catch (Exception e) {
            return 1;
        }
        return 0;
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
        try {
            if (mBufferOut != null) {
                mBufferOut.flush();
                mBufferOut.close();
            }
            mBufferIn = null;
            mBufferOut = null;
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}
