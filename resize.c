/**
 * copy.c
 *
 * Computer Science 50
 * Problem Set 4
 *
 * Copies a BMP piece by piece, just because.
 */
       
#include <stdio.h>
#include <stdlib.h>

#include "bmp.h"

int main(int argc, char* argv[])
{
    // ensure proper usage
    if (argc != 4)
    {
        printf("Usage: ./resize n infile outfile\n");
        return 1;
    }
    
    // remember resize factor
    int n = atoi(argv[1]);
    
    if ( n <= 0 && n > 100)
    {
        printf("n, the resize factor, must satisfy 0 < n <= 100.");
        return 2;
    }

    // remember filenames
    char* infile = argv[2];
    char* outfile = argv[3];

    // open input file 
    FILE* inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        printf("Could not open %s.\n", infile);
        return 3;
    }

    // open output file
    FILE* outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 4;
    }

    // read infile's BITMAPFILEHEADER
    BITMAPFILEHEADER inbf, outbf;
    fread(&inbf, sizeof(BITMAPFILEHEADER), 1, inptr);

    // read infile's BITMAPINFOHEADER
    BITMAPINFOHEADER inbi, outbi;
    fread(&inbi, sizeof(BITMAPINFOHEADER), 1, inptr);
    outbi = inbi;

    // ensure infile is (likely) a 24-bit uncompressed BMP 4.0
    if (inbf.bfType != 0x4d42 || inbf.bfOffBits != 54 || inbi.biSize != 40 || 
        inbi.biBitCount != 24 || inbi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 4;
    }
    
    
    // change BITMAPINFOHEADER
    outbi = inbi;
    outbi.biWidth = inbi.biWidth * n;
    outbi.biHeight = inbi.biHeight * n;
    int outpadding =  (4 - (outbi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    outbi.biSizeImage = (outbi.biWidth * sizeof(RGBTRIPLE) + outpadding) * abs(outbi.biHeight);
    
    // change BITMAPFILEHEADER
    outbf = inbf;
    outbf.bfSize = outbi.biSizeImage + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

    // write outfile's BITMAPFILEHEADER
    fwrite(&outbf, sizeof(BITMAPFILEHEADER), 1, outptr);

    // write outfile's BITMAPINFOHEADER
    fwrite(&outbi, sizeof(BITMAPINFOHEADER), 1, outptr);
    
    // determine padding for scanlines
    int inpadding =  (4 - (inbi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;

    // iterate over infile's scanlines
    for (int i = 0, inbiHeight = abs(inbi.biHeight); i < inbiHeight; i++)
    {
        RGBTRIPLE temp[inbi.biWidth];
        // iterate over pixels in scanline
        for (int j = 0; j < inbi.biWidth; j++)
        {
            // temporary storage
            RGBTRIPLE triple;

            // read RGB triple from infile
            fread(&triple, sizeof(RGBTRIPLE), 1, inptr);
            
            // remember scanlines
            temp[j] = triple;

            // write RGB triple to outfile
            for(int r = 0; r < n; r++)
            {
                fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
            }
        }

        // skip over padding, if any
        fseek(inptr, inpadding, SEEK_CUR);

        // add paddinf in outfile
        for (int k = 0; k < outpadding; k++)
        {
            fputc(0x00, outptr);
        }
        
        // resize vertically
        for (int k = 1; k < n; k++)
        {
            for (int j = 0; j < inbi.biWidth; j++)
            {
                // write RGB triple to outfile from temp
                for(int r = 0; r < n; r++)
                {
					fwrite(&temp[j], sizeof(RGBTRIPLE), 1, outptr);
                }
            }
            
            // add paddinf in outfile
            for (int k = 0; k < outpadding; k++)
            {
                fputc(0x00, outptr);
            }
        }
    }

    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // that's all folks
    return 0;
}
