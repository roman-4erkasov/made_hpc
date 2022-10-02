#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <pthread.h>

const size_t N = 1024;
double *A, *B, *C;

void ZeroMatrix(double * A, size_t N)
{
    for(size_t i=0; i<N; i++)
    {
        for(size_t j=0; j<N; j++)
        {
            A[i * N + j] = 0.0;
        }
    }
}

void RandomMatrix(double * A, size_t N)
{
    srand(time(NULL));

    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            A[i * N + j] = rand() / RAND_MAX;
        }
    }
}

double CalcMatMulTime_ijk(double * A, double * B, double * C, size_t N)
{
    struct timeval start, end;
    double r_time = 0.0;
    size_t i, j, k;

    ZeroMatrix(&C[0], N);

    gettimeofday(&start, NULL);
    
    for (i = 0; i < N; i++)
        for(j = 0; j < N; j++)
        {
            for(k = 0; k < N; k++)
                C[i * N + j] = C[i * N + j] + A[i * N + k] * B[k * N + j];
        }
    gettimeofday(&end, NULL);
    
    r_time = end.tv_sec - start.tv_sec + ((double) (end.tv_usec - start.tv_usec)) / 1000000;
    
    return r_time;
}

double CalcMatMulTime_jik(double * A, double * B, double * C, size_t N)
{
    struct timeval start, end;
    double r_time = 0.0;
    size_t i, j, k;

    ZeroMatrix(&C[0], N);


    gettimeofday(&start, NULL);
    


    for (j = 0; j < N; j++)
        for(i = 0; i < N; i++)
        {
            for(k = 0; k < N; k++)
                C[i * N + j] = C[i * N + j] + A[i * N + k] * B[k * N + j];
        }
    gettimeofday(&end, NULL);
    
    r_time = end.tv_sec - start.tv_sec + ((double) (end.tv_usec - start.tv_usec)) / 1000000;
    
    return r_time;
}

double CalcMatMulTime_kij(double * A, double * B, double * C, size_t N)
{
    struct timeval start, end;
    double r_time = 0.0;
    size_t i, j, k;

    ZeroMatrix(&C[0], N);

    gettimeofday(&start, NULL);
    
    for (k = 0; k < N; k++)
        for(i = 0; i < N; i++)
        {
            for(j = 0; j < N; j++)
                C[i * N + j] = C[i * N + j] + A[i * N + k] * B[k * N + j];
        }
    gettimeofday(&end, NULL);
    
    r_time = end.tv_sec - start.tv_sec + ((double) (end.tv_usec - start.tv_usec)) / 1000000;
    
    return r_time;
}

double CalcMatMulTime_kij_opt(double * A, double * B, double * C, size_t N)
{
    struct timeval start, end;
    double r_time = 0.0;
    size_t i, j, k;

    size_t dummy = 0;

    ZeroMatrix(&C[0], N);

    gettimeofday(&start, NULL);
    
    for (k = 0; k < N; k++)
        for(i = 0; i < N; i++)
        {
            dummy = i * N;
            for(j = 0; j < N; j++)
                C[dummy + j] = C[dummy + j] + A[dummy + k] * B[k * N + j];
        }
    gettimeofday(&end, NULL);
    
    r_time = end.tv_sec - start.tv_sec + ((double) (end.tv_usec - start.tv_usec)) / 1000000;
    
    return r_time;
}

void print_matrix( double **matrix, int size )
{
  int i, j;

  for (i = 0; i < size; ++i) {
    for (j = 0; j < size-1; ++j) {
      printf( "%lf, ", matrix[ i ][ j ] );
    }
    printf( "%lf", matrix[ i ][ j ] );
    putchar( '\n' );
  }
}

struct v {
   int k; /* row */
   int id; /* column */
};
//The thread will begin control in this function
void *worker(void *param) {
    struct v *data = param; // the structure that holds our data
    int offset; //the counter and sum
    double sum=0;
    printf("pthread %d started", data->k);
    for(int i = 0; i < N; i++) {
        offset = N * i;
        for(int j = 0; j< N; j++) {
            C[offset+j] = C[offset+j] + A[offset+data->k] * B[data->k * N + j];
        }
    }
    printf("pthread %d finished", data->k);
    pthread_exit(0);
}

double matmul_pthread() {
    struct timeval start, end;
    double r_time = 0.0;
    size_t i, j, k;
    size_t dummy = 0;
    ZeroMatrix(&C[0], N);
    gettimeofday(&start, NULL);
    struct v *data = (struct v *) malloc(sizeof(struct v));
    for(int k = 0; k < N; k++) {
        data->k = k;
        pthread_t tid;
        pthread_attr_t attr;
        pthread_attr_init(&attr);
        pthread_create(&tid, &attr, worker, data);
        pthread_join(tid, NULL);
    }
    gettimeofday(&end, NULL);
    r_time = end.tv_sec - start.tv_sec + ((double) (end.tv_usec - start.tv_usec)) / 1000000;
    return r_time;
}


double matmul_kij_omp()
{
    struct timeval start, end;
    double r_time = 0.0;
    size_t i, j, k;

    size_t dummy = 0;

    ZeroMatrix(&C[0], N);

    gettimeofday(&start, NULL);
    #pragma omp parallel for private(i,j,k) shared(A,B,C)
    for (k = 0; k < N; k++)
        for(i = 0; i < N; i++)
        {
            dummy = i * N;
            for(j = 0; j < N; j++)
                C[dummy + j] = C[dummy + j] + A[dummy + k] * B[k * N + j];
        }
    gettimeofday(&end, NULL);
    
    r_time = end.tv_sec - start.tv_sec + ((double) (end.tv_usec - start.tv_usec)) / 1000000;
    
    return r_time;
}

int main()
{
    
     int NRuns = 5;
     size_t i, j, k;

     double *runtimes;
     
     A = (double *) malloc(N * N * sizeof(double));
     B = (double *) malloc(N * N * sizeof(double));
     C = (double *) malloc(N * N * sizeof(double));
     runtimes = (double *) malloc(NRuns * sizeof(double));

     RandomMatrix(&A[0], N);
     RandomMatrix(&B[0], N);

     // ijk ordering
     double average_runtime = 0.0;
     for(int n=0; n<NRuns; n++)
     {
         runtimes[n]=CalcMatMulTime_ijk(&A[0], &B[0], &C[0], N);
         printf("runtime %lf seconds\n", runtimes[n]);
         average_runtime += runtimes[n]/NRuns;
     }

     printf("average runtime ijk %lf seconds\n", average_runtime);
     printf("---------------------------------\n");


     // jik ordering
     average_runtime = 0.0;
     for(int n=0; n<NRuns; n++)
     {
         runtimes[n]=CalcMatMulTime_jik(&A[0], &B[0], &C[0], N);
         printf("runtime %lf seconds\n", runtimes[n]);
         average_runtime += runtimes[n]/NRuns;
     }

     printf("average runtime jik %lf seconds\n", average_runtime);
     printf("---------------------------------\n");
  

     // kij ordering
     average_runtime = 0.0;
     for(int n=0; n<NRuns; n++)
     {
         runtimes[n]=CalcMatMulTime_kij(&A[0], &B[0], &C[0], N);
         printf("runtime %lf seconds\n", runtimes[n]);
         average_runtime += runtimes[n]/NRuns;
     }
     printf("average runtime kij %lf seconds\n", average_runtime);
     printf("---------------------------------\n");
  
     // kij ordering naive optimization (useless for -O3)
     average_runtime = 0.0;
     for(int n=0; n<NRuns; n++)
     {
         runtimes[n]=CalcMatMulTime_kij_opt(&A[0], &B[0], &C[0], N);
         printf("runtime %lf seconds\n", runtimes[n]);
         average_runtime += runtimes[n]/NRuns;
     }
     printf("average runtime kij opt %lf seconds\n", average_runtime);
     printf("---------------------------------\n");

     average_runtime = 0.0;
     // runtime 3.672470 seconds
     // runtime 3.654613 seconds
     // runtime 3.730149 seconds
     // runtime 3.726599 seconds
     // runtime 3.658192 seconds
     // average runtime kij opt 3.688405 seconds
     for(int n=0; n<NRuns; n++)
     {
         runtimes[n]=matmul_kij_omp(); //matmul_pthread();
         printf("runtime %lf seconds\n", runtimes[n]);
         average_runtime += runtimes[n]/NRuns;
     }
     printf("average runtime openmp %lf seconds\n", average_runtime);
     printf("---------------------------------\n");
 
     free(A); 
     free(B);
     free(C);
     return 0;
}
