#include <stdio.h>

int main(void)
{
    int min = (-2147483647);
    int c = 0;

    while(1)
    {
        c = min * 4;
        if (c == 44)
        {
            printf("%d %d\n",c, min);
        }
        ++min;
    }
}