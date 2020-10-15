//code implementation of color to grayscale transformation
#include <stdio.h>
#include <stdlib.h>

int main()
{
    FILE *image, *outputImage, *lecturas;
    image = fopen("sample.bmp","rb");          //Original image
    outputImage = fopen("img2_dd.bmp","wb");    //Transformed image
    int c = 0;

    unsigned char r, g, b;               //Pixel

    for(int i=0; i<54; i++) fputc(fgetc(image), outputImage);    //Copy the header of the original to the output image

    while(!feof(image)){                                        //Read input image pixel's channels
       b = fgetc(image);
       g = fgetc(image);
       r = fgetc(image);

       unsigned char pixel = 0.21*r+0.72*g+0.07*b;  //Obtain grays and write to output image
       fputc(pixel, outputImage);
       fputc(pixel, outputImage);
       fputc(pixel, outputImage);

       c++;
    }

    fclose(image);
    fclose(outputImage);
    printf("%d\n", c);
    return 0;
}
