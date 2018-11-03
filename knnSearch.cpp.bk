// Copy of mat_knnSearch to be compiled directly with g++ for profiling purposes

#include <cmath>
#include <float.h>
#include <stdlib.h>
#include <stdio.h>

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

int main(){
        int M=8;
        int N=3;
        int i, j, count, left, right, n;
        double deconv[N][M] = {
                {0.0, 3.2, 7.5, 0.0, 1.8, 4.5, 8.9, 5.6},
                {0.0, 0.0, 4.2, 8.5, 3.6, 4.5, 0.0, 0.0},
                {0.0, 3.2, 4.2, 0.0, 3.6, 4.5, 0.0, 0.0}
        };

        // double **deconv = new double*[N];
        // for(i = 0; i < N; i++) deconv[i] = new double[M];
        // for(i = 0; i < N; i++)
        //         for(j = 0; j < M; j++)
        //                 deconv[i][j] = (double) rand();

        double d_left, d_right, buff;
        double *A = (double *) malloc(M * 2 * sizeof(double));
        int *ret = (int *) calloc(M * (N-1), sizeof(int));
        struct blockHead* block = (struct blockHead*) malloc(M * sizeof(struct blockHead));
        for(i = 0; i < M; i++) {
                block[i].block = deconv[0][i];
                block[i].index = i;
        }
        qsort(block, M, sizeof(struct blockHead), compare);

        count = 0;
        while(block[count].block > 0) {
                A[count] = block[count].block;
                count++;
        }
        for(i = 0; i < M; i++) printf("%f ", A[i]); printf("\n");

        for(n = 0; n < N-1; n++) {
                for(i = 0; i < count; i++)
                        A[M+i] = deconv[n+1][block[i].index];
                for(i = 0; i < M; i++) printf("%f ", A[M+i]); printf("\n");

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
                        // printf("%d %d, %f\n", left, right, buff);
                        printf("%d %d, %f\t %f\n", left, right, d_left, d_right);
                        if(buff == d_left)
                                while(right && (A[i] - A[i+right] > d_left)) right--;
                        else
                                while(left && (A[i-left] - A[i] > d_right)) left--;
                        // ret[block[i].index] = left + right;
                        ret[(n*M)+i] = left + right;
                        // printf("%d %d\n", left, right);
                }
        }
        for(i = 0; i < (N-1); i++) {
                for(j = 0; j < M; j++)
                        printf("%d ", ret[(i*M)+j]);
                printf("\n");
        }
        free(A); free(ret); free(block);
}
