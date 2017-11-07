#include <stdint.h>
// TI-RTOS Header files
#include <ti/drivers/GPIO.h>
// NoRTOS Header file
#include <NoRTOS.h>
// Example/Board Header files
#include "Board.h"
// TI-DRIVERS Header files
#include <ti/drivers/net/wifi/simplelink.h>

int main(void)
{
    // Must be called to initialize the board
    Board_initGeneral();
    NoRTOS_start();
    Board_initGPIO();

    while(1)
    {
        // Turn on LED
        GPIO_write(Board_LED0, Board_LED_ON);
        ClockP_usleep(500000);
        // Turn off LED
        GPIO_write(Board_LED0, Board_LED_OFF);
        ClockP_usleep(100000);
    }
    return (0);
}
