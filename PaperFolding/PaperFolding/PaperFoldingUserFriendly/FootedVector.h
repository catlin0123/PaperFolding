#include "Complex.h"
#pragma once; 

class FootedVector
{
    public:
        FootedVector(); 
        FootedVector(Complex tail, Complex head);
        bool operator==(const FootedVector& vector);
        Complex Tail;
        Complex Head;
};

