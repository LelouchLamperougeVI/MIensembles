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
        int M=6;
        int N=2;
        int i, j, count, left, right;
        double deconv[N][M] = {
                {0.0, 3.2, 7.5, 0.0, 1.8, 4.5},
                {0.0, 0.0, 4.2, 8.5, 3.6, 4.5}
        };
        double d_left, d_right, buff;
        double *A = (double *) malloc(M * N * sizeof(double));
        int *ret = (int *) calloc(M, sizeof(int));
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

        for(i = 0; i < count; i++)
                A[M+i] = deconv[1][block[i].index];
        for(i = 0; i < M; i++) printf("%f ", A[i]);
        printf("\n");
        for(i = 0; i < M; i++) printf("%f ", A[M+i]);
        printf("\n");

        for(i = 0; i < count; i++) {
                left = 0; buff = DBL_MAX; d_left = buff;
                while((i-left) > -1 && (buff <= d_left)) {
                        d_left = buff;
                        left++;
                        buff = fmax(A[i-left] - A[i], abs(A[M+i-left] - A[M+i]));
                }

                right = 0; buff = DBL_MAX; d_right = buff;
                while((i+right) < count && (buff <= d_right)) {
                        d_right = buff;
                        right++;
                        buff = fmax(A[i+right] - A[i], abs(A[M+i+right] - A[M+i]));
                }

                buff = fmin(d_left, d_right);
                if(buff == d_left) {
                        while(right > 0 && (A[i+right] > d_left)) right--;
                }else{
                        while(left > 0 && (A[i-left] > d_right)) left--;
                }
                // ret[block[i].index] = left + right;
                ret[i] = left + right;
                printf("%d %d\n", left, right);
        }

        for(i = 0; i < M; i++) printf("%d ", ret[i]);
        printf("\n");
        free(A); free(ret);
}
