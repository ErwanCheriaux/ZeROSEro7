package fr.telecom_paristech.spytalk;

import android.util.Log;

import java.io.UnsupportedEncodingException;
import java.security.InvalidAlgorithmParameterException;
import java.security.InvalidKeyException;
import java.security.NoSuchAlgorithmException;
import java.security.spec.InvalidKeySpecException;
import java.security.spec.InvalidParameterSpecException;
import java.util.Arrays;

import javax.crypto.BadPaddingException;
import javax.crypto.Cipher;
import javax.crypto.IllegalBlockSizeException;
import javax.crypto.NoSuchPaddingException;
import javax.crypto.SecretKey;
import javax.crypto.spec.SecretKeySpec;

import blecommon.GAPService;

public class LoraProtocolParser {

    private static final byte[] KEY_BYTES = {(byte) 0xA6, (byte) 0xAB, (byte) 0xFA, (byte) 0x9C, (byte) 0xD4, (byte) 0xF5, (byte) 0x1D, (byte) 0x9C, (byte) 0x1A, (byte) 0x15, (byte) 0x32, (byte) 0xC7, (byte) 0xE8, (byte) 0xF9, (byte) 0xD,
            (byte) 0xA6, (byte) 0xAB, (byte) 0xFA, (byte) 0x9C, (byte) 0xD4, (byte) 0xF5, (byte) 0x1D, (byte) 0x9C, (byte) 0x1A, (byte) 0x15, (byte) 0x32, (byte) 0xC7, (byte) 0xE8, (byte) 0xF9, (byte) 0xD,
            (byte) 0xA6, (byte) 0xAB};
    private static SecretKey secret;
    private byte localAddress;

    public LoraProtocolParser(byte localAddress) {
        this.localAddress = localAddress;
        try {
            generateKey();
        } catch (NoSuchAlgorithmException | InvalidKeySpecException e) {
            e.printStackTrace();
        }
    }

    // Copied from https://stackoverflow.com/questions/40123319/easy-way-to-encrypt-decrypt-string-in-android by Patrick R
    public static SecretKey generateKey()
            throws NoSuchAlgorithmException, InvalidKeySpecException {
        secret = new SecretKeySpec(KEY_BYTES, "AES");
        return secret;
    }

    public static byte[] encryptMsg(byte[] message, SecretKey secret)
            throws NoSuchAlgorithmException, NoSuchPaddingException, InvalidKeyException, InvalidParameterSpecException, IllegalBlockSizeException, BadPaddingException, UnsupportedEncodingException {
   /* Encrypt the message. */
        Cipher cipher = null;
        cipher = Cipher.getInstance("AES/ECB/PKCS5Padding");
        cipher.init(Cipher.ENCRYPT_MODE, secret);
        byte[] cipherText = cipher.doFinal(message);
        return cipherText;
    }

    public static String decryptMsg(byte[] cipherText, SecretKey secret)
            throws NoSuchPaddingException, NoSuchAlgorithmException, InvalidParameterSpecException, InvalidAlgorithmParameterException, InvalidKeyException, BadPaddingException, IllegalBlockSizeException, UnsupportedEncodingException {
    /* Decrypt the message, given derived encContentValues and initialization vector. */
        Cipher cipher = null;
        cipher = Cipher.getInstance("AES/ECB/PKCS5Padding");
        cipher.init(Cipher.DECRYPT_MODE, secret);
        String decryptString = new String(cipher.doFinal(cipherText), "UTF-8");
        return decryptString;
    }

    public LoraMessage parseMessage(byte[] blePacket) {
        byte senderAddress = blePacket[0];
        byte[] payload = Arrays.copyOfRange(blePacket, 1, blePacket.length);
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
