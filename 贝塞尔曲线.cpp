#include <iostream>
#include <cmath>
#include <gl\glut.h>
#include <gl\GL.h>
#include <gl\GLU.h>
#include <cstdio>
#include <windows.h>

#define HEIGHT 500

using namespace std;

static int point_num = 0, right_button_num = 0;
static int factorial[11];

// 点
class Point {
	private:
		double posx, posy;
	public:
		Point(double x = 0, double y = 0):posx(x), posy(y){}
		Point operator + (const Point &next) {
			return Point(posx + next.posx, posy + next.posy);
		}
		void operator += (const Point &next) {
			posx += next.posx;
			posy += next.posy;
		}
		Point operator * (const double &next) {
			return Point(posx * next, posy * next);
		}
		void operator = (const Point &next) {
			posx = next.posx;
			posy = next.posy;
		}
		int getX() const {
			return posx;
		}
		int getY() const {
			return posy;
		}
};

Point points_arr[10];

// 画点函数
void drawPoint(const Point &now) {
	printf("%d %d\n", now.getX(), now.getY());
	glBegin(GL_POINTS);
	glVertex2f(now.getX(), now.getY());
	glEnd();
	glFlush();
}

// 两点画线函数
void drawLine(const Point &a, const Point &b) {
	glBegin(GL_LINES);
	glVertex2f(a.getX(), a.getY());
	glVertex2f(b.getX(), b.getY());
	glEnd();
	glFlush();
}

int combination(int n, int i) {
	return factorial[n] / (factorial[n - i] * factorial[i]);
}

// 画贝塞尔曲线
void drawBezier() {
	Point before;
	double t = 0.0;
	while (t <= 1.0) {
		Point now;
		for (int i = 0; i < point_num; ++i) {
			now += points_arr[i] * (pow(1 - t, point_num - i - 1) * pow(t, i) * combination(point_num - 1, i));
		}
		if (t > 0.0) {
			drawLine(before, now);
		}
		before = now;
		t += 0.005;
	}
}

void init() {
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glPointSize(3.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, 500, 0.0, 500);
	factorial[0] = 1;
	for (int i = 1; i <= 10; ++i) {
		factorial[i] = factorial[i - 1] * i;
	}
}

// 鼠标控制函数
void onMouse(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		printf("Left down.\n");
		point_num ++;
		if (point_num < 10) {
			glColor4f(1.0, 0.0, 0.0, 0.0);
			points_arr[point_num - 1] = Point(x, HEIGHT - y);
			drawPoint(points_arr[point_num - 1]); // 画点
			if (point_num > 1) {
				glColor4f(0.0, 0.0, 1.0, 0.0);
				drawLine(points_arr[point_num - 2], points_arr[point_num - 1]); // 画线
			}
		}
		else {
			point_num --;
			glColor4f(0.0, 1.0, 0.0, 0.0);
			drawBezier();
			point_num = 0;
			right_button_num ++;
		}
	}
	else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
		printf("Right down.\n");
		if (point_num >= 2 && right_button_num < 1) {
			glColor4f(0.0, 1.0, 0.0, 0.0);
			drawBezier();
			point_num = 0;
			right_button_num++;  // 第一次右键画图
		}
		else if(right_button_num >= 1){
			glClearColor(0.0, 0.0, 0.0, 0.0);
			glutPostRedisplay();
			right_button_num = 0; // 按两次右键清屏
		}
	}
}

void myDisplay() {
	glClear(GL_COLOR_BUFFER_BIT);
	glFlush();
}

int main(int argc, char * argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(500, 500);
	glutCreateWindow("贝塞尔曲线");
	init();
	glutDisplayFunc(myDisplay);
	glutMouseFunc(onMouse);
	glutMainLoop();
	return 0;
}
