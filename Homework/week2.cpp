//#include <windows.h> //0use proper includes for your system
//#include <math.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/Glut.h>
const int screenWidth = 640;	   // width of the screen window in pixels 
const int screenHeight = 480;	   // height of the screen window in pixels
//<<<<<<<<<<<<<<<<<<<<<<< myInit >>>>>>>>>>>>>>>>>>>>
 void myInit(void)
 {
    glClearColor(0.0,0.0,0.0,0.0);       // the background color is white
	glPointSize(4.0);		       
	glMatrixMode(GL_PROJECTION); 	   
	glLoadIdentity();
	gluOrtho2D(0.0f, (GLdouble)screenWidth,0.0f,(GLdouble)screenHeight);
}
//<<<<<<<<<<<<<<<<<<<<<<<< myDisplay >>>>>>>>>>>>>>>>>
void myDisplay(void)
{
	glClear(GL_COLOR_BUFFER_BIT);     // clear the screen
	
	glTranslatef(350,100,0);
	glBegin(GL_TRIANGLES);								// Drawing Using Triangles
		glColor3f(1.0f, 1.0f, 0.0f);         // the drawing color
		glVertex3i(0,200,0 );					// Top
		glVertex3i(-100,0,0);					// Bottom Left
		glVertex3i(100,0,0);					// Bottom Right
	glEnd();											// Finished Drawing The Triangle
	
	glTranslatef(300,0,0);					
	glBegin(GL_QUADS);									// Draw A Quad
		glColor3f(0.0f, 1.0f, 0.0f);         // the drawing color
		glVertex3f(-100,0,0);					// Top Left
		glVertex3f(100,0,0);					// Top Right
		glVertex3f(100,200,0);					// Bottom Right
		glVertex3f(-100,200,0);					// Bottom Left
	glEnd();	
	glFlush();		   // send all output to display 
}
//<<<<<<<<<<<<<<<<<<<<<<<< main >>>>>>>>>>>>>>>>>>>>>>
int main(int argc, char** argv)
{
	glutInit(&argc, argv);          // initialize the toolkit
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB); // set the display mode
	glutInitWindowSize(screenWidth, screenHeight); // set the window size
	glutInitWindowPosition(200, 150); // set the window position on screen
	glutCreateWindow("Plot the Traianle and the roct"); // open the screen window
	glutDisplayFunc(myDisplay);     // register the redraw function
	myInit();                   
	glutMainLoop(); 		     // go into a perpetual loop
	return 0;
}
