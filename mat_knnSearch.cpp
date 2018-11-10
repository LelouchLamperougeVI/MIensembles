#include <stdio.h>
#include <mex.h>
#include <matrix.h>
#include "knnSearch.h"
#include "MI.h"
#include <thread>

#define NUM_THREADS 8 //tweek number of threads for best performance for distinct systems

int M, N, *index, *count, *ret, accum = 0;
double *deconv, *sorted, *A, *I;

void opRow(int i){
        int j, k;
        for(j = i + 1; j < N; j++) {
                for(k = 0; k < count[i]; k++)
                        A[k] = deconv[M*j + index[i*M + k]];
                knnSearch(sorted+(i*M), index+(i*M), A, count[i], ret);

                for(k = 0; k < count[j]; k++)
                        A[k] = deconv[M*i + index[j*M + k]];
                knnSearch(sorted+(j*M), index+(j*M), A, count[j], ret+M);

                I[accum] = MI(ret, M, count[i], count[j]);
                accum++;
                for(k = 0; k < count[i]; k++) ret[index[i*M + k]] = 0.0; //reset array elements
                for(k = 0; k < count[j]; k++) ret[M + index[j*M + k]] = 0.0;
        }
}

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) {
        if(nrhs < 1)
                mexErrMsgTxt("An input matrix is required");
        if(nrhs > 1)
                mexWarnMsgTxt("Ignoring extra input arguments");

        deconv = mxGetPr(prhs[0]);
        M = mxGetM(prhs[0]);
        N = mxGetN(prhs[0]);

        if(M < 2)
                mexErrMsgTxt("Recording has less than 2 sample frames");
        if(N < 2)
                mexErrMsgTxt("At least 2 neurons are required to make comparison");

        int i, j, k;

        index = (int *) malloc(M*N * sizeof(int));
        count = (int *) malloc(N * sizeof(int));
        sorted = (double *) malloc(M*N * sizeof(double));
        struct blockHead *block = (struct blockHead*) malloc(M * sizeof(struct blockHead));

        A = (double *) malloc(M * sizeof(double));
        ret = (int *) calloc(M * 2, sizeof(int));

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

        // int dims[2] = {M, N*(N-1)/2};
        plhs[0] = mxCreateDoubleMatrix(1, N*(N-1)/2, mxREAL);
        I = mxGetPr(plhs[0]);

        std::thread t[NUM_THREADS];
        int *last_elem = (int *) malloc(NUM_THREADS * sizeof(int));
        for(i = 0; i < NUM_THREADS; i++) {
                t[i] = std::thread(opRow, i);
                t[i].detach();
                last_elem[i] = (i + 1)*M - i - 1;
        }
        int next_row = NUM_THREADS;

        while(next_row < N){

        }

        // for(i = 0; i < N; i++) {
        //         opRow(i);
        // }

        free(A); free(ret); free(index); free(count); free(sorted); free(last_elem);
}
