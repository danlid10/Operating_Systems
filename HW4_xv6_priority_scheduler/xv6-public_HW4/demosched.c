#include "types.h"
#include "user.h"
#define NUM_CHILDREN 24
#define MAX_P 7

// Return the n'th Fibonacci number, calculated recursively.
int fib_rec(int n)
{
    if (n < 2)
        return n;

    return fib_rec(n - 1) + fib_rec(n - 2);
}

// Run CPU bound code and return its runtime in clock ticks.
int CPU_bound_code(void)
{
    int ticks_start, ticks_end;

    ticks_start = uptime();

    // Our CPU bound code: calculating the 35th Fibonacci number recursively.
    fib_rec(35);

    ticks_end = uptime();

    return ticks_end - ticks_start;
}

int main(void)
{
    int PIDs[NUM_CHILDREN], i;

    for (i = 0; i < NUM_CHILDREN; i++)
    {
        PIDs[i] = fork();
        if (PIDs[i] < 0)
        {
            printf(1, "fork failed\n");
            break;
        }
        else if (PIDs[i] == 0) // Child process
        {
            setprio(i % (MAX_P + 1));
            printf(1, "[PID %d] P = %d, CPU bound code runtime =  %d clock ticks\n", getpid(), getprio(), CPU_bound_code());
            exit();
        }
    }

    for (i = 0; i < NUM_CHILDREN; i++)
        wait();

    exit();
}