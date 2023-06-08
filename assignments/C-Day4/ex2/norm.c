// compute norm with parallel processing

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <math.h>

int main(int argc, char** argv) {
  	int procID, numP;
    
    // initialize MPI vectors
    double* globalVector = NULL;
    double* globalSum = NULL; // stores all of the sums later.
    double* localVector = NULL;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &procID);
    MPI_Comm_size(MPI_COMM_WORLD, &numP);

    if (argc != 2) {
      printf("Error correct usage: app vectorSize\n");
      return 0;
    }
    int vectorSize = atoi(argv[1]);
    int remainder = vectorSize % numP; 
    // pad vector size with 0's
    int bufferSize = numP - remainder;
    vectorSize += bufferSize;


    // Only the root process initializes the global array
    if (procID == 0) {
      globalVector = malloc(sizeof *globalVector * vectorSize );
      globalSum = malloc(sizeof *globalSum * numP);
      srand(50); // set random seed
      for (int i = 0; i < vectorSize; i++) {
      	if (i <= vectorSize - bufferSize) {
			double random_number = 1.0 + (double)rand() / RAND_MAX;
			globalVector[i] = random_number;
      	} else {
      		globalVector[i] = 0; //pad with 0s
      	}
      }
    }

    // Determine the size of the local array for each process
    int localSize = vectorSize / numP; // no remainder since we padded it

    // Allocate memory for the local array
    localVector = malloc(sizeof *localVector * localSize);

    // Scatter the global array to all processes
    MPI_Scatter(globalVector, localSize, MPI_DOUBLE,
                localVector, localSize, MPI_DOUBLE,
                0, MPI_COMM_WORLD);

    // Print the local array for each process, and calc sum of squares locally
    double localSum = 0;
    printf("Process %d received: ", procID);
    for (int i = 0; i < localSize; i++) {
        printf("%.2f ", localVector[i]);
        localSum += localVector[i] * localVector[i];
    }
    printf("\n");

    // put back all local sum to global
    MPI_Gather(localSum, 1, MPI_DOUBLE,
    	globalSum, 1, MPI_DOUBLE,
    	0, MPI_COMM_WORLD);

  	if (procID == 0) { // print
  		double sum = 0;
	    for (int i=0; i<numP; i++) {
	    	sum += globalSum[i];
	    }
	    sum = sqrt(sum);
	    printf("Final Norm is %8.6f", sum);
  	}

    // Clean up memory
    if (procID == 0){
    	free(globalVector);
    	free(globalSum);
    }
    free(localVector);

    MPI_Finalize();
    return 0;

}