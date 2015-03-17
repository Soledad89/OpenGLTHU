//#include <windows.h> //0use proper includes for your system
//#include <math.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/Glut.h>
// This is in the Xcode project directory.
// to test if the xcode project can sync the change by vim
// to test if the xcode exchange can be synced to the github
// to see this can be immediatelly shown in xcode .
// to test this file can be commited by xcode.

const int screenWidth = 640;	   // width of the screen window in pixels 
const int screenHeight = 480;	   // height of the screen window in pixels
GLfloat tri_angle = 0.0;
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

	glBegin(GL_TRIANGLES);								// Drawing Using Triangles
		glColor3f(1.0f, 1.0f, 0.0f);         // the drawing color
		glVertex3f(100,100,0);					// Top
		glVertex3f(300,100,0);					// Bottom Left
		glVertex3f(200,300,0);					// Bottom Right
	glEnd();											// Finished Drawing The Triangle
	
	glBegin(GL_QUADS);									// Draw A Quad
		glColor3f(0.0f, 1.0f, 0.0f);         // the drawing color
		glVertex3f(400,100,0);					// Top Left
		glVertex3f(600,100,0);					// Top Right
		glVertex3f(600,300,0);					// Bottom Right
		glVertex3f(400,300,0);					// Bottom Left
	glEnd();	

	glBegin(GL_QUADS);									// Draw A Quad
		glColor3f(0.0f, 1.0f, 0.0f);         // the drawing color
		glVertex3f(-0.5,-0.5,0);					// Top Leftgit
		glVertex3f(0.5,-0.5,0);					// Top Right
		glVertex3f(0.5,0.5,0);					// Bottom Right
		glVertex3f(-0.5,0.5,0);					// Bottom Left
	glEnd();	
	
	glFlush();		   // send all output to display

	glutSwapBuffers();

}

/*
void processMenuEvents(int option) {

	switch (option) {
		case RED : red = 1.0; green = 0.0; blue = 0.0; break;
		case GREEN : red = 0.0; green = 1.0; blue = 0.0; break;
		case BLUE : red = 0.0; green = 0.0; blue = 1.0; break;
		case WHITE : red = 1.0; green = 1.0; blue = 1.0; break;
	}
*/



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
	//call our function to create the menu
	/*	
	glutCreateMenu(processMenuEvents);
	glutAddMenuEntry("Red",RED);
	glutAddMenuEntry("Blue",BLUE);
	glutAddMenuEntry("Green",GREEN);
	glutAddMenuEntry("White",WHITE);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
	*/
	//

	glutMainLoop(); 		     // go into a perpetual loop
	return 0;
}
