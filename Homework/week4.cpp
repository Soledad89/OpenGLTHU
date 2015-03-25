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

GLfloat	xrot = 0.0;				// X Rotation ( NEW )
GLfloat	yrot = 0.0;				// Y Rotation ( NEW )
GLfloat	zrot = 0.0;				// Z Rotation ( NEW )

static GLuint texture[1];

int LoadGLTextures()
{
    /* load an image file directly as a new OpenGL texture */
    glGenTextures(1, &texture[0]);
    
    texture[0] = SOIL_load_OGL_texture
    (
        "Data/NeHe.bmp",
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
        return 0;
    
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    
    glShadeModel(GL_SMOOTH);
    glClearColor(0.0f,0.0f,0.0f,0.5f);
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);
    return 1;
}

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glTranslatef(0.0f,0.0f,-5.0f);
    glRotatef(xrot,1.0f,0.0f,0.0f);
    glRotatef(yrot,0.0f,1.0f,0.0f);
    glRotatef(zrot,0.0f,0.0f,1.0f);
    
    glBindTexture(GL_TEXTURE_2D,texture[0]);
    
    glBegin(GL_QUADS);
    // Front Face
    // Bottom Left Of The Texture and Quad
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);
    // Bottom Right Of The Texture and Quad
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);
    // Top Right Of The Texture and Quad
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f);
    // Top Left Of The Texture and Quad
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);
    glEnd();
    
    glBindTexture(GL_TEXTURE_2D,texture[0]);
    glBegin(GL_QUADS);
    // Back Face
    // Bottom Right Of The Texture and Quad
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
    // Top Right Of The Texture and Quad
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);
    // Top Left Of The Texture and Quad
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);
    // Bottom Left Of The Texture and Quad
    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f);
    glEnd();
    
    glBindTexture(GL_TEXTURE_2D,texture[0]);
    glBegin(GL_QUADS);
    // Top Face
    // Top Left Of The Texture and Quad
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);
    // Bottom Left Of The Texture and Quad
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f,  1.0f,  1.0f);
    // Bottom Right Of The Texture and Quad
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f,  1.0f,  1.0f);
    // Top Right Of The Texture and Quad
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);
    glEnd();
    
    glBindTexture(GL_TEXTURE_2D,texture[0]);
    glBegin(GL_QUADS);
    // Bottom Face
    // Top Right Of The Texture and Quad
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
    // Top Left Of The Texture and Quad
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f, -1.0f, -1.0f);
    // Bottom Left Of The Texture and Quad
    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);
    // Bottom Right Of The Texture and Quad
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);
    glEnd();
    
    glBindTexture(GL_TEXTURE_2D,texture[0]);
    glBegin(GL_QUADS);
    // Right face
    // Bottom Right Of The Texture and Quad
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f);
    // Top Right Of The Texture and Quad
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);
    // Top Left Of The Texture and Quad
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f);
    // Bottom Left Of The Texture and Quad
    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);
    glEnd();
    
    glBindTexture(GL_TEXTURE_2D,texture[0]);
    glBegin(GL_QUADS);
    // Left Face
    // Bottom Left Of The Texture and Quad
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
    // Bottom Right Of The Texture and Quad
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);
    // Top Right Of The Texture and Quad 
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f); 
    // Top Left Of The Texture and Quad 
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f); 
    glEnd(); 
    
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

void keyboard(int key, int x, int y)
//void keyboard(unsigned char key, int x, int y)
{
    switch (key) {
        case 27:
            exit(0);
            break;
        case GLUT_KEY_LEFT:
            xrot -= 1.0f;
            glutPostRedisplay();
            break;
        case GLUT_KEY_UP:
            yrot += 1.0f;
            glutPostRedisplay();
            break;
        case GLUT_KEY_DOWN:
            yrot -= 1.0f;
            glutPostRedisplay();
            break;
        case GLUT_KEY_RIGHT:
            xrot += 1.0f;
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
    //glutKeyboardFunc(keyboard);
    glutSpecialFunc(keyboard);
	glutReshapeFunc(reshape);
    glutMainLoop(); 		     // go into a perpetual loop
    return 0;
}
