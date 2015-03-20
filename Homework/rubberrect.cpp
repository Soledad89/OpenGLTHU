//#include <windows.h>
#include <OpenGL/Gl.h>
#include <GLUT/glut.h>

struct GLintPoint {

	GLint x, y;

};

GLintPoint	corners[2];
bool		selected = false;
int			screenWidth = 640, screenHeight = 480;

void myDisplay() {

	glClear( GL_COLOR_BUFFER_BIT );
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();
	glColor3f( 1.0f, 1.0f, 1.0f );

	if( selected ) {

		glBegin( GL_QUADS );

		glVertex2i( corners[0].x, corners[0].y );
		glVertex2i( corners[0].x, corners[1].y );
		glVertex2i( corners[1].x, corners[1].y );
		glVertex2i( corners[1].x, corners[0].y );

		glEnd();

	}

	glutSwapBuffers();
	glFlush();

}

void myMouse( int button, int state, int x, int y ) {

	if( button == GLUT_LEFT_BUTTON && state == GLUT_DOWN ) {

		corners[0].x = x;
		corners[0].y = screenHeight - y;
		selected = true;

	}

	glutPostRedisplay();

}

void myPassiveMotion( int x, int y )  {

	corners[1].x = x;
	corners[1].y = screenHeight - y;
	glutPostRedisplay();

}

int main( int argc, char ** argv ) {

	glutInit( &argc, argv );

	// initialize window
	glutInitWindowSize( screenWidth, screenHeight );
	glutInitWindowPosition( 0, 0 );
	glutInitDisplayMode( GLUT_RGB | GLUT_DOUBLE );

	// create window
	glutCreateWindow( "Rubber Rect Demo" );

	// set the projection matrix
	glMatrixMode( GL_PROJECTION ); 
	glLoadIdentity();
	gluOrtho2D( 0, screenWidth, 0, screenHeight );

	glMatrixMode( GL_MODELVIEW );
	// clear rendering surface
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);  // background is black
	glViewport(0, 0, screenWidth, screenHeight);


	glutMouseFunc( myMouse );
	glutDisplayFunc( myDisplay );
	glutPassiveMotionFunc( myPassiveMotion );
	glutMainLoop();

	return( 0 );

}
