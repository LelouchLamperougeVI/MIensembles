// #include "boost_1_68_0/boost/math/special_functions/digamma.hpp"
// #include <boost/math/special_functions/digamma.hpp>
#include <stdio.h>
#include <mex.h>

// int main(int argc, char const *argv[]) {
//   // int a = 0
//   // double b = 4.6
//   // for(int i = 0; i < 20; i++) printf("%d\n", i % 8);
//   printf("%d\n", (int) 1000.55);
// }

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) {
        int shuffles;
        if(nrhs < 1) {
                shuffles = 1000;
        }else
                shuffles = (int) *mxGetPr(prhs[0]);
        mexPrintf("%d\n", shuffles);
}
