#ifndef _SQUARE
#define _SQUARE

#include "ColoredShape.h"

// specify what methods and arguments are in the class
class Square: public ColoredShape {
 public: //public methods
  Square(double w, string color); //constructor
  ~Square(); //destructor
  double getArea(void);
  void printArea(std::ostream &s);  

 protected:

 private: //properties
  double width, height;
  static int numSelf;
};

#endif // _Square
