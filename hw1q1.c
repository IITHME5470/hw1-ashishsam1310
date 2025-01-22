#include <stdio.h>
#include <stdlib.h>


void generate_file(int n,double **A,int format_flag) {
    char filename[30];
    sprintf(filename,"array_%06d_%s.out",n,format_flag==0?"asc":"bin");
    FILE *file=fopen(filename,format_flag==0?"w":"wb");
    int i=0; int j=0;
    if(format_flag==0) {
        for(i=0;i<n;i++) {
            for(j=0;j<n;j++) {
                fprintf(file,"%.15lf ",A[i][j]);
            }
            fprintf(file,"\n");
        }
    } else {
        for(i=0;i<n;i++) {
            fwrite(A[i],sizeof(double),n,file);
        }
    }
    fclose(file);
}


int main() {
    FILE *input_file=fopen("input.in","r");
    int n;
    fscanf(input_file,"%d",&n);
    fclose(input_file);

    double **A=(double **)malloc(n*sizeof(double *));
    int i=0; int j=0;
    for(i=0;i<n;i++) {
        A[i]=(double *)malloc(n*sizeof(double));
        for(j=0;j<n;j++) {
            A[i][j]=i+j;
        }
    }
    int formatFlag=1; //changeable; 0->ascii, 1->binary
    generate_file(n, A, formatFlag);
    
    for(i=0;i<n;i++) {
        free(A[i]);
    }
    free(A);

    return 0;
}

