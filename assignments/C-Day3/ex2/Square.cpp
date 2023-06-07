#include "Square.h"
// define class methods here
int Square::numSelf = 0; // the number

//destructor
Square::~Square() {
  numSelf--;
  std::cout << "Square Destructor " << this->getColor() << "\n"; // -> same as (*x).
}

//constructor
Square::Square(double w, string color)
  :ColoredShape(color), width(w)
{
  numSelf++;
}

double Square::getArea(void) {
  return width*width;
}

void Square::printArea(std::ostream &s) {
  s << "Square: color: " << this->getColor() << ", area: "
    << width * width << " numSelf: " << numSelf << "\n";
}
