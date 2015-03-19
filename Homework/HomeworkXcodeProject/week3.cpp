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

// 全局变量
const int screenWidth = 640;	   // width of the screen window in pixels 
const int screenHeight = 480;	   // height of the screen window in pixels
GLfloat tri_angle = 0.0;
GLfloat quads_angle = 0.0;

struct GLintPoint {
    GLint x, y;
};

GLintPoint corner[2];
bool selected = false;


//回调函数
//<<<<<<<<<<<<<<<<<<<<<<<< myDisplay >>>>>>>>>>>>>>>>>
void myDisplay(void)
{
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glLoadIdentity();
    glColor3f(0.0f, 1.0f, 0.0f);
    glTranslatef(-0.5, 0.5, 0.0);
    glRotatef(tri_angle,0.0,0.0,1.0);
    glBegin(GL_TRIANGLES);
        glVertex3f(-0.4, -0.4, 0);
        glVertex3f(0.4, -0.4, 0);
        glVertex3f(0, 0.4, 0);
    glEnd();
    tri_angle ++;
    
    
    glLoadIdentity();
    glColor3f(0.0f, 0.0f, 1.0f);
    glTranslatef(0.5, 0.5, 0.0);
    glRotatef(quads_angle,0.0,0.0,1.0);
    glBegin(GL_QUADS);
        glVertex3f(-0.4,-0.4,0.0);
        glVertex3f(0.4,-0.4,0.0);
        glVertex3f(0.4,0.4,0.0);
        glVertex3f(-0.4, 0.4, 0);
    glEnd();
    quads_angle += 2;
    
    glFlush();
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
/*
void myMouse(int button, int state, GLint x, GLint y)
{
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        corner[0].x = x;
        corner[0].y = screenHeight - y;
        selected = true;
        
    }
    
    glutPostRedisplay();
}
 
void myMovedMouse(GLint x, GLint y)
{
    corner[1].x = x;
    corner[1].y = screenHeight - y;
    
    glutPostRedisplay();
}
*/

//主函数
//<<<<<<<<<<<<<<<<<<<<<<<< main >>>>>>>>>>>>>>>>>>>>>>
int main(int argc, char** argv)

{
    //设置窗口
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGB);
    glutInitWindowSize(screenWidth, screenHeight); // set the window size
    glutInitWindowPosition(200, 150); // set the window position on screen
    glutCreateWindow("Plot the Traianle and the roct"); // open the screen window
    glutDisplayFunc(myDisplay);
    glutIdleFunc(myDisplay);

	
    //设置渲染
    glClearColor(0.0,0.0,0.0,0.0);       // the background color is white
    glPointSize(4.0);
    //glViewport(0, 0, (GLdouble)screenWidth, (GLdouble)screenHeight);

    //glutMouseFunc(myMouse);
    //glutPassiveMotionFunc(myMovedMouse);
    //glutMotionFunc(myMovedMouse);
    
    
    glutMainLoop(); 		     // go into a perpetual loop
    return 0;
}
