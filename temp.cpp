#include <stdio.h>
#include "knnSearch.h"
#include "MI.h"

int main(){
        int i, j, k;
        int M=8;
        int N=3;
        double deconv[N*M] = {
                0.0, 3.2, 7.5, 0.0, 1.8, 4.5, 8.9, 5.6,
                0.0, 0.0, 4.2, 8.5, 3.6, 4.5, 0.0, 0.0,
                0.0, 3.2, 4.2, 0.0, 3.6, 4.5, 0.0, 0.0
        };
        int *index = (int *) malloc(M*N * sizeof(int));
        int *count = (int *) malloc(N * sizeof(int));
        double *sorted = (double *) malloc(M*N * sizeof(double));
        struct blockHead *block = (struct blockHead*) malloc(M * sizeof(struct blockHead));

        double *A = (double *) malloc(M * sizeof(double));
        int *ret = (int *) calloc(M*2, sizeof(int));

        for(i = 0; i < N; i++) {
                for(j = 0; j < M; j++) {
                        block[j].block = deconv[i*M + j];
                        block[j].index = j;
                }
                qsort(block, M, sizeof(struct blockHead), compare);
                j = 0;
                while((M-j) && (block[j].block != 0.0)) { //reorganize memory for sequential addresssing
                        index[i*M + j] = block[j].index;
                        sorted[i*M + j] = block[j].block;
                        j++;
                }
                count[i] = j;
        }
        free(block);

        for(i = 0; i < N; i++) {
                for(j = i + 1; j < N; j++) {
                        for(k = 0; k < count[i]; k++)
                                A[k] = deconv[M*j + index[i*M + k]];
                        knnSearch(sorted+(i*M), index+(i*M), A, count[i], ret);

                        for(k = 0; k < count[j]; k++)
                                A[k] = deconv[M*i + index[j*M + k]];
                        knnSearch(sorted+(j*M), index+(j*M), A, count[j], ret+M);

                        printf("%f ", MI(ret, M, count[i], count[j]));

                        for(k = 0; k < count[i]; k++) ret[index[i*M + k]] = 0.0; //reset array elements
                        for(k = 0; k < count[j]; k++) ret[M + index[j*M + k]] = 0.0;
                }
                printf("\n");
        }
}
