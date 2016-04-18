#include "Main.h"
#include <iostream> 
#include <fstream>
#include <vector>
using namespace std; 
 
list<Complex> Folds = list<Complex>(); 
list<Complex> Cuts = list<Complex>(); 

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
    CreateClickMenu();

    //Open and get file data
    GetData(argc, argv); 

    glutMainLoop();
    return 1; 
}

void GetData(int argc, char *argv[])
{
    string filename = ""; 
    if (argc > 2)
    {
        cout << "Improper usage. Only one filename can be specified"; 
    }
    else if (argc == 2)
    {
        filename = argv[1]; 
    }
    else if (argc == 1)
    {
        filename = "Text.txt"; 
    }

    
    if (filename != "")
    {
        fstream stream(filename);
        if (stream.is_open())
        {
            GetFileContents(stream); 
        }
        else
        {
            cout << "Could not open the specified file"; 
        }
    }
}

void GetFileContents(fstream &stream)
{
    double x1, y1; 
    int numFolds, numCuts; 

    stream >> numFolds >> numCuts; 
    for (int i = 0; i < numFolds; i++)
    {
        stream >> x1 >> y1; 
        Folds.push_front(Complex(x1, y1));
    }

    for (int i = 0; i < numCuts; i++)
    {
        stream >> x1 >> y1;
        Cuts.push_front(Complex(x1, y1));
    }
}

void Display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    
    //for (FootedVector vector : Folds)
    //{
    //    DrawLine(vector, true, GREEN); 
    //}

    //for (FootedVector vector : Cuts)
    //{
    //    DrawLine(vector, false, BLUE); 
    //}

    //if (Folds.size() > 0)
    //{
    //    DrawLine(Folds.back(), true, RED); 
    //}
    

    glutSwapBuffers();
}

void Reshape(int width, int height)
{
    ScreenWidth = width;
    ScreenHeight = height;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity(); 
    gluOrtho2D(0, 6, 0, 6); 

    if (float(width) / height > 1)
        glViewport((width - height) / 2, 0, height, height);
    else
        glViewport(0, (height - width) / 2, width, width);
}

void Keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 'N':
    case 'n':
        FindNewPoints(Folds, Cuts); 
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
        case -1:
            break; 
    }
}

void CreateClickMenu()
{
    glutCreateMenu(Menu);
    glutAddMenuEntry("Not actual menu entry", -1);
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

bool IsClockWise(list<Complex> pointList)
{
    vector<Complex> vPointList = vector<Complex>(pointList.begin(), pointList.end()); 
    double sum = 0; 

    for (int i = 0; i < vPointList.size(); i++)
    {
        int j = (i + 1) % vPointList.size(); 
        sum += (vPointList[j].Y + vPointList[i].Y) * (vPointList[j].X - vPointList[i].X);
    }

    if (sum < 0)
    {
        return false;
    }
    else
    {
        return true; 
    }
}

list<Complex> FindNewPoints(list<Complex> first, list<Complex> second)
{
    //Get both lists going clockwise 
    if (!IsClockWise(first))
    {
        first.reverse(); 
    }
    if (!IsClockWise(second))
    {
        second.reverse(); 
    }
    vector<Complex> vFirst(first.begin(), first.end()); 
    vector<Complex> vSecond(second.begin(), second.end()); 

    //find the left most point
    float leftMost = 1000000; 
    int index = 0; 
    bool isFirst = true; 

    for (int i = 0; i < vFirst.size(); i++)
    {
        if (leftMost > vFirst[i].X)
        {
            leftMost = vFirst[i].X; 
            isFirst = true; 
            index = i; 
        }
    }
    for (int i = 0; i < vSecond.size(); i++)
    {
        if (leftMost > vSecond[i].X)
        {
            leftMost = vSecond[i].X;
            isFirst = false; 
            index = i; 
        }
    }
    
    list<Complex> newList = list<Complex>(); 
    vector<Complex> currentList = isFirst ? vFirst : vSecond; 
    Complex startingPoint = currentList[index]; 

    //start finding intersecting points


    return first; 
}

double Intersect(Complex c1, Complex c2, Complex c3, Complex c4)
{
    double t; 
    double s; 

    t = ((c4.Y - c3.Y)*(c1.X - c3.X) - (c4.X - c3.X)*(c1.Y - c3.Y));
    t /= ((c4.X - c3.X)*(c2.Y - c1.Y) - (c4.Y - c3.Y)*(c2.X - c1.X)); 

    if (t >= 0 && t <= 1)
    {
        s = (c1.X - c3.X + (c2.X - c1.X)*t) / (c4.X - c3.X); 
        if (s >= 0 && s <= 1)
        {
            return t; 
        }
    }
    return 2; 
}