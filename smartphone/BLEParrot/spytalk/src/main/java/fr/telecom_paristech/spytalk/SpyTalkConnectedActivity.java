package fr.telecom_paristech.spytalk;

import android.os.Bundle;
import android.os.Vibrator;
import android.text.method.ScrollingMovementMethod;
import android.util.Log;
import android.view.KeyEvent;
import android.view.View;
import android.widget.ArrayAdapter;
import android.widget.EditText;
import android.widget.Spinner;
import android.widget.TextView;

import java.util.Arrays;

import blecommon.ConnectedActivity;
import blecommon.GAPService;


public class SpyTalkConnectedActivity extends ConnectedActivity {

    public static final byte[] PANICK_MESSAGE = {0xD, 0xE, 0xA, 0xD};

    private TextView logWindow;
    private EditText messageField;
    private Spinner receiverSelect;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        setContentView(R.layout.chat_activity);
        logWindow = (TextView) findViewById(R.id.logWindow);
        messageField = (EditText) findViewById(R.id.messageField);
        receiverSelect = (Spinner) findViewById(R.id.receiverSelect);

        receiverSelect.setAdapter(ArrayAdapter.createFromResource(this,
                R.array.usernames, android.R.layout.simple_spinner_item));

        logWindow.setMovementMethod(new ScrollingMovementMethod());
        messageField.setOnKeyListener(new View.OnKeyListener() {
            @Override
            public boolean onKey(View v, int keyCode, KeyEvent event) {
                if (keyCode == KeyEvent.KEYCODE_ENTER && event.getAction() == KeyEvent.ACTION_DOWN) {
                    String receiver = receiverSelect.getSelectedItem().toString();
                    bleSend(GAPService.concat(LogInActivity.getAppId(receiver), messageField.getText().toString().getBytes()));
                    logWindow.append(receiver + " -> " + messageField.getText() + "\n");
                    Log.i("ConnectedActivity", "Sending " + messageField.getText());
                }
                return false;
            }
        });
    }

    @Override
    public void onNotificationReceived(byte[] msg) {

        switch (msg[0]) {
            case LogInActivity.YELLOW_ADDR:
                logWindow.append("Jaune ");
                break;
            case LogInActivity.BLUE_ADDR:
                logWindow.append("Bleu ");
                break;
            case LogInActivity.PINK_ADDR:
                logWindow.append("Rose ");
                break;
            default:
                logWindow.append("Unknown ");
                break;
        }

        byte[] slice = Arrays.copyOfRange(msg, 1, msg.length);
        String content = GAPService.parseByteArray(slice);

        if (Arrays.equals(slice, PANICK_MESSAGE)) {
            Vibrator v = (Vibrator) getSystemService(this.VIBRATOR_SERVICE);
            v.vibrate(500); // ms
            content = "Alerte !";
        }

        logWindow.append("<- " + content + "\n");

    }

    public void onPanickButton(View v) {
        bleSend(GAPService.concat(LogInActivity.APP_ID_BROADCAST, PANICK_MESSAGE));
        logWindow.append("-> Alerte générale\n");
    }

}
