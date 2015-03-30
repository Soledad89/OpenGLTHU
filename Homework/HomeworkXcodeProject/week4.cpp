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

GLuint texture[1];

bool   gFlag = false;                                              // G健是否按下
GLuint filter = 0;                                          // 使用哪一个纹理过滤器
GLuint fogMode[]= { GL_EXP, GL_EXP2, GL_LINEAR};		// 雾气的模式
GLuint fogfilter= 0;                                    // 使用哪一种雾气
GLfloat fogColor[4]= {0.5f, 0.5f, 0.5f, 1.0f};          // 雾的颜色设为白色

int LoadGLTextures()
{
    
    texture[0] = SOIL_load_OGL_texture
    (
        "/Users/wangbo1/Documents/OpenCV/OpenGL/OpenGL-THU/Homework/HomeworkXcodeProject/Data/NeHe.bmp",//为什么只有全路径才可以？？？
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

    glClearColor(0.5f,0.5f,0.5f,1.0f);			// 设置背景的颜色为雾气的颜色
    glFogi(GL_FOG_MODE, fogMode[fogfilter]);		// 设置雾气的模式
    glFogfv(GL_FOG_COLOR, fogColor);			// 设置雾的颜色
    glFogf(GL_FOG_DENSITY, 0.35f);              // 设置雾的密度
    glHint(GL_FOG_HINT, GL_DONT_CARE);			// 设置系统如何计算雾气
    glFogf(GL_FOG_START, 1.0f);                 // 雾气的开始位置
    glFogf(GL_FOG_END, 5.0f);                   // 雾气的结束位置
    glEnable(GL_FOG);					// 使用雾气
    
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    
    glShadeModel(GL_SMOOTH);
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

void keyboard(unsigned char key, int x, int y)
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
    glutKeyboardFunc(keyboard);
    glutKeyboardUpFunc(keyboardup);
    glutSpecialFunc(keyboard);
	glutReshapeFunc(reshape);
    glutMainLoop(); 		     // go into a perpetual loop
    return 0;
}
