/**
 * Copies a BMP piece by piece, just because.
 */
       
#include <stdio.h>
#include <stdlib.h>

#include "bmp.h"

int main(int argc, char *argv[])
{
    // ensure proper usage
    if (argc != 4)
    {
        fprintf(stderr, "Usage: ./resize n infile outfile\n");
        return 1;
    }
    
    //remember resize factor (n)
    int n = atoi(argv[1]);

    // remember filenames
    char *infile = argv[2];
    char *outfile = argv[3];
    
    //validate resize factor (n)
    if (n < 1 || n > 100)
    {
        fprintf(stderr, "Resize factor (n) must be a positive integer less than or equal to 100");
        return 1;
    }


    // open input file 
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 2;
    }

    // open output file
    FILE *outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 3;
    }

    // read infile's BITMAPFILEHEADER and create new BITMAPFILEHEADER for resized copy
    BITMAPFILEHEADER bf, bf_n;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);
    bf_n = bf;

    // read infile's BITMAPINFOHEADER and create new BITMAPINFOHEADER for resized copy
    BITMAPINFOHEADER bi, bi_n;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);
    bi_n = bi;

    // ensure infile is (likely) a 24-bit uncompressed BMP 4.0
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 || 
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 4;
    }
    
    // update size dimensions
    bi_n.biWidth = bi.biWidth * n;
    bi_n.biHeight = bi.biHeight * n;
    
    // get infile and outfile padding using refactor value of (n)
    int infilePadding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    int outfilePadding = (4 - (bi_n.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    
    // update header files with image sizes
    bi_n.biSizeImage = ((sizeof(RGBTRIPLE) * bi_n.biWidth) + outfilePadding) * abs(bi_n.biHeight);
    bf_n.bfSize = bi_n.biSizeImage + sizeof(bf_n) + sizeof(bi_n);

    // write outfile's BITMAPFILEHEADER
    fwrite(&bf_n, sizeof(BITMAPFILEHEADER), 1, outptr);

    // write outfile's BITMAPINFOHEADER
    fwrite(&bi_n, sizeof(BITMAPINFOHEADER), 1, outptr);

      // iterate over infile's scanlines (rows)
    for (int i = 0, biHeight = abs(bi.biHeight); i < biHeight; i++)
    {
        // Read/write each scanline refactor value of n times
        for (int j = 0; j < n; j++)
        {  
            // iterate over pixels in scanline (pixels)
            for (int k = 0; k < bi.biWidth; k++)
            {
                // temporary storage
                RGBTRIPLE triple;

                // read RGB triple from infile
                fread(&triple, sizeof(RGBTRIPLE), 1, inptr);

                // write RGB triple to outfile, multiplied by factor
                for (int l = 0; l < n; l++)
                    fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);    
            }
            
            // Add a new padding
            for (int l = 0; l < outfilePadding; l++)
                fputc(0x00, outptr);
            
            // Return to the beginning of a scanline
            if (j < n - 1)
                fseek(inptr, -bi.biWidth * sizeof(RGBTRIPLE), SEEK_CUR);
        }

        // Skip over padding, if any
        fseek(inptr, infilePadding, SEEK_CUR);
    }

    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // success
    return 0;
}
