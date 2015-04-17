/*
 * Skeleton code for CSE471 Intro to Computer Graphics
 *
 * Won-Ki Jeong, wkjeong@unist.ac.kr
 */
#include <stdio.h>
#include <math.h>
#include "mesh.h"

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif

#define WIDTH 500
#define HEIGHT 500

GLuint vertexBufferID;
GLuint normalBufferID;
GLuint indexBufferID;
Mesh mesh("../../mesh-data/bunny.off");

// camera position, center position, up vector
float cx = 0, cy = 0, cz = 4, ox = 0, oy = 0, oz = 0, ux = 0, uy = 1, uz = 0;

// panning
float dx = 0, dy = 0, storeDx = 0, storeDy = 0;

// rotating
float angle = 0, storeAngle = 0;
vec3 rotateDirection(0), storeRotateDirection(0);
vec4 q1(0,0,0,1), q2(0,0,0,1); // rotation quaternions. x,y,z for imaginary part and w for scalar part
						       // q=(v1*i + v2*j + v3*k)*sin(theta/2) + cos(theta/2)

// zooming for perspective view
float dz = 0, storeDz = 0;

// zooming for orthogonal view
float zoom = 1.0, storeZoom = 1.0;

// flag for checking what current view is. true for orthogonal, false for perspective
bool isOrtho = true;

// used for recording mouse location when it's clicked
float mouseX = 0;
float mouseY = 0;
float mouseZ = 0;

// flags for checking which mouse button is pressed
bool mouseLeft = false;
bool mouseMiddle = false;
bool mouseRight = false;

void init()
{
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClearDepth(1.0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);

	// vertex buffer, normal buffer, & index buffer
	glGenBuffers(1, &vertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * mesh.nVertices, mesh.vertices, GL_STATIC_DRAW);
	glGenBuffers(1, &normalBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, normalBufferID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * mesh.nVertices, mesh.normalSmooth, GL_STATIC_DRAW);
	glGenBuffers(1, &indexBufferID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * 3 * mesh.nFaces, mesh.faces, GL_STATIC_DRAW);

	// light settings
	GLfloat lightPos1[] = { 200, 200, 100, 1 };
	GLfloat diffuse1[] = { 0.8, 0.55, 0.1, 1 };
	GLfloat specular1[] = { 0.2, 0.2, 0.2, 1 };
	GLfloat ambient1[] = { 1, 1, 1, 1 };
	GLfloat lightPos2[] = { -200, 200, 100, 1 };
	GLfloat diffuse2[] = { 0.2, 0.2, 0, 1 };
	GLfloat specular2[] = { 0.2, 0.2, 0.2, 1 };
	GLfloat ambient2[] = { 1, 1, 1, 1 };
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse1);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular1);
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient1);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos1);
	glEnable(GL_LIGHT1);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuse2);
	glLightfv(GL_LIGHT1, GL_SPECULAR, specular2);
	glLightfv(GL_LIGHT1, GL_AMBIENT, ambient2);
	glLightfv(GL_LIGHT1, GL_POSITION, lightPos2);


	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glShadeModel(GL_SMOOTH);

	// initial view setting
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-0.12, 0.08, 0.0, 0.2, 0.1, 20);
}

void renderScene(void) {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(storeDx + dx, storeDy + dy, 0); // panning
	
	// rotating
	glTranslatef(mesh.centerX - cx, mesh.centerY - cy, mesh.centerZ - cz + storeDz + dz);
	glRotatef(angle * 180 / M_PI, rotateDirection.x, rotateDirection.y, rotateDirection.z);
	glTranslatef(-mesh.centerX + cx, -mesh.centerY + cy, -mesh.centerZ + cz - storeDz - dz);
	glTranslatef(mesh.centerX - cx, mesh.centerY - cy, mesh.centerZ - cz + storeDz + dz);
	glRotatef(storeAngle * 180 / M_PI, storeRotateDirection.x, storeRotateDirection.y, storeRotateDirection.z);
	glTranslatef(-mesh.centerX + cx, -mesh.centerY + cy, -mesh.centerZ + cz - storeDz - dz);

	// zooming
	if (isOrtho == true) {
		glTranslatef(mesh.centerX - cx, mesh.centerY - cy, mesh.centerZ - cz);
		glScalef(zoom, zoom, zoom);
		glTranslatef(-mesh.centerX + cx, -mesh.centerY + cy, -mesh.centerZ + cz);
		glTranslatef(mesh.centerX - cx, mesh.centerY - cy, mesh.centerZ - cz);
		glScalef(storeZoom, storeZoom, storeZoom);
		glTranslatef(-mesh.centerX + cx, -mesh.centerY + cy, -mesh.centerZ + cz);
		gluLookAt(cx, cy, cz, ox, oy, oz, ux, uy, uz);
	}
	else gluLookAt(cx, cy, cz - storeDz - dz, ox, oy, oz, ux, uy, uz);

	// Draw something here!
	glEnableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
	glVertexPointer(3, GL_FLOAT, 0, 0);
	glEnableClientState(GL_NORMAL_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, normalBufferID);
	glNormalPointer(GL_FLOAT, 0, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);
	glDrawElements(GL_TRIANGLES, 3 * mesh.nFaces, GL_UNSIGNED_INT, 0);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);

	glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y) {
	switch (key)
	{
	case 'Q':
	case 'q':
	case 27: //ESC
		exit(0);
		break;
	case 'o':
	case 'O':
		cx = 0, cy = 0, cz = 4, ox = 0, oy = 0, oz = 0, ux = 0, uy = 1, uz = 0;
		dx = dy = dz = storeDx = storeDy = storeDz = 0;
		angle = storeAngle = 0;
		zoom = 1, storeZoom = 1;
		q1 = q2 = vec4(0, 0, 0, 1);
		isOrtho = true;
		gluLookAt(0, 0, 2, 0, 0, 0, 0, 1, 0);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(-0.12, 0.08, 0.0, 0.2, 0.1, 20);
		glutPostRedisplay();
		break;
	case 'p':
	case 'P':
		cx = 0, cy = 0, cz = 20, ox = 0, oy = 0, oz = 0, ux = 0, uy = 1, uz = 0;
		dx = dy = dz = storeDx = storeDy = storeDz = 0;
		angle = storeAngle = 0;
		zoom = 1, storeZoom = 1;
		q1 = q2 = vec4(0, 0, 0, 1);
		isOrtho = false;
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glFrustum(-0.12/18, 0.08/18, -0.01/18, 0.19/18, 1, 300);
		glutPostRedisplay();
		break;		
	case 'w':
	case 'W':
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glutPostRedisplay();
		break;
	case 'f':
	case 'F':
		glBindBuffer(GL_ARRAY_BUFFER, normalBufferID);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * mesh.nVertices, mesh.normalFlat, GL_STATIC_DRAW);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glShadeModel(GL_FLAT);
		glutPostRedisplay();
		break;
	case 's':
	case 'S':
		glBindBuffer(GL_ARRAY_BUFFER, normalBufferID);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * mesh.nVertices, mesh.normalSmooth, GL_STATIC_DRAW);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glShadeModel(GL_SMOOTH);
		glutPostRedisplay();
		break;
	}
}

void mouseButton(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON) switch (state)
	{
	case GLUT_DOWN:
		mouseX = ((float)x - WIDTH / 2) / WIDTH * 2;
		mouseY = (-(float)y + HEIGHT / 2) / HEIGHT * 2;
		if (std::sqrt(mouseX*mouseX + mouseY*mouseY) >= 1.0/2) {
			// use hyperbola
			mouseZ = (1.0 / 2) / std::sqrt(mouseX*mouseX + mouseY*mouseY);
		}
		else mouseZ = std::sqrt(1 - mouseX*mouseX - mouseY*mouseY);
		mouseLeft = true;
		break;
	case GLUT_UP:
		vec4 q3(q1.w*q2.x + q1.x*q2.w + q1.y*q2.z - q1.z*q2.y,
				q1.w*q2.y - q1.x*q2.z + q1.y*q2.w + q1.z*q2.x,
				q1.w*q2.z + q1.x*q2.y - q1.y*q2.x + q1.z*q2.w,
				q1.w*q2.w - q1.x*q2.x - q1.y*q2.y - q1.z*q2.z);
		q2 = q3;
		q1 = vec4(0, 0, 0, 1);
		storeAngle = 2 * std::acos(q3.w);
		angle = 0;
		storeRotateDirection.x = q3.x;
		storeRotateDirection.y = q3.y;
		storeRotateDirection.z = q3.z;
		storeRotateDirection = normalize(storeRotateDirection);
		rotateDirection = 0;
		mouseLeft = false;
		break;
	}

	if (button == GLUT_MIDDLE_BUTTON) switch (state)
	{
	case GLUT_DOWN:
		mouseX = x, mouseY = y;
		mouseMiddle = true;
		break;
	case GLUT_UP:
		storeDx += dx, storeDy += dy;
		dx = dy = 0;
		mouseMiddle = false;
		break;
	}

	if (button == GLUT_RIGHT_BUTTON) switch (state)
	{
	case GLUT_DOWN:
		mouseY = y;
		mouseRight = true;
		break;
	case GLUT_UP:
		storeDz += dz;
		storeZoom *= zoom;
		dz = 0;
		zoom = 1;
		mouseRight = false;
		break;
	}
}

void mouseMotion(int x, int y)
{
	if (mouseLeft == true) {
		float xx = ((float)x - WIDTH / 2) / WIDTH * 2;
		float yy = (-(float)y + HEIGHT / 2) / HEIGHT * 2;
		vec3 p1(mouseX, mouseY, mouseZ), p2(xx, yy, 0);
		if (length(p2) >= 1.0 / 2) {
			// use hyperbola
			p2.z = (1.0 / 2) / std::sqrt(xx*xx + yy*yy);
		}
		else p2.z = std::sqrt(1 - p2.x*p2.x - p2.y*p2.y);
		p1 = normalize(p1);
		p2 = normalize(p2);
		rotateDirection = normalize(cross(p1, p2));
		dot(p1, p2) >= 1 ? angle = 0 : 
			angle = std::acos(dot(p1, p2));
		q1.x = rotateDirection.x*std::sin(angle / 2);
		q1.y = rotateDirection.y*std::sin(angle / 2);
		q1.z = rotateDirection.z*std::sin(angle / 2);
		q1.w = std::cos(angle / 2);
		glutPostRedisplay();
	}

	if (mouseMiddle == true) {
		dx = (x - mouseX)*0.0005;
		dy = -(y - mouseY)*0.0005;
		glutPostRedisplay();
	}

	if (mouseRight == true) {
		if (isOrtho == true) {
			zoom = mouseY / (float)y;
			glutPostRedisplay();
		}
		else {
			dz = -(y - mouseY)*0.05;
			glutPostRedisplay();
		}
	}
}

int main(int argc, char **argv) {

	// init GLUT and create Window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(WIDTH, HEIGHT);
	glutCreateWindow("CSE471 - Assignment 1");

	// register callbacks
	glutDisplayFunc(renderScene);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouseButton);
	glutMotionFunc(mouseMotion);

	glewInit();
	init();

	// enter GLUT event processing cycle
	glutMainLoop();
	return 1;
}