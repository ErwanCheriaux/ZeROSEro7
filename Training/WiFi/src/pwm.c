//pwm.c

#include "pwm.h"

static uint16_t width = 1;

/* PWM LED Intensity Callback */
static void pwmcb0(PWMDriver *pwmp)
{
    (void)pwmp;
    palSetPad(GPIOC, GPIOC_LED);  //led off
}

/* PWM LED Intensity Callback */
static void pwmcb1(PWMDriver *pwmp)
{
    (void)pwmp;
    palClearPad(GPIOC, GPIOC_LED);  //led on
}

static PWMConfig pwmcfg = {
    10000,  /* 10KHz PWM clock frequency */
    100,    /* PWM period in ticks       */
    pwmcb0, /* Callback                  */
    {
        {PWM_OUTPUT_ACTIVE_HIGH, pwmcb1},
        {PWM_OUTPUT_DISABLED, NULL},
        {PWM_OUTPUT_DISABLED, NULL},
        {PWM_OUTPUT_DISABLED, NULL}},
    0, /* TIM CR2 register initialization data  */
    0  /* TIM DIER register initialization data */
};

void pwm_init(void)
{
    pwmStart(&PWMD1, &pwmcfg);
    pwmEnablePeriodicNotification(&PWMD1);
    pwm_off();
}

void pwm_on(void)
{
    pwmEnableChannel(&PWMD1, 0, width);
    pwmEnableChannelNotification(&PWMD1, 0);
}

void pwm_off(void)
{
    pwmEnableChannel(&PWMD1, 0, 0);
    pwmEnableChannelNotification(&PWMD1, 0);
}

/*
 * [       1      -       99      ]
 * [low intensity - high intensity]
 */
void pwm_width(uint16_t w)
{
    //logic ordre
    width = 100 - w;
    pwm_on();
}
