//timer.c

#include "timer.h"

static virtual_timer_t timerv;
static bool            run = false;
static enum color { RED,
                    GREEN,
                    BLUE,
                    BLACK,
                    WHITE } color_t = BLACK;

static void timercb(void *arg)
{
    (void)arg;
    switch(color_t) {
        case BLACK:
            led_rgb(0, 0, 0);
            color_t = WHITE;
            break;
        case WHITE:
            led_rgb(1, 1, 1);
            color_t = RED;
            break;
        case RED:
            led_rgb(1, 0, 0);
            color_t = GREEN;
            break;
        case GREEN:
            led_rgb(0, 1, 0);
            color_t = BLUE;
            break;
        case BLUE:
            led_rgb(0, 0, 1);
            color_t = BLACK;
            break;
        default:
            led_rgb(1, 1, 0);  //YELLOW
            color_t = BLACK;
            break;
    }

    //re arm timer
    if(run) {
        chSysLockFromISR();
        chVTSetI(&timerv, MS2ST(500), timercb, NULL);
        chSysUnlockFromISR();
    }
}

void timer_init(void)
{
    chVTObjectInit(&timerv);
}

void timer_on(void)
{
    chVTSet(&timerv, MS2ST(500), timercb, NULL);
    run = true;
}

void timer_off(void)
{
    run = false;
}
