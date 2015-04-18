/*
 * Skeleton code for CSE471 Spring 2015
 *
 * Won-Ki Jeong, wkjeong@unist.ac.kr
 */

#include <stdio.h>
#include <GL/glew.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <iostream>
#include <assert.h>
#include "textfile.h"
#include "Angel.h"

#define width glutGet(GLUT_WINDOW_WIDTH)
#define height glutGet(GLUT_WINDOW_HEIGHT)

// panning
float dx = 0, dy = 0, storeDx = 0, storeDy = 0;

// rotating
float angle = 0, storeAngle = 0;
vec3 rotateDirection(0), storeRotateDirection(0);
vec4 q1(0, 0, 0, 1), q2(0, 0, 0, 1); // rotation quaternions. x,y,z for imaginary part and w for scalar part
// q=(v1*i + v2*j + v3*k)*sin(theta/2) + cos(theta/2)

// zooming for perspective view
float dz = 0, storeDz = 0;

// used for recording mouse location when it's clicked
float mouseX = 0;
float mouseY = 0;
float mouseZ = 0;

// flags for checking which mouse button is pressed
bool mouseLeft = false;
bool mouseMiddle = false;
bool mouseRight = false;


// Shader programs
GLuint p[3];

// light position & color
float lpos[4] = {-1.0, 0.2, 2, 1};
GLfloat diffuse[] = { 0.8, 0.25, 0.1, 1 };
GLfloat specular[] = { 0.6, 0.6, 0.6, 1 };
GLfloat ambient[] = { 1, 1, 1, 1 };

void changeSize(int w, int h) {

	// Prevent a divide by zero, when window is too short
	// (you cant make a window of zero width).
	if(h == 0) h = 1;

	float ratio = 1.0f * (float) w / (float)h;

	// Reset the coordinate system before modifying
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	// Set the viewport to be the entire window
    glViewport(0, 0, w, h);

	// Set the correct perspective.
	gluPerspective(45,ratio,1,1000);
	glMatrixMode(GL_MODELVIEW);
}


void keyboard(unsigned char key, int x, int y)
{
	if(key == 'p') {
		// ToDo
	}

	if (key == 27) {
		// ESC
		exit(0);
	}

	glutPostRedisplay();
}

void mouseButton(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON) switch (state)
	{
	case GLUT_DOWN:
		{
			mouseX = ((float)x - width / 2) / width * 2;
			mouseY = (-(float)y + height / 2) / height * 2;
			if (std::sqrt(mouseX*mouseX + mouseY*mouseY) >= 1.0 / 2) {
				// use hyperbola
				mouseZ = (1.0 / 2) / std::sqrt(mouseX*mouseX + mouseY*mouseY);
			}
			else mouseZ = std::sqrt(1 - mouseX*mouseX - mouseY*mouseY);
			mouseLeft = true;
			break;
		}
	case GLUT_UP:
		{
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
	}

	if (button == GLUT_MIDDLE_BUTTON) switch (state)
	{
	case GLUT_DOWN:
		{
			mouseX = x, mouseY = y;
			mouseMiddle = true;
			break;
		}
	case GLUT_UP:
		{
			storeDx += dx, storeDy += dy;
			dx = dy = 0;
			mouseMiddle = false;
			break;
		}
	}

	if (button == GLUT_RIGHT_BUTTON) switch (state)
	{
	case GLUT_DOWN:
		{
			mouseY = y;
			mouseRight = true;
			break;
		}
	case GLUT_UP:
		{
			storeDz += dz;
			dz = 0;
			mouseRight = false;
			break;
		}
	}
}

void mouseMotion(int x, int y)
{
	if (mouseLeft == true) {
		float xx = ((float)x - width / 2) / width * 2;
		float yy = (-(float)y + height / 2) / height * 2;
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
		dx = (x - mouseX)*0.005;
		dy = -(y - mouseY)*0.005;
		glutPostRedisplay();
	}

	if (mouseRight == true) {
		dz = -(y - mouseY)*0.005;
		glutPostRedisplay();
	}

}

void renderScene(void) {
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// ToDo

	glLoadIdentity();

	// panning
	glTranslatef(storeDx + dx, storeDy + dy, 0);

	// rotating
	glTranslatef(0 - 0.0, 0 - 0.0, 0 - 5.0 + storeDz + dz);
	glRotatef(angle * 180 / M_PI, rotateDirection.x, rotateDirection.y, rotateDirection.z);
	glTranslatef(-0 + 0.0, -0 + 0.0, -0 + 5.0 - storeDz - dz);
	glTranslatef(0 - 0.0, 0 - 0.0, 0 - 5.0 + storeDz + dz);
	glRotatef(storeAngle * 180 / M_PI, storeRotateDirection.x, storeRotateDirection.y, storeRotateDirection.z);
	glTranslatef(-0 + 0.0, -0 + 0.0, -0 + 5.0 - storeDz - dz);

	// zooming
	gluLookAt(0.0, 0.0, 5.0 - storeDz - dz,
		0.0, 0.0, -1.0,
		0.0, 1.0, 0.0);

//	glFrontFace(GL_CW);
	glutSolidSphere(1,360,360);
	glFrontFace(GL_CCW);

	glutSwapBuffers();
}

void idle()
{
	glutPostRedisplay();
}


int main(int argc, char **argv) {

	// init GLUT and create Window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(600,600);
	glutCreateWindow("CSE471 - Assignment 2");

	// register callbacks
	glutDisplayFunc(renderScene);
	glutIdleFunc(renderScene);
	glutReshapeFunc(changeSize);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouseButton);
	glutMotionFunc(mouseMotion);
	glutIdleFunc(idle);

	glEnable(GL_DEPTH_TEST);
	glClearColor(1.0,1.0,1.0,1.0);
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_POSITION, lpos);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	GLfloat white[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, white);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glShadeModel(GL_SMOOTH);

	glewInit();
	if (glewIsSupported("GL_VERSION_3_3"))
		printf("Ready for OpenGL 3.3\n");
	else {
		printf("OpenGL 3.3 is not supported\n");
		exit(1);
	}

	// Create shader program
	p[0] = createGLSLProgram( "../phong.vert", NULL, "../phong.frag" ); // Phong
	p[1] = createGLSLProgram( "../silhouette.vert", NULL, "../silhouette.frag" ); // Silhouette
	p[2] = createGLSLProgram( "../toon.vert", NULL, "../toon.frag" ); // Cartoon
	
	// at first, use Phong shader
	glUseProgram(p[0]);

	// enter GLUT event processing cycle
	glutMainLoop();

	return 1;
}