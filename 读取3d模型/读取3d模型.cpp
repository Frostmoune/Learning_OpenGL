#include <OpenMesh/Core/IO/MeshIO.hh>  
#include <OpenMesh/Core/Mesh/TriMesh_ArrayKernelT.hh>  
#include <iostream>
#include <cmath>
#include <gl\glut.h>
#include <gl\GL.h>
#include <gl\GLU.h>
#include <cstdio>
#include <windows.h>
#include <string>

#define PI 3.1415926535897

using namespace std;

typedef OpenMesh::TriMesh_ArrayKernelT<> Mesh;

Mesh myMesh;

static GLuint showLine, showFlat; // 显示模式控制
static const int radius[3] = { 950, 10, 20 }; // 不同模型对应不同半径
static int nowFile = 3; // 当前模型文件
static float scale = 1; // 扩大/缩小倍数
static int scaleFlag = 0; // 是否扩大/缩小
static bool middleMouseState = 0; // 鼠标滚轮状态
static int patternChoose = 2; // 显示模式选择
static float oldX = 0, oldY = 0; // 之前的坐标
static float roundX = 0, roundY = PI / 2, roundDelta = PI / 90; // 现在的坐标
static float centerX = 0.0, centerY = 0.0, centerZ = 0.0; // 中心点坐标
static float eyeX = radius[nowFile - 1] * cos(roundX) * cos(roundY);
static float eyeY = radius[nowFile - 1] * sin(roundX);
static float eyeZ = radius[nowFile - 1] * cos(roundX) * sin(roundY); // 观察点坐标
static const string fileName[3] = { "Armadillo.off", "cactus.ply", "cow.obj" }; // 模型路径

// 更新观察点坐标
inline void updateEye() {
	eyeX = radius[nowFile - 1] * cos(roundX) * cos(roundY);
	eyeY = radius[nowFile - 1] * sin(roundX);
	eyeZ = radius[nowFile - 1] * cos(roundX) * sin(roundY);
}

// 更新中心点坐标
inline void updateCenter() {
	centerX = 0.0;
	centerY = 0.0;
	centerZ = 0.0;
}

// 读取模型文件
void readFile(int fileIndex) {
	nowFile = fileIndex + 1;
	OpenMesh::IO::Options opt;
	if (!OpenMesh::IO::read_mesh(myMesh, fileName[fileIndex], opt)) {
		cout << "Read error!\n";
		return;
	}
	cout << "Read successfully!\n";
	// 得到顶点法向量
	myMesh.request_vertex_normals();
	if (!opt.check(OpenMesh::IO::Options::VertexNormal)) {
		myMesh.request_face_normals();
		myMesh.update_normals();
		myMesh.release_face_normals();
	}
}

void init() {
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClearDepth(2.0);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);
}

// 画线
void drawLines(float *a, float *b) {
	glBegin(GL_LINES);
	glVertex3fv(a);
	glVertex3fv(b);
	glEnd();
}

// 画三角形
void drawTriangle(Mesh::FaceIter f_it) {
	glBegin(GL_TRIANGLES);
	for (auto fv_it = myMesh.fv_iter(*f_it); fv_it.is_valid(); ++fv_it) {
		glNormal3fv(myMesh.normal(*fv_it).data());
		glVertex3fv(myMesh.point(*fv_it).data());
	}
	glEnd();
}

// 画图
inline void drawPicture() {
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	showLine = glGenLists(1);
	showFlat = glGenLists(1);
	glNewList(showLine, GL_COMPILE); // 画线List
	glDisable(GL_LIGHTING); // 光照
	glColor3f(0.1, 0.3, 0.8);
	glLineWidth(1.5);
	for (auto he_it = myMesh.halfedges_begin(); he_it != myMesh.halfedges_end(); ++he_it) {
		auto from_handle = myMesh.from_vertex_handle(*he_it), to_handle = myMesh.to_vertex_handle(*he_it);
		drawLines(myMesh.point(from_handle).data(), myMesh.point(to_handle).data());
	}
	glEndList();
	glNewList(showFlat, GL_COMPILE); // 画图List
	glEnable(GL_LIGHTING);
	for (auto f_it = myMesh.faces_begin(); f_it != myMesh.faces_end(); ++f_it) {
		drawTriangle(f_it);
	}
	glEndList();
}

// 放大/缩小循环
void scaleLoop() {
	if (scaleFlag != 0) {
		if (scaleFlag == 1 && scale < 3.8) {
			scale += 0.002;
		}
		else if (scaleFlag == -1 && scale > 0.2) {
			scale -= 0.002;
		}
	}
	glutPostRedisplay();
}

void myDisplay() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	gluLookAt(eyeX, eyeY, eyeZ, 0, 0, 0, 0, 1.0, 0);
	glTranslated(centerX, centerY, centerZ);
	glScalef(scale, scale, scale);
	// 选择显示模式
	if(patternChoose == 1 || patternChoose == 2)glCallList(showFlat);
	if(patternChoose == 0 || patternChoose == 2)glCallList(showLine);
	glutSwapBuffers();
	glFlush();
	scaleLoop();
}

void reshape(int w, int h) {
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(10.0, (GLfloat)w / (GLfloat)h, 0.1, 1000.0);
	glMatrixMode(GL_MODELVIEW);
}

// 鼠标按键控制
void myMouse(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		scaleFlag = 1;
	}
	else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
		scaleFlag = 0;
	}
	else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
		scaleFlag = -1;
	}
	else if (button == GLUT_RIGHT_BUTTON && state == GLUT_UP) {
		scaleFlag = 0;
	}
	else if (button == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN) {
		middleMouseState = 1;
	}
	else if (button == GLUT_MIDDLE_BUTTON && state == GLUT_UP) {
		middleMouseState = 0;
	}
	glutPostRedisplay();
}

// 鼠标移动控制
void myMouseMove(int x, int y) {
	if (middleMouseState) {
		if (oldX != -1 && oldY != -1 && x != -1 && y != -1) {
			double tDx = x - oldX;
			double tDy = y - oldY;
			double tDis = sqrt(tDx * tDx + tDy * tDy);
			if (tDx > 0.) {
				roundY += roundDelta * tDx / tDis;
				if (roundY < 0.) { roundY += 2 * PI; }
				if (roundY > 2 * PI) { roundY -= 2 * PI; }
			}
			else if (tDx < 0.) {
				roundY += roundDelta * tDx / tDis;
				if (roundY < 0.) { roundY += 2 * PI; }
				if (roundY > 2 * PI) { roundY -= 2 * PI; }
			}
			if (tDy > 0.) {
				roundX = roundX + roundDelta * tDy / tDis;
				if (roundX > PI / 2) { roundX = PI / 2; }
			}
			else if (tDy < 0.) {
				roundX = roundX + roundDelta * tDy / tDis;
				if (roundX < -PI / 2) { roundX = -PI / 2; }
			}
			updateEye();
			oldX = x;
			oldY = y;
		} // 观察点在一个圆上运动
		glutPostRedisplay();
	}
}

// 键盘控制
void myKeyboard(unsigned char key, int x, int y) {
	switch (key) {
		case '1':
			cout << "Loading File 1...\n";
			readFile(0);
			scale = 0.8;
			updateEye();
			updateCenter();
			drawPicture();
			break;
		case '2':
			cout << "Loading File 2...\n";
			readFile(1);
			scale = 1.2;
			updateEye();
			updateCenter();
			drawPicture();
			break;
		case '3':
			cout << "Loading File 3...\n";
			readFile(2);
			scale = 1;
			updateEye();
			updateCenter();
			drawPicture();
			break;
		case '4':
			patternChoose = (patternChoose + 1) % 3;
			break;
		case 'w':
			if (nowFile != 1) {
				if (centerY < 1)centerY += 0.05;
			}
			else {
				if (centerY < 20)centerY += 1;
			}
			break;
		case 's':
			if (nowFile != 1) {
				if (centerY > -1)centerY -= 0.05;
			}
			else {
				if (centerY > -20)centerY -= 1;
			}
			break;
		case 'd':
			if (nowFile != 1) {
				if (centerX < 1)centerX += 0.05;
			}
			else {
				if (centerX < 20)centerX += 1;
			}
			break;
		case 'a':
			if (nowFile != 1) {
				if (centerX > -1)centerX -= 0.05;
			}
			else {
				if (centerX > -20)centerX -= 1;
			}
			break;
		case 'q':
			if (nowFile != 1) {
				if (centerZ < 1)centerZ += 0.05;
			}
			else {
				if (centerZ < 20)centerZ += 1;
			}
			break;
		case 'e':
			if (nowFile != 1) {
				if (centerZ > -1)centerZ -= 0.05;
			}
			else {
				if (centerZ > -20)centerZ -= 1;
			}
			break;
		default:
			break;
	}
	glutPostRedisplay();
}

int main(int argc, char * argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(500, 500);
	glutCreateWindow("3D");
	init();
	readFile(2);
	drawPicture();
	cout << "1¡¢File 1\n2¡¢File 2\n3¡¢File 3\n";
	glutKeyboardFunc(myKeyboard);
	glutMouseFunc(myMouse);
	glutMotionFunc(myMouseMove);
	glutReshapeFunc(reshape);
	glutDisplayFunc(myDisplay);
	glutMainLoop();

	return 0;
}