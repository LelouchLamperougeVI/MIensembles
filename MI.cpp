#include "MI.h"

double MI(int *ret, int M, int c1, int c2){
        double I = 0.0;
        int k0 = 0;

        double n1 = boost::math::digamma(M - c1);
        double n2 = boost::math::digamma(M - c2);

        for(int i = 0; i < M; i++) {
                if((ret[i] + ret[M+i]) == 0.0)
                        k0++;
                else if(ret[i] == 0.0)
                        I += n1 + boost::math::digamma(ret[M+i]);
                else if(ret[M+i] == 0.0)
                        I += boost::math::digamma(ret[i]) + n2;
                else
                        I += boost::math::digamma(ret[i]) + boost::math::digamma(ret[M+i]);
        }
        I = ((M-k0)*boost::math::digamma(1) + k0*boost::math::digamma(k0) - I - k0*(n1 + n2)) / M + boost::math::digamma(M);

        return I;
}
