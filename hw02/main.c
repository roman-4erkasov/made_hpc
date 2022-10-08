#include<stdlib.h>
#include "score.c"

int main(){
    int nrun = 5;
    double average_runtime, runtime;
    size_t n_arr[] = {500, 512, 1000, 1024, 2000, 2048};
    // size_t n_arr[] = {1, 2, 5, 10, 15, 20};
    // test_mm(n_arr, nrun, IJK, "IJK");
    // test_mm(n_arr, nrun, IKJ, "IKJ");
    //test_mm(n_arr, nrun, JIK, "JIK");
    //test_mm(n_arr, nrun, JKI, "JKI");
    test_mm(n_arr, nrun, KIJ, "KIJ");
    test_mm(n_arr, nrun, KJI, "KJI");
    test_mv(n_arr, nrun, IJ, "IJ");
    test_mv(n_arr, nrun, JI, "JI");
    return 0;
}

