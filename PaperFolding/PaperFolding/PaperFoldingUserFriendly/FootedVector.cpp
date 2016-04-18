#include "FootedVector.h"
#include <cmath>


FootedVector::FootedVector()
{
    Tail = Complex(); 
    Head = Complex(); 
}

FootedVector::FootedVector(Complex tail, Complex head)
{
    Tail = tail; 
    Head = head; 
}

bool FootedVector::operator==(const FootedVector& vector)
{
    return abs(Tail.X - vector.Tail.X) < 0.0001 &&
        abs(Tail.Y - vector.Tail.Y) < 0.0001 &&
        abs(Head.X - vector.Head.X) < 0.0001 &&
        abs(Head.Y - vector.Head.Y) < 0.0001;
}




