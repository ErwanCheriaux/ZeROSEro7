package fr.telecom_paristech.spytalk;

public class LoraMessage {

    private byte senderAddress;
    private byte receiverAddress;
    private byte[] message;

    public LoraMessage(byte receiverAddress, byte senderAddress, byte[] message) {
        this.senderAddress = senderAddress;
        this.receiverAddress = receiverAddress;
        this.message = message;
    }

    public byte[] getMessage() {
        return message;
    }

    public void setMessage(byte[] message) {
        this.message = message;
    }

    public byte getSenderAddress() {

        return senderAddress;
    }

    public byte getReceiverAddress() {
        return receiverAddress;
    }

    public void setReceiverAddress(byte receiverAddress) {
        this.receiverAddress = receiverAddress;
    }

    public void setSenderAddress(byte senderAddress) {
        this.senderAddress = senderAddress;
    }
}
