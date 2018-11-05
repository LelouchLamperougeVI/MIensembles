#ifndef __knnSearch__
#define __knnSearch__

#include <cmath>
#include <float.h>
#include <stdlib.h>

struct blockHead {
        double block;
        int index;

};

int compare( const void* a, const void* b);

void knnSearch(double *sorted, int *index, double *A, int count, int *ret);

#endif
