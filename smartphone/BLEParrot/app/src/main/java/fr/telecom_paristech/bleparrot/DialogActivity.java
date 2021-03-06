package fr.telecom_paristech.bleparrot;

import android.os.Bundle;
import android.text.method.ScrollingMovementMethod;
import android.util.Log;
import android.view.KeyEvent;
import android.view.View;
import android.widget.EditText;
import android.widget.TextView;

import blecommon.ConnectedActivity;
import blecommon.GAPService;


public class DialogActivity extends ConnectedActivity {


    private TextView logWindow;
    private EditText commandField;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        setContentView(R.layout.dialog_activity);
        logWindow = (TextView) findViewById(R.id.logWindow);
        commandField = (EditText) findViewById(R.id.commandField);


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
    public void onNotificationReceived(byte[] msg) {
        logWindow.append("<- " + GAPService.parseByteArray(msg) + "\n");
        logWindow.scrollTo(0, 0);
    }
}
