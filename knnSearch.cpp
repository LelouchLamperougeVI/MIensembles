#include "knnSearch.h"

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
                                buff = d_left;
                                while((i-left) && (buff <= d_left)) {
                                        d_left = buff;
                                        left++;
                                        buff = fmax(sorted[i-left] - sorted[i], std::abs(A[i-left] - A[i]));
                                }
                                if(buff <= d_left)
                                        d_left = buff;
                                // else
                                        // left--;
                        }
                }else{
                        // left = 0;
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
                                buff = d_right;
                                while((count-1-i-right) && (buff <= d_right)) {
                                        d_right = buff;
                                        right++;
                                        buff = fmax(sorted[i] - sorted[i+right], std::abs(A[i+right] - A[i]));
                                }
                                if(buff <= d_right)
                                        d_right = buff;
                                // else
                                        // right--;
                        }
                }else{
                        // right = 0;
                        d_right = DBL_MAX;
                }

                buff = fmin(d_left, d_right);
                left = 0; right = 0;
                while((i-left+1) && (sorted[i-left] - sorted[i]) < buff) left++;
                while((count-i-right) && (sorted[i] - sorted[i+right]) < buff) right++;
                // if(buff == d_left)
                        // while(right && (sorted[i] - sorted[i+right] > d_left)) right--;
                // else
                        // while(left && (sorted[i-left] - sorted[i] > d_right)) left--;

                ret[index[i]] = left + right - 1;
        }
}
