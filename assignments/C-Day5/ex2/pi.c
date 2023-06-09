#include <stdio.h>
#include <time.h>
#include <math.h>
#include <omp.h>

static long int numSteps = 1000000000;

int main() {

  // perform calculation
  double pi   = 0;
  double dx = 1./numSteps;
  double x  = 0;
  
  omp_set_num_threads(4);
  #pragma omp parallel reduction(:+pi)
  {
    int tid = omp_get_thread_num(); //id of the thread
    int numT = omp_get_num_threads(); //total number of threads
    printf("Num threads: %d\n", numT);
    for (int i=tid; i<numSteps; i+=numT) {
      x = (i + 0.5) * dx;
      pi += 4./(1.+x*x);
    }
  }
  pi *= dx;
  
  printf("PI = %16.14f Difference from math.h definition %16.14f \n",pi, pi-M_PI);
  return 0;
}