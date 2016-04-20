#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <GL/glut.h>
#include <cmath>
#include "helper.h"
#include "struct.h"
using namespace std;

int FPS = 60;
GLuint textureID[2];
Reader reader("teapot_0.obj");
float fRotateAngle0 = 0.0f;
float fRotateAngle1 = 0.0f;
float fLightRotateAngle0 = 0.0f;
float fLightRotateAngle1 = 0.0f;

unsigned int width, height;
GLfloat zoom = 6.0;
GLubyte* tdata;
int texmode = 0;
bool* key_pressed;

GLfloat lAmbient[] = { 0.6, 0.6, 0.6, 1.0 };
GLfloat lColor[] = { 1.0,1.0,1.0,1.0 };
GLfloat lpos[] = { 1.0,1.0,1.0,0.0 };
GLfloat amb[] = { 60 / 255.0, 60 / 255.0, 60 / 255.0, 1.0 };
GLfloat spec[] = { 255 / 255.0, 255 / 255.0, 255 / 255.0, 1.0 };
GLfloat diff[] = { 240 / 255.0, 112 / 255.0, 92 / 255.0, 1.0 };

/** http://ivl.calit2.net/wiki/index.php/Texture.cpp
Load a ppm file from disk.
@input filename The location of the PPM file.  If the file is not found, an error message
will be printed and this function will return 0
@input width This will be modified to contain the width of the loaded image, or 0 if file not found
@input height This will be modified to contain the height of the loaded image, or 0 if file not found
@return Returns the RGB pixel data as interleaved unsigned chars (R0 G0 B0 R1 G1 B1 R2 G2 B2 .... etc) or 0 if an error ocured
**/
unsigned char* loadPPM(const char* filename)
{
	const int BUFSIZE = 128;
	FILE* fp;
	unsigned int read;
	unsigned char* rawData;
	char buf[3][BUFSIZE];
	char* retval_fgets;
	size_t retval_sscanf;

	if ((fp = fopen(filename, "rb")) == NULL)
	{
		std::cerr << "error reading ppm file, could not locate " << filename << std::endl;
		width = 0;
		height = 0;
		return NULL;
	}
	retval_fgets = fgets(buf[0], BUFSIZE, fp);
	do
	{
		retval_fgets = fgets(buf[0], BUFSIZE, fp);
	} while (buf[0][0] == '#');
	retval_sscanf = sscanf(buf[0], "%s %s", buf[1], buf[2]);
	width = atoi(buf[1]);
	height = atoi(buf[2]);

	// Read maxval:
	do
	{
		retval_fgets = fgets(buf[0], BUFSIZE, fp);
	} while (buf[0][0] == '#');

	// Read image data:
	rawData = new unsigned char[width * height * 3];
	read = fread(rawData, width * height * 3, 1, fp);
	fclose(fp);
	if (read != 1)
	{
		std::cerr << "error parsing ppm file, incomplete data" << std::endl;
		delete[] rawData;
		width = 0;
		height = 0;
		return NULL;
	}

	return rawData;
}

void init(void)
{
	// init your data, setup OpenGL environment here
	//set the background color
	glClearColor(0.0 / 255, 0.0 / 255, 0.0 / 255, 1.0);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	glLightfv(GL_LIGHT0, GL_AMBIENT, lAmbient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lColor);
	glLightfv(GL_LIGHT0, GL_SPECULAR, lColor);
	glGenTextures(2, textureID);

	tdata = loadPPM("imagetex.ppm");
	glBindTexture(GL_TEXTURE_2D, textureID[0]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, tdata);

	tdata = loadPPM("imageenv.ppm");
	glBindTexture(GL_TEXTURE_2D, textureID[1]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, tdata);

	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glClearDepth(1.0f);                         // Depth Buffer Setup
	glEnable(GL_DEPTH_TEST);                        // Enables Depth Testing
	glDepthFunc(GL_LEQUAL);                         // The Type Of Depth Testing To Do
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);          // Really Nice Perspective Calculation
	glMatrixMode(GL_MODELVIEW);
	gluLookAt(0.f, 0.f, 3.f, 0.f, 0.0, -3.f, 0.f, 1.f, 0.f);
	key_pressed = (bool*)malloc(sizeof(bool) * 256);
	memset(key_pressed, 0, sizeof(bool) * 256);
}

void display(void)
{
	//Enable texture
	if (texmode == 1)
	{
		glEnable(GL_TEXTURE_GEN_S);
		glEnable(GL_TEXTURE_GEN_T);
	}
	else
	{
		glDisable(GL_TEXTURE_GEN_S);
		glDisable(GL_TEXTURE_GEN_T);
	}
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity(); // reset transformation matrix to identity
	gluLookAt(0.0, 1.0, zoom, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

	glPushMatrix();
	glRotatef(fLightRotateAngle0, 1.0, 0.0, 0.0);
	glRotatef(fLightRotateAngle1, 0.0, 1.0, 0.0);
	glLightfv(GL_LIGHT0, GL_POSITION, lpos);
	glPopMatrix();

	//draw the background
	glPushAttrib(GL_TEXTURE_BIT);
	glPushMatrix();
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);
	glEnd();
	glPopMatrix();
	glPopAttrib();
	glRotatef(fRotateAngle0, 1.0, 0.0, 0.0);
	glRotatef(fRotateAngle1, 0.0, 1.0, 0.0);
	glTranslatef(0.0, -1.25, 0.0);
	//Sets the material
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, amb);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diff);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, spec);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 10.0);
	glBindTexture(GL_TEXTURE_2D, textureID[texmode]);
	reader.render();
	glFlush();
	glutSwapBuffers();	// swap front/back framebuffer to avoid flickering
	glutPostRedisplay(); // trigger display function by sending redraw into message queue
}

void reshape(int w, int h)
{
	// reset viewport ( drawing screen ) size
	glViewport(0, 0, w, h);
	float fAspect = ((float)w) / h;
	// reset OpenGL projection matrix
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(70.f, fAspect, 0.001f, 30.f);
	glMatrixMode(GL_MODELVIEW);
}

void keyboard(unsigned char key, int x, int y) {
	key_pressed[key] = true;
}

void keyboardup(unsigned char key, int x, int y) {
	key_pressed[key] = false;
}

void specialkeyboard(int key, int x, int y) {
	if (key >= 100 && key < 104)
		key_pressed[key - 100] = true; // subtract 100 to prevent a conflict with 'D' ASCII
}

void specialkeyboardup(int key, int x, int y) {
	if (key >= 100 && key < 104)
		key_pressed[key - 100] = false; // subtract 100 to prevent a conflict with 'D' ASCII
}

float dRot = 2.0f;
float dLightRot = 4.0f;
void timer(int v) {
	if (key_pressed['q']) { // zoom out
		zoom += 0.1;
	}
	if (key_pressed['e']) { // zoom in
		zoom -= 0.1;
	}
	if (key_pressed['w']) {
		fRotateAngle0 -= dRot;
	}
	if (key_pressed['s']) {
		fRotateAngle0 += dRot;
	}
	if (key_pressed['a']) {
		fRotateAngle1 -= dRot;
	}
	if (key_pressed['d']) {
		fRotateAngle1 += dRot;
	}
	if (key_pressed['t']) {
		texmode = 1 - texmode;
	}
	if (key_pressed[27]) {
		exit(0);
	}
	if (key_pressed[GLUT_KEY_UP - 100]) {
		fLightRotateAngle0 -= dLightRot;
	}
	if (key_pressed[GLUT_KEY_DOWN - 100]) {
		fLightRotateAngle0 += dLightRot;
	}
	if (key_pressed[GLUT_KEY_LEFT - 100]) {
		fLightRotateAngle1 -= dLightRot;
	}
	if (key_pressed[GLUT_KEY_RIGHT - 100]) {
		fLightRotateAngle1 += dLightRot;
	}
	glutPostRedisplay();
	glutTimerFunc(1000 / FPS, timer, v);
}

int main(int argc, char* argv[])
{
	glutInit(&argc, (char**)argv);
	// set up for double-buffering & RGB color buffer & depth test
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(100, 100);
	glutCreateWindow(argv[0]);

	init(); // setting up user data & OpenGL environment

	glutDisplayFunc(display);  // called when drawing
	glutReshapeFunc(reshape);  // called when change window size
	glutKeyboardFunc(keyboard);
	glutKeyboardUpFunc(keyboardup);
	glutSpecialFunc(specialkeyboard);
	glutSpecialUpFunc(specialkeyboardup);
	glutTimerFunc(100, timer, FPS);
	glutMainLoop(); // start the main message-callback loop
	delete(tdata);
	return 0;
}