package fr.telecom_paristech.wifiparrot;

public class Upload extends Transfer
{
    protected Integer doInBackground(String... input)
    {
        String selectedFilePath = input[0];
        mTcpClient.openSocket();
        mTcpClient.send(START_SEQ + "U" + selectedFilePath + "\n");
        mTcpClient.closeSocket();
        return 0;
    }
}
