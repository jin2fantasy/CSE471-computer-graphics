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
#include "math.h"

#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>

//
// Definitions
//

typedef struct{
    unsigned char x, y, z, w;
} uchar4;
typedef unsigned char uchar;

// BMP loader
void LoadBMPFile(uchar4 **dst, int *width, int *height, const char *name);

#define screenSize 1024
#define	imageSize 512

//
// Variables
//

// array to store synthetic cubemap raw image
static GLubyte image1[4][4][4];
static GLubyte image2[4][4][4];
static GLubyte image3[4][4][4];
static GLubyte image4[4][4][4];
static GLubyte image5[4][4][4];
static GLubyte image6[4][4][4];

// texture object handles, FBO handles
GLuint cube_tex, color_tex;
GLuint fb, depth_rb;

//
// Functions
//

// create synthetic data for static cubemap
void makeSyntheticImages(void)
{
	int i, j, c;
    
	for (i = 0; i < 4; i++) {
		for (j = 0; j < 4; j++) {
			c = ((((i&0x1)==0)^((j&0x1))==0))*255;
			image1[i][j][0] = (GLubyte) c;
			image1[i][j][1] = (GLubyte) c;
			image1[i][j][2] = (GLubyte) c;
			image1[i][j][3] = (GLubyte) 255;
			
			image2[i][j][0] = (GLubyte) c;
			image2[i][j][1] = (GLubyte) c;
			image2[i][j][2] = (GLubyte) 0;
			image2[i][j][3] = (GLubyte) 255;
			
			image3[i][j][0] = (GLubyte) c;
			image3[i][j][1] = (GLubyte) 0;
			image3[i][j][2] = (GLubyte) c;
			image3[i][j][3] = (GLubyte) 255;
			
			image4[i][j][0] = (GLubyte) 0;
			image4[i][j][1] = (GLubyte) c;
			image4[i][j][2] = (GLubyte) c;
			image4[i][j][3] = (GLubyte) 255;
			
			image5[i][j][0] = (GLubyte) 255;
			image5[i][j][1] = (GLubyte) c;
			image5[i][j][2] = (GLubyte) c;
			image5[i][j][3] = (GLubyte) 255;
			
			image6[i][j][0] = (GLubyte) c;
			image6[i][j][1] = (GLubyte) c;
			image6[i][j][2] = (GLubyte) 255;
			image6[i][j][3] = (GLubyte) 255;
		}
	}
}

void init(void)
{
	// You need to ues glew
	glewInit();
	
	GLfloat diffuse[4] = {1.0, 1.0, 1.0, 1.0};
	
	glClearColor (0.0, 0.0, 0.0, 0.0);
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);

	// make synthetic cubemap data
	makeSyntheticImages();
	
	//
	// Creating a 2D texture from image
	// 
	int width, height;
	uchar4 *dst;
	LoadBMPFile(&dst, &width, &height, "../mob.bmp"); // this is how to load image
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, &color_tex);
	glBindTexture(GL_TEXTURE_2D, color_tex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, dst);

	
#define DYNAMIC_CUBEMAP //STATIC_CUBEMAP 

	//
	// creating cube map texture (either static or dynamic)
	//

#ifdef STATIC_CUBEMAP
	// create static cubemap from synthetic data
	glGenTextures(1, &cube_tex);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cube_tex);

	// ToDo...
	
#endif


// generate cubemap on-the-fly
#ifdef DYNAMIC_CUBEMAP
	//RGBA8 Cubemap texture, 24 bit depth texture, 256x256
	glGenTextures(1, &cube_tex);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cube_tex);
	
	// ToDo...


	//
	// creating FBO and attach cube map texture
	//

	//-------------------------
	glGenFramebuffersEXT(1, &fb);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fb);
	//Attach one of the faces of the Cubemap texture to this FBO
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_CUBE_MAP_POSITIVE_X, cube_tex, 0);
	//-------------------------
	glGenRenderbuffersEXT(1, &depth_rb);
	glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, depth_rb);
	glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT24, imageSize, imageSize);
	//-------------------------
	//Attach depth buffer to FBO
	glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, depth_rb);
	//-------------------------
	//Does the GPU support current FBO configuration?
	GLenum status;
	status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
	switch(status)
	{
	case GL_FRAMEBUFFER_COMPLETE_EXT:
		std::cout<<"good"<<std::endl; break;
	default:
		assert(false); break;
	}
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
	
#endif
}

void idle()
{
	// do something for animation here b/c this will be called when idling

	glutPostRedisplay();
}

// dynamically update cubemap
void update_cubemap()
{
	// bind FBO to render to texture
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fb);

	// render to +x face
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_CUBE_MAP_POSITIVE_X, cube_tex, 0);

	// render the entire scene here...

	// render to -x face
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_CUBE_MAP_NEGATIVE_X, cube_tex, 0);

	// render the entire scene here...

	// render to +y face
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_CUBE_MAP_POSITIVE_Y, cube_tex, 0);

	// render the entire scene here...

	// render to -y face
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, cube_tex, 0);

	// render the entire scene here...

	// render to +z face
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_CUBE_MAP_POSITIVE_Z, cube_tex, 0);

	// render the entire scene here...

	// render to -z face
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, cube_tex, 0);

	// render the entire scene here...

	// unbind FBO
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
}

void display(void)
{
	// update dynamic cubemap per frame
#ifdef DYNAMIC_CUBEMAP
	update_cubemap();
#endif

	// render something here...

	glutSwapBuffers();
}

void reshape(int w, int h)
{
	glViewport(0, 0, (GLsizei) w, (GLsizei) h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(40.0, (GLfloat) w/(GLfloat) h, 1.0, 300.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0.0, 0.0, -20.0);
}

void keyboard (unsigned char key, int x, int y)
{

}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(screenSize, screenSize);
	glutInitWindowPosition(100, 100);
	glutCreateWindow (argv[0]);
	init ();
	glutDisplayFunc(display);
	glutIdleFunc(idle);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutMainLoop();
	return 0;
}