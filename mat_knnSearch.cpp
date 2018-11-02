#include <cmath>
#include <float.h>
#include <stdlib.h>
#include <stdio.h>
#include <mex.h>
#include <matrix.h>

struct blockHead {
        double block;
        int index;
};

int compare( const void* a, const void* b){
        if( ((double) ((struct blockHead*) b)->block) < ((double) ((struct blockHead*) a)->block) )
                return -1;
        else if( ((double) ((struct blockHead*) b)->block) > ((double) ((struct blockHead*) a)->block) )
                return 1;
        else
                return 0;
}

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) {
        if(nrhs < 1)
                mexErrMsgTxt("An input matrix is required");
        if(nrhs > 1)
                mexWarnMsgTxt("Ignoring extra input arguments");

        double *deconv = mxGetPr(prhs[0]);
        int M = mxGetM(prhs[0]);
        int N = mxGetN(prhs[0]);

        if(M < 2)
                mexErrMsgTxt("Recording has less than 2 sample frames");
        if(N < 2)
                mexErrMsgTxt("At least 2 neurons are required to make comparison");

        int i, j, k, count, left, right, n;
        int accum = 0;

        double d_left, d_right, buff;
        double *A = (double *) malloc(M * 2 * sizeof(double));

        int dims[2] = {M, N*(N-1)/2};
        plhs[0] = mxCreateNumericArray(2, dims, mxINT32_CLASS, mxREAL);
        int *ret = (int*) mxGetPr(plhs[0]);
        struct blockHead* block = (struct blockHead*) malloc(M * sizeof(struct blockHead));

        for(k = 0; k < N-1; k++) {
                for(i = 0; i < M; i++) {
                        block[i].block = deconv[k*M + i];
                        block[i].index = i;
                }
                qsort(block, M, sizeof(struct blockHead), compare);

                count = 0;
                while(block[count].block > 0) {
                        A[count] = block[count].block;
                        count++;
                }
                // for(i = 0; i < M; i++) mexPrintf("%f ", A[i]); mexPrintf("\n");

                for(n = k; n < N-1; n++) {
                        for(i = 0; i < count; i++)
                                A[M+i] = deconv[(n+1)*M + block[i].index];

                        for(i = 0; i < count; i++) { //jump once at the end
                                if(i) { // jump once at beginning
                                        left = 1;
                                        if(A[M+i] == 0.0) { // low # jumps since most data points are in marginal space
                                                while((i-left) && A[M+i-left] != 0.0) left++;
                                                if(A[M+i-left] == 0.0) // most points won't exceed limits
                                                        d_left = A[i-left] - A[i];
                                                else
                                                        d_left = DBL_MAX;
                                        }else{
                                                d_left = fmax(A[i-left] - A[i], std::abs(A[M+i-left] - A[M+i]));
                                                buff = d_left;
                                                while((i-left) && (buff <= d_left)) {
                                                        d_left = buff;
                                                        left++;
                                                        buff = fmax(A[i-left] - A[i], std::abs(A[M+i-left] - A[M+i]));
                                                }
                                                if(buff <= d_left)
                                                        d_left = buff;
                                                else
                                                        left--;
                                        }
                                }else{
                                        left = 0;
                                        d_left = DBL_MAX;
                                }

                                if(count-1-i) {
                                        right = 1;
                                        if(A[M+i] == 0.0) {
                                                while((count-1-i-right) && A[M+i+right] != 0.0) right++;
                                                if(A[M+i+right] == 0.0)
                                                        d_right = A[i] - A[i+right];
                                                else
                                                        d_right = DBL_MAX;
                                        }else{
                                                d_right = fmax(A[i] - A[i+right], std::abs(A[M+i+right] - A[M+i]));
                                                buff = d_right;
                                                while((count-1-i-right) && (buff <= d_right)) {
                                                        d_right = buff;
                                                        right++;
                                                        buff = fmax(A[i] - A[i+right], std::abs(A[M+i+right] - A[M+i]));
                                                }
                                                if(buff <= d_right)
                                                        d_right = buff;
                                                else
                                                        right--;
                                        }
                                }else{
                                        right = 0;
                                        d_right = DBL_MAX;
                                }

                                buff = fmin(d_left, d_right);
                                if(buff == d_left)
                                        while(right && (A[i] - A[i+right] > d_left)) right--;
                                else
                                        while(left && (A[i-left] - A[i] > d_right)) left--;

                                // ret[accum*M + i] = left + right;
                                ret[accum*M + block[i].index] = left + right;
                        }
                        accum++;
                }
        }
        free(A); free(block);
}
