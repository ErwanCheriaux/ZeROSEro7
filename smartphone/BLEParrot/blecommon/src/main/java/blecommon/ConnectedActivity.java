package blecommon;

import android.content.BroadcastReceiver;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.ServiceConnection;
import android.os.Bundle;
import android.os.IBinder;
import android.support.v4.content.LocalBroadcastManager;
import android.support.v7.app.AppCompatActivity;
import android.widget.Toast;

public abstract class ConnectedActivity extends AppCompatActivity {

    private Intent gapIntent;
    private GAPService gapService;
    // Defines services connection callbacks
    private ServiceConnection mConnection = new ServiceConnection() {

        @Override
        public void onServiceConnected(ComponentName className,
                                       IBinder service) {
            if (className.getClassName().equals(GAPService.class.getName())) {
                GAPService.LocalBinder binder = (GAPService.LocalBinder) service;
                gapService = binder.getService();
            } else {
                throw new UnsupportedOperationException("Unsupported service connection");
            }
        }

        @Override // Never disconnected
        public void onServiceDisconnected(ComponentName arg0) {
        }

    };
    // Device disconnected callback
    private BroadcastReceiver disconnectedBroadcastReceiver = new BroadcastReceiver() {
        @Override
        public void onReceive(Context context, Intent intent) {
            if (intent.getAction().equals(GAPService.DEVICE_DISCONNECTED_ACTION)) {
                Toast.makeText(getApplicationContext(), "Lost connection !", Toast.LENGTH_SHORT).show();
                finish();
            }
        }
    };
    // Device disconnected callback
    private BroadcastReceiver notificationBroadcastReceiver = new BroadcastReceiver() {
        @Override
        public void onReceive(Context context, Intent intent) {
            if (intent.getAction().equals(GAPService.DEVICE_NOTIFICATION_ACTION)) {
                onNotificationReceived(intent.getStringExtra("Message"));
            }
        }
    };

    public abstract void onNotificationReceived(String msg);

    public void bleSend(String msg) {
        gapService.send(msg);
    }

    public void bleSend(byte[] b) {
        gapService.send(b);
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        gapIntent = new Intent(this, GAPService.class);
        bindService(gapIntent, mConnection, Context.BIND_AUTO_CREATE);
        startService(gapIntent);

        LocalBroadcastManager.getInstance(this).registerReceiver(disconnectedBroadcastReceiver, new IntentFilter(GAPService.DEVICE_DISCONNECTED_ACTION));
        LocalBroadcastManager.getInstance(this).registerReceiver(notificationBroadcastReceiver, new IntentFilter(GAPService.DEVICE_NOTIFICATION_ACTION));
    }

    @Override
    public void onDestroy() {
        unbindService(mConnection);
        LocalBroadcastManager.getInstance(this).unregisterReceiver(disconnectedBroadcastReceiver);
        LocalBroadcastManager.getInstance(this).unregisterReceiver(notificationBroadcastReceiver);
        super.onDestroy();
    }

}
