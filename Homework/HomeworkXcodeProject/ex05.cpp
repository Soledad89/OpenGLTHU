//#include <windows.h> //0use proper includes for your system
//#include <math.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/Glut.h>
#include "SOIL.h"
#include <math.h>
#include <iostream>
#include <string>
using namespace std;
#define dim 100

GLfloat	xrot = 00.0;
GLfloat	yrot = 180.0;
GLfloat	zrot = 00.0;

float points[dim][dim][3];
int vel = 0;

GLfloat hold;

GLuint	texture[1];			// Storage For One Texture ( NEW )

int LoadGLTextures()
{
    
    texture[0] = SOIL_load_OGL_texture
    (
        "/Users/wangbo1/Documents/Computer_Graphics/OpenGL/OpenGL-THU/Homework/HomeworkXcodeProject/Data/Tim.bmp",//为什么只有全路径才可以？？？
        //"NeHe.bmp",
        SOIL_LOAD_AUTO,
        SOIL_CREATE_NEW_ID,
        SOIL_FLAG_INVERT_Y
     );
    
    if(!texture[0])
        return false;
    
    glBindTexture(GL_TEXTURE_2D, texture[0]);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    
    return true;
}

int init()
{
    if(!LoadGLTextures())
    {
        exit(0);
        return 0;
    }

	for(int x=0; x<dim; x++)
	{
		for(int y=0; y<dim; y++)
		{
			points[x][y][0]=float((x*9.0f/(dim*1.0))-4.5f);
			points[x][y][1]=float((y*9.0f/(dim*1.0))-4.5f);
			points[x][y][2]=float(sin((x/(dim*0.5))*3.141592654*2.0f));
		}
	}

	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    
	glPolygonMode( GL_BACK, GL_FILL );
    glPolygonMode( GL_FRONT, GL_FILL);
    //glPolygonMode( GL_FRONT, GL_FILL);
    
    glShadeModel(GL_SMOOTH);
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);
    return 1;
}

void display(void)
{
	
	int x, y;
	float float_x, float_y, float_xb, float_yb;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	glTranslatef(0.0f,0.0f,-12.0f);
	  
	glRotatef(xrot,1.0f,0.0f,0.0f);
	glRotatef(yrot,0.0f,1.0f,0.0f);  
	glRotatef(zrot,0.0f,0.0f,1.0f);
//	glRotatef(90,1.0f,0.0f,0.0f);

	glBindTexture(GL_TEXTURE_2D, texture[0]);

	glBegin(GL_QUADS);
	for( x = 0; x < dim-1; x++ )
	{
		for( y = 0; y < dim-1; y++ )
		{
			float_x = float(x)/(dim/1.0-1);
			float_y = float(y)/(dim/1.0-1);
			float_xb = float(x+1)/(dim/1.0-1);
			float_yb = float(y+1)/(dim/1.0-1);

			glTexCoord2f( float_x, float_y);
			glVertex3f( points[x][y][0], points[x][y][1], points[x][y][2] );

			glTexCoord2f( float_x, float_yb );
			glVertex3f( points[x][y+1][0], points[x][y+1][1], points[x][y+1][2] );

			glTexCoord2f( float_xb, float_yb );
			glVertex3f( points[x+1][y+1][0], points[x+1][y+1][1], points[x+1][y+1][2] );

			glTexCoord2f( float_xb, float_y );
			glVertex3f( points[x+1][y][0], points[x+1][y][1], points[x+1][y][2] );
		}
	}
	glEnd();

	if( vel == 1)
	{
		for( y = 0; y < dim; y++ )
		{
			hold=points[0][y][2];
			for( x = 0; x < dim-1; x++)
			{
				points[x][y][2] = points[x+1][y][2];
			}
			points[dim-1][y][2]=hold;
		}
		vel = 0;
	}

	vel++;

    glutSwapBuffers();
}

void reshape(int w, int h)
{
    glViewport(0, 0, (GLsizei) w, (GLsizei) h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, (GLfloat) w/(GLfloat) h, 1.0, 30.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0.0, 0.0, -3.6);
}

void mykeyboard(unsigned char key, int x, int y)
{
    switch (key) {
        case 27:
            exit(0);
            break;
        case ' ':
            break;
        default:
            //gFlag = false;
            break;
    }
}

void keyboardup(unsigned char key, int x, int y)
{
    switch (key) {
        case ' ':
            
            break;
            
        default:
            break;
    }
}

void keyboard(int key, int x, int y)
//void keyboard(unsigned char key, int x, int y)
{
    switch (key) {
        case GLUT_KEY_LEFT:
            yrot -= 3.0f;
            glutPostRedisplay();
            break;
        case GLUT_KEY_UP:
            xrot += 3.0f;
            glutPostRedisplay();
            break;
        case GLUT_KEY_DOWN:
            xrot -= 3.0f;
            glutPostRedisplay();
            break;
        case GLUT_KEY_RIGHT:
            yrot += 3.0f;
            glutPostRedisplay();
            break;
        default:
            break;
    }
}


int main(int argc, char** argv)

{
    //设置窗口
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGB);
    glutInitWindowSize(480, 480); // set the window size
    glutInitWindowPosition(200, 160); // set the window position on screen
    glutCreateWindow("OpenGL Learning Platform"); // open the screen window
    init();
    glutDisplayFunc(display);
    glutIdleFunc(display);
    glutKeyboardFunc(mykeyboard);
    glutKeyboardUpFunc(keyboardup);
    glutSpecialFunc(keyboard);
	glutReshapeFunc(reshape);
    glutMainLoop(); 		     // go into a perpetual loop
    return 0;
}
