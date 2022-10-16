#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <sys/time.h>
#include <omp.h>

#define INTERVAL 10000
#define N_TRIALS 1e8
#define N_RUNS 7

double get_pi_seq(long n_trials){
  double rand_x, rand_y, radius;//, r_time;
  long circle_points = 0;//, square_points = 0;
  srand(time(NULL));
  for(long trial=0;trial<n_trials; ++trial){
    rand_x = (double)(rand() % (INTERVAL + 1)) / INTERVAL;
    rand_y = (double)(rand() % (INTERVAL + 1)) / INTERVAL; 
    radius = rand_x * rand_x + rand_y * rand_y;
    if(radius <= 1) circle_points++;
    //square_points++;
  }
  return 4.0*(double)circle_points/n_trials;
}

double get_pi_omp(long n_trials){
  long circle_points = 0;
  #pragma omp parallel reduction(+:circle_points)
  {
    double rand_x, rand_y, radius;
    int thread_id = omp_get_thread_num();
    unsigned int seed_raw = (unsigned) time(NULL);
    unsigned int seed = (seed_raw&0xFFFFFFF0)|(thread_id+1);
    srand(seed);
    #pragma omp for
    for(long  trial=0;trial<n_trials;++trial){
      rand_x = (double)(rand_r(&seed) % (INTERVAL + 1)) / INTERVAL;
      rand_y = (double)(rand_r(&seed) % (INTERVAL + 1)) / INTERVAL;
      radius = rand_x * rand_x + rand_y * rand_y;
      if(radius <= 1) circle_points++;
    }
  }
  return 4.0 * (double)circle_points / (double)n_trials;
}

void estimate(double (*fun)(long), long n_trials, int n_runs){
  long double r_time_sum = 0.0, r_time, value_sum = 0.0, value;
  //struct timeval start, end;
  double start, end;
  for(int run_index=0; run_index < n_runs; ++run_index){
    start = omp_get_wtime();
    value = fun(n_trials);
    end = omp_get_wtime();
    r_time = end - start;
    r_time_sum += r_time;
    value_sum += value;
    printf(
      "\t %d) value=%Lf time=%Lf sec\n", 
      run_index, value, r_time
    );
  }
  printf(
    "sequential pi estimantion: avg_runtime=%Lf sec, avg_value=%Lf\n",
    r_time_sum/(long double)n_runs, 
    value_sum/(long double)n_runs
  );
}

int main(){
  printf("sequntial pi estimation:\n");
  estimate(get_pi_seq, N_TRIALS, N_RUNS);
  printf("omp pi estimation:\n");
  estimate(get_pi_omp, N_TRIALS, N_RUNS);
}
