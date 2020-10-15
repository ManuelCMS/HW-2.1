//code implementation of malloc for color to grayscale transformation
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define NUM_THREADS 50

int main()
{
    FILE *image, *outputImage, *lecturas;
    image = fopen("sample.bmp","rb");          //Original image
    outputImage = fopen("imgMalloc2.bmp","wb");    //Transformed image
    double t1, t2, time;

    unsigned char r, g, b;               //Pixel

    unsigned char *rgb, *gs;
    rgb = (unsigned char*)malloc(262145*3*sizeof(unsigned char));
    gs = (unsigned char*)malloc(262145*3*sizeof(unsigned char));
    int j = 0;

    for(int i=0; i<54; i++) fputc(fgetc(image), outputImage);   //Copy the header of the original to the output image

    omp_set_num_threads(NUM_THREADS);

    while(!feof(image)){
        *(rgb + j) = fgetc(image);
        j++;
    }

    t1 = omp_get_wtime();
    #pragma omp parallel
    {
           #pragma omp for ordered
           for(int i =0; i < 262145*3; i+=3){ //Read the channels of each pixel
               r = *(rgb + i);
               g = *(rgb + i + 1);
               b = *(rgb + i + 2);
               unsigned char pixel = 0.21*r+0.72*g+0.07*b; //Obtain grays
               //write the gray pixels to the new image
               #pragma omp ordered
               {
                   *(gs + i) = pixel;
                   *(gs + i + 1) = pixel;
                   *(gs + i + 2) = pixel;
               }
           }
    }

    t2 = omp_get_wtime();
    time = t2 - t1;//execution time
    printf("Time = %f\n",time);

    for(int i = 0; i < 262145*3; i++){
        fputc(*(gs + i), outputImage);
    }
    free(rgb);
    free(gs);
    fclose(image);
    fclose(outputImage);
    return 0;
}
