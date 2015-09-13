#include <gl/freeglut.h>
#include <gl/GLU.h>
#include <vector>
#include <string>
#include  "Complex.h"

using namespace std; 


int main(int argc, char *argv[]);
void Display();
void Reshape(int width, int height);
void Keyboard(unsigned char key, int x, int y);
void Menu(int value);
void CreateClickMenu();
void DrawLine(int x1, int y1, int x2, int y2, bool isDotted, double color[]);

const int INIT_SCREEN_WIDTH = 400;
const int INIT_SCREEN_HEIGHT = 300; 
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

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glClearColor(WHITE[0], WHITE[1], WHITE[2], 1.0);
    glutInitWindowSize(INIT_SCREEN_WIDTH, INIT_SCREEN_HEIGHT); 
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Paper Folding");

    glutDisplayFunc(Display);
    glutReshapeFunc(Reshape);
    glutKeyboardFunc(Keyboard);
    CreateClickMenu(); 

    glutMainLoop(); 
}

void Display()
{
    glClear(GL_COLOR_BUFFER_BIT );
    
    //Draw my lines and whatnot

    glutSwapBuffers(); 
}

void Reshape(int width, int height)
{
    ScreenWidth = width; 
    ScreenHeight = height; 
}

void Keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
        case ESCAPE_KEY:
            glutLeaveMainLoop(); 
    }
}

void Menu(int value)
{
    switch (value)
    {
        case -1:
            //This is how you would get something to happen when "Not actual menu entry" was pressed
    }
}

void CreateClickMenu()
{
    glutCreateMenu(Menu);
    glutAddMenuEntry("Not actual menu entry", -1); 
    glutAttachMenu(GLUT_RIGHT_BUTTON); 
}

void DrawLine(int x1, int y1, int x2, int y2, bool isDotted, double color[])
{

}