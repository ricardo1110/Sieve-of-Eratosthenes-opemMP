#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

typedef struct info_pos {
    int prime;
    long pos;
    long end;
} info_pos_t;

typedef struct info {
    int num_threads;
    long max_num;
    int count;
    info_pos_t cur_pos;
} info_t;

void get_next(info_pos_t *info);

int main(int argc, char **argv) {
    int i;
    info_pos_t info;
    info_t data;

    if(argc < 3) {
        printf("Wrong usage. You should type:\n\t./control <target_number> <number_of_threads>\n");
        return 0;
    }

    data.num_threads = atoi(argv[2]);
    data.max_num = atoi(argv[1]);
    data.cur_pos.prime = 2;
    data.cur_pos.pos = 0;
    data.cur_pos.end = data.max_num/data.num_threads;
    data.count = 1;

    number = (int *) calloc(data.max_num-1, sizeof(int));

    #pragma omp prallel shared(number, data) private(info, i)
    {

        info.pos = 0;
        while(1) {
            #pragma critical
            {
                info = data.cur_pos;
        
                if(data.cur_pos.end >= data.max_num) {
                    for(i = 0; i <= sqrt(data.max_num); i++) {
                        if(!number[i] && (i+2 > data.cur_pos.prime)) {
                            data.cur_pos.prime = i+2;
                            data.cur_pos.pos = 0;
                            data.count = 1;
                            data.cur_pos.end = data.max_num/data.num_threads;
                            break;
                        }
                    }
                    if(i >= floor(sqrt(data.max_num))) {
                        data.cur_pos.pos = -1;
                    }
                } else {
                    data.count++;
                    data.cur_pos.pos = data.cur_pos.end+1;
                    data.cur_pos.end = (data.count * data.max_num/data.num_threads);
                }
            }

            if(info.pos < 0) {
                break;
            }

            for(i = (info.pos/info.prime)*info.prime + ((info.pos % info.prime > 0)? info.prime : 0); i <= info.end; i += info.prime) {
                if(i > info.prime) {
                    number[i-2] = 1;
                }
            }
        }
    }

    // long j = 0;
    // for(i = 0; i < data.max_num-1; i++) {
    //     if(!number[i]) {
    //         // printf("%ld\t", i+2);
    //         j++;
    //     }
    // }
    // printf ("\n%ld\n", j);

    free(number);
    return 0;
}