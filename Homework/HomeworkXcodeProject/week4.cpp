//#include <windows.h> //0use proper includes for your system
//#include <math.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/Glut.h>
#include <math.h>
#include <iostream>
#include <string>

using namespace std;

#define PI 3.14159265
#define RED 1
#define GREEN 2
#define BLUE 3
#define WHITE 4


//const int screenWidth = 640;	   // width of the screen window in pixels 
//const int screenHeight = 480;	   // height of the screen window in pixels
const int startwinsize = 640; // Starting window width & height, in pixels
int winw, winh;               // Window width & height, in pixels, saved by reshape
const double pointsize = 4;  // Size of point

// Keyboard
const int ESCKEY = 27;        // ASCII value of escape character

GLfloat tri_angle = 0.0;
GLfloat quads_angle = 0.0;

struct GLintPoint
{
    GLint x, y;
};

static GLintPoint corner[2];
bool mouseleftdown  = false;

float a, b;
float d = 0.0;
float len = 0.0;
int flagx = 1;
int flagy = 1;
float red = 0.0, blue = 1.0, green = 1.0;
float deg_pyr = 0.0;

//<<<<<<<<<<<<<<<<<<<<<<<< myDisplay >>>>>>>>>>>>>>>>>
// printbitmap
// Prints the given string at the given raster position
//  using GLUT bitmap fonts.
// You probably don't want any rotations in the model/view
//  transformation when calling this function.
void printbitmap(const string msg, double x, double y)
{
   glRasterPos2d(x, y);
   for (string::const_iterator ii = msg.begin();
        ii != msg.end();
        ++ii)
   {
      glutBitmapCharacter(GLUT_BITMAP_9_BY_15, *ii);
   }
}


void myDisplay(void)
{
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    float angle = 0.0;
    glLoadIdentity();
    if (mouseleftdown)
    {
        len = pow((corner[1].y - corner[0].y),2) + pow((corner[1].x - corner[0].x),2);
        angle = sqrt(len)/winw * 540;
        flagx = (corner[1].x > corner[0].x) ? 1 : -1;
        flagy = (corner[1].y > corner[0].y) ? 1 : -1;
        if (corner[1].y == corner[0].y)
        {
            if (corner[1].x >= corner[0].x)
            {
                d = 0; a = 0; b = 1;
            }
            else
            {
                d = 0; a = 0; b = -1;
            }
        }
        
        if (corner[1].y != corner[0].y && corner[1].x != corner[0].x)
        {
            d = ((double)(corner[1].y - corner[0].y)/(double)(corner[1].x - corner[0].x));
            a = flagx; b = - flagx / (d * flagy);
        }
    }
    /*
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
   // glTranslatef(-0.5f,-0.5f,0.0f);
    glRotatef(angle,a,b,0.0);
    //glRotatef(deg_pyr,1.0f,0.0f,0.0f);
	glBegin(GL_TRIANGLES);					
		glColor3f(1.0f,0.0f,0.0f);		
		glVertex3f( 0.0f, 0.25f, 0.0f);
        glVertex3f(-0.25f,-0.25f, 0.25f);
		glVertex3f( 0.25f,-0.25f, 0.25f);			
		
        glColor3f(0.0f,1.0f,0.0f);					
		glVertex3f( 0.0f, 0.25f, 0.0f);			
		glVertex3f( 0.25f,-0.25f, 0.25f);	
		glVertex3f( 0.25f,-0.25f, -0.25f);
	
        glColor3f(0.0f,0.0f,0.5f);
		glVertex3f( 0.0f, 0.25f, 0.0f);	
		glVertex3f( 0.25f,-0.25f, -0.25f);				
		glVertex3f(-0.25f,-0.25f, -0.25f);			

        glColor3f(0.5f,0.5f,0.0f);
		glVertex3f( 0.0f, 0.25f, 0.0f);		
		glVertex3f(-0.25f,-0.25f,-0.25f);
		glVertex3f(-0.25f,-0.25f, 0.25f);

		glColor3f(0.0f,0.5f,0.5f);
		glVertex3f(-0.25f,-0.25f,0.25f);
		glVertex3f(-0.25f,-0.25f,-0.25f);
		glVertex3f(0.25,-0.25f,-0.25f);

		glColor3f(0.0f, 0.5f, 0.5f);
		glVertex3f(0.25f,-0.25f,-0.25f);
		glVertex3f(0.25f,-0.25f,0.25f);
		glVertex3f(-0.25f,-0.25f,0.25f);
	glEnd();
    //deg_pyr ++;
    */
    //glMatrixMode(GL_PROJECTION);
	
    glLoadIdentity();
	//glTranslatef(0.5f,-0.5f,0.0)
    glRotatef(angle,a,b,0.0f);
	glBegin(GL_QUADS);			
		glColor3f(0.0f,1.0f,0.0f);						
		glVertex3f( 0.25f, 0.25f,-0.25f);				
		glVertex3f(-0.25f, 0.25f,-0.25f);			
		glVertex3f(-0.25f, 0.25f, 0.25f);		
		glVertex3f( 0.25f, 0.25f, 0.25f);	
		
        glColor3f(1.0f,0.5f,0.0f);
		glVertex3f( 0.25f,-0.25f, 0.25f);				
		glVertex3f(-0.25f,-0.25f, 0.25f);			
		glVertex3f(-0.25f,-0.25f,-0.25f);					
		glVertex3f( 0.25f,-0.25f,-0.25f);				
    
        glColor3f(1.0f,0.0f,0.0f);
        glVertex3f( 0.25f, 0.25f, 0.25f);
		glVertex3f(-0.25f, 0.25f, 0.25f);	
		glVertex3f(-0.25f,-0.25f, 0.25f);
		glVertex3f( 0.25f,-0.25f, 0.25f);				

        glColor3f(1.0f,1.0f,0.0f);
		glVertex3f( 0.25f,-0.25f,-0.25f);		
		glVertex3f(-0.25f,-0.25f,-0.25f);			
		glVertex3f(-0.25f, 0.25f,-0.25f);				
		glVertex3f( 0.25f, 0.25f,-0.25f);					
		
        glColor3f(0.0f,0.0f,1.0f);
		glVertex3f(-0.25f, 0.25f, 0.25f);	
		glVertex3f(-0.25f, 0.25f,-0.25f);		
		glVertex3f(-0.25f,-0.25f,-0.25f);			
		glVertex3f(-0.25f,-0.25f, 0.25f);				
		
        glColor3f(1.0f,0.0f,1.0f);
		glVertex3f( 0.25f, 0.25f,-0.25f);	
		glVertex3f( 0.25f, 0.25f, 0.25f);		
		glVertex3f( 0.25f,-0.25f, 0.25f);			
		glVertex3f( 0.25f,-0.25f,-0.25f);				
	glEnd();											
    //glMatrixMode(GL_MODELVIEW);
    
    glFlush();
    glutSwapBuffers();
}


void processMenuEvents(int option) {

	switch (option) {
		case RED : red = 1.0; green = 0.0; blue = 0.0; break;
		case GREEN : red = 0.0; green = 1.0; blue = 0.0; break;	
		case BLUE : red = 0.0; green = 0.0; blue = 1.0; break;
		case WHITE : red = 1.0; green = 1.0; blue = 1.0; break;
	}
}

void myMouse(int button, int state, GLint x, GLint y)
{
    //static GLintPoint corner[2];
    //static int numCorners = 0;
    if (button == GLUT_LEFT_BUTTON)
    {
        if (state == GLUT_DOWN)
        {
            corner[0].x = x;
            corner[0].y = winw - y;
            mouseleftdown = true;
        }else if (state == GLUT_UP)
        {
            glClear(GL_COLOR_BUFFER_BIT);
            mouseleftdown = false;
            glFlush();
        }
    }
    
    glutPostRedisplay();
}
 
void myMovedMouse(GLint x, GLint y)
{
    if (mouseleftdown)
	{
		glutPostRedisplay();
	}
	
	corner[1].y = winh - y;
	corner[1].x = x;
}

void keyboard(unsigned char key, int x, int y)
{
   switch (key)
   {
   case ESCKEY:  // ESC: Quit
      exit(0);
      break;
   }
}

void reshape(int w, int h)
{
   // Set the viewport to be the entire drawing area of the window
   glViewport(0, 0, w, h);
   // glViewport(0, 0, w/2, h/2);
   // Save the window size
   winw = w;
   winh = h;

   // Set up the projection
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   gluOrtho2D(-1.0, 1.0, -1.0, 1.0);//默认就是这样的, 如果前面不设置成gL_Projection，这条语句将不会起作用。
   //gluOrtho2D(-1.0, 0.0, -1.0, 0.0);
    glMatrixMode(GL_MODELVIEW);  // Always go back to model/view mode
}


//主函数
//<<<<<<<<<<<<<<<<<<<<<<<< main >>>>>>>>>>>>>>>>>>>>>>
int main(int argc, char** argv)

{
    //设置窗口
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGB);
    glutInitWindowSize(startwinsize, startwinsize); // set the window size
    glutInitWindowPosition(200, 150); // set the window position on screen
    glutCreateWindow("OpenGL Learning Platform"); // open the screen window

	
    //设置渲染
    glClearColor(0.0,0.0,0.0,0.0);       // the background color is white
    glPointSize(pointsize);
    glEnable(GL_DEPTH_TEST); //没加入的话导致的问题就是会有“透明”的效果。近处的东西不会挡住远处的。
    //glShadeModel(GL_SMOOTH);
    //glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    
    glutDisplayFunc(myDisplay);
    glutIdleFunc(myDisplay);
    //glutPassiveMotionFunc(myMovedMouse);
	
	glutCreateMenu(processMenuEvents);
	glutAddMenuEntry("Red",RED);
	glutAddMenuEntry("Blue",BLUE);
	glutAddMenuEntry("Green",GREEN);
	glutAddMenuEntry("White",WHITE);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
	
    glutMouseFunc(myMouse);
    glutMotionFunc(myMovedMouse);
    glutKeyboardFunc(keyboard);
	glutReshapeFunc(reshape);
    
    glutMainLoop(); 		     // go into a perpetual loop
    return 0;
}
