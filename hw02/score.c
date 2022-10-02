#ifndef SCORE_C
#define SCORE_C
#include <stdlib.h>
#include <stdio.h>
#include "linalg.c"

double score_matmul(size_t n, MMStrategy strategy) {
    struct timeval start, end;
    double r_time = 0.0;
    gettimeofday(&start, NULL);
    struct Matrix left = make_random_matrix(n, n);
    struct Matrix right = make_random_matrix(n, n);
    matmul(left, right, strategy);
    gettimeofday(&end, NULL);
    r_time = end.tv_sec - start.tv_sec + ((double) (end.tv_usec - start.tv_usec)) / 1000000;
    return r_time;
}

double score_matvecmul(size_t n, MVStrategy strategy) {
    struct timeval start, end;
    double r_time = 0.0;
    gettimeofday(&start, NULL);
    struct Matrix left = make_random_matrix(n, n);
    struct Vector right = make_random_vec(n);
    matvecmul(left, right, strategy);
    gettimeofday(&end, NULL);
    r_time = end.tv_sec - start.tv_sec + ((double) (end.tv_usec - start.tv_usec)) / 1000000;
    return r_time;
}

void test_mm(size_t n_arr[], size_t nrun, MMStrategy strategy, const char * title){
    double average_runtime=0, runtime;
    printf("Testing %s: ", title);
    fflush(stdout);
    for(int i=0;i<6;++i){
        average_runtime=0;
        for(int j=0;j<nrun;++j){
            runtime=score_matmul(n_arr[i], strategy);
            average_runtime+=runtime;
        }
        printf("  %lf", average_runtime);
        fflush(stdout);
    }
    printf("\n");
    fflush(stdout);
}

void test_mv(size_t n_arr[], size_t nrun, MVStrategy strategy, const char * title){
    double average_runtime=0, runtime;
    printf("Testing %s: ", title);
    fflush(stdout);
    for(int i=0;i<6;++i){
        average_runtime=0;
        for(int j=0;j<nrun;++j){
            runtime=score_matvecmul(n_arr[i], strategy);
            average_runtime+=runtime;
        }
        printf("  %lf", runtime);
        fflush(stdout);
    }
    printf("\n");
    fflush(stdout);
}
#endif

