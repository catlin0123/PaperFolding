#pragma once
class Complex
{
public:
    Complex();
    Complex(double x); 
    Complex(double x, double y);
    Complex Compliment(); 
    Complex Multiply(Complex rhs);
    Complex Divide(Complex rhs);
    Complex Add(Complex rhs);
    Complex Subtract(Complex rhs);
    Complex operator-(); 
    Complex operator!();
    Complex operator-(const Complex& complex);
    Complex operator+(const Complex& complex); 
    Complex operator*(const Complex& complex); 
    Complex operator/(const Complex& complex);
    bool operator==(const Complex& complex); 
    bool operator!=(const Complex& complex);
    double Re();
    double Im();
    
    double Magnitude(); 
    double MagnitudeSquared(); 
    double X, Y;
};

