#include "helpers.h"
#include <stdbool.h>
#include <math.h>
#include <stdio.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            RGBTRIPLE currPixel = image[i][j];
            int midValue = (int)(roundf((currPixel.rgbtBlue + currPixel.rgbtGreen + currPixel.rgbtRed) / 3.0));
            image[i][j].rgbtBlue = midValue;
            image[i][j].rgbtGreen = midValue;
            image[i][j].rgbtRed = midValue;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int leftPointer = 0, rightPointer = width - 1; leftPointer < rightPointer; leftPointer++, rightPointer--)
        {
            RGBTRIPLE temp = image[i][leftPointer];
            image[i][leftPointer] = image[i][rightPointer];
            image[i][rightPointer] = temp;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    // Do not mutate image. Because every next pixel count it's value based on previous pixels
    RGBTRIPLE convertedImage[height][width];

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int sumRgbtBlue = 0;
            int sumRgbtGreen = 0;
            int sumRgbtRed = 0;
            float counter = 0;
            int yTopBorder = (i - 1 > 0) ? i - 1 : 0;
            int yBottomBorder = (i + 2 < height) ? i + 2 : height;
            int xLeftBorder = (j - 1 > 0) ? j - 1 : 0;
            int xRightBorder = (j + 2 < width) ? j + 2 : width;

            for (int y = yTopBorder; y < yBottomBorder; y++)
            {
                for (int x = xLeftBorder; x < xRightBorder; x++)
                {
                    sumRgbtBlue += image[y][x].rgbtBlue;
                    sumRgbtGreen += image[y][x].rgbtGreen;
                    sumRgbtRed += image[y][x].rgbtRed;
                    counter++;
                }
            }

            convertedImage[i][j].rgbtBlue = (int)roundf(sumRgbtBlue / counter);
            convertedImage[i][j].rgbtGreen = (int)roundf(sumRgbtGreen / counter);
            convertedImage[i][j].rgbtRed = (int)roundf(sumRgbtRed / counter);
        }
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = convertedImage[i][j];
        }
    }

    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    // Do not mutate image. Because every next pixel count it's value based on previous pixels
    RGBTRIPLE convertedImage[height][width];

    // Count pixels using Sobel filter algorithm
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int xKernelSumRgbtBlue = 0;
            int xKernelSumRgbtGreen = 0;
            int xKernelSumRgbtRed = 0;

            int yKernelSumRgbtBlue = 0;
            int yKernelSumRgbtGreen = 0;
            int yKernelSumRgbtRed = 0;

            int xKernel[3][3] = { { -1, 0, 1 }, { -2, 0, 2 }, { -1, 0, 1 } };
            int yKernel[3][3] = { { -1, -2, -1 }, { 0, 0, 0 }, { 1, 2, 1 } };

            for (int y = i - 1, ky = 0; y < i + 2; y++, ky++)
            {
                for (int x = j - 1, kx = 0; x < j + 2; x++, kx++)
                {
                    // If we outside image border, consider pixel as black
                    bool isInsideImage = y >= 0 && x >= 0 && y < height && x < width;
                    xKernelSumRgbtBlue += (isInsideImage) ? xKernel[ky][kx] * image[y][x].rgbtBlue : 0;
                    xKernelSumRgbtGreen += (isInsideImage) ? xKernel[ky][kx] * image[y][x].rgbtGreen : 0;
                    xKernelSumRgbtRed += (isInsideImage) ? xKernel[ky][kx] * image[y][x].rgbtRed : 0;

                    yKernelSumRgbtBlue += (isInsideImage) ? yKernel[ky][kx] * image[y][x].rgbtBlue : 0;
                    yKernelSumRgbtGreen += (isInsideImage) ? yKernel[ky][kx] * image[y][x].rgbtGreen : 0;
                    yKernelSumRgbtRed += (isInsideImage) ? yKernel[ky][kx] * image[y][x].rgbtRed : 0;
                }
            }

            // Applying Soble filter algorithm to connect x and y kernel.
            int sobelFilterAlgorithmBlue = (int)roundf(sqrt(pow(xKernelSumRgbtBlue, 2) + pow(yKernelSumRgbtBlue, 2)));
            int sobelFilterAlgorithmGreen = (int)roundf(sqrt(pow(xKernelSumRgbtGreen, 2) + pow(yKernelSumRgbtGreen, 2)));
            int sobelFilterAlgorithmRed = (int)roundf(sqrt(pow(xKernelSumRgbtRed, 2) + pow(yKernelSumRgbtRed, 2)));
            convertedImage[i][j].rgbtBlue = (sobelFilterAlgorithmBlue > 255) ? 255 : sobelFilterAlgorithmBlue;
            convertedImage[i][j].rgbtGreen = (sobelFilterAlgorithmGreen > 255) ? 255 : sobelFilterAlgorithmGreen;
            convertedImage[i][j].rgbtRed = (sobelFilterAlgorithmRed > 255) ? 255 : sobelFilterAlgorithmRed;
        }
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = convertedImage[i][j];
        }
    }

    return;
}