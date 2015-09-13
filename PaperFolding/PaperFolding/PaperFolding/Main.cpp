#include "Main.h"
#include <iostream> 
#include <fstream>
 
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

    //Open and get file data
    //GetData(argc, argv); 
    Folds.push_front(FootedVector(Complex(1, 1), Complex(5, 5)));
    Folds.push_front(FootedVector(Complex(1, 1), Complex(4, 5))); 
    Folds.push_front(FootedVector(Complex(5, 1), Complex(1, 5)));
    Cuts.push_back(FootedVector(Complex(2, 1), Complex(4, 1)));
    Cuts.push_back(FootedVector(Complex(3, 2), Complex(2, 4)));
    Cuts.push_back(FootedVector(Complex(5, 3), Complex(5, 5)));
    Cuts.push_back(FootedVector(Complex(1, 3), Complex(1, 4)));

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
        ifstream stream = ifstream(filename);
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

void GetFileContents(ifstream &stream)
{

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
    gluOrtho2D(0, 6, 0, 6); 

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
            else if (head.Y > 0 || tail.Y > 0)
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
    double slope = (vector.Head.Y - vector.Tail.Y) / (vector.Head.X - vector.Tail.X); 
    double intercept = vector.Tail.Y - slope * vector.Tail.X; 
    double yint = -intercept / slope; 

    FootedVector newVector = FootedVector(); 
    newVector.Head = Complex(yint, 0); 
    newVector.Tail = vector.Head.Y < 0 ? vector.Head : vector.Tail;
    return newVector; 
}