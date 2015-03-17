//#include <windows.h> //0use proper includes for your system
//#include <math.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/Glut.h>

// global variables defining here
const int screenWidth = 640;	   // width of the screen window in pixels 
const int screenHeight = 480;	   // height of the screen window in pixels
Glfloat tri_angle = 0.0;
GLfloat squ_angle = 0.0;

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
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);     // clear the screen 
	glTranslatef(150,100,0);
	glBegin(GL_TRIANGLES);								// Drawing Using Triangles
	
	glEnd();											// Finished Drawing The Triangle
	glTranslatef(300,0,0);					
	
	glBegin(GL_QUADS);									// Draw A Quad
	
	glEnd();	
	glFlush();		   // send all output to display 
}
//<<<<<<<<<<<<<<<<<<<<<<<< main >>>>>>>>>>>>>>>>>>>>>>
int main(int argc, char** argv)
{
	glutInit(&argc, argv);          // initialize the toolkit
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGB); // set the display mode
	glutInitWindowSize(screenWidth, screenHeight); // set the window size
	glutInitWindowPosition(200, 150); // set the window position on screen
	glutCreateWindow("Plot the Traianle and the roct"); // open the screen window
	glutDisplayFunc(myDisplay);     // register the redraw function
	glutIdleFunc(myDisplay);
	
	myInit();                   
	
	// 
	// add menu and other components here
	
	//

	glutMainLoop(); 		     // go into a perpetual loop
	return 0;
}
