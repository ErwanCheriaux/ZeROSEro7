package fr.telecom_paristech.wifiparrot;

public class Upload extends Transfer
{
    protected Integer doInBackground(String... input)
    {
        mTcpClient.run();
        return 0;
    }
}
