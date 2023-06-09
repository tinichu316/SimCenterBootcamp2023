#include <iostream>
#include <cmath>
#include "Vector.h"

Vector::Vector(int sz)
{
  size = 0;
  double* data = new data[size];
}

Vector::~Vector()
{
  if (data != 0) {
    delete[] data; // free memory
    std::cout << "Deleting Vector\n"; // PROVIDE CODE //what to do here? just print that we destructed?
  }
}

// some methods
int 
Vector::Size(void) const
{
  return size;
}

void 
Vector::zero(void)
{
  for (int i=0; i<size; i++)
    data[i] = 0;
}

double //euclidean norm
Vector::norm(void) const // cannot modify data of the member
{
  double sum = 0;
  for (int i = 0; i < size; i++){
    sum += data[i]*data[i];
  }
  return sqrt(sum);
}

double 
Vector::dot(const Vector &other) const //pass by reference, but still use . to access?
{
  double result = 0;
  
  if (other.size != size) {
    std::cerr << "Vector::dot ERROR vectors not of same size, returning 0\n";
    return result;
  }
  
  for (int i = 0; i < size; i++){
    result += data[i] * other.data[i];
  }
  
  return result;
}

void 
Vector::print(void)
{
  for (int i=0; i<size; i++)
    std::cerr << " " << data[i];
  std::cerr << "\n";
}

// overload some operators to look Matlabish
Vector 
Vector::operator+(const Vector &other) const
{
  Vector result(size);

  if (other.size != size) {
    std::cerr << "Vector::operator ERROR vectors not of same size, returning empty vector\n";
    return result;
  }

  for (int i = 0; i < size; i++){
    result.data[i] = data[i] + other.data[i];
  }

  return result;
}

// assign new vector
void 
Vector::operator=(const Vector &other)
{
  size = other.size;
  delete[] data;
  double* data = new data[size];
  for (int i = 0; i < size; i++){
    data[i] = other.data[i];
  }
}

// adds a single value
void 
Vector::operator+=(double val)
{
  for (int i = 0; i<size; i++){
    data[i] += val;
  }
}

// adds another vector
void 
Vector::operator+=(const Vector &other)
{
  if (other.size != size) {
    std::cerr << "Vector::operator ERROR vectors not of same size, returning empty vector\n";
  }

  for (int i = 0; i < size; i++){
    data[i] += other.data[i];
  }
}


// defines method of vector(4) for instance
//return by value
double 
Vector::operator()(int x) const
{
  if (x < 0 || x >= size) {
    static double errorResult = 0;
    std::cerr << "Vector::operator() " << x << " outside range 0 through " << size-1 << "\n";
    return errorResult;
  }
  return data[x];
}

// return by reference
double &
Vector::operator()(int x)
{
  if (x < 0 || x >= size) {
    static double errorResult = 0;
    std::cerr << "Vector::operator() " << x << " outside range 0 through " << size-1 << "\n";
    return errorResult;
  }
  return &data[x]; //same as data + x?
}
