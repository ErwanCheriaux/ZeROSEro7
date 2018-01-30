package fr.telecom_paristech.spytalk;

import android.app.AlertDialog;
import android.content.DialogInterface;
import android.content.Intent;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.view.View;
import android.widget.EditText;

import blecommon.AdvertisingActivity;

public class LogInActivity extends AppCompatActivity {

    public static final byte YELLOW_ADDR = 0x04;
    public static final byte BLUE_ADDR = 0x05;
    public static final byte PINK_ADDR = 0x06;
    public static final byte BROADCAST_ADDR = 0x0F;

    public static final byte[] APP_ID_YELLOW = {YELLOW_ADDR};
    public static final byte[] APP_ID_BLUE = {BLUE_ADDR};
    public static final byte[] APP_ID_PINK = {PINK_ADDR};
    public static final byte[] APP_ID_BROADCAST = {BROADCAST_ADDR};

    public static final String PASSWORD_EXTRA = "password";

    private String password = null;

    public static byte[] getAppId(String username) {
        switch (username) {
            case "Jaune":
                return APP_ID_YELLOW;
            case "Bleu":
                return APP_ID_BLUE;
            case "Rose":
                return APP_ID_PINK;
            case "Tous":
                return APP_ID_BROADCAST;
            default:
                throw new NullPointerException("User not defined");
        }
    }

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
        advertisingActivityIntent.putExtra(PASSWORD_EXTRA, password);
        startActivity(advertisingActivityIntent);
        finish();
    }

    public void onPasswordButton(View v) {
        AlertDialog.Builder builder = new AlertDialog.Builder(this);
        final EditText passwordField = new EditText(getApplicationContext());
        builder.setMessage("Nouveau mot de passe :")
                .setPositiveButton("Ok", new DialogInterface.OnClickListener() {
                    public void onClick(DialogInterface dialog, int id) {
                        password = passwordField.getText().toString();
                        Log.i("LogInActivity","New password : "+password);
                    }
                })
                .setView(passwordField);
        builder.create().show();
    }
}
