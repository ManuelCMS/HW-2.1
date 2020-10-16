//code implementation of matrix multiplication times a scalar using openmp parallelization
#include <stdio.h>
#include <omp.h>
#define NUM_THREADS 5

int n = 1000;
void evaluacion(int i, float mat[n][n]);

int main(int argc, char const *argv[]) {
  omp_set_num_threads(NUM_THREADS);
  double t, t1, t2;
  float mat[n][n];
  float num = 1.0;
  int j;
  int i;
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      mat[i][j] = j;
    }
  }

t1 = omp_get_wtime();
#pragma omp parallel
{
  #pragma omp for

    for ( i = 0; i < n; i++){
        evaluacion(i, mat);
  }
}
t2 = omp_get_wtime();
t = t2-t1;
printf("t = %f \n", t );

}

void evaluacion(int i, float mat[n][n]){
int j;

      for ( j = 0; j < n; j++){
       printf("%f , ", mat[i][j]*65);
        if (j*1 == 9) {
          printf("\n");
      }

        }
}
// #pragma omp for{
//
// }
//
// }
