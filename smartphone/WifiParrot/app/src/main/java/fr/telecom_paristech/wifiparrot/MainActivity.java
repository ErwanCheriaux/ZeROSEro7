package fr.telecom_paristech.wifiparrot;

import android.Manifest;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.ServiceConnection;
import android.content.pm.PackageManager;
import android.os.Build;
import android.os.IBinder;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.Gravity;
import android.view.View;
import android.widget.Button;
import android.widget.ImageButton;
import android.widget.ImageView;
import android.widget.LinearLayout;

import java.net.URISyntaxException;

public class MainActivity extends AppCompatActivity
{
    Wifi mService;
    Button buttonScan;
    Button buttonUpload;
    Intent wifiIntent;
    LinearLayout layout;
    CallbackInterface callbacks;

    static final int FILE_PICK = 1;

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data)
    {
        if (requestCode == FILE_PICK) {
            // Make sure the request was successful
            if (resultCode == RESULT_OK) {
                try {
                    String path = PathUtil.getPath(getApplicationContext(), data.getData());
                    Log.d("File manager data", path);
                    mService.uploadFile(path);
                } catch (URISyntaxException e) {
                    e.printStackTrace();
                }
                return;
            }
            Log.e("File manager", "File manger result is not correct");
        }
    }

    @Override
    protected void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        callbacks = new CallbackInterface() {
            @Override
            public void addDownloadButton(String text) {
                addButton(text);
            }

            @Override
            public void removeDownloadButtons() {
                layout.removeAllViews();
            }

            @Override
            public void removeDownloadButton(String filename) {
                for(int i = 0; i < layout.getChildCount(); i++)
                    if(((Button)((LinearLayout)layout.getChildAt(i)).getChildAt(0)).getText().equals(filename))
                        layout.removeViewAt(i);
            }
        };

        layout = new LinearLayout(getApplicationContext());

        buttonUpload = (Button)findViewById(R.id.button_upload);
        buttonUpload.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                Intent intent = new Intent();
                intent.setAction(Intent.ACTION_GET_CONTENT);
                intent.setType("*/*");
                startActivityForResult(intent, FILE_PICK);
            }
        });

        buttonScan = (Button)findViewById(R.id.button_scan);
        buttonScan.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                mService.getFiles(callbacks);
            }
        });
        layout = (LinearLayout)findViewById(R.id.layout_scan);
        // Allow WiFi
        // Location access is only used to make a network scan.
        // If Scan function doesn't works, please enable location on your mobile phone
        if(Build.VERSION.SDK_INT >= Build.VERSION_CODES.M && checkSelfPermission(Manifest.permission.ACCESS_COARSE_LOCATION) != PackageManager.PERMISSION_GRANTED)
            requestPermissions(new String[]{Manifest.permission.ACCESS_COARSE_LOCATION}, 1);
        // Allow File
        if(Build.VERSION.SDK_INT >= Build.VERSION_CODES.M && checkSelfPermission(Manifest.permission.WRITE_EXTERNAL_STORAGE) != PackageManager.PERMISSION_GRANTED)
            requestPermissions(new String[]{Manifest.permission.WRITE_EXTERNAL_STORAGE}, 2);
    }

    @Override
    protected void onStart() {
        super.onStart();
        // Bind to LocalService
        wifiIntent = new Intent(this, Wifi.class);
        bindService(wifiIntent, mConnection, Context.BIND_AUTO_CREATE);
    }

    @Override
    protected void onStop() {
        super.onStop();
        unbindService(mConnection);
    }

    public void addButton(String filename)
    {
        Button downloadBtn = new Button(getApplicationContext());
        downloadBtn.setText(filename);
        downloadBtn.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                mService.downloadFile(((Button)v).getText().toString());
            }
        });
        ImageButton removeBtn = new ImageButton(getApplicationContext());
        removeBtn.setBackgroundResource(R.drawable.delete);
        removeBtn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                mService.removeFile(((Button)(((LinearLayout)v.getParent()).getChildAt(0))).getText().toString(), callbacks);
            }
        });
        LinearLayout hLayout = new LinearLayout(getApplicationContext());
        hLayout.setOrientation(LinearLayout.HORIZONTAL);
        hLayout.setGravity(Gravity.CENTER_VERTICAL);
        hLayout.addView(downloadBtn, 0);
        hLayout.addView(removeBtn, 1);
        layout.addView(hLayout);
    }

    private ServiceConnection mConnection = new ServiceConnection()
    {
        @Override
        public void onServiceConnected(ComponentName className, IBinder service)
        {
            // We've bound to LocalService, cast the IBinder and get LocalService instance
            Wifi.LocalBinder binder = (Wifi.LocalBinder) service;
            mService = binder.getService();
        }

        @Override
        public void onServiceDisconnected(ComponentName arg0)
        {}
    };
}
