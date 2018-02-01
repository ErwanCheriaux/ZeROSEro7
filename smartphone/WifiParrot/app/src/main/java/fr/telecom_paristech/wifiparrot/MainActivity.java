package fr.telecom_paristech.wifiparrot;

import android.Manifest;
import android.app.AlertDialog;
import android.bluetooth.BluetoothAdapter;
import android.content.BroadcastReceiver;
import android.content.ComponentName;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.IntentFilter;
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
import android.widget.LinearLayout;
import android.widget.ProgressBar;

import java.net.URISyntaxException;
import java.util.ArrayList;

import blecommon.AdvertiserService;
import blecommon.AdvertisingActivity;
import blecommon.GAPService;

public class MainActivity extends AppCompatActivity
{
    Wifi mService;
    Button buttonScan;
    Button buttonUpload;
    Intent wifiIntent;
    LinearLayout layout;
    ProgressBar progress;
    CallbackInterface callbacks;
    ArrayList<String> filenames;

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
                    mService.uploadFile(getApplicationContext(), path, progress);
                } catch (URISyntaxException e) {
                    e.printStackTrace();
                }
                return;
            }
            Log.e("File manager", "File manger result is not correct");
        }
    }

    private Intent advertisementIntent;
    private AdvertiserService advertiser;

    private void startAdvertising() {
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M && checkSelfPermission(Manifest.permission.ACCESS_COARSE_LOCATION) != PackageManager.PERMISSION_GRANTED) {
            requestPermissions(new String[]{Manifest.permission.ACCESS_COARSE_LOCATION}, 1);
        }

        final BluetoothAdapter mBluetoothAdapter = BluetoothAdapter.getDefaultAdapter();
        if (!mBluetoothAdapter.isEnabled()) {
            AlertDialog.Builder builder = new AlertDialog.Builder(this);
            builder.setMessage("Le Bluetooth va être activé.")
                    .setPositiveButton("Ok", new DialogInterface.OnClickListener() {
                        public void onClick(DialogInterface dialog, int id) {
                            mBluetoothAdapter.enable();
                            IntentFilter filter = new IntentFilter(BluetoothAdapter.ACTION_STATE_CHANGED);
                            registerReceiver(new BroadcastReceiver() {
                                @Override
                                public void onReceive(Context context, Intent intent) {
                                    final String action = intent.getAction();
                                    if (action.equals(BluetoothAdapter.ACTION_STATE_CHANGED)) {
                                        final int state = intent.getIntExtra(BluetoothAdapter.EXTRA_STATE,
                                                BluetoothAdapter.ERROR);
                                        switch (state) {
                                            case BluetoothAdapter.STATE_ON:
                                                startBluetoothServices();
                                                break;
                                            default:
                                                break;
                                        }
                                    }
                                }
                            }, filter);
                        }
                    });
            builder.show();
        } else {
            startBluetoothServices();
        }

    }

    @Override
    protected void onDestroy() {
        stopService(advertisementIntent);
        unbindService(advConnection);

        super.onDestroy();
    }

    private ServiceConnection advConnection = new ServiceConnection() {

        @Override
        public void onServiceConnected(ComponentName className,
                                       IBinder service) {
            if (className.getClassName().equals(AdvertiserService.class.getName())) {
                // We've bound to LocalService, cast the IBinder and get LocalService instance
                AdvertiserService.LocalBinder binder = (AdvertiserService.LocalBinder) service;
                advertiser = binder.getService();
            }
        }

        @Override // Never disconnected
        public void onServiceDisconnected(ComponentName arg0) {
        }
    };

    public static final byte[] APP_ID_STEALTH_DROP = {0x03};

    private void startBluetoothServices() {
        advertisementIntent = new Intent(this, AdvertiserService.class);
        advertisementIntent.putExtra(AdvertisingActivity.APP_ID_EXTRA, APP_ID_STEALTH_DROP);
        bindService(advertisementIntent, advConnection, Context.BIND_AUTO_CREATE);
        startService(advertisementIntent);
    }

    @Override
    protected void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        startAdvertising();
        
        filenames = new ArrayList<>();

        callbacks = new CallbackInterface() {
            @Override
            public void addDownloadButton(String text) {
                filenames.add(text);
                addButton(text);
            }

            @Override
            public void removeDownloadButtons() {
                filenames.clear();
                layout.removeAllViews();
            }

            @Override
            public void removeDownloadButton(String filename) {
                filenames.remove(filename);
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
                mService.getFiles(getApplicationContext(), callbacks);
            }
        });
        layout = (LinearLayout)findViewById(R.id.layout_scan);
        progress = (ProgressBar)findViewById(R.id.progress_bar);
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
                mService.downloadFile(getApplicationContext(), ((Button)v).getText().toString(), progress);
            }
        });
        ImageButton removeBtn = new ImageButton(getApplicationContext());
        removeBtn.setBackgroundResource(R.drawable.delete);
        removeBtn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                mService.removeFile(getApplicationContext(), ((Button)(((LinearLayout)v.getParent()).getChildAt(0))).getText().toString(), callbacks);
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

    // Save the filenames before activity restarts (on screen rotation for example)
    @Override
    protected void onSaveInstanceState(final Bundle outState) {
        super.onSaveInstanceState(outState);
        outState.putStringArrayList("ScanResult", filenames);
    }

    @Override
    protected void onRestoreInstanceState(final Bundle savedInstanceState) {
        super.onRestoreInstanceState(savedInstanceState);
        // Read the state of item position
        filenames = savedInstanceState.getStringArrayList("ScanResult");
        for(String filename : filenames)
            addButton(filename);
    }
}
