#include <iostream>
#include <cmath>
#include <gl\glut.h>
#include <gl\GL.h>
#include <gl\GLU.h>

using namespace std;


void tryDisplay() {
	glClear(GL_COLOR_BUFFER_BIT);
	glRectf(-0.5, -0.5, 0.5, 0.5);
	glFlush();//ˢ�»���
}

int main(int argc, char * argv[]) {

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(500, 500);
	glutCreateWindow("��ʼ����");
	glutDisplayFunc(tryDisplay);
	glutMainLoop();

	return 0;
}
