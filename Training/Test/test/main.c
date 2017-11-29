//main.c

/*
 * Test all of the folling items:
 *
 * - Fibonacci function
 *
 */

#include <stdio.h>
#include "fibo/fibo.h"

#define NB_TEST 10

static int fibo_result[NB_TEST];

int main(void)
{
    fibo_result[0] = 1;
    fibo_result[1] = 1;
    fibo_result[2] = 2;
    fibo_result[3] = 3;
    fibo_result[4] = 5;
    fibo_result[5] = 8;
    fibo_result[6] = 13;
    fibo_result[7] = 21;
    fibo_result[8] = 34;
    fibo_result[9] = 55;

    for(int i = 0; i < NB_TEST; i++)
        if(fibo_static(i) != fibo_result[i])
            return 1;

    for(int i = 0; i < NB_TEST; i++)
        if(fibo_recursive(i) != fibo_result[i])
            return 1;

    return 0;
}