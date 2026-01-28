#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

int converter (string word);

int main (void)
{
    //return score
    string p1 = get_string("Player 1: ");
    string p2 = get_string("Player 2: ");
    int s1 = converter(p1);
    int s2 = converter(p2);
    if (s1 > s2)
    {
        printf("Player 1 wins!\n");
    }
    else if (s2 > s1)
    {
        printf("Player 2 wins!\n");
    }
    else
    {
        printf("Tie!\n");
    }
}

//Converter
int converter (string word)
{
    char ten[] = {'z','q'};
    char eight[] = {'j','x'};
    char five[] = {'k'};
    char four[] = {'f','h','v','w','y'};
    char three[] = {'b','c','m','p'};
    char two[] = {'d','g'};
    char one[] = {'a','e','i','l','n','o','r','s','t','u'};
    int n = 0;
    for (int i = 0; i < strlen(word); i++)
    {
        if (strchr(ten,tolower(word[i])))
        {n += 10;}
        else if (strchr(eight,tolower(word[i])))
        {n += 8;}
        else if (strchr(five,tolower(word[i])))
        {n += 5;}
        else if (strchr(four,tolower(word[i])))
        {n += 4;}
        else if (strchr(three,tolower(word[i])))
        {n += 3;}
        else if (strchr(two,tolower(word[i])))
        {n += 2;}
        else if (strchr(one,tolower(word[i])))
        n += 1;
    }
    return n;
}
