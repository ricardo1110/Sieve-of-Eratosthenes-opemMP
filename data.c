#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define FALSE 0
#define TRUE 1
typedef char bool;


void usage(char* prog) {
    printf("Usage: %s [positive whole number]\n", prog);
    exit(-1);
}

int main(int argc, char* argv[0]) {
    int i, idx;
    int N, sqrt_N;
    int p;
    bool* prime;
    int currentPrime;
    int beg, end;
    int nThreads, nQuo, nRem, tid;

    //Init currentPrime
    currentPrime = 0;

    //Wrong arguments handling
    if (argc != 2) {
        usage(argv[0]);
    }

    //Input conversion
    N = (int) atoi(argv[1]);
    if (N < 1) {
        usage(argv[0]);
    }

    //Set sqrt_N
    sqrt_N = floor(sqrt(N));

    //Allocates array of bools for each number n: 2 <= n <= N
    prime = (bool*) malloc((N-1)*sizeof(bool));

    #pragma omp parallel \
                private(p, i, tid, beg, end) \
                shared(prime, N, sqrt_N, currentPrime, nThreads, nQuo, nRem) \
                num_threads(8)
    {
        //Set every entry of array as TRUE
        #pragma omp for
        for (i = 0; i <= N-1; i++) {
            prime[i] = TRUE;
        }

        //Load calculations
        nThreads = omp_get_num_threads();
        nQuo = (N-1) / nThreads;
        nRem = (N-1) % nThreads;
        tid = omp_get_thread_num();
        if(tid < nRem ) {
            beg = tid*(nQuo+1);
            end = tid*(nQuo+1)+nQuo;
        } else {
            beg = tid*nQuo+nRem;
            end = (tid+1)*nQuo+nRem-1;
        }
        //printf("<%d> %d, %d\n", tid, beg, end);

        for (p = 2; p <= sqrt_N; p++) {
            if (prime[p-2]) {
                if (currentPrime <= end && currentPrime >= beg) {
                    if (currentPrime <= p) currentPrime = p;
                }
                i = ceil((beg+2)/(float)p)*p;
                if(i == p) i += p;
                //printf("<%d> i=%d\n", args->id, i);
                for (; i <= end+2; i += p) {
                    prime[i-2] = FALSE;
                    //printf("<%d> x%d\t", args->id, i);
                }
            }
        }
    }

    //Output printing
    // printf("All prime numbers less then or equal to %d are:\n", N);
    // for (p = 2; p <= N; p++) {
    //     if (prime[p-2]) {
    //         printf("%d\t", p);
    //     } else {
    //         //printf("x%d\t", p);
    //     }
    // }
    // printf("\n");

    free(prime);
    exit(0);
}
