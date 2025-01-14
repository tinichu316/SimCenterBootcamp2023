#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <omp.h>

int main(int argc, char **argv) {
  int nThreads = 0;
  double norm = 0;
  // create vector with random numbers
  if (argc != 2) {
    printf("Error correct usage: app vectorSize\n");
    return 0;
  }
  int vectorSize = atoi(argv[1]);
  double *globalVector = malloc(sizeof *globalVector * vectorSize);
  srand(50);
  for (int i = 0; i < vectorSize; i++) {
    double random_number = 1.0 + (double)rand() / RAND_MAX;
    globalVector[i] = random_number;
  }  

  // calculate norm. pad to avoid false sharing
  double sum[64][16]; //assume max 64 threads, pad at 16. only use [i][0]
  for (int i=0; i<64; i++){sum[i][0] = 0;} // initialize to 0

  #pragma omp parallel
  {
    int tid = omp_get_thread_num(); //id of the thread
    int numT = omp_get_num_threads(); //total number of threads
    if (tid == 0) {nThreads = numT;} //only master sets this global variable
    // dot product
    for (int i=0; i<vectorSize; i++) {
      sum[i][0] += globalVector[i]*globalVector[i];
    }
  }

  // add back
  for (int i = 0; i < nThreads; i++){
    norm += sum[i][0];
  }
  norm = sqrt(norm);

  // print norm
  printf("Norm = %f \n", norm);
  free(globalVector);
  return 0;
}