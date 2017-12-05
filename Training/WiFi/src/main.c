//main.c

#include "ch.h"
#include "hal.h"

#include "led.h"
#include "pwm.h"
#include "timer.h"
#include "wifi.h"
#include "rtt.h"
#include "sha3.h"

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
    wifi_command("scan\r\n");
    find_devices();

    sha3_context c;
    uint8_t *hash;
    sha3_Init256(&c);
    sha3_Update(&c, "ZeROSEro7", 9);
    hash = sha3_Finalize(&c);
    
    rtt_printf(0, "SHA3 (ZeROSEro7): ", hash);
    for(int i = 0; hash[i] != '\0'; i++)
        rtt_printf(0, "%x", hash[i]);

    chThdSleep(TIME_INFINITE);
    return 0;
}
