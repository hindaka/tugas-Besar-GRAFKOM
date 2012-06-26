#include <Windows.h>
#include <iostream>
#include <gl\GL.h>
#include <gl\GLU.h>
#include <gl\glut.h>
#include <math.h>

static int putarx=0 ,putary=0, spin=0;

void panzer()
{
    glRotatef((GLfloat)putarx,1.0,0.0,0.0);
    glRotatef((GLfloat)putary,0.0,1.0,0.0);
    glRotatef((GLfloat)spin,0.0,0.0,1.0);
//============== BEGIN BODY GARASI=====================//

    //atap kanan
    glBegin(GL_POLYGON);
    glColor3f(0.3,0.2,0.8);
        glVertex3f(0.75,0.7,0.0);
        glVertex3f(1.5,0.5,0.0);
        glVertex3f(1.5,0.5,-1.8);
        glVertex3f(0.75,0.7,-1.8);
    glEnd();

    //atap kiri
    glBegin(GL_POLYGON);
    glColor3f(0.0,0.2,0.8);
        glVertex3f(0.75,0.7,0.0);
        glVertex3f(0.0,0.5,0.0);
        glVertex3f(0.0,0.5,-1.8);
        glVertex3f(0.75,0.7,-1.8);
    glEnd();

    //bagian atas
    glBegin(GL_POLYGON);
    glColor3f(0.3,0.2,0.1);
        glVertex3f(0.0,0.5,0.0);
        glVertex3f(1.5,0.5,0.0);
        glVertex3f(1.5,0.5,-1.8);
        glVertex3f(0.0,0.5,-1.8);
    glEnd();

    //bagian belakang
    glBegin(GL_POLYGON);
    glColor3f(0.2,0.2,0.2);
        glVertex3f(0.0,0.0,-1.8);
        glVertex3f(1.5,0.0,-1.8);
        glVertex3f(1.5,0.5,-1.8);
        glVertex3f(0.0,0.5,-1.8);
    glEnd();

    //bagian kanan
    glBegin(GL_POLYGON);
    glColor3f(0.4,0.5,0.1);
        glVertex3f(0.0,0.0,0.0);
        glVertex3f(0.0,0.5,0.0);
        glVertex3f(0.0,0.5,-1.8);
        glVertex3f(0.0,0.0,-1.8);
    glEnd();

    //bagian tutup kanan
    glBegin(GL_POLYGON);
    glColor3f(0.2,0.2,0.2);
        glVertex3f(0.0,0.0,0.0);
        glVertex3f(0.25,0.0,0.0);
        glVertex3f(0.25,0.5,0.0);
        glVertex3f(0.0,0.5,0.0);
    glEnd();

    //bagian kiri
    glBegin(GL_POLYGON);
    glColor3f(0.2,0.3,0.5);
        glVertex3f(1.5,0.0,0.0);
        glVertex3f(1.5,0.5,0.0);
        glVertex3f(1.5,0.5,-1.8);
        glVertex3f(1.5,0.0,-1.8);
    glEnd();

    //bagian tutup kanan
    glBegin(GL_POLYGON);
    glColor3f(0.2,0.2,0.2);
        glVertex3f(1.5,0.0,0.0);
        glVertex3f(1.25,0.0,0.0);
        glVertex3f(1.25,0.5,0.0);
        glVertex3f(1.5,0.5,0.0);
    glEnd();
//============== END BODY GARASI=====================//
    glutSwapBuffers();
}

void mykey(int key, int x, int y){
switch(key){
    case GLUT_KEY_HOME      : putarx = (putarx + 5) % 360;
                              glutPostRedisplay();
                              break;
    case GLUT_KEY_END       : putarx = (putarx - 5) % 360;
                              glutPostRedisplay();
                              break;
    case GLUT_KEY_PAGE_UP   : putary = (putary + 5) % 360;
                              glutPostRedisplay();
                              break;
    case GLUT_KEY_PAGE_DOWN : putary = (putary - 5) % 360;
                              glutPostRedisplay();
                              break;
    default             : break;
    }
}

void spinDisplay(void){
    spin=spin+1.0;
    if (spin > 360.0);
    glutPostRedisplay();
}

void mouse(int button, int state, int x, int y){
switch(button){
    case GLUT_LEFT_BUTTON : if (state == GLUT_DOWN)
                                glutIdleFunc(spinDisplay);
                                break;
    case GLUT_RIGHT_BUTTON : if(state == GLUT_DOWN)
                                glutIdleFunc(NULL);
                                break;
    default                 : break;
    }
}

void setMaterial()
{
    //set properties of surfaces material
    GLfloat mat_ambient[] = {0.7f,0.7f,0.7f,1.0f};
    GLfloat mat_diffuse[] = {0.6f,0.6f,0.6f,1.0f};
    GLfloat mat_specular[] = {1.0f,1.0f,1.0f,1.0f};
    GLfloat mat_shininess[] = {50.0f};
    glColorMaterial(GL_FRONT_AND_BACK,GL_EMISSION);
    glMaterialfv(GL_FRONT,GL_AMBIENT,mat_ambient);
    glMaterialfv(GL_FRONT,GL_DIFFUSE,mat_diffuse);
    glMaterialfv(GL_FRONT,GL_SPECULAR,mat_specular);
    glMaterialfv(GL_FRONT,GL_SHININESS,mat_shininess);
}


void setLighting()
{
    //set light sources
    GLfloat lightIntensity[] = {0.7f,0.7f,0.7f,1.0f};//mensetting pencahayaan
    GLfloat light_position[] = {3.0f,2.0f,6.0f,0.0f};
    glLightfv(GL_LIGHT0,GL_DIFFUSE,lightIntensity);
    glLightfv(GL_LIGHT0,GL_POSITION,light_position);
}


void setViewport()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    double winHt = 1.0;//half height of the window
    glOrtho(-winHt*64/48,winHt*64/48,-winHt,winHt,0.1,100.0);
}


void setCamera()
{
    //set the camera
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(30.0,20.0,60.0,0.0,0.0,-1.5,0.0,2.0,0.0);
    }


void displayObject()
{
    setMaterial();
    setLighting();
    setViewport();
    setCamera();
    //startDrawing
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    panzer();//memanggil fungsi menggambar objek sate
    glFlush();//mengirim semua objek untuk dirender
}


int main(int argc, char **argv)
{
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB|GLUT_DEPTH);
    glutInitWindowSize(640,480);
    glutInitWindowPosition(250,200);
    glutCreateWindow("bangun 3D");
    glutDisplayFunc(displayObject);//fungsi dari display object yang menggabungkan objek sate lighting material dan kamera
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);
    //glEnable(GL_LIGHT0);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);
    glutSpecialFunc(mykey);
    //glutMouseFunc(mouse);
    glClearColor(0.0f,0.0f,0.0f,0.0f);
    glViewport(0,0,640,480);
    glutMainLoop();
}
