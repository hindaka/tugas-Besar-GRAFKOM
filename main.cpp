#include <Windows.h>
#include <iostream>
#include <gl\GL.h>
#include <gl\GLU.h>
#include <gl\glut.h>
#include <math.h>


void cube()
{
//menggambar kubus dan transformasi tarnslasi ke titik 0.5 0.5 0.5 dan skala 1 1 1
glPushMatrix();
glTranslated(0.5,0.5,0.5);//cube
glScaled(1.0,1.0,1.0);
glutSolidCube(1.0);
glPopMatrix();

}


void setMaterial()
{
//set properties of surfaces material
GLfloat mat_ambient[] = {0.7f,0.7f,0.7f,1.0f}; // ada 4 jenis material yang dipakai, dengan kombinasi warna tertentu
GLfloat mat_diffuse[] = {0.6f,0.6f,0.6f,1.0f};
GLfloat mat_specular[] = {1.0f,1.0f,1.0f,1.0f};
GLfloat mat_shininess[] = {50.0f};
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
gluLookAt(3.0,3.0,3.0,0,-0.25,0,0.0,1.0,0.0);
}


void displayObject()
{
setMaterial();
setLighting();
setViewport();
setCamera();
//startDrawing
glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
cube();//memanggil fungsi menggambar kubus
glFlush();//mengirim smua objek untuk dirender
}


int main(int argc, char **argv)
{
glutInit(&argc,argv);
glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB|GLUT_DEPTH);
glutInitWindowSize(640,480);
glutInitWindowPosition(100,100);
glutCreateWindow("cube sample bro !!");
glutDisplayFunc(displayObject);//fungsi dari display object yang menggabungkan kubus lighting material dan kamera
glEnable(GL_LIGHTING);
glEnable(GL_LIGHT0);
glShadeModel(GL_SMOOTH);
glEnable(GL_DEPTH_TEST);
glEnable(GL_NORMALIZE);
glClearColor(1.0f,1.0f,1.0f,0.0f);
glViewport(0,0,640,480);
glutMainLoop();
}
