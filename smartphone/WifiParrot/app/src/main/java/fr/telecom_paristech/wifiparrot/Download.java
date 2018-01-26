package fr.telecom_paristech.wifiparrot;

import android.content.Context;
import android.os.Environment;
import android.util.Log;
import android.widget.ProgressBar;
import android.widget.Toast;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;

public class Download extends Transfer
{
    ProgressBar progress;

    Download(Context context, ProgressBar progress)
    {
        super(context);
        this.progress = progress;
    }

    @Override
    protected Integer doInBackground(String... input)
    {
        String[] split = input[0].split("[(B]");
        String filename = split[0];
        int totalDataLen = Integer.parseInt(split[1]);
        int conn_res = mTcpClient.openSocket();
        int dataSent = 0;
        if(conn_res != 0)
            return conn_res;
        String path = Environment.getExternalStoragePublicDirectory(Environment.DIRECTORY_DOWNLOADS) + File.separator;
        File file = new File(path, filename);
        try {
            file.createNewFile();
            if(file.exists()) {
                FileOutputStream fo = new FileOutputStream(file);
                mTcpClient.send(START_SEQ + "D" + filename + "\n");
                // get file content
                byte[] data = new byte[BUFF_LEN];
                while(true) {
                    int nb_chars = mTcpClient.receive(data);
                    if(data != null && nb_chars > 0)
                        fo.write(data, 0, nb_chars);
                    else
                        break;
                    dataSent += nb_chars;
                    publishProgress((int) ((dataSent / (float) totalDataLen) * 100));
                }
                fo.close();
                Log.i("Download", "Success");
            } else {
                Log.e("Download", "Cannot create file");
                return 3;
            }
        } catch (IOException e) {
            e.printStackTrace();
            mTcpClient.closeSocket();
            return 1;
        }
        mTcpClient.closeSocket();
        return 0;
    }

    @Override
    protected void onPostExecute(Integer result) {
        switch (result) {
            case 0: Toast.makeText(context, "Download successful", Toast.LENGTH_SHORT).show(); break;
            case 2: Toast.makeText(context, "Connection timeout", Toast.LENGTH_SHORT).show(); break;
            case 3: Toast.makeText(context, "File cannot be created", Toast.LENGTH_SHORT).show(); break;
            case 4: Toast.makeText(context, "File do not exists\nPlease scan again", Toast.LENGTH_SHORT).show(); break;
            default: Toast.makeText(context, "Download error", Toast.LENGTH_SHORT).show();
        }
    }

    @Override
    protected void onProgressUpdate(Integer... progress) {
        this.progress.setProgress(progress[0]);
    }
}
