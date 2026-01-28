#include <stdio.h>
#include <stdlib.h>
#include<cs50.h>
#include <string.h>

long CardNum (void);
long Counter (long x);
string CardStart (long y, long x);
int CardProduct (long y);

int main (void)
{
    long cn = CardNum();
    long cc = Counter(cn);
    string cs = CardStart(cn,cc);
    int cp = CardProduct(cn);
    string answer = "INVALID";
    //printf("Card: %ld\n",cn);
    printf("Length: %ld\n",cc);
    printf("Type: %s\n",cs);
    printf("Sum: %i\n",cp);
    if (cc == 15 && strcmp(cs, "AMEX") == 0 && cp == 0)
    {
        answer = "AMEX";
    }
    else if (cc == 16 && strcmp(cs, "MASTERCARD") == 0 && cp == 0)
    {
        answer = "MASTERCARD";
    }
    else if ((cc == 13 || cc == 16) && strcmp(cs, "VISA") == 0 && cp == 0)
    {
        answer = "VISA";
    }
    printf("%s\n",answer);
}

long CardNum (void)
{
    long x;
    do
    {
        x = get_long("Card Number: ");
    }
    while (false);
    return x;
}

long Counter (long x)
{
    long count = 0;
    if (x ==0)
    {
        count = 1;
    }
    else
    {
        while (x > 0)
        {
            x /= 10;
            count++;
        }
    }
    return count;
}

string CardStart (long y, long x)
{
    long z1 = y;
    long z2 = y;
    string z = "NOT VALID";
    for (int i = 0; i < x - 2; i++)
    {
        z1 = z1 / 10;
    }
    for (int i = 0; i < x - 1; i++)
    {
        z2 = z2 / 10;
    }
    if (z1 == 34 || z1 == 37)
    {
        z = "AMEX";
    }
    else if (z1 == 51 || z1 == 52 || z1 == 53 || z1 == 54 || z1 == 55)
    {
        z = "MASTERCARD";
    }
    else if(z2 == 4)
    {
        z = "VISA";
    }
    return z;
}

int CardProduct (long y)
{
    long p1 = y/10;
    long p2 = y;
    int s1 = 0;
    int s2 = 0;
    while (p1 > 0)
    {
        s1 += (2 * (p1 % 10) >= 10) ? 1 + (2 * (p1 % 10)) % 10 : 2 * (p1 % 10);
        p1 /= 100;
    }
    while (p2 > 0)
    {
        s2 = s2 + (p2 % 10);
        p2 /= 100;
    }
    int s = (s1 + s2) % 10;
    return s;
}
