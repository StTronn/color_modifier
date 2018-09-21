/**
 * converts any two colors of an bmp formate file to
 * any desired two colors.
 *
 * user need to provide RGB value of color he/she wishes to change
 *
 *
 *
 *
 *
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include "bmp.h"


     int orgr1,orgb1,orgg1,orgr2,orgb2,orgg2;
     int outr1,outr2,outb1,outb2,outg1,outg2;


int main(int argc, char* argv[])
{
    // ensure proper usage
    if (argc != 3)
    {
        printf("Usage: ./whodunit infile outfile\n");
        return 1;
    }



    //taking rgb values of two colors to be changed
    printf("\n Enter rgb values of color 1 :");
    scanf("%d %d %d", &orgr1,&orgb1,&orgg1);
    printf("\n Enter rgb values of color 2 :");
    scanf("%d %d %d", &orgr2,&orgb2,&orgg2);
    //taking rgb values of desired two colors to get
    printf("\n Enter output rgb values of color 1 :");
    scanf("%d %d %d", &outr1,&outb1,&outg1);
    printf("\n Enter output rgb values of color 2 :");
    scanf("%d %d %d", &outr2 ,&outb2 ,&outg2);



    // remember filenames
    char* infile = argv[1];
    char* outfile = argv[2];

    // open input file
    FILE* inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        printf("Could not open %s.\n", infile);
        return 2;
    }

    // open output file
    FILE* outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 3;
    }

    // read infile's BITMAPFILEHEADER
    BITMAPFILEHEADER bf;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);

    // read infile's BITMAPINFOHEADER
    BITMAPINFOHEADER bi;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);

    // ensure infile is (likely) a 24-bit uncompressed BMP 4.0
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 ||
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 4;
    }

    // write outfile's BITMAPFILEHEADER
    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, outptr);

    // write outfile's BITMAPINFOHEADER
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, outptr);

    // determine padding for scanlines
    int padding =  (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;

    // iterate over infile's scanlines
    for (int i = 0, biHeight = abs(bi.biHeight); i < biHeight; i++)
    {
        // iterate over pixels in scanline
        for (int j = 0; j < bi.biWidth; j++)
        {
            // temporary storage
            RGBTRIPLE triple;

            // read RGB triple from infile
            fread(&triple, sizeof(RGBTRIPLE), 1, inptr);

            // change all pure red pixels to white pixels
            if (triple.rgbtBlue == orgb1 && triple.rgbtGreen == orgg1 && triple.rgbtRed == orgr1) {

                triple.rgbtBlue = outb1;
                triple.rgbtGreen = outg1;
                triple.rgbtRed = outr1;


            }

            if (triple.rgbtBlue == orgb2 && triple.rgbtGreen == orgg2 && triple.rgbtRed == orgr2) {

                triple.rgbtBlue = outb2;
                triple.rgbtGreen = outg2;
                triple.rgbtRed = outr2;

            }

            // write RGB triple to outfile
            fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
        }

        // skip over padding, if any
        fseek(inptr, padding, SEEK_CUR);

        // then add it back (to demonstrate how)
        for (int k = 0; k < padding; k++)
        {
            fputc(0x00, outptr);
        }
    }

    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // that's all folks
    return 0;
}
