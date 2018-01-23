package fr.telecom_paristech.spytalk;

import android.content.Intent;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.view.View;

import blecommon.AdvertisingActivity;

public class LogInActivity extends AppCompatActivity {

    public static final byte[] APP_ID_YELLOW = {0x04};
    public static final byte[] APP_ID_BLUE = {0x05};
    public static final byte[] APP_ID_PINK = {0x06};

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_log_in);
    }

    public void onYellowClick(View v) {
        startAdvertisingActivity(APP_ID_YELLOW);
    }

    public void onBlueClick(View v) {
        startAdvertisingActivity(APP_ID_BLUE);
    }

    public void onPinkClick(View v) {
        startAdvertisingActivity(APP_ID_PINK);
    }

    private void startAdvertisingActivity(byte[] appId) {
        Intent advertisingActivityIntent = new Intent(this, SpyTalkAdvertisingActivity.class);
        advertisingActivityIntent.putExtra(AdvertisingActivity.APP_ID_EXTRA, appId);
        startActivity(advertisingActivityIntent);
        finish();
    }
}
