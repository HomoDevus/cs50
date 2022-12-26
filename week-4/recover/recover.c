#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <stdint.h>
#include <stdbool.h>

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Wrong number of command line arguments. There should be one argument.\n");
        return 1;
    }

    char *infile = argv[optind];

    // Open input file
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        printf("Could not open %s.\n", infile);
        return 1;
    }

    int BLOCK_SIZE = 512;
    uint8_t buffer[BLOCK_SIZE];
    int fileCounter = 0;
    char *fileName = malloc(8);
    bool hadFirstJpeg = false;
    FILE *outImg;

    while (fread(buffer, 1, BLOCK_SIZE, inptr) == BLOCK_SIZE)
    {
        bool isJpegStart = buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && buffer[3] >= 0xe0 && buffer[3] <= 0xef;

        if (!hadFirstJpeg && isJpegStart)
        {
            hadFirstJpeg = true;
        }

        if (isJpegStart)
        {
            if (fileCounter)
            {
                fclose(outImg);
            }

            sprintf(fileName, "%03i.jpg", fileCounter);
            outImg = fopen(fileName, "w");
            fileCounter++;
        }

        if (hadFirstJpeg)
        {
            fwrite(buffer, 1, BLOCK_SIZE, outImg);
        }
    }

    // Garbage clean
    fclose(inptr);
    if (fileCounter)
    {
        fclose(outImg);
    }
    free(fileName);
}