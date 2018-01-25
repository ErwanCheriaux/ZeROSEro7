package fr.telecom_paristech.usbsniffer;

import android.app.AlertDialog;
import android.content.DialogInterface;
import android.os.Bundle;
import android.text.method.ScrollingMovementMethod;
import android.util.Log;
import android.widget.TextView;

import java.io.FileOutputStream;

import blecommon.ConnectedActivity;
import blecommon.GAPService;

public class USBSnifferConnectedActivity extends ConnectedActivity {

    public static final String FILENAME = "usb_sniffer_dump.txt";
    private TextView logWindow;
    private FileOutputStream dumpFileStream;

    @Override
    public void onNotificationReceived(byte[] msg) {
        String s = GAPService.parseByteArray(msg);

        logWindow.append(s);

        dumpInFile(s);

        // Reached end of file
        if (s.length() < GAPService.MAX_MTU_NRF) {
            alertEndOfFile();
        }
    }

    private void alertEndOfFile() {
        AlertDialog.Builder builder = new AlertDialog.Builder(USBSnifferConnectedActivity.this);
        builder.setMessage("Transfert terminé. Voir Downloads/" + FILENAME)
                .setNeutralButton("Ok", new DialogInterface.OnClickListener() {
                    public void onClick(DialogInterface dialog, int id) {

                    }
                });
        builder.create().show();
    }

    private void dumpInFile(String s) {
        /*try {
            dumpFileStream.write(s.getBytes());
        } catch (IOException e) {
            e.printStackTrace();
        }*/
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_usbsniffer_connected);

        logWindow = (TextView) findViewById(R.id.logWindow);
        logWindow.setMovementMethod(new ScrollingMovementMethod());
    }

    @Override
    protected void onRestart() {
        super.onRestart();

        /*String path = Environment.getExternalStoragePublicDirectory(Environment.DIRECTORY_DOWNLOADS) + File.separator;
        File dumpFile = new File(path, FILENAME);
        if (dumpFile.exists()) {
            dumpFile.delete();
        }
        try {
            dumpFile.createNewFile();
            dumpFileStream = openFileOutput(FILENAME, Context.MODE_PRIVATE);
        } catch (IOException e) {
            e.printStackTrace();
        }*/

    }

    private void initiateTransfer() {
        bleSend("Would you kindly ?");
        Log.i("ConnectedActivity", "Initiating transfert");
    }

    @Override
    public void onStop() {

        /*try {
            dumpFileStream.close();
        } catch (IOException e) {
            e.printStackTrace();
        }*/
        super.onStop();
    }

    @Override
    protected void onGAPServceConnected() { // GAP Service isn't connected (as an Android connection) during onStart
        initiateTransfer();
    }
}
