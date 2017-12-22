package fr.telecom_paristech.bleparrot;

import android.content.ComponentName;
import android.content.Context;
import android.content.ServiceConnection;
import android.os.IBinder;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.content.Intent ;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.ProgressBar;
import android.widget.TextView;
import android.widget.Toast;

public class MainActivity extends AppCompatActivity {

    private Intent advertisementIntent ;
    private AdvertiserService advertiser ;
    private Button pauseResumeButton ;
    private ProgressBar advertisingProgress ;
    private TextView advertisingTitle ;

    // Defines connection callbacks
    private ServiceConnection mConnection = new ServiceConnection() {

        @Override
        public void onServiceConnected(ComponentName className,
                                       IBinder service) {
            // We've bound to LocalService, cast the IBinder and get LocalService instance
            AdvertiserService.LocalBinder binder = (AdvertiserService.LocalBinder) service;
            advertiser = binder.getService();
        }

        @Override // Never disconnected
        public void onServiceDisconnected(ComponentName arg0) {
        }

    } ;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState) ;
        setContentView(R.layout.activity_main) ;

        pauseResumeButton = (Button) findViewById(R.id.pauseResumeButton) ;
        advertisingProgress = (ProgressBar) findViewById(R.id.advertisingProgress) ;
        advertisingTitle = (TextView) findViewById(R.id.advertisingTitle) ;

        advertisementIntent = new Intent(this, AdvertiserService.class) ;
        bindService(advertisementIntent, mConnection, Context.BIND_AUTO_CREATE);
        startService(advertisementIntent) ;
        // Callback initialized advertiser local attribute
    }

    boolean advertiserStarted = true ;
    public void onPauseResumeButton(View v) {
        if(advertiserStarted) {
            advertiser.pause() ;
            advertisingProgress.setVisibility(View.INVISIBLE) ;
            pauseResumeButton.setText("Resume") ;
            advertisingTitle.setText("Advertising Stopped") ;
        } else {
            advertiser.resume() ;
            advertisingProgress.setVisibility(View.VISIBLE) ;
            pauseResumeButton.setText("Pause") ;
            advertisingTitle.setText("Advertising in BLE") ;
        }
        advertiserStarted = !advertiserStarted ;
    }

    @Override
    public void onBackPressed() {
        moveTaskToBack(true) ;  // Doesn't stop the app
    }

    @Override
    public void onPause() {
        Toast.makeText(getApplicationContext(), "BLE Advertisement running in the backgruond", Toast.LENGTH_LONG).show() ;
        Log.i("MainActivity","Activity paused") ;
        unbindService(mConnection) ;
        super.onPause() ;
    }

    @Override
    public void onResume() {
        Log.i("MainActivity","Activity resumed") ;
        super.onResume() ;
        bindService(advertisementIntent, mConnection, Context.BIND_AUTO_CREATE) ;
    }

    @Override
    public void onDestroy() {
        Toast.makeText(getApplicationContext(), "BLE Advertisement stopped", Toast.LENGTH_LONG).show() ;
        Log.i("MainActivity","Activity destroyed") ;
        stopService(advertisementIntent) ;
        super.onDestroy() ;
    }

}
