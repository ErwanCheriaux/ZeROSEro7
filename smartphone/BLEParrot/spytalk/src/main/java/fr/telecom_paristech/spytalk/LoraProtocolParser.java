package fr.telecom_paristech.spytalk;

import android.util.Log;

import java.io.UnsupportedEncodingException;
import java.security.InvalidAlgorithmParameterException;
import java.security.InvalidKeyException;
import java.security.NoSuchAlgorithmException;
import java.security.spec.InvalidKeySpecException;
import java.security.spec.InvalidParameterSpecException;
import java.security.spec.KeySpec;
import java.util.Arrays;

import javax.crypto.BadPaddingException;
import javax.crypto.Cipher;
import javax.crypto.IllegalBlockSizeException;
import javax.crypto.NoSuchPaddingException;
import javax.crypto.SecretKey;
import javax.crypto.SecretKeyFactory;
import javax.crypto.spec.PBEKeySpec;

import blecommon.GAPService;

public class LoraProtocolParser {

    public static final String DEFAULT_PASSWORD = "?n&IrF`d1!=m&-2)z~<*l4,w~}+(pR*YHZo@jxwdYn-Y4(D(%wRG&YQG0*(6R[gu";
    private static SecretKey secret;
    private byte localAddress;

    public LoraProtocolParser(byte localAddress) {
        this.localAddress = localAddress;
        generateKey(DEFAULT_PASSWORD);
    }


    // Copied from https://android-developers.googleblog.com/2013/02/using-cryptography-to-store-credentials.html
    public void generateKey(String password) {
        // Number of PBKDF2 hardening rounds to use. Larger values increase
        // computation time. You should select a value that causes computation
        // to take >100ms.
        final int iterations = 1000;

        // Generate a 256-bit key
        final int outputKeyLength = 256;

        SecretKeyFactory secretKeyFactory = null;
        try {
            secretKeyFactory = SecretKeyFactory.getInstance("PBKDF2WithHmacSHA1");
            KeySpec keySpec = new PBEKeySpec(password.toCharArray(), "common salt".getBytes(), iterations, outputKeyLength);
            secret = secretKeyFactory.generateSecret(keySpec);
        } catch (NoSuchAlgorithmException | InvalidKeySpecException e) {
            e.printStackTrace();
        }
    }

    public static byte[] encryptMsg(byte[] message, SecretKey secret)
            throws NoSuchAlgorithmException, NoSuchPaddingException, InvalidKeyException, InvalidParameterSpecException, IllegalBlockSizeException, BadPaddingException, UnsupportedEncodingException {
        /* Encrypt the message. */
        Cipher cipher = Cipher.getInstance("AES/ECB/PKCS5Padding");
        cipher.init(Cipher.ENCRYPT_MODE, secret);
        byte[] cipherText = cipher.doFinal(message);
        return cipherText;
    }

    public static byte[] decryptMsg(byte[] cipherText, SecretKey secret)
            throws NoSuchPaddingException, NoSuchAlgorithmException, InvalidParameterSpecException, InvalidAlgorithmParameterException, InvalidKeyException, BadPaddingException, IllegalBlockSizeException, UnsupportedEncodingException {
        /* Decrypt the message, given derived encContentValues and initialization vector. */
        Cipher cipher = Cipher.getInstance("AES/ECB/PKCS5Padding");
        cipher.init(Cipher.DECRYPT_MODE, secret);
        return cipher.doFinal(cipherText);
    }

    public LoraMessage parseMessage(byte[] blePacket) {
        byte senderAddress = blePacket[0];
        byte[] payload = Arrays.copyOfRange(blePacket, 1, blePacket.length);
        if (!(Arrays.equals(payload, SpyTalkConnectedActivity.ACKNOWLEDGE_MESSAGE)
                || Arrays.equals(payload, SpyTalkConnectedActivity.TX_FAILED_MESSAGE))) { // Messages generated by the spy talk, hence not encrypted.
            try {
                payload = decryptMsg(payload, secret);
            } catch (NoSuchPaddingException | NoSuchAlgorithmException | InvalidParameterSpecException | InvalidAlgorithmParameterException | InvalidKeyException | BadPaddingException | IllegalBlockSizeException | UnsupportedEncodingException e) {
                e.printStackTrace();
            }
        }
        return new LoraMessage(localAddress, senderAddress, payload);
    }

    public byte[] buildMessage(LoraMessage loraMsg) {
        byte[] receiverArray = {loraMsg.getReceiverAddress()};
        byte[] cipheredMessage = {};
        try {
            cipheredMessage = encryptMsg(loraMsg.getMessage(), secret);
        } catch (NoSuchAlgorithmException | NoSuchPaddingException | InvalidKeyException | InvalidParameterSpecException | IllegalBlockSizeException | BadPaddingException | UnsupportedEncodingException e) {
            e.printStackTrace();
        }
        byte[] blePacket = GAPService.concat(receiverArray, cipheredMessage);
        Log.i("LoraProtocolParser", "Cipher " + cipheredMessage.length + " : " + Arrays.toString(cipheredMessage));
        return blePacket;
    }

}
