//Ian André Ortega Suárez & Manuel Cantero Manzano
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#define NUM_THREADS 20

struct my_pixel{
  unsigned char r, g, b;
};

double t1, t2, totalT;

void aVer(char* input_img, char* output_img, int kernel_size);


int main(){
  omp_set_num_threads(NUM_THREADS);
    int kernel_size = 1;
    double t1,t2;
    printf("Insert kernel size: ");
    scanf("%d", &kernel_size);
    t1 = omp_get_wtime();
  #pragma omp parallel
  {
    #pragma omp sections
    {
      #pragma omp section
      {
        (void)aVer("f1.bmp", "f1_sch.bmp", kernel_size);
      }
      #pragma omp section
      {
        (void)aVer("f2.bmp", "f2_sch.bmp", kernel_size);
      }
      #pragma omp section
      {
        (void)aVer("f3.bmp", "f3_sch.bmp", kernel_size);
      }
      #pragma omp section
      {
        (void)aVer("f4.bmp", "f4_sch.bmp", kernel_size);
      }
      #pragma omp section
      {
        (void)aVer("f5.bmp", "f5_sch.bmp", kernel_size);
      }
      #pragma omp section
      {
        (void)aVer("f6.bmp", "f6_sch.bmp", kernel_size);
      }
      #pragma omp section
      {
        (void)aVer("f7.bmp", "f7_sch.bmp", kernel_size);
      }
      #pragma omp section
      {
        (void)aVer("f8.bmp", "f8_sch.bmp", kernel_size);
      }
      #pragma omp section
      {
        (void)aVer("f9.bmp", "f9_sch.bmp", kernel_size);
      }
      #pragma omp section
      {
        (void)aVer("f10.bmp", "f10_sch.bmp", kernel_size);
      }
    }
  }
  t2 = omp_get_wtime();
  printf("Tiempo: %f \n", (t2 - t1));
  return 0;
}

void aVer(char* input_img, char* output_img, int kernel_size){
    FILE *image, *outputImage, *lecturas;
    long ancho;
    long alto;
    int pivot = 0;
    image = fopen(input_img, "rb");           //Imagen original a transformar
    outputImage = fopen(output_img, "wb");    //Imagen transformada
    unsigned char r, g, b;                    //Pixel
    double kernel[kernel_size * kernel_size];
    for (int i = 0; i < kernel_size; i++)
    {
      for (int j = 0; j < kernel_size; j++)
      {
        kernel[(i * kernel_size) + j] = 1.0 / (double)(kernel_size * kernel_size);
      }
    }
    int m = kernel_size - 1;
    int n = m / 2;

    unsigned char xx[54];
    for (int i = 0; i < 54; i++)
    {
      xx[i] = fgetc(image);
      fputc(xx[i], outputImage); //Copia cabecera a nueva imagen
    }
    ancho = (long)xx[21] * (long)xx[20] * 65536 + (long)xx[19] * 256 + (long)xx[18];
    alto = (long)xx[24] * 65536 + (long)xx[23] * 256 + (long)xx[22];
    printf("largo img %li\n", alto);
    printf("ancho img %li\n", ancho);

    while (((ancho * 3) + pivot) % 4 != 0)
    {
      pivot++;
    }

    unsigned char *arr_in = (unsigned char *)malloc(((ancho * 3) + pivot) * alto * sizeof(unsigned char));
    unsigned char *arr_out = (unsigned char *)malloc(((ancho * 3) + pivot) * alto * sizeof(unsigned char));
    struct my_pixel *arr_gray = (struct my_pixel *)malloc(ancho * alto * sizeof(struct my_pixel));
    struct my_pixel *arr_flip = (struct my_pixel *)malloc(ancho * alto * sizeof(struct my_pixel));
    struct my_pixel *arr_blur = (struct my_pixel *)malloc((ancho + m) * (alto + m) * sizeof(struct my_pixel));

    int j = 0;
    struct my_pixel pixel_null;
    pixel_null.b = 0;
    pixel_null.g = 0;
    pixel_null.r = 0;

    for (int i = 0; i < ((ancho * 3) + pivot) * alto; i++)
    {
      *(arr_in + i) = fgetc(image);
      *(arr_out + i) = *(arr_in + i);
    }
    //Imagen a blanco y negro
    int index = 0;
    int count = 0;

    #pragma omp parallel
    {
      #pragma omp for ordered schedule(static,1)
        for (int i = 0; i < ((ancho * 3) + pivot) * alto; i += 3){
          b = *(arr_in + i);
          g = *(arr_in + i + 1);
          r = *(arr_in + i + 2);
          unsigned char pixel = 0.21 * r + 0.72 * g + 0.07 * b;
          arr_gray[index].b = pixel;
          arr_gray[index].g = pixel;
          arr_gray[index].r = pixel;
          index++;

          count += 3;
          if (count == ancho * 3){
            i += pivot;
            count = 0;
          }
        }

    //Efecto Espejo
      for (int i = 0; i < alto - 1; i++)
      {
        for(int j = 0; j < ancho-1; j++){
          arr_flip[(i*ancho)+j].b = arr_gray[(i*ancho)+ancho-j].b;
          arr_flip[(i*ancho)+j].g = arr_gray[(i*ancho)+ancho-j].g;
          arr_flip[(i*ancho)+j].r = arr_gray[(i*ancho)+ancho-j].r;

        }
      }

    }
    //Efecto Blur
    for (int i = 0; i < alto + m; i++)
    {
      for (int j = 0; j < ancho + m; j++)
      {
        arr_blur[(i * (ancho + m)) + j] = pixel_null;
      }
    }

    for (int i = 0; i < alto; i++)
    {
      for (int j = 0; j < ancho; j++)
      {
        arr_blur[((i + n) * (ancho + m)) + j + n] = arr_flip[(i * ancho) + j];
      }
    }

    double suma_b = 0;
    double suma_g = 0;
    double suma_r = 0;
    for (int i = 0; i < alto; i++)
    {
      for (int j = 0; j < ancho; j++)
      {
        for (int k = 0; k < kernel_size; k++)
        {
          for (int l = 0; l < kernel_size; l++)
          {
            suma_b += (double)(arr_blur[((i + k) * (ancho + m)) + j + l].b) * kernel[(k * kernel_size) + l];
            suma_g += (double)(arr_blur[((i + k) * (ancho + m)) + j + l].g) * kernel[(k * kernel_size) + l];
            suma_r += (double)(arr_blur[((i + k) * (ancho + m)) + j + l].r) * kernel[(k * kernel_size) + l];
          }
        }
        arr_flip[(i * ancho) + j].b = (unsigned char)(suma_b);
        arr_flip[(i * ancho) + j].g = (unsigned char)(suma_g);
        arr_flip[(i * ancho) + j].r = (unsigned char)(suma_r);
        suma_b = 0;
        suma_g = 0;
        suma_r = 0;
      }
    }

    index = 0;
    count = 0;
    for (int i = 0; i < ((ancho * 3) + pivot) * alto; i += 3)
    {
      *(arr_out + i) = arr_flip[index].b;
      *(arr_out + i + 1) = arr_flip[index].g;
      *(arr_out + i + 2) = arr_flip[index].r;
      index++;

      count += 3;
      if (count == ancho * 3)
      {
        i += pivot;
        count = 0;
      }
    }

    for (int i = 0; i < ((ancho * 3) + pivot) * alto; i++)
    {
      fputc(*(arr_out + i), outputImage);
    }

    t2 = omp_get_wtime();
    totalT = t2 - t1;
    printf("Time: %.3f s\n", totalT);
    free(arr_in);
    free(arr_out);
    free(arr_gray);
    free(arr_flip);
    free(arr_blur);
    fclose(image);
    fclose(outputImage);
}
