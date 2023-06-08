// compute pi based on numerical integration
// add MPI features

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

double leftIntegration(int);
double midpointIntegration(int);

int main(int argc, char **argv) { // want main to return 0 to indicate that the program has ran successfully, else -1
  
  if(argc != 2){
    printf("Usage: appName numDivisions\n");
    exit(-1);
  }

  int numDivisions = atoi(argv[1]);
  double dx = 1.0/numDivisions;

  // initialize MPI
  int numP, procID;
  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &numP); //output number of cores to numP
  MPI_Comm_rank(MPI_COMM_WORLD, &procID); //output this core's rank ID number

  int chunk = numDivisions/numP; // how many divisions for each core to computer
  // may have extra computes in case numDivisions not divisible by numP
  double* globalData = NULL; //each element is the partial contribution from each core
  double localData; // contribution for this core

  if (procID == 0){
    globalData = malloc(numP * sizeof(double));
    for (int i = 0; i < numP; i++) {globalData[i] = 0;} // initialize to 0
  }
  
  // calculate separately
  double startX = chunk*dx*numP;
  // add any leftover to last one
  if (procID == numP - 1){
    chunk = numDivisions - chunk * (numP - 1); // make last chunk size bigger
  }
  localData = midpointIntegration(chunk, startX, dx);

  // put back together
  MPI_Gather(&localData, 1, MPI_DOUBLE, globalData, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);

  //sum and print
  if (procID == 0) {
    double pi = 0;
    for (int i = 0; i < numP; i++){
      pi += globalData[i];
    }
    printf("PI = %16.14f, diff(%16.14f)\n", pi, fabs(pi-M_PI)); //M_PI is from math.h
    free(globalData); // since we did malloc
  }

  MPI_Finalize();
  return 0;

}

double midpointIntegration(int numDivisions, double startX, double dx) {
  double res = 0;

  // add start and end
  // res += 3.0 * dx; // 2 * dx / (1) + 2 * dx / (1 + 1);

  double x = startX + dx * 0.5;
  // could also partition to start at procID and increment by numP.
  for (int i = 0; i < numDivisions; i++) {
    //double x = (double)i * dx; // value of x for that step, left rectangle integration
    res += 4.0 * dx / (1 + x*x);
    x += dx;
  }
  
  return res;
}