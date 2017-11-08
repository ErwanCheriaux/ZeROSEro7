//fibo.c

#include "fibo.h"

int fibo_static(int n)
{
    static int fibo_table[3];
    fibo_table[0] = 1;
    fibo_table[1] = 1;
    for(int i=2; i<n; i++)
    {
        fibo_table[i%3] = fibo_table[(i-1)%3] + fibo_table[(i-2)%3];
    }
    return fibo_recursive[n%3];
}

int fibo_recursive(int n)
{
   if(n == 0 || n == 1)
       return 1;
   else
       return (fibo_recursive(n-1) + fibo_recursive(n-2));
}
