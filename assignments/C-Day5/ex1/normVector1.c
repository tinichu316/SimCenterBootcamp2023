#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <omp.h>

int main(int argc, char **argv) {
  /i/nt nThreads = 0;
  double norm = 0;
  // create vector with random numbers
  if (argc != 2) {
    printf("Error correct usage: app vectorSize\n");
    return 0;
  }
  int vectorSize = atoi(argv[1]);
  //double *globalVector = malloc(sizeof *globalVector * vectorSize);
  double globalVector[vectorSize];
  srand(50);
  for (int i = 0; i < vectorSize; i++) {
    double random_number = 1.0 + (double)rand() / RAND_MAX;
    globalVector[i] = random_number;
  }  

  // calculate norm. pad to avoid false sharing
  double sum[64][16]; //assume max 64 threads, pad at 16. only use [i][0]
  for (int i=0; i<64; i++){sum[i][0] = 0;} // initialize to 0

  #pragma omp parallel reduction(+:norm) //only 1 thread will do plus operators on 'dot'
  {
    int tid = omp_get_thread_num(); //id of the thread
    int numT = omp_get_num_threads(); //total number of threads
    #pragma omp parallel for
      for (int i =tid; i < vectorSize; i+= numT){
        norm += globalVector[i]*globalVector[i];
      }
  }

  norm = sqrt(norm);
  // print norm
  printf("Norm = %f \n", norm);
  //free(globalVector);
  return 0;
}