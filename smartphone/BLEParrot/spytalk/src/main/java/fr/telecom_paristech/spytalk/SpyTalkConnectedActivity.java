package fr.telecom_paristech.spytalk;

import android.os.Bundle;
import android.os.Vibrator;
import android.text.method.ScrollingMovementMethod;
import android.util.Log;
import android.view.KeyEvent;
import android.view.View;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Spinner;
import android.widget.TextView;
import android.widget.Toast;

import java.util.Arrays;

import blecommon.ConnectedActivity;
import blecommon.GAPService;


public class SpyTalkConnectedActivity extends ConnectedActivity {

    public static final byte[] PANICK_MESSAGE = {0xD, 0xE, 0xA, 0xD};
    public static final byte[] TX_FAILED_MESSAGE = {0x2, 0x2, 0x2, 0x2};
    public static final byte[] ACKNOWLEDGE_MESSAGE = {0x1, 0x1, 0x1, 0x1};

    private byte localAddress;

    private TextView logWindow;
    private EditText messageField;
    private Spinner receiverSelect;
    private Button panickButton;

    private LoraProtocolParser loraParser;
    private boolean loraIsSending = false;

    @Override
    public void bleSend(byte[] b) {
        if (!loraIsSending) {
            loraIsSending = true;
            messageField.setEnabled(false);
            panickButton.setEnabled(false);
            super.bleSend(b);
        } else {
            Toast.makeText(getApplicationContext(), getString(R.string.cancelled_because_already_sending), Toast.LENGTH_SHORT).show();
        }
    }

    @Override
    protected void onRestart() {
        loraIsSending = false;  // In case we lost BLE connection during the transfert, we do not freeze the app.
        // If a transfer was already ongoing, the Spy Talk will simply notify us with a tx failed.

        super.onRestart();
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        localAddress = getIntent().getByteExtra(SpyTalkAdvertisingActivity.LOCAL_ADDRESS_EXTRA, localAddress);

        loraParser = new LoraProtocolParser(localAddress);

        setContentView(R.layout.chat_activity);
        logWindow = (TextView) findViewById(R.id.logWindow);
        messageField = (EditText) findViewById(R.id.messageField);
        receiverSelect = (Spinner) findViewById(R.id.receiverSelect);
        panickButton = (Button) findViewById(R.id.panickButton);

        receiverSelect.setAdapter(ArrayAdapter.createFromResource(this,
                R.array.usernames, android.R.layout.simple_spinner_item));

        logWindow.setMovementMethod(new ScrollingMovementMethod());
        messageField.setOnKeyListener(new View.OnKeyListener() {
            @Override
            public boolean onKey(View v, int keyCode, KeyEvent event) {
                if (keyCode == KeyEvent.KEYCODE_ENTER && event.getAction() == KeyEvent.ACTION_DOWN) {
                    String receiver = receiverSelect.getSelectedItem().toString();
                    String message = messageField.getText().toString();

                    LoraMessage loraMsg = new LoraMessage(LogInActivity.getAppId(receiver)[0], localAddress, message.getBytes());
                    byte[] blePacket = loraParser.buildMessage(loraMsg);
                    bleSend(blePacket);

                    messageField.setText("");
                    logWindow.append(receiver + " -> " + message + "\n");
                    Log.i("ConnectedActivity", "Sending " + message);
                }
                return false;
            }
        });
    }

    @Override
    public void onNotificationReceived(byte[] msg) {
        Vibrator v = (Vibrator) getSystemService(this.VIBRATOR_SERVICE);

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
            v.vibrate(2000); // ms
            content = "Alerte !";
        } else if (Arrays.equals(payload, ACKNOWLEDGE_MESSAGE)) {
            v.vibrate(200); // ms
            content = "Message Reçu";
            loraIsSending = false;
            messageField.setEnabled(true);
            panickButton.setEnabled(true);
        } else if (Arrays.equals(payload, TX_FAILED_MESSAGE)) {
            v.vibrate(200); // ms
            content = "Erreur de transmission";
            loraIsSending = false;
            messageField.setEnabled(true);
            panickButton.setEnabled(true);
        } else {
            v.vibrate(500); // ms
        }

        logWindow.append("<- " + content + "\n");

    }

    public void onPanickButton(View v) {
        String receiver = receiverSelect.getSelectedItem().toString();

        LoraMessage loraMsg = new LoraMessage(LogInActivity.getAppId(receiver)[0], localAddress, PANICK_MESSAGE);
        byte[] blePacket = loraParser.buildMessage(loraMsg);
        bleSend(blePacket);

        logWindow.append(receiver + " -> " + "Alert !" + "\n");
        Log.i("ConnectedActivity", "Sending " + messageField.getText());
    }

}
