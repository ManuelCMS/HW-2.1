//Ian André Ortega Suárez & Manuel Cantero Manzano

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#define NUM_THREADS 60

struct my_pixel{
  unsigned char r, g, b;
};

unsigned char* imgPros(unsigned char* arr_in, unsigned char* arr_out, struct my_pixel* arr_gray,  struct my_pixel* arr_flip, struct my_pixel* arr_blur, int pivot, long ancho, long alto, double kernel[], int m, int n, int kernel_size);
void aVer(char* str, char* str2, int kernel_size);

int main()
{
    int kernel_size = 1;
    double t1,t2;
    printf("Insert kernel size: ");
    scanf("%d", &kernel_size);
    omp_set_num_threads(NUM_THREADS);
    t1 = omp_get_wtime();
    #pragma omp parallel
    {
        #pragma omp sections
        {
            #pragma omp section
            {
                (void)aVer("f1.bmp","f1_sec.bmp", kernel_size);
            }
            #pragma omp section
            {
                (void)aVer("f2.bmp","f2_sec.bmp", kernel_size);
            }
        }
    }
    #pragma omp parallel
    {
        #pragma omp sections
        {
            #pragma omp section
            {
                (void)aVer("f3.bmp","f3_sec.bmp", kernel_size);
            }
            #pragma omp section
            {
                (void)aVer("f4.bmp","f4_sec.bmp", kernel_size);
            }
        }
    }
    #pragma omp parallel
    {
        #pragma omp sections
        {
            #pragma omp section
            {
                (void)aVer("f5.bmp","f5_sec.bmp", kernel_size);
            }
            #pragma omp section
            {
                (void)aVer("f6.bmp","f6_sec.bmp", kernel_size);
            }
        }
    }
    #pragma omp parallel
    {
        #pragma omp sections
        {
            #pragma omp section
            {
                (void)aVer("f7.bmp","f7_sec.bmp", kernel_size);
            }
            #pragma omp section
            {
                (void)aVer("f8.bmp","f8_sec.bmp", kernel_size);
            }
        }
    }
    #pragma omp parallel
    {
        #pragma omp sections
        {
            #pragma omp section
            {
                (void)aVer("f9.bmp","f9_sec.bmp", kernel_size);
            }
            #pragma omp section
            {
                (void)aVer("f10.bmp","f10_sec.bmp", kernel_size);
            }
        }
    }
    t2 = omp_get_wtime();
    printf("Tiempo: %f \n",(t2-t1));
    return 0;
}

void aVer(char* str, char* str2, int kernel_size){
    FILE *image, *outputImage, *lecturas;
    long ancho;
    long alto;
    int pivot = 0;
    image = fopen(str,"rb");          //Imagen original a transformar
                outputImage = fopen(str2,"wb");    //Imagen transformada
                double kernel[kernel_size*kernel_size];
                for(int i = 0; i < kernel_size; i++){
                    for(int j = 0; j < kernel_size; j++){
                        kernel[(i*kernel_size)+j] = 1.0/(double)(kernel_size*kernel_size);
                    }
                }
                int m = kernel_size - 1;
                int n = m/2;

                unsigned char xx[54];
                for(int i=0; i<54; i++) {
                  xx[i] = fgetc(image);
                  fputc(xx[i], outputImage);   //Copia cabecera a nueva imagen
                }
                ancho = (long)xx[21]*(long)xx[20]*65536+(long)xx[19]*256+(long)xx[18];
                alto = (long)xx[24]*65536+(long)xx[23]*256+(long)xx[22];
                printf("largo img %li\n",alto);
                printf("ancho img %li\n",ancho);
                while(((ancho*3)+pivot)%4 != 0){
                    pivot++;
                }
                long test=((ancho*3)+pivot)*alto;
                long test2=ancho*alto;
                long test3=(ancho+m)*(alto+m);
                long int ta=test;
                long int te=test2;
                long int ti=test3;
                unsigned char* arr_in1 = (unsigned char*)malloc(ta*sizeof(unsigned char));
                unsigned char* arr_out1 = (unsigned char*)malloc(ta*sizeof(unsigned char));
                struct my_pixel* arr_gray1 = (struct my_pixel*)malloc(te*sizeof(struct my_pixel));
                struct my_pixel* arr_flip1 = (struct my_pixel*)malloc(te*sizeof(struct my_pixel));
                struct my_pixel* arr_blur1 = (struct my_pixel*)malloc(ti*sizeof(struct my_pixel));

                unsigned char* arr_in2 = (unsigned char*)malloc(ta*sizeof(unsigned char));
                unsigned char* arr_out2 = (unsigned char*)malloc(ta*sizeof(unsigned char));
                struct my_pixel* arr_gray2 = (struct my_pixel*)malloc(te*sizeof(struct my_pixel));
                struct my_pixel* arr_flip2 = (struct my_pixel*)malloc(te*sizeof(struct my_pixel));
                struct my_pixel* arr_blur2 = (struct my_pixel*)malloc(ti*sizeof(struct my_pixel));

                unsigned char* arr_in3 = (unsigned char*)malloc(ta*sizeof(unsigned char));
                unsigned char* arr_out3 = (unsigned char*)malloc(ta*sizeof(unsigned char));
                struct my_pixel* arr_gray3 = (struct my_pixel*)malloc(te*sizeof(struct my_pixel));
                struct my_pixel* arr_flip3 = (struct my_pixel*)malloc(te*sizeof(struct my_pixel));
                struct my_pixel* arr_blur3 = (struct my_pixel*)malloc(ti*sizeof(struct my_pixel));

                unsigned char* arr_in4 = (unsigned char*)malloc(ta*sizeof(unsigned char));
                unsigned char* arr_out4 = (unsigned char*)malloc(ta*sizeof(unsigned char));
                struct my_pixel* arr_gray4 = (struct my_pixel*)malloc(te*sizeof(struct my_pixel));
                struct my_pixel* arr_flip4 = (struct my_pixel*)malloc(te*sizeof(struct my_pixel));
                struct my_pixel* arr_blur4 = (struct my_pixel*)malloc(ti*sizeof(struct my_pixel));

              for(int i = 0; i < (((ancho*3)+pivot)*alto)/4; i++){
                    *(arr_in1 + i) = fgetc(image);
                    *(arr_out1 + i) = *(arr_in1 + i);
                }

                for(int i = 0; i < (((ancho*3)+pivot)*alto)/4; i++){
                    *(arr_in2 + i) = fgetc(image);
                    *(arr_out2 + i) = *(arr_in2 + i);
                }

                for(int i = 0; i < (((ancho*3)+pivot)*alto)/4; i++){
                    *(arr_in3 + i) = fgetc(image);
                    *(arr_out3 + i) = *(arr_in3 + i);
                }

                for(int i = 0; i < (((ancho*3)+pivot)*alto)/4; i++){
                    *(arr_in4 + i) = fgetc(image);
                    *(arr_out4 + i) = *(arr_in4 + i);
                }


                arr_out1=(unsigned char*)imgPros(arr_in1, arr_out1, arr_gray1, arr_flip1, arr_blur1, pivot, ancho, alto, kernel, m, n, kernel_size);

                arr_out2=(unsigned char*)imgPros(arr_in2, arr_out2, arr_gray2, arr_flip2, arr_blur2, pivot, ancho, alto, kernel, m, n, kernel_size);

                arr_out3=(unsigned char*)imgPros(arr_in3, arr_out3, arr_gray3, arr_flip3, arr_blur3, pivot, ancho, alto, kernel, m, n, kernel_size);

                arr_out4=(unsigned char*)imgPros(arr_in4, arr_out4, arr_gray4, arr_flip4, arr_blur4, pivot, ancho, alto, kernel, m, n, kernel_size);
                 free(arr_in1);

                    free(arr_gray1);
                    free(arr_flip1);
                    free(arr_blur1);
                    free(arr_in2);
                    free(arr_gray2);
                    free(arr_flip2);
                    free(arr_blur2);
                    free(arr_in3);
                    free(arr_gray3);
                    free(arr_flip3);
                    free(arr_blur3);
                    free(arr_in4);
                    free(arr_gray4);
                    free(arr_flip4);
                    free(arr_blur4);

    for(int i = 0; i < (((ancho*3)+pivot)*alto)/4; i++){
        fputc(*(arr_out1 + i), outputImage);}
        free(arr_out1);
    for(int i = 0; i < (((ancho*3)+pivot)*alto)/4; i++){
        fputc(*(arr_out2 + i), outputImage);}
        free(arr_out2);
    for(int i = 0; i < (((ancho*3)+pivot)*alto)/4; i++){
        fputc(*(arr_out3 + i), outputImage);}
        free(arr_out3);
    for(int i = 0; i < (((ancho*3)+pivot)*alto)/4; i++){
        fputc(*(arr_out4 + i), outputImage);}
        free(arr_out4);
    fclose(image);
    fclose(outputImage);
}

unsigned char* imgPros(unsigned char* arr_in, unsigned char* arr_out, struct my_pixel* arr_gray,  struct my_pixel* arr_flip, struct my_pixel* arr_blur, int pivot, long ancho, long alto, double kernel[], int m, int n, int kernel_size)
{

    //blanco y negro
    int j= 0;
    struct my_pixel pixel_null;
    pixel_null.b = 0;
    pixel_null.g = 0;
    pixel_null.r = 0;
    int x=0;
    int index = 0;
    int count = 0;
    unsigned char r, g, b;               //Pixel
    for(int i = 0; i < ((ancho*3)+pivot)*alto; i+=3){

            b = *(arr_in + i);
            g = *(arr_in + i + 1);
            r = *(arr_in + i + 2);
            unsigned char pixel = 0.21*r+0.72*g+0.07*b;
            arr_gray[index].b = pixel;
            arr_gray[index].g = pixel;
            arr_gray[index].r = pixel;
            index++;

            count += 3;
            if(count == ancho*3){
                i+=pivot;
                count = 0;
            }

    }

//Efecto Espejo
for(int i = 0; i < alto-1; i++){
        for(int j = 0; j < ancho-1; j++){
            arr_flip[(i*ancho)+j].b = arr_gray[(i*ancho)+ancho-j].b;
            arr_flip[(i*ancho)+j].g = arr_gray[(i*ancho)+ancho-j].g;
            arr_flip[(i*ancho)+j].r = arr_gray[(i*ancho)+ancho-j].r;

        }
    }

//Efecto Blur
    for(int i = 0; i < alto+m; i++){
        for(int j = 0; j < ancho+m; j++){
            arr_blur[(i*(ancho+m))+j] = pixel_null;
        }
    }

    for(int i = 0; i < alto; i++){
        for(int j = 0; j < ancho; j++){
            arr_blur[((i+n)*(ancho+m))+j+n] = arr_flip[(i*ancho)+j];
        }
    }

    double suma_b = 0;
    double suma_g = 0;
    double suma_r = 0;
    for(int i = 0; i < alto; i++){
        for(int j = 0; j < ancho; j++){
            for(int k = 0; k < kernel_size; k++){
                for(int l = 0; l < kernel_size; l++){
                    suma_b += (double)(arr_blur[((i+k)*(ancho+m))+j+l].b)*kernel[(k*kernel_size)+l];
                    suma_g += (double)(arr_blur[((i+k)*(ancho+m))+j+l].g)*kernel[(k*kernel_size)+l];
                    suma_r += (double)(arr_blur[((i+k)*(ancho+m))+j+l].r)*kernel[(k*kernel_size)+l];
                }
            }
            arr_flip[(i*ancho)+j].b = (unsigned char)(suma_b);
            arr_flip[(i*ancho)+j].g = (unsigned char)(suma_g);
            arr_flip[(i*ancho)+j].r = (unsigned char)(suma_r);
            suma_b = 0;
            suma_g = 0;
            suma_r = 0;
        }
    }

    index = 0;
    count = 0;
    for(int i = 0; i < ((ancho*3)+pivot)*alto; i+=3){
            *(arr_out + i) = arr_flip[index].b;
            *(arr_out + i + 1) = arr_flip[index].g;
            *(arr_out + i + 2) = arr_flip[index].r;
            index++;

            count += 3;
            if(count == ancho*3){
                i+=pivot;
                count = 0;
            }

    }

    return arr_out;
}
