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
    public static final byte[] TX_FAILED_MESSAGE = {0x2, 0x2, 0x2, 0x2};
    public static final byte[] ACKNOWLEDGE_MESSAGE = {0x1, 0x1, 0x1, 0x1};

    byte localAddress;

    private TextView logWindow;
    private EditText messageField;
    private Spinner receiverSelect;

    private LoraProtocolParser loraParser;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        localAddress = getIntent().getByteExtra(SpyTalkAdvertisingActivity.LOCAL_ADDRESS_EXTRA, localAddress);

        loraParser = new LoraProtocolParser(localAddress);

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

                    LoraMessage loraMsg = new LoraMessage(LogInActivity.getAppId(receiver)[0], localAddress, messageField.getText().toString().getBytes());
                    byte[] blePacket = loraParser.buildMessage(loraMsg);
                    bleSend(blePacket);

                    logWindow.append(receiver + " -> " + messageField.getText() + "\n");
                    Log.i("ConnectedActivity", "Sending " + messageField.getText());
                }
                return false;
            }
        });
    }

    @Override
    public void onNotificationReceived(byte[] msg) {
        LoraMessage loraMsg = loraParser.parseMessage(msg);

        switch (loraMsg.getSenderAddress()) {
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

        byte[] payload = loraMsg.getMessage();
        String content = GAPService.parseByteArray(payload);

        if (Arrays.equals(payload, PANICK_MESSAGE)) {
            Vibrator v = (Vibrator) getSystemService(this.VIBRATOR_SERVICE);
            v.vibrate(500); // ms
            content = "Alerte !";
        }

        if (Arrays.equals(payload, ACKNOWLEDGE_MESSAGE)) {
            content = "Message Reçu";
        }

        if (Arrays.equals(payload, TX_FAILED_MESSAGE)) {
            content = "Erreur de transmission";
        }

        logWindow.append("<- " + content + "\n");

    }

    public void onPanickButton(View v) {
        bleSend(GAPService.concat(LogInActivity.APP_ID_BROADCAST, PANICK_MESSAGE));
        logWindow.append("-> Alerte générale\n");
    }

}
