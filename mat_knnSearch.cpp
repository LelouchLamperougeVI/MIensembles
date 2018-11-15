/* Estimator of pair-wise mutual information for two-photon calcium imaging data
*  Based on the KSG estimator (Kraskov, Stogbauer & Grassberger, 2004, Phys. Rev. E)
*
*  Usage:
*     [I, D, D_shuffled] = mat_knnSearch(deconv, num_shuffles, prctile)
*
*  Inputs:
*     deconv - Deconvolved matrix of calcium time series with ROWS samples and COLS neurons
*     num_shuffles - number of times to temporally shuffle data (default 1000)
*     prctile - percentile of null distribution (default 1)
*
*  Outputs:
*     I - 1 x N(N-1)/2 vector of mutual information values
*     D - normalized mutual information distance  metric
*     D_shuffled - distance from shuffled distribution
*
*  Compilation:
*     mex CXXFLAGS='$CXXFLAGS -std=c++11 -pthread' -I./boost_1_68_0/ mat_knnSearch.cpp MI.cpp knnSearch.cpp
*
*  Written by:
*  HaoRan Chang, Canadian Centre for Behavioural Neuroscience, University of Lethbridge, Lethbridge, Alberta, Canada, 2018
*/

#include <stdio.h>
#include <mex.h>
#include <matrix.h>
#include "knnSearch.h"
#include "MI.h"
#include <thread>
#include <mutex>

#define NUMBER_OF_THREADS 16 //tweek the number of threads for best performance on your own system

bool *worker;
std::mutex *worker_lock = new std::mutex[NUMBER_OF_THREADS];
int M, N, *index, *count, *ret;
double *deconv, *sorted, *A, *I;

int triang(int n){ //calculate triangular number
        int t = 0;
        for(int i = 1; i < n; i++)
                t += i;
        return t;
}

void opRow(int i, int thread_id){
        int j, k;
        int carry = i*(N-1) - triang(i);
        for(j = i + 1; j < N; j++) {
                for(k = 0; k < count[i]; k++)
                        A[thread_id*M + k] = deconv[M*j + index[i*M + k]];
                knnSearch(sorted+(i*M), index+(i*M), A+(thread_id*M), count[i], ret+(thread_id*M*2));

                for(k = 0; k < count[j]; k++)
                        A[thread_id*M + k] = deconv[M*i + index[j*M + k]];
                knnSearch(sorted+(j*M), index+(j*M), A+(thread_id*M), count[j], ret+(thread_id*M*2)+M);

                I[carry] = MI(ret+(thread_id*M*2), M, count[i], count[j]);
                carry++;
                for(k = 0; k < count[i]; k++) ret[thread_id*M*2 + index[i*M + k]] = 0.0; //reset array elements
                for(k = 0; k < count[j]; k++) ret[thread_id*M*2 + M + index[j*M + k]] = 0.0;
        }
        worker_lock[thread_id].lock();
        worker[thread_id] = true;
        worker_lock[thread_id].unlock();
}

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) {
        if(nrhs < 1)
                mexErrMsgTxt("An input matrix is required");
        if(nrhs > 2)
                mexWarnMsgTxt("Ignoring extra input arguments");

        int shuffles;
        if(nrhs < 2) {
                shuffles = 1000;
                mexPrintf("default option selected - Shuffling data 1000 times\n");
        }else
                shuffles = (int) *mxGetPr(prhs[1]);

        deconv = mxGetPr(prhs[0]);
        M = mxGetM(prhs[0]);
        N = mxGetN(prhs[0]);

        if(M < 2)
                mexErrMsgTxt("Recording has less than 2 sample frames");
        if(N < 2)
                mexErrMsgTxt("At least 2 neurons are required to make comparison");

        static int NUM_THREADS = NUMBER_OF_THREADS;
        if (NUM_THREADS > N)
                NUM_THREADS = N - 1;

        int i, j, k;

        index = (int *) malloc(M*N * sizeof(int));
        count = (int *) malloc(N * sizeof(int));
        sorted = (double *) malloc(M*N * sizeof(double));
        struct blockHead *block = (struct blockHead*) malloc(M * sizeof(struct blockHead));

        A = (double *) malloc(M * sizeof(double) * NUM_THREADS);
        ret = (int *) calloc(M * 2 * NUM_THREADS, sizeof(int));

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

        plhs[0] = mxCreateDoubleMatrix(1, N*(N-1)/2, mxREAL);
        I = mxGetPr(plhs[0]);

        std::thread t[NUM_THREADS];
        worker = (bool *) malloc(NUM_THREADS * sizeof(bool));
        for(i = 0; i < NUM_THREADS; i++) {
                worker[i] = false;
                t[i] = std::thread(opRow, i, i);
                t[i].detach();
        }
        int next_row = NUM_THREADS;

        i = 0;
        while(next_row < (N-1)) {
                if(worker_lock[i].try_lock()) {
                        if(worker[i]) {
                                worker[i] = false;
                                t[i] = std::thread(opRow, next_row, i);
                                t[i].detach();
                                next_row++;
                        }
                        worker_lock[i].unlock();
                }
                i = (i+1) % NUM_THREADS;
        }

        for(i = 0; i < NUM_THREADS; i++) {
                while(1) {
                        if(worker_lock[i].try_lock()) {
                                if(worker[i]) {
                                        worker_lock[i].unlock();
                                        break;
                                }
                                worker_lock[i].unlock();
                        }
                }
        }

        free(A); free(ret); free(index); free(count); free(sorted);
}
