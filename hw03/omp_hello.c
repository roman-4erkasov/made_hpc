#include <stdio.h>
#include <omp.h>

int main(){
  #pragma omp parallel num_thteads(4)
  {
    print("hello from thread %d of $d", omp_get_thread_num(), omp_get_num_threads());
  }
  return 0;
}

