#include <ctype.h>
#include <cs50.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

int Sentences (string x);
int Words (string x);
int Letters (string x);

int main(void)
{
    // Prompt the user for some text
    string input = get_string("Enter sentence: ");
    // Count the number of letters, words, and sentences in the text
    int s = Sentences(input);
    int w = Words(input);
    int l = Letters(input);
    float L = l/(w/100.0);
    float S = s/(w/100.0);
    // Compute the Coleman-Liau index
    float index = 0.0588 * L - 0.296 * S - 15.8;

    printf("%i,%i,%i",s,w,l);

    string answer;
    if  (index < 1)
    {
        answer = "Before Grade 1";
        printf("%s\n",answer);
    }
    else if (index > 16)
    {
        answer = "Grade 16+";
        printf("%s\n",answer);
    }
    else
    {
        answer = "Grade";;
        printf("%s %i\n",answer, (int) index);
    }
}

int Sentences (string x)
{
    int n = 0;
    for (int i = 0; i < strlen(x);i++)
    {
        if (x[i] == '.' || x[i] == '!' || x[i] == '?')
        {
            n += 1;
        }
    }
    return n;
}

int Words (string x)
{
    int n = 0;
    for (int i = 0; i < strlen(x);i++)
    {
        if (x[i] == ' ' && x[i] != x[i+1])
        {
            n += 1;
        }
    }
    return n+1;
}

int Letters (string x)
{
    int n = 0;
    for (int i = 0; i < strlen(x);i++)
    {
        if (x[i] != ' ' && x[i] != '.' && x[i] != '!' && x[i] != '?')
        {
            n += 1;
        }
    }
    return n;
}
