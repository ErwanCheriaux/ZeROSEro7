package fr.telecom_paristech.spytalk;

import java.util.Arrays;

import blecommon.GAPService;

public class LoraProtocolParser {

    private byte localAddress;

    // TODO encryption

    public LoraProtocolParser(byte localAddress) {
        this.localAddress = localAddress;
    }

    public LoraMessage parseMessage(byte[] blePacket) {
        byte senderAddress = blePacket[0];
        byte[] payload = Arrays.copyOfRange(blePacket, 1, blePacket.length);
        return new LoraMessage(localAddress, senderAddress, payload);
    }

    public byte[] buildMessage(LoraMessage loraMsg) {
        byte[] receiverArray = {loraMsg.getReceiverAddress()};
        byte[] blePacket = GAPService.concat(receiverArray, loraMsg.getMessage());
        return blePacket;
    }

}
