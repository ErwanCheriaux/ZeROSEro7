package fr.telecom_paristech.spytalk;

import android.content.Intent;
import android.os.Bundle;
import android.widget.Button;
import android.widget.ProgressBar;
import android.widget.TextView;

import blecommon.AdvertisingActivity;

public class SpyTalkAdvertisingActivity extends AdvertisingActivity {

    private byte[] appId;

    private Button pauseResumeButton;
    private ProgressBar advertisingProgress;
    private TextView advertisingTitle;
    private Intent connectedActivityIntent = null;

    @Override
    public ProgressBar getAdvertisingProgress() {
        return advertisingProgress;
    }

    @Override
    public Button getPauseResumeButton() {
        return pauseResumeButton;
    }

    @Override
    public TextView getAdvertisingTitle() {
        return advertisingTitle;
    }

    @Override
    public void onPhoneConnected() {
        if (connectedActivityIntent == null) {
            connectedActivityIntent = new Intent(this, SpyTalkConnectedActivity.class);
        }
        startActivity(connectedActivityIntent);
    }

    @Override
    public byte[] getAppID() {
        return appId;
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        Intent createIntent = getIntent();
        appId = createIntent.getByteArrayExtra(APP_ID_EXTRA);

        super.onCreate(savedInstanceState);

        setContentView(R.layout.activity_main);
        pauseResumeButton = (Button) findViewById(R.id.pauseResumeButton);
        advertisingProgress = (ProgressBar) findViewById(R.id.advertisingProgress);
        advertisingTitle = (TextView) findViewById(R.id.advertisingTitle);
    }

}
