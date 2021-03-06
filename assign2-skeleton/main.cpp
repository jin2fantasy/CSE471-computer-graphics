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
#include "assign1.h"


// flag for checking the rendering mode. 
bool isCartoon = false;

// Shader programs
GLuint p[3];

GLuint mvpLoc, mvLoc, normLoc, colorLoc;
GLuint lightposLoc;
GLuint colorBlockID;

const mat4 Rotate(float theta, vec3 direction)
// generate rotation matrix for a rotation of theta degrees about a line in the direction of the vector direction
{
	float d = std::sqrt(direction.y*direction.y + direction.z*direction.z);
	float thetaX = std::atan2(direction.y, direction.z) * 180 / M_PI;
	float thetaY = -std::atan2(direction.x, d) * 180 / M_PI;
	return RotateX(-thetaX)*RotateY(-thetaY)*RotateZ(theta)*RotateY(thetaY)*RotateX(thetaX);
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
	gluPerspective(45,ratio,0.2,1000);
	glMatrixMode(GL_MODELVIEW);
}


void keyboard(unsigned char key, int x, int y)
{
	if(key == 'p') {
		// ToDo
		if (isCartoon) {
			isCartoon = false;
			glUseProgram(p[0]);
		}
		else {
			isCartoon = true;
			glUseProgram(p[1]);
			glUseProgram(p[2]);
		}
	}

	if (key == 27 || key == 'q' || key == 'Q') exit(0);

	glutPostRedisplay();
}

void init()
{
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glClearDepth(1.0);
	glEnable(GL_DEPTH_TEST);

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

	// light position & color
	float lpos[4] = { -2, 0, 5, 1 };
	GLfloat diffuse[] = { 0.8, 0.3, 0.5, 1 };
	GLfloat specular[] = { 0.6, 0.6, 0.6, 1 };
	GLfloat ambient[] = { 0.3, 0.3, 0.3, 1 };
	GLfloat white[] = { 1.0f, 1.0f, 1.0f, 1.0f };

#if 0
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_POSITION, lpos);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, white);
#endif // 0
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// getting uniform location
	mvpLoc = glGetUniformLocation(p[0], "mvp");
	mvLoc = glGetUniformLocation(p[0], "mv");
	normLoc = glGetUniformLocation(p[0], "norm");
	lightposLoc = glGetUniformLocation(p[0], "lightpos");

	// settings for uniform of light & material
	vec4 colorBlock[] = { vec4(0.8, 0.3, 0.5, 1), vec4(0.3, 0.3, 0.3, 1), vec4(0.6, 0.6, 0.6, 1),
		vec4(0, 0, 0, 1), vec4(0.2, 0.2, 0.2, 1), vec4(0, 0, 0, 1),
		vec4(0.8, 0.8, 0.8, 1), vec4(0.2, 0.2, 0.2, 1), vec4(1.0, 1.0, 1.0, 1) };
	colorLoc = glGetUniformBlockIndex(p[0], "colorBlock");
	glUniformBlockBinding(p[0], colorLoc, 0);
	glGenBuffers(1, &colorBlockID);
	glBindBuffer(GL_UNIFORM_BUFFER, colorBlockID);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(colorBlock), colorBlock, GL_DYNAMIC_DRAW);
	glBindBufferBase(GL_UNIFORM_BUFFER, 0, colorBlockID);
}


void renderScene(void) {
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// ToDo

	glLoadIdentity();

#if 0

	// panning
	glTranslatef(storeDx + dx, storeDy + dy, 0);

	// rotating
	glTranslatef(mesh.centerX, mesh.centerY, mesh.centerZ - 5.0 + storeDz + dz);
	glRotatef(angle * 180 / M_PI, rotateDirection.x, rotateDirection.y, rotateDirection.z);
	glRotatef(storeAngle * 180 / M_PI, storeRotateDirection.x, storeRotateDirection.y, storeRotateDirection.z);
	glTranslatef(-mesh.centerX, -mesh.centerY, -(mesh.centerZ - 5.0 + storeDz + dz));

	// zooming
	gluLookAt(0.0, 0.0, 5.0 - storeDz - dz,
		0.0, 0.0, -1.0,
		0.0, 1.0, 0.0);

#endif
	// uniform variables setting
	vec4 eye(0, 0, 5 - storeDz - dz, 1);
	vec4 at(0, 0, -1, 1);
	vec4 up(0, 1, 0, 0);
	mat4 myMv = Translate(storeDx + dx, storeDy + dy, 0) *
		Translate(mesh.centerX, mesh.centerY, mesh.centerZ - 5.0 + storeDz + dz) *
		Rotate(angle * 180 / M_PI, rotateDirection) * Rotate(storeAngle * 180 / M_PI, storeRotateDirection) *
		Translate(-mesh.centerX, -mesh.centerY, -(mesh.centerZ - 5.0 + storeDz + dz)) *
		LookAt(eye, at, up);
	float w = width, h = height;
	mat4 myMvp = Perspective(45, w / h, 0.2, 1000) * myMv;
	mat4 myNorm = transpose(Translate(eye) *
		Translate(mesh.centerX, mesh.centerY, mesh.centerZ - 5.0 + storeDz + dz) *
		transpose(Rotate(storeAngle * 180 / M_PI, storeRotateDirection)) *
		transpose(Rotate(angle * 180 / M_PI, rotateDirection)) *
		Translate(-mesh.centerX, -mesh.centerY, -(mesh.centerZ - 5.0 + storeDz + dz)) *
		Translate(-(storeDx + dx), -(storeDy + dy), 0));
	
	float lpos[4] = { -5, 0, 5, 1 };
	glProgramUniformMatrix4fv(p[0], mvpLoc, 1, GL_TRUE, myMvp);
	glProgramUniformMatrix4fv(p[0], mvLoc, 1, GL_TRUE, myMv);
	glProgramUniformMatrix4fv(p[0], normLoc, 1, GL_TRUE, myNorm);
	glProgramUniform4fv(p[0], lightposLoc, 1, LookAt(eye, at, up) * vec4(lpos[0], lpos[1], lpos[2], lpos[3]));


	glEnableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
	glVertexPointer(3, GL_FLOAT, 0, 0);
	glEnableClientState(GL_NORMAL_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, normalBufferID);
	glNormalPointer(GL_FLOAT, 0, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);
	glDrawElements(GL_TRIANGLES, 3 * mesh.nFaces, GL_UNSIGNED_INT, 0);
	glutSwapBuffers();
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
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
	
	init();

	// at first, use Phong shader
	glUseProgram(p[0]);

	// enter GLUT event processing cycle
	glutMainLoop();

	return 1;
}