#include "Main.h"
#include <iostream> 
#include <fstream>
using namespace std; 
 
list<FootedVector> Folds = list<FootedVector>(); 
list<FootedVector> Cuts = list<FootedVector>(); 
FootedVector newFoldOrCut = FootedVector(); 
bool takingFolds = true;
bool takingCuts = false; 

double MinX = 0; 
double MinY = 0;
double MaxX = 500; 
double MaxY = 500; 

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE); 
    glutInitWindowSize(INIT_SCREEN_WIDTH, INIT_SCREEN_HEIGHT);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Paper Folding");
    glClearColor(WHITE[0], WHITE[1], WHITE[2], 1.0);

    glutDisplayFunc(Display);
    glutReshapeFunc(Reshape);
    glutKeyboardFunc(Keyboard);
    glutMouseFunc(Mouse); 
    CreateClickMenu();

    glutMainLoop();
    return 1; 
}

void Mouse(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        newFoldOrCut.Tail = Complex(x, MaxY - MinY - y); 
    }
    else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
    {
        newFoldOrCut.Head = Complex(x, MaxY - MinY - y);
        if (takingFolds)
        {
            Folds.push_back(newFoldOrCut);
        }
        else if (takingCuts)
        {
            Cuts.push_back(newFoldOrCut);
            takingCuts = false; 
        }
        newFoldOrCut = FootedVector(); 
    }
    glutPostRedisplay();
}

void Display()
{
    double minx = 0; 
    double miny = 0; 
    double maxx = 0; 
    double maxy = 0; 
    glClear(GL_COLOR_BUFFER_BIT);
    
    for (FootedVector vector : Folds)
    {
        DrawLine(vector, true, GREEN); 
        FindMinAndMax(vector, minx, miny, maxx, maxy);
    }

    for (FootedVector vector : Cuts)
    {
        DrawLine(vector, false, BLUE); 
        FindMinAndMax(vector, minx, miny, maxx, maxy);
    }

    if (Folds.size() > 0)
    {
        DrawLine(Folds.back(), true, RED); 
    }

    if (minx < MinX)
    {
        MinX = minx; 
        glutReshapeWindow(MaxX - MinX, MaxY - MinY); 
    }
    if (miny < MinY)
    {
        MinY = miny; 
        glutReshapeWindow(MaxX - MinX, MaxY - MinY);
    }
    if (maxx > MaxX)
    {
        MaxX = maxx;
        glutReshapeWindow(MaxX - MinX, MaxY - MinY);
    }
    if (maxy > MaxY)
    {
        MaxY = maxy; 
        glutReshapeWindow(MaxX - MinX, MaxY - MinY);
    }
    glutSwapBuffers();
}

void Reshape(int width, int height)
{

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity(); 
    gluOrtho2D(MinX, MaxX, MinY, MaxY); 

    glViewport(MinX, MinY, width, height);

}

void Keyboard(unsigned char key, int x, int y)
{
    static FootedVector current = FootedVector(); 
    list<FootedVector> remove = list <FootedVector>();
    list<FootedVector> add = list<FootedVector>();

    switch (key)
    {
    case 13: 
        if (takingFolds)
        {
            takingFolds = false; 
            takingCuts = true; 
        }
        break;
    case 'N':
    case 'n':
        if (Folds.size() > 0)
        {
        current.Head = Folds.back().Head - Folds.back().Tail;
        current.Tail = Folds.back().Tail;     
        for (FootedVector vector : Cuts)
        {
            Complex head = (vector.Head - current.Tail) * !current.Head;
            Complex tail = (vector.Tail - current.Tail) * !current.Head;

            FootedVector translated = FootedVector(tail, head); 

            if (head.Y >= 0 && tail.Y >= 0)
            {
                remove.push_back(vector);
            }
            else if (head.Y > 0.0001 || tail.Y > 0.0001)
            {
                remove.push_back(vector);
                FootedVector segmentedVector = FindNewVector(translated);
                FootedVector reflected = FootedVector(); 
                reflected.Head = !segmentedVector.Head / !current.Head + current.Tail;
                reflected.Tail = !segmentedVector.Tail / !current.Head + current.Tail;
                segmentedVector.Head = segmentedVector.Head / !current.Head + current.Tail;
                segmentedVector.Tail = segmentedVector.Tail / !current.Head + current.Tail;
                add.push_back(segmentedVector); 
                add.push_back(reflected);
            }
            else
            {
                FootedVector reflected = FootedVector();
                reflected.Head = !head / !current.Head + current.Tail;
                reflected.Tail = !tail / !current.Head + current.Tail;
                add.push_back(reflected); 
            }
        }
        for (FootedVector vector : remove)
        {
            Cuts.remove(vector);
        }
        for (FootedVector vector : add)
        {
            Cuts.push_back(vector); 
        }
        Folds.pop_back(); 
        }
        break; 
    case ESCAPE_KEY:
        glutLeaveMainLoop();
        break; 
    }
    glutPostRedisplay(); 
}

void Menu(int value)
{
    switch (value)
    {
        case 1:
            if (!takingFolds && !takingCuts)
            {
                Keyboard('N', 0, 0); 
            }
            break; 
    }
}

void CreateClickMenu()
{
    glutCreateMenu(Menu);
    glutAddMenuEntry("Unfold next line - n", 1);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void DrawLine(FootedVector vector, bool isDotted, const double color[])
{
    glColor3dv(color);

    if (isDotted)
    {
        glEnable(GL_LINE_STIPPLE);
        glLineStipple(1, 0xff00);
    }

    glBegin(GL_LINES);
        glVertex2d(vector.Tail.X, vector.Tail.Y);
        glVertex2d(vector.Head.X, vector.Head.Y);
    glEnd();

    if (isDotted)
    {
        glDisable(GL_LINE_STIPPLE);  
    }
}

FootedVector FindNewVector(FootedVector vector)
{
    double slope = (vector.Head.Y - vector.Tail.Y) / (vector.Head.X - vector.Tail.X); 
    double intercept = vector.Tail.Y - slope * vector.Tail.X; 
    double yint = -intercept / slope; 

    FootedVector newVector = FootedVector(); 
    newVector.Head = Complex(yint, 0); 
    newVector.Tail = vector.Head.Y < 0 ? vector.Head : vector.Tail;
    return newVector; 
}

void FindMinAndMax(FootedVector v, double & minx, double & miny, double & maxx, double & maxy)
{
    if (v.Head.X < minx)
    {
        minx = v.Head.X;
    }
    if (v.Head.X > maxx)
    {
        maxx = v.Head.X;
    }
    if (v.Tail.X < minx)
    {
        minx = v.Tail.X;
    }
    if (v.Tail.X > maxx)
    {
        maxx = v.Tail.X;
    }

    if (v.Head.Y < miny)
    {
        miny = v.Head.Y;
    }
    if (v.Head.Y > maxy)
    {
        maxy = v.Head.Y;
    }
    if (v.Tail.Y < miny)
    {
        miny = v.Tail.Y;
    }
    if (v.Tail.Y > maxy)
    {
        maxy = v.Tail.Y;
    }
}