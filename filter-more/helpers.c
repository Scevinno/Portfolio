#include "helpers.h"
#include <math.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            int r, g, b;
            float sum;
            r = image[h][w].rgbtRed;
            g = image[h][w].rgbtGreen;
            b = image[h][w].rgbtBlue;
            sum = (r+g+b)/3;
            image[h][w].rgbtRed = (int) sum;
            image[h][w].rgbtGreen = (int) sum;
            image[h][w].rgbtBlue = (int) sum;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int h = 0; h < height ; h++)
    {
        for (int w = 0; w <= width/2; w++)
        {
            int r1, g1, b1, r2, g2, b2;
            r1 = image[h][w].rgbtRed;
            g1= image[h][w].rgbtGreen;
            b1 = image[h][w].rgbtBlue;
            r2 = image[h][width-w].rgbtRed;
            g2 = image[h][width-w].rgbtGreen;
            b2 = image[h][width-w].rgbtBlue;
            image[h][w].rgbtRed = r2;
            image[h][w].rgbtGreen = g2;
            image[h][w].rgbtBlue = b2;
            image[h][width-w].rgbtRed = r1;
            image[h][width-w].rgbtGreen = g1;
            image[h][width-w].rgbtBlue = b1;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE temp[height][width];

    for (int h = 0; h < height ; h++)
    {
        for (int w = 0; w < width; w++)
        {
            temp[h][w] = image[h][w];
        }
    }

    for (int h = 0; h < height ; h++)
    {
        for (int w = 0; w < width; w++)
        {
            float counter = 0.00;
            int sum = 0;
            int totalRed, totalGreen, totalBlue;
            totalRed = totalBlue = totalGreen = 0;

            for (int x = -1; x < 2; x++)
            {
                for (int y = -1; y < 2; y++)
                {
                    if (h+x < 0 || h+x > (height-1) || w+y < 0 || w+y > (width-1))
                    {
                        continue;
                    }
                        totalRed += temp[h+x][w+y].rgbtRed;
                        totalGreen += temp[h+x][w+y].rgbtGreen;
                        totalBlue += temp[h+x][w+y].rgbtBlue;

                        counter ++;
                }
                image[h][w].rgbtRed = round(totalRed / counter);
                image[h][w].rgbtGreen = round(totalGreen / counter);
                image[h][w].rgbtBlue = round(totalBlue / counter);

            }
        }
    }

    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE temp[height][width];

    for (int h = 0; h < height ; h++)
    {
        for (int w = 0; w < width; w++)
        {
            temp[h][w] = image[h][w];
        }
    }

    int Gx[3][3] = {{-1,0,1},{-2,0,2},{-1,0,1}};
    int Gy[3][3] = {{-1,-2,-1},{0,0,0},{1,2,1}};


    for (int h = 0; h < height ; h++)
    {
        for (int w = 0; w < width; w++)
        {
            int redX = 0;
            int redY = 0;
            int greenX = 0;
            int greenY = 0;
            int blueX = 0;
            int blueY = 0;

            for (int x = 0; x < 3; x++)
            {
                for (int y = 0; y < 3; y++)
                {
                    if (h-1+x < 0 || h-1+x > (height-1) || w-1+y < 0 || w-1+y > (width-1))
                    {
                        continue;
                    }

                    redX = redX + (temp[h-1+x][w-1+y].rgbtRed * Gx[x][y]);
                    greenX = greenX + (temp[h-1+x][w-1+y].rgbtGreen * Gx[x][y]);
                    blueX = blueX + (temp[h-1+x][w-1+y].rgbtBlue * Gx[x][y]);

                    redY = redY + (temp[h-1+x][w-1+y].rgbtRed * Gy[x][y]);
                    greenY = greenY + (temp[h-1+x][w-1+y].rgbtGreen * Gy[x][y]);
                    blueY = blueY + (temp[h-1+x][w-1+y].rgbtBlue * Gy[x][y]);
                }
            }

            int red = round(sqrt((redX * redX) + (redY * redY)));
            int green = round(sqrt((greenX * greenX) + (greenY * greenY)));
            int blue = round(sqrt((blueX * blueX) + (blueY * blueY)));

            if (red > 255)
            {
                red = 255;
            }
            if (green > 255)
            {
                green = 255;
            }
            if (blue > 255)
            {
                blue = 255;
            }

            image[h][w].rgbtRed = red;
            image[h][w].rgbtGreen = green;
            image[h][w].rgbtBlue = blue;

        }
    }

    return;
}
