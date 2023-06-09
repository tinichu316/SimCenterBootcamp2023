
// program to read values from a file, each file a csv list of int and two double
// written: fmk

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {

  if (argc != 3) {
    //fprintf(stdout, "ERROR correct usage appName inputFile\n";);
    std::cout << "ERROR correct usage appName inputFile outputBinaryFile\n";
    return -1;
  }
  
  FILE *filePtr = fopen(argv[1],"r"); 

  int i = 0;
  float float1, float2;
  int maxVectorSize = 100; //limit on number of rows in incoming csv file.
  int bufferAdd = 100; // size of chunk to add if we have more data.
  //double *vector1 = malloc(maxVectorSize * sizeof *vector1); // casts not needed? sizeof can be applied to expression to get variable type
  //double *vector2 = (double *)malloc(maxVectorSize*sizeof(double));  
  ::std::vector<double> vector1(maxVectorSize, 0); // initialize, set value to 0
  ::std::vector<double> vector2(maxVectorSize, 0); // initialize, set value to 0

  int vectorSize = 0;
  
  while (fscanf(filePtr,"%d, %f, %f\n", &i, &float1, &float2) != EOF) {
    vector1[vectorSize] = float1;
    vector2[vectorSize] = float2;
    printf("%d, %f, %f\n", i, vector2[i], vector1[i]);

    vectorSize++;

    if (vectorSize == maxVectorSize) { // next loop will break.
      // some code needed here I think .. programming exercise
      // but want memory to be contiguous, malloc doesn't guarantee that.
      maxVectorSize += bufferAdd;
      //vector1 = realloc(vector1, maxVectorSize * sizeof *vector1); // allocate more memory
      //vector2 = realloc(vector2, maxVectorSize * sizeof *vector2);
      vector1.resize(maxVectorSize);
      vector2.resize(maxVectorSize);

      if (vector1 == NULL || vector2 == NULL){
        std::cout << "Error allocating memory\n";
        exit(-1);
      }

      // alternatively, create new vectors that are bigger, copy over data, 
      // free old vectors, and re-set reference to the new vectors
    }
  }

  fclose(filePtr);  
  //fclose(filePtrB);

  //write to binary file too.
  //FILE* filePtrB = fopen(argv[2], "wb");
  //fwrite(vector1, sizeof *vector1, maxVectorSize, filePtrB);
  //fwrite(vector2, sizeof *vector2, maxVectorSize, filePtrB);

  //free(vector1);
  //free(vector2);
  delete[] vector1;
  delete[] vector2;

}