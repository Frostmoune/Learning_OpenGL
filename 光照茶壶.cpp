#include <iostream>
#include <cmath>
#include <gl\glut.h>
#include <gl\GL.h>
#include <gl\GLU.h>
#include <cstdio>
#include <windows.h>

using namespace std;

void init() {
	glClearColor(0.0, 0.0, 0.0, 0.0);
	GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light_position[] = { 0.7, 0.7, 0.0, 0.0 };
	GLfloat white_light[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light_model[] = { 0.8 , 0.8 , 0.8 , 1.0 };  
	glShadeModel(GL_SMOOTH);
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE); // 设置材料
	glColor4fv(light_model);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position); // 设置光源位置
	//glLightfv(GL_LIGHT0, GL_AMBIENT, white_light); // 设置环境光
	glLightfv(GL_LIGHT0, GL_DIFFUSE, white_light); // 设置漫射光
	glLightfv(GL_LIGHT0, GL_SPECULAR, white_light);// 设置镜面光
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_DEPTH_TEST);
}

void reshape(int w, int h) {
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(93.0, (GLfloat)w / (GLfloat)h, 1.0, 55.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void myDisplay() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	gluLookAt(0, 0, 1, 0, 0, 0, 0, 1.0, 0);
	glutSolidTeapot(0.45);
	glutSwapBuffers();
	glFlush();
}

int main(int argc, char * argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(500, 500);
	glutCreateWindow("光照茶壶");
	init();
	glutDisplayFunc(myDisplay);
	glutReshapeFunc(reshape);
	glutMainLoop();
	return 0;
}