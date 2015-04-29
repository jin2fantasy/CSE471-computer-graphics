#include "Angel.h"
#include "Mesh.h"

#define width glutGet(GLUT_WINDOW_WIDTH)
#define height glutGet(GLUT_WINDOW_HEIGHT)

// panning
float dx = 0, dy = 0, storeDx = 0, storeDy = 0;

// rotating
float angle = 0, storeAngle = 0;
vec3 rotateDirection(0), storeRotateDirection(0);
vec4 q1(0, 0, 0, 1), q2(0, 0, 0, 1); // rotation quaternions. x,y,z for imaginary part and w for scalar part
// q = (v1*i + v2*j + v3*k)*sin(theta/2) + cos(theta/2)

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

// VBO handlers
GLuint vertexBufferID;
GLuint normalBufferID;
GLuint indexBufferID;

// Mesh data input
Mesh mesh("../mesh-data/bunny.off");


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
		dx = (x - mouseX)*0.001;
		dy = -(y - mouseY)*0.001;
		glutPostRedisplay();
	}

	if (mouseRight == true) {
		dz = -(y - mouseY)*0.005;
		glutPostRedisplay();
	}

}
