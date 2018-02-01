//pwm.c

#include "pwm.h"

static uint16_t width = 50;
static int      led_r = 0;
static int      led_g = 0;
static int      led_b = 0;

/* PWM LED Intensity Callback */
static void pwmcb0(PWMDriver *pwmp)
{
    (void)pwmp;
    palSetPad(GPIOC, GPIOC_RGB_R);  //led off
    palSetPad(GPIOC, GPIOC_RGB_G);  //led off
    palSetPad(GPIOC, GPIOC_RGB_B);  //led off
}

/* PWM LED Intensity Callback */
static void pwmcb1(PWMDriver *pwmp)
{
    (void)pwmp;
    if(led_r)
        palClearPad(GPIOC, GPIOC_RGB_R);  //led on
    if(led_g)
        palClearPad(GPIOC, GPIOC_RGB_G);  //led on
    if(led_b)
        palClearPad(GPIOC, GPIOC_RGB_B);  //led on
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

void pwm_init(uint16_t w)
{
    width = w;
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

void pwm_width(uint16_t w)
{
    //logic ordre
    width = 100 - w;
}

void pwm_rgb(int r, int g, int b)
{
    led_r = r;
    led_g = g;
    led_b = b;
}

void pwm_r(int r)
{
    led_r = r;
}

void pwm_g(int g)
{
    led_g = g;
}

void pwm_b(int b)
{
    led_b = b;
}