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


public class SpyTalkConnectedActivity extends ConnectedActivity {

    private TextView logWindow;
    private EditText commandField;
    private Spinner receiverSelect;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        setContentView(R.layout.chat_activity);
        logWindow = (TextView) findViewById(R.id.logWindow);
        commandField = (EditText) findViewById(R.id.messageField);
        receiverSelect = (Spinner) findViewById(R.id.receiverSelect);

        receiverSelect.setAdapter(ArrayAdapter.createFromResource(this,
                R.array.usernames, android.R.layout.simple_spinner_item));

        logWindow.setMovementMethod(new ScrollingMovementMethod());
        commandField.setOnKeyListener(new View.OnKeyListener() {
            @Override
            public boolean onKey(View v, int keyCode, KeyEvent event) {
                if (keyCode == KeyEvent.KEYCODE_ENTER && event.getAction() == KeyEvent.ACTION_DOWN) {
                    bleSend(commandField.getText().toString());
                    logWindow.append("-> " + commandField.getText() + "\n");
                    Log.i("ConnectedActivity", "Sending " + commandField.getText());
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
