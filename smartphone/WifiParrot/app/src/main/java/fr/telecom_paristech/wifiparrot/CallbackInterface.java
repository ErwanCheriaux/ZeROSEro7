package fr.telecom_paristech.wifiparrot;

public interface CallbackInterface {
    void addDownloadButton(String text);
    void removeDownloadButtons();
    void removeDownloadButton(String filename);
}
