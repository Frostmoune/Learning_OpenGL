#define PI 3.14159265
#include <iostream>
#include <cmath>
#include <gl\glut.h>
#include <gl\GL.h>
#include <gl\GLU.h>
#include <cstdio>

using namespace std;

//Բ����
struct myCircle {
	pair<GLdouble, GLdouble> oPos;
	GLdouble Color[3];
	GLdouble Radius;
	myCircle(GLdouble x = 0, GLdouble y = 0, GLdouble r = 0.25) :oPos(make_pair(x, y)), Radius(r) {}
	void setColor(GLdouble arr[]) {
		Color[0] = arr[0];
		Color[1] = arr[1];
		Color[2] = arr[2];
	}
	void setPos(GLdouble arr[]) {
		oPos = make_pair(arr[0], arr[1]);
	}
};

//�õ��Ƕ�ֵ
GLdouble getAngle(GLdouble i) {
	return i / 180.0 * PI;
}

//��һ��Բ
void drawCircle(const myCircle& now) {
	glColor4f(now.Color[0], now.Color[1], now.Color[2], 0.5);
	glBegin(GL_POINTS);
	glVertex2d(now.oPos.first, now.oPos.second);
	GLdouble nowx = 0, nowy = 0;
	for (int i = 0; i < 1440; ++i) {
		nowx = now.oPos.first + now.Radius*cos(getAngle(GLdouble(i) / 4));
		nowy = now.oPos.second + now.Radius*sin(getAngle(GLdouble(i) / 4));
		printf("%lf,%lf\n", nowx, nowy);
		glVertex2d(nowx, nowy);
	}
	glEnd();
	glFlush();
}

//��һ����ͼ����
void myDisplay() {
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
	glPointSize(2.0);
	GLdouble posArr[5][2] = { {-0.37,0.25},{0.0,0.25},{0.37,0.25},{-0.2,-0.12},{0.2,-0.12} };
	GLdouble colorArr[5][3] = { { 0,1.0,1.0 },{ 0,0,0 },{ 1.0,0,0 },{ 1.0,1.0,0 },{ 0,1.0,0 } };
	myCircle now;
	for (int i = 0; i < 5; ++i) {
		now.setColor(colorArr[i]);
		now.setPos(posArr[i]);
		drawCircle(now);
	}
}

int main(int argc, char * argv[]) {

	glutInit(&argc, argv); // ��ʼ�� GLUT
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA); // ������|color buffer

												  // ��������
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(500, 500);
	glutCreateWindow("�廷"); 
	glutDisplayFunc(myDisplay);
	glutMainLoop(); // ��ʼѭ�������Ҽ����ص�����

	return 0;
}
