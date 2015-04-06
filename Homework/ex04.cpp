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

GLfloat	xrot = 30.0;				// X Rotation ( NEW )
GLfloat	yrot = 30.0;				// Y Rotation ( NEW )
GLfloat	zrot = 30.0;				// Z Rotation ( NEW )

float points[45][45][3];    // The Array For The Points On The Grid Of Our "Wave"
int wiggle_count = 0;		// Counter Used To Control How Fast Flag Waves

GLfloat hold;				// Temporarily Holds A Floating Point Value
int flag = 0;

GLuint	texture[1];			// Storage For One Texture ( NEW )

int LoadGLTextures()
{
    
    texture[0] = SOIL_load_OGL_texture
    (
        "/Users/wangbo1/Documents/Computer_Graphics/OpenGL/OpenGL-THU/Homework/HomeworkXcodeProject/Data/NeHe.bmp",//为什么只有全路径才可以？？？
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

	for(int x=0; x<45; x++)
	{
		for(int y=0; y<45; y++)
		{
			points[x][y][0]=float((x/5.0f)-4.5f);
			points[x][y][1]=float((y/5.0f)-4.5f);
			points[x][y][2]=float(sin((((x/5.0f)*40.0f)/360.0f)*3.141592654*2.0f));
		}
	}

	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);				// Black Background
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    
	glPolygonMode( GL_BACK, GL_FILL );					// Back Face Is Solid
	glPolygonMode( GL_FRONT, GL_LINE );					// Front Face Is Made Of Lines
	
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

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear The Screen And The Depth Buffer
	glLoadIdentity();									// Reset The View

	glTranslatef(0.0f,0.0f,-12.0f);
	  
	glRotatef(xrot,1.0f,0.0f,0.0f);
	glRotatef(yrot,0.0f,1.0f,0.0f);  
	glRotatef(zrot,0.0f,0.0f,1.0f);
//	glRotatef(90,1.0f,0.0f,0.0f);

	glBindTexture(GL_TEXTURE_2D, texture[0]);

	glBegin(GL_QUADS);
	for( x = 0; x < 44; x++ )
	{
		for( y = 0; y < 44; y++ )
		{
			float_x = float(x)/44.0f;
			float_y = float(y)/44.0f;
			float_xb = float(x+1)/44.0f;
			float_yb = float(y+1)/44.0f;

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

	if( wiggle_count == 2 )
	{
		for( y = 0; y < 45; y++ )
		{
			hold=points[0][y][2];
			for( x = 0; x < 44; x++)
			{
				points[x][y][2] = points[x+1][y][2];
			}
			points[44][y][2]=hold;
		}
		wiggle_count = 0;
	}

	wiggle_count++;

	xrot+=0.3f;
	yrot+=0.2f;
	zrot+=0.4f;

    glutSwapBuffers();
	return 1;
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
            if (!gFlag)
            {
                gFlag = true;
                fogfilter += 1;
                if (fogfilter > 2)
                {
                    fogfilter = 0;
                }
                glFogi(GL_FOG_MODE, fogMode[fogfilter]);   // Fog Mode

            }
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
            gFlag = false;
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
