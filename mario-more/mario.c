#include <stdio.h>
#include <cs50.h>

void print_rowC(int bricks);
void print_rowA(int bricks, int length);
void print_rowB(int bricks);
int get_number(void);

int main(void)
{
    int h;
    do
    {
        h = get_int("Define height: ");
    }
    while(h < 1);
    for (int i = 0; i < h; i++)
    {
        print_rowA(i,h);
        print_rowB(i);
        print_rowC(i);
    }
}


void print_rowC(int bricks)
{
    for (int i = 0; i <= bricks; i++)
    {
        printf("#");
    }
    printf("\n");
}

void print_rowA(int bricks, int length)
{
    for(int i = bricks; i < length -1; i++)
    {
        printf(" ");
    }
}

void print_rowB(int bricks)
{
    for (int i = 0; i <= bricks; i++)
    {
        printf("#");
     }
     printf("  ");
}
