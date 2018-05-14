#include <iostream>
#include <cmath>
#include <gl\glut.h>
#include <gl\GL.h>
#include <gl\GLU.h>
#include <cstdio>
#include <windows.h>

using namespace std;

static int year = 0, day = 0, suntime = 0, moonday = 0, moonyear = 0;
//�����ꡢ�����ա�̫��ʱ�������ա������꣨�ֱ����ڵ���Ĺ�ת����ת��̫������ת���������ת�͹�ת��
static double row = 0, col = 0;
//�����ƶ��۲��

void init() {
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_FLAT);
}

void tryDisplay() {
	glLoadIdentity();
	gluLookAt(row/10, col/10, 5.0, row/10, col/10, 0, 0, 1, 0);//�ƶ��۲��
	glClear(GL_COLOR_BUFFER_BIT);
	glColor4f(1.0, 0.0, 0.0, 0.0);
	glPushMatrix();
	glRotatef((GLfloat)suntime, 0, 0, 1.0);//̫����ת
	glutWireSphere(1.0, 25, 25);//����
	glRotatef((GLfloat)(year-suntime), 0, 0, 1.0);//����ת
	glColor4f(0.0, 0.0, 1.0, 0.0);
	glTranslated(2.0, 0, 0);//�ƶ�����ϵ
	glRotatef((GLfloat)day, 0, 0, 1.0);//������ת
	glutWireSphere(0.3, 10, 10);
	glColor4f(0.0, 1.0, 0.0, 0.0);
	glRotatef((GLfloat)(moonyear-day), 0, 0, 1.0);//����ת
	glTranslated(0.5, 0, 0);
	glRotatef((GLfloat)moonday, 0, 0, 1.0);//������ת
	glutWireSphere(0.1, 6, 6);
	glPopMatrix();
	glFlush();
	glutSwapBuffers();
}

void reshape(int w, int h) {
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(55.0, (GLfloat)w / (GLfloat)h, 2.0, 25.0);
	glMatrixMode(GL_MODELVIEW);
}

void setInput(unsigned char key, int x, int y) {
	//��ȡ��������ĺ�����wasd�����ƶ��۲�㣬jJkK���ڿ�����ת�͹�ת
	switch (key) {
		case 'w':
			col--;
			glutPostRedisplay();//���»�
			break;
		case 's':
			col++;
			glutPostRedisplay();
			break;
		case 'd':
			row--;
			glutPostRedisplay();
			break;
		case 'a':
			row++;
			glutPostRedisplay();
			break;
		case 'j':
			day = (day + 8) % 360;
			suntime = (suntime + 4) % 360;
			moonday = (moonday + 16) % 360;
			glutPostRedisplay();
			break;
		case 'J':
			day = (day - 8) % 360;
			suntime = (suntime - 4) % 360;
			moonday = (moonday - 16) % 360;
			glutPostRedisplay();
			break;
		case 'k':
			year = (year + 6) % 360;
			suntime = (suntime + 3) % 360;
			moonyear = (moonyear + 9) % 360;
			glutPostRedisplay();
			break;
		case 'K':
			year = (year - 6) % 360;
			suntime = (suntime - 3) % 360;
			moonyear = (moonyear - 9) % 360;
			glutPostRedisplay();
			break;
		default:
			break;
	}
}

int main(int argc, char * argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(500, 500);
	glutCreateWindow("��ת��ת");
	init();
	glutDisplayFunc(tryDisplay);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(setInput);//��������
	glutMainLoop();
	return 0;
}
