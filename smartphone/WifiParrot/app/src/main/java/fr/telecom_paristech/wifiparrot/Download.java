package fr.telecom_paristech.wifiparrot;

import android.os.Environment;
import android.util.Log;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;

public class Download extends Transfer
{
    protected Integer doInBackground(String... input)
    {
        String filename = input[0];
        String data = GET("http://setup.com/" + filename);
        Log.d("Download", "Response (get " + filename + "): " + data);
        String path = Environment.getExternalStoragePublicDirectory(Environment.DIRECTORY_DOWNLOADS) + File.separator;
        Log.d("Download", "Saved at: " + path + filename);
        File file = new File(path, filename);
        try {
            file.createNewFile();
            if(file.exists()) {
                FileOutputStream fo = new FileOutputStream(file);
                fo.write(data.getBytes());
                Log.i("Download", "Success");
            } else {
              Log.e("Download", "Cannot create file");
            }
        } catch (FileNotFoundException e) {
            e.printStackTrace();
            return 1;
        } catch (IOException e) {
            e.printStackTrace();
            return 1;
        }
        return 0;
    }
}
