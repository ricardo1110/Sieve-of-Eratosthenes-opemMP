#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

typedef struct info_pos { //Saves the information about the work each thread should do
    int prime;
    long pos;
    long end;
} info_pos_t;

typedef struct info { //Saves the information about the next that should be done as soon as a thread becomes available
    int num_threads;
    long max_num;
    int count;
    info_pos_t cur_pos;
} info_t;

void get_next(info_pos_t *info);

int main(int argc, char **argv) {
    int i;
    int *number;
    info_pos_t info;
    info_t data;

    if(argc < 3) { //Check if the number od arguments is valid
        printf("Wrong usage. You should type:\n\t./control <target_number> <number_of_threads>\n");
        return 0;
    }
    //Initializes the variables
    data.num_threads = atoi(argv[2]);
    data.max_num = atoi(argv[1]);
    data.cur_pos.prime = 2;
    data.cur_pos.pos = 0;
    data.cur_pos.end = data.max_num/data.num_threads;
    data.count = 1;
    //Allocates memory for the boolean vector, in the end those that reamain false are primes
    number = (int *) calloc(data.max_num-1, sizeof(int));

    #pragma omp parallel shared(number, data) private(info, i) num_threads(data.num_threads)
    {
        info.pos = 0;
     
        while(1) {
            #pragma omp critical
            {//Critical region to delegate a new task to a thread and update it so the next get the other job 
                info = data.cur_pos;
                if(data.cur_pos.end >= data.max_num) { //The work for a prime has ended and it should go to the next if there is one
                    for(i = 0; i <= sqrt(data.max_num); i++) {
                        if(!number[i] && (i+2 > data.cur_pos.prime)) {
                            data.cur_pos.prime = i+2;
                            data.cur_pos.pos = 0;
                            data.count = 1;
                            data.cur_pos.end = data.max_num/data.num_threads;
                            break;
                        }
                    }
                    if(i >= floor(sqrt(data.max_num))) { //No more jobs available
                        data.cur_pos.pos = -1;
                    }
                } else { //Still the same prime, just get the next region of the vector
                    data.count++;
                    data.cur_pos.pos = data.cur_pos.end+1;
                    data.cur_pos.end = (data.count * data.max_num/data.num_threads);
                }
            }

            if(info.pos < 0) { //No more jobs, exit the loop
                break;
            }

            for(i = (info.pos/info.prime)*info.prime + ((info.pos % info.prime > 0)? info.prime : 0); i <= info.end; i += info.prime) { //Get those that are divisible by the given prime in the delegated region
                if(i > info.prime) {
                    number[i-2] = 1;
                }
            }
        }
    }

    // long j = 0;
    // for(i = 0; i < data.max_num-1; i++) {
    //     if(!number[i]) {
    //         // printf("%d\t", i+2);
    //         j++;
    //     }
    // }
    // printf ("\n%ld\n", j);

    free(number); //Clean up
    return 0;
}