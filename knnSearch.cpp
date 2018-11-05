#include "knnSearch.h"

int compare( const void* a, const void* b){
        if( ((double) ((struct blockHead*) b)->block) < ((double) ((struct blockHead*) a)->block) )
                return -1;
        else if( ((double) ((struct blockHead*) b)->block) > ((double) ((struct blockHead*) a)->block) )
                return 1;
        else
                return 0;
}

void knnSearch(double *sorted, int *index, double *A, int count, int *ret){
        int i, left, right;
        double d_left, d_right, buff;
        for(i = 0; i < count; i++) { //jump once at the end
                if(i) { // jump once at beginning
                        left = 1;
                        if(A[i] == 0.0) { // low # jumps since most data points are in marginal space
                                while((i-left) && A[i-left] != 0.0) left++;
                                if(A[i-left] == 0.0) // most points won't exceed limits
                                        d_left = sorted[i-left] - sorted[i];
                                else
                                        d_left = DBL_MAX;
                        }else{
                                d_left = fmax(sorted[i-left] - sorted[i], std::abs(A[i-left] - A[i]));
                                left++;
                                while((i-left+1) && ((sorted[i-left] - sorted[i]) <= d_left)) {
                                        buff = fmax(sorted[i-left] - sorted[i], std::abs(A[i-left] - A[i]));
                                        if(buff < d_left) d_left = buff;
                                        left++;
                                }
                        }
                }else{
                        d_left = DBL_MAX;
                }

                if(count-1-i) {
                        right = 1;
                        if(A[i] == 0.0) {
                                while((count-1-i-right) && A[i+right] != 0.0) right++;
                                if(A[i+right] == 0.0)
                                        d_right = sorted[i] - sorted[i+right];
                                else
                                        d_right = DBL_MAX;
                        }else{
                                d_right = fmax(sorted[i] - sorted[i+right], std::abs(A[i+right] - A[i]));
                                right++;
                                while((count-i-right) && ((sorted[i] - sorted[i+right]) <= d_right)) {
                                        buff = fmax(sorted[i] - sorted[i+right], std::abs(A[i+right] - A[i]));
                                        if(buff < d_right) d_right = buff;
                                        right++;
                                }
                        }
                }else{
                        d_right = DBL_MAX;
                }

                buff = fmin(d_left, d_right);
                left = 0; right = 0;
                while((i-left+1) && (sorted[i-left] - sorted[i]) < buff) left++;
                while((count-i-right) && (sorted[i] - sorted[i+right]) < buff) right++;

                ret[index[i]] = left + right - 1;
        }
}
