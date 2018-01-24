package fr.telecom_paristech.spytalk;

import android.os.Bundle;
import android.text.method.ScrollingMovementMethod;
import android.util.Log;
import android.view.KeyEvent;
import android.view.View;
import android.widget.ArrayAdapter;
import android.widget.EditText;
import android.widget.Spinner;
import android.widget.TextView;

import blecommon.ConnectedActivity;
import blecommon.GAPService;


public class SpyTalkConnectedActivity extends ConnectedActivity {

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
    public void onNotificationReceived(String msg) {
        logWindow.append("<- " + msg + "\n");
        logWindow.scrollTo(0, 0);
    }
}
