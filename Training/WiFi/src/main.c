//main.c

#include "ch.h"
#include "hal.h"

#include "led.h"
#include "pwm.h"
#include "timer.h"
#include "wifi.h"
#include "rtt.h"
#include "sha256.h"

int main(void)
{
    halInit();
    chSysInit();

    led_init();
    pwm_init();
    timer_init();
    wifi_init();
    rtt_init();

    rtt_printf(0, "\n======== INITIALIZATION SUCCED ========\n\n");
    led_on();

    wifi_command("ver\r\n");
    //wifi_command("scan\r\n");
    //find_devices();

    unsigned char hash[SHA256_LEN];
    unsigned char text[] = "ZeROSEro7";
    sha256(text, hash);
    
    rtt_printf(0, "SHA3 (ZeROSEro7): ", hash);
    for(int i = 0; i < SHA256_LEN; i++)
        rtt_printf(0, "%02x", hash[i]);

    chThdSleep(TIME_INFINITE);
    return 0;
}
