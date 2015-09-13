#include "Complex.h"
#include <cmath>

Complex::Complex()
{
    X = 0;
    Y = 0;
}

Complex::Complex(double x, double y)
{
    X = x;
    Y = y;
}

Complex::Complex(double x)
{
    X = x; 
    Y = 0; 
}

Complex Complex::Compliment()
{
    return Complex(X, -Y);
}

Complex Complex::Multiply(Complex rhs)
{
    return Complex(X*rhs.X - Y*rhs.Y, Y*rhs.X + X*rhs.Y); 
}

Complex Complex::Divide(Complex rhs)
{
    double denominator = rhs.X * rhs.X + rhs.Y * rhs.Y;
    double newX = (X * rhs.X + Y * rhs.Y) / denominator;
    double newY = (Y * rhs.X + X * rhs.Y) / denominator; 
    return Complex(newX, newY); 
}

Complex Complex::Add(Complex rhs)
{
    return Complex(X + rhs.X, Y + rhs.Y); 
}

Complex Complex::Subtract(Complex rhs)
{
    return Complex(X - rhs.X, Y - rhs.Y); 
}

Complex Complex::operator-()
{
    return Complex(-X, -Y); 
}

Complex Complex::operator!()
{
    return Compliment(); 
}

Complex Complex::operator-(const Complex& complex)
{
    return Subtract(complex);
}

Complex Complex::operator+(const Complex& complex)
{
    return Add(complex);
}

Complex Complex::operator*(const Complex& complex)
{
    return Multiply(complex); 
}

Complex Complex::operator/(const Complex& complex)
{
    return Divide(complex); 
}

Complex& Complex::operator=(const double& x)
{
    return Complex(x, 0); 
}

double Complex::Re()
{
    return X; 
}
double Complex::Im()
{
    return Y; 
}

double Complex::Magnitude()
{
    return sqrt(X*X + Y*Y); 
}

double Complex::MagnitudeSquared()
{
    return X*X + Y*Y; 
}
