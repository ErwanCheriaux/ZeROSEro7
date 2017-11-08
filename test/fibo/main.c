//main.c

/*
 * Test of Fibonacci function
 */

#include <stdio.h>
#include "fibo.h"

#define NB_TEST 10

int main(void)
{
    for(int i=0; i<NB_TEST; i++)
        printf("fibo_static(%d) = %d\n", i, fibo_static(i));

    for(int i=0; i<NB_TEST; i++)
        printf("fibo_recursive(%d) = %d\n", i, fibo_recursive(i));
}
