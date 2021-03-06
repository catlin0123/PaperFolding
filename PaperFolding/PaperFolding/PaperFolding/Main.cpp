#include "Main.h"
#include <iostream> 
#include <fstream>
using namespace std; 
 
list<FootedVector> Folds = list<FootedVector>(); 
list<FootedVector> Cuts = list<FootedVector>(); 

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

	glLineWidth(3);

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
    double x1, x2, y1, y2; 
    int numFolds, numCuts; 

    stream >> numFolds >> numCuts; 
    for (int i = 0; i < numFolds; i++)
    {
        stream >> x1 >> y1 >> x2 >> y2; 
        Folds.push_front(FootedVector(Complex(x1, y1), Complex(x2, y2)));
    }

    for (int i = 0; i < numCuts; i++)
    {
        stream >> x1 >> y1 >> x2 >> y2;
        Cuts.push_front(FootedVector(Complex(x1, y1), Complex(x2, y2)));
    }
}

void Display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    
    for (FootedVector vector : Folds)
    {
        DrawLine(vector, true, GREEN); 
    }

    for (FootedVector vector : Cuts)
    {
        DrawLine(vector, false, BLUE); 
    }

    if (Folds.size() > 0)
    {
        DrawLine(Folds.back(), true, RED); 
    }
    

    glutSwapBuffers();
}

void Reshape(int width, int height)
{
    ScreenWidth = width;
    ScreenHeight = height;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity(); 
    gluOrtho2D(-10, 10, -10, 10); 

    if (float(width) / height > 1)
        glViewport((width - height) / 2, 0, height, height);
    else
        glViewport(0, (height - width) / 2, width, width);
}

void Keyboard(unsigned char key, int x, int y)
{
    static FootedVector current = FootedVector(); 
    list<FootedVector> remove = list <FootedVector>();
    list<FootedVector> add = list<FootedVector>();

    switch (key)
    {
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
	FootedVector newVector = FootedVector();
	if (vector.Head.X - vector.Tail.X != 0)
	{
		double slope = (vector.Head.Y - vector.Tail.Y) / (vector.Head.X - vector.Tail.X);
		double intercept = vector.Tail.Y - slope * vector.Tail.X;
		double yint = -intercept / slope;

		newVector.Head = Complex(yint, 0);		
	}
	else
	{
		newVector.Head = Complex(vector.Head.X, 0);
	}
	newVector.Tail = vector.Head.Y < 0 ? vector.Head : vector.Tail;
	return newVector;
}