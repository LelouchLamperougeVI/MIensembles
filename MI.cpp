#include "MI.h"

double psi(int x){ //digamma function
        // This series approximation is fairly accurate for x in the interval (2, inf), while smaller x can be estimated via the recusion psi(x+1) = psi(x)/x

        double rval = (double) x;
        if(x > 1)
                rval = log(rval) - 1/2/rval - 1/12/std::pow(rval,2) + 1/120/std::pow(rval,4) - 1/252/std::pow(rval,6) + 1/240/std::pow(rval,8) - 5/660/std::pow(rval,10) + 691/32760/std::pow(rval,12) - 1/12/std::pow(rval,14);
        else
                rval = psi(x + 1)/rval;

        return rval;
}

double MI(int *ret, int M, int c1, int c2){
        double I = 0.0;
        int k0 = 0;

        double n1 = boost::math::digamma(M - c1 -1);
        double n2 = boost::math::digamma(M - c2 -1);

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
