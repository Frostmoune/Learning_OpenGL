#define PI 3.14159265
#include <iostream>
#include <cmath>
#include <gl\glut.h>
#include <gl\GL.h>
#include <gl\GLU.h>
#include <cstdio>
#include <windows.h>

using namespace std;

//��������
const GLfloat points[8][3] = { {-0.5, -0.5, -0.5}, {0.5, -0.5, -0.5}, {0.5, 0.5, -0.5}, {-0.5, 0.5, -0.5},
						{-0.5, -0.5, 0.5}, {0.5, -0.5, 0.5}, {0.5, 0.5, 0.5}, {-0.5, 0.5, 0.5} };
//���������Ӧ�ļ�������
const int selects[6][4] = { {4, 0, 1, 5}, {5, 2, 3, 6}, {4, 5, 6, 7},
						{7, 3, 0, 4}, {6, 2, 3, 7}, {0, 1, 2, 3} };
//������ķ�����
const GLint vectors[6][3] = { {0, -1, 0}, {1, 0, 0}, {0, 0, 1},
						{-1, 0, 0}, {0, 1, 0}, {0, 0, -1} };

//�����������
void drawSurface(int i) {
	glBegin(GL_QUADS);
		glNormal3iv(vectors[i]);
		glVertex3fv(points[selects[i][0]]);
		glVertex3fv(points[selects[i][1]]);
		glVertex3fv(points[selects[i][2]]);
		glVertex3fv(points[selects[i][3]]);
	glEnd();
}

//�����������
void drawLine(int i,int j) {
	glBegin(GL_LINES);
		glVertex3fv(points[i]);
		glVertex3fv(points[j]);
	glEnd();
}

void tryDisplay() {
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();
	gluLookAt(4.5, 1, 2.5, 0, 0, 0, 1, 1, 0);//�����ӽ�
	glColor4f(0.0, 1.0, 0.0, 0.0);
	for (int i = 0; i < 6; ++i) {
		drawSurface(i);
	}
	glColor3f(0, 0, 0);
	glLineWidth(1.5);
	for (int i = 0; i < 4; ++i) {
		drawLine(i, (i + 1) % 4);
		drawLine(i, i + 4);
		drawLine(i + 4, (i + 1) % 4 + 4);
	}
	glFlush();
	Sleep(1000);
}

void reshape(int w, int h) {
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(35.0, (GLfloat)w / (GLfloat)h, 2.0, 10.0);
	glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char * argv[]) {

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA); 
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(500, 500);
	glutCreateWindow("��һ��������");
	glutDisplayFunc(tryDisplay);
	glutReshapeFunc(reshape);
	glutMainLoop();

	return 0;
}
