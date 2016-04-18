#include <gl/freeglut.h>
#include <gl/GLU.h>
#include <list>
#include <fstream> 
#include "Complex.h"
#include "FootedVector.h"
#pragma once; 

using namespace std; 

int main(int argc, char *argv[]);
void Display();
void Reshape(int width, int height);
void Keyboard(unsigned char key, int x, int y);
void Menu(int value);
void CreateClickMenu();
void DrawLine(FootedVector vector, bool isDotted, const double color[]);
FootedVector FindNewVector(FootedVector vector); 
void GetData(int argc, char *argv[]);
void GetFileContents(fstream& stream);
bool IsClockWise(list<Complex> pointList);
list<Complex> FindNewPoints(list<Complex> first, list<Complex> second);

const int INIT_SCREEN_WIDTH = 500;
const int INIT_SCREEN_HEIGHT = 500; 
const int ESCAPE_KEY = 27; 

int ScreenWidth; 
int ScreenHeight; 

const double BLUE[] = { 0.0, 0.0, 1.0 };
const double RED[] = { 1.0, 0.0, 0.0 };
const double GREEN[] = { 0.0, 153.0/255, 0.0 };
const double YELLOW[] = { 1.0, 1.0, 0.0 }; 
const double ORANGE[] = { 1.0, 128.0/255, 0 };
const double PURPLE[] = { 76.0/255, 0.0, 153.0/255 }; 
const double WHITE[] = { 1.0, 1.0, 1.0 }; 
const double BLACK[] = { 0.0, 0.0, 0.0 };

int main(int argc, char *argv[]);

void Display();

void Reshape(int width, int height);

void Keyboard(unsigned char key, int x, int y);

void Menu(int value);

void CreateClickMenu();

void DrawLine(int x1, int y1, int x2, int y2, bool isDotted, double color[]);