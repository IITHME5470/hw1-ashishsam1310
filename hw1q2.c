#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define TOLERANCE 1e-12

void read_matrix(const char *filename, int n, double **matrix) {
    FILE *file = fopen(filename, "r");
    int i, j;
    for (i=0;i<n;i++) {
        for (j=0;j<n;j++) {
            fscanf(file, " %lf,", &matrix[i][j]);}}
    fclose(file);
}


void read_vector(const char *filename, int n, double *vector) {
    FILE *file = fopen(filename, "r");
    int i;
    for (i=0;i<n;i++) {
        fscanf(file, "%lf,", &vector[i]);
    }
    fclose(file);
}



int is_eigen(double **matrix, double *vector, int n, double *eigenvalue) {
    double *result = malloc(n * sizeof(double));
    double ratio = 0.0;
    int is_eigen = 1; 
    int first_non_zero = 0;
    int i, j;    
    for (i=0;i<n;i++) {
        result[i]=0.0;
        for (j=0;j<n;j++) {
            result[i] += matrix[i][j]*vector[j];
        }  
    }
    for (i=0;i<n;i++) {
        if (fabs(vector[i])>TOLERANCE) { 
            double current_ratio = result[i]/vector[i];

              if (fabs(first_non_zero)<TOLERANCE) {
                ratio = current_ratio;
                first_non_zero=1;
            } else if (fabs(current_ratio-ratio)>TOLERANCE) {
                is_eigen=0; 
                break;
            }
        } else if (fabs(result[i]) > TOLERANCE) {
            is_eigen=0; 
            break;
        }
    }

    free(result);
    if (is_eigen) {
        *eigenvalue = ratio; 
    }
    return is_eigen;
}


void append_eigen(const char *filename, double eigenvalue) {
    FILE *file = fopen(filename, "a");
    fprintf(file, "\n%.15e", eigenvalue);
    fclose(file);
}



int main() {
    FILE *input_file = fopen("input.in", "r");
    int n;
    fscanf(input_file, "%d", &n);
    fclose(input_file);

    char matrix_file[30];
    sprintf(matrix_file, "mat_%06d.in", n);
    double **matrix = malloc(n * sizeof(double *));
    int i;
    for (i=0;i<n;i++) {
        matrix[i] = malloc(n * sizeof(double));
    }

    read_matrix(matrix_file, n, matrix);

    int vecnum;
    for (vecnum = 1;; vecnum++) {
        char vector_file[40];
        sprintf(vector_file, "vec_%06d_%06d.in", n, vecnum);

        FILE *file = fopen(vector_file, "r");
        if (!file) {
            break;
        }
        fclose(file);
        double *vector = malloc(n * sizeof(double));
        read_vector(vector_file, n, vector);

        double eigenvalue;
        if (is_eigen(matrix, vector, n, &eigenvalue)) {
            printf("%s:Yes:%e\n", vector_file, eigenvalue);
            append_eigen(vector_file, eigenvalue);
        }
	 else {
            printf("%s:Not an eigenvector\n", vector_file);
        }
        free(vector);
    }

    for (i=0;i<n;i++) {
        free(matrix[i]);
    }
    free(matrix);
    return 0;
}

