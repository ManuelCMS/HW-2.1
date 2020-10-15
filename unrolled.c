//code implementation of the unrolled algorithm
 //Import libraries
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

//functions to multiply the matrix and print the results
void multiply(int a, int b, int c, double m1[a*b], double m2[b*c], double p[a*c]);
void print_txt(FILE* fptr, int a, int c, double m1[a*c]);

int main(){
	//Variables declaration
	FILE* fptr;
	fptr = fopen("matrix_result.txt", "w");
	double t1, t2, time;
	int a, b, c;
	a = 100;
	b = 100;
	c = 100;
	double m1[a*b], m2[b*c], p[a*c];
	//filling the matrices with arbitrary values
	for(int i = 0; i < a; i++){
		for(int j = 0; j < b; j++){
			m1[(i*b)+j] = (double)j;
		}
	}
	for(int i = 0; i < b; i++){
		for(int j = 0; j < c; j++){
			m2[(i*c)+j] = (double)2*j;
		}
	}

	t1 = omp_get_wtime();
	multiply(a, b, c, m1, m2, p);	//Perform multiplication
	t2 = omp_get_wtime();
	time = t2 - t1; //execution time
	printf("Time = %f\b",time);
	//Store results
	print_txt(fptr, a, c, p);
	fclose(fptr);

	return 0;
}

//Unrolled multiplication
void multiply(int a, int b, int c, double m1[a*b], double m2[b*c], double p[a*c]){
	for(int j = 0; j < c; j+=2){
		for(int i = 0; i < a; i++){
			p[(i*c)+j] = 0;
			for(int k = 0; k < b; k++){
				p[(i*c)+j] += (m1[(i*b)+k]*m2[(k*c)+j]);
			}
		}
		for(int i = 0; i < a; i++){
			p[(i*c)+j+1] = 0;
			for(int k = 0; k < b; k++){
				p[(i*c)+j+1] += (m1[(i*b)+k]*m2[(k*c)+j+1]);
			}
		}
	}
}

 //product of the multiplication is written to the text file
void print_txt(FILE* fptr, int a, int c, double m1[a*c]){
	for(int i = 0; i < a; i++){
		for(int j = 0; j < c; j++){
			fprintf(fptr, "%f ", m1[(i*c)+j]);
		}
		fprintf(fptr, "\b");
	}
}
