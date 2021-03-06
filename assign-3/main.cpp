/*
 * Skeleton code for CSE471 Spring 2015
 *
 * Won-Ki Jeong, wkjeong@unist.ac.kr
 */

#include <stdio.h>
#include <GL/glew.h>
#include <Windows.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <iostream>
#include <assert.h>
#include "textfile.h"


// Shader programs
GLuint p;

// light position
float lpos[4] = {2,-4,3,0};

void draw_cube()
{
	glBegin(GL_QUADS);
		glVertex3f(0,0,0);
		glVertex3f(1,0,0);
		glVertex3f(1,1,0);
		glVertex3f(0,1,0);

		glVertex3f(0,0,0);
		glVertex3f(0,1,0);
		glVertex3f(0,1,1);
		glVertex3f(0,0,1);

		glVertex3f(0,0,0);
		glVertex3f(1,0,0);
		glVertex3f(1,0,1);
		glVertex3f(0,0,1);

		glVertex3f(0,0,1);
		glVertex3f(1,0,1);
		glVertex3f(1,1,1);
		glVertex3f(0,1,1);

		glVertex3f(1,0,0);
		glVertex3f(1,1,0);
		glVertex3f(1,1,1);
		glVertex3f(1,0,1);

		glVertex3f(0,1,0);
		glVertex3f(1,1,0);
		glVertex3f(1,1,1);
		glVertex3f(0,1,1);
	glEnd();
}


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

	glutPostRedisplay();
}

void renderScene(void) {
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLightfv(GL_LIGHT0, GL_POSITION, lpos);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.0,0.0,3.0, 
		      0.0,0.0,0.0,
			  0.0f,1.0f,0.0f);
	
	// ToDo
	//glutSolidTeapot(1);
	//glutSolidCube(1);
		
	glRotatef(45,1,1,1);
	glTranslatef(-0.5,-0.5,-0.5);
	
	draw_cube();
		
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
	glutCreateWindow("CSE471 - Assignment 3");

	// register callbacks
	glutDisplayFunc(renderScene);
	glutIdleFunc(renderScene);
	glutReshapeFunc(changeSize);
	glutKeyboardFunc(keyboard);
	glutIdleFunc(idle);

	glEnable(GL_DEPTH_TEST);
    glClearColor(1.0,1.0,1.0,1.0);

	glewInit();
	if (glewIsSupported("GL_VERSION_3_3"))
		printf("Ready for OpenGL 3.3\n");
	else {
		printf("OpenGL 3.3 is not supported\n");
		return 0;
	}

	// Create shader program
	p = createGLSLProgram( "../ex.vert", "../ex.geom", "../ex.frag" ); 

	// enter GLUT event processing cycle
	glutMainLoop();

	return 1;
}