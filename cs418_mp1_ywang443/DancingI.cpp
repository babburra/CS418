//  
// Example code for OpenGL programming
//
#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <ctime>
#include <math.h>
//#include <SOIL/SOIL.h>

int nFPS = 40;
float fRotateAngle = 0.f;
float change = 0.f;
float changecolor = 0.f;
clock_t startClock=0,curClock;
long long int prevF=0,curF=0;
int dipMode=1;
int colorflag = 1;

void init(void)
{
	// init your data, setup OpenGL environment here
	glClearColor(0.0,1.0,1.0,1.0); // clear color is gray		
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // uncomment this function if you only want to draw wireframe model
					// GL_POINT / GL_LINE / GL_FILL (default)
	glPointSize(4.0);
}

void display(void)
{
	if(dipMode==1)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}else{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	

	curF++;
	// put your OpenGL display commands here
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	// reset OpenGL transformation matrix
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity(); // reset transformation matrix to identity

	// setup look at transformation so that 
	// eye is at : (0,0,3)
	// look at center is at : (0,0,0)
	// up direction is +y axis
	gluLookAt(0.f,0.f,3.f,0.f,0.f,0.f,0.f,1.f,0.f);
	glRotatef(fRotateAngle,.3f,1.f,.3f);
	//rand() * 2 - 1;

		// Test drawing a solid teapot

/*
	else{
		glLineWidth(2.5);
		//glColor3f(1. , 0. , 0.);
	}
*/

	glColor3f(changecolor + 1., changecolor + .5, changecolor);// set current color to orange
	//glutSolidTeapot(1.f); // call glut utility to draw a solid teapot 
  glBegin(GL_TRIANGLE_FAN);
  
	glVertex2f(-.6 + sin(change - 10) / 20, 1. + cos(change - 10) / 20);
	glVertex2f(-.6 + cos(change - 10) / 20, .6 + sin(change - 10) / 20);
	glVertex2f(-.2 + cos(change - 10) / 20, .6 + cos(change - 10) / 20); // v3
	glVertex2f(.2 + cos(change - 10) / 20, .6 + sin(change - 10) / 20); // v10
	glVertex2f(.6 + sin(change - 10) / 20, .6 + sin(change - 10) / 20);
	glVertex2f(.6 + cos(change - 10) / 20, 1. + sin(change - 10) / 20);

  glEnd();

  glBegin(GL_TRIANGLE_FAN);
/*	if (colorflag) {
		glColor3f(changecolor + 1., changecolor + .5, changecolor);// set current color to orange
	}
	else {
		glColor3f(changecolor + 1., changecolor + .5, changecolor);
	}*/

	glVertex2f(0. + sin(change - 10) / 20, -1. + sin(change - 10) / 20);
	glVertex2f(-.6 + sin(change - 10) / 20, -1. + sin(change - 10) / 20);
	glVertex2f(-.6 + cos(change - 10) / 20, -.6 + cos(change - 10) / 20);
	glVertex2f(-.2 + cos(change - 10) / 20, -.6 + sin(change - 10) / 20);
	glVertex2f(-.2 + cos(change - 10) / 20, .6 + cos(change - 10) / 20); // v3
	glVertex2f(.2 + cos(change - 10) / 20, .6 + sin(change - 10) / 20); // v10
	glVertex2f(.2 + sin(change - 10) / 20, -.6 + cos(change - 10) / 20);
	glVertex2f(.6 + cos(change - 10) / 20, -.6 + sin(change - 10) / 20);
	glVertex2f(.6 + sin(change - 10) / 20, -1. + cos(change - 10) / 20);

  glEnd();

	//glFlush();
	glutSwapBuffers();	// swap front/back framebuffer to avoid flickering 

	curClock=clock();
	float elapsed=(curClock-startClock)/(float)CLOCKS_PER_SEC;
	if(elapsed>1.0f){
		float fps=(float)(curF-prevF)/elapsed;
		printf("fps:%f\n",fps);
		prevF=curF;
		startClock=curClock;
	}
}
 
void reshape (int w, int h)
{
	// reset viewport ( drawing screen ) size
	glViewport(0, 0, w, h);
	float fAspect = ((float)w)/h; 
	// reset OpenGL projection matrix
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(70.f,fAspect,0.001f,30.f); 
}



void keyboard(unsigned char key, int x, int y)
{
	// put your keyboard control here
	if (key == 27) 
	{
		// ESC hit, so quit
		printf("demonstration finished.\n");
		exit(0);
	}
	if( key == 'h' || key == 'H'){
		dipMode = 1-dipMode;
	}
	if (key == 'w' || key == 'W') { 
		if (nFPS <= 300) {
			nFPS += 10;
			printf("Speeding up");
		}
		else {
			printf("FPS has reached upper limit.");
		}

	}
	if (key == 's' || key == 'S') {
		if (nFPS >= 35) {
			nFPS -= 10;
			printf("Slowing down");
		}
		else {
			printf("FPS has reached lower limit.");
		}
	}
}

void mouse(int button, int state, int x, int y)
{
	// process your mouse control here
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
		printf("mouse control");
}


void timer(int v)
{
	fRotateAngle += 1.f; // change rotation angles
	change += .1f;
	changecolor = sin(change/10);
	glutPostRedisplay(); // trigger display function by sending redraw into message queue
	glutTimerFunc(1000 / nFPS, timer, v); // restart timer again
}


int main(int argc, char* argv[])
{
	glutInit(&argc, (char**)argv);
	// set up for double-buffering & RGB color buffer & depth test
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH); 
	glutInitWindowSize (500, 500); 
	glutInitWindowPosition (100, 100);
	glutCreateWindow ((const char*)"Dancing I");

	init(); // setting up user data & OpenGL environment
	
	// set up the call-back functions 
	glutDisplayFunc(display);  // called when drawing 
	glutReshapeFunc(reshape);  // called when change window size
	glutKeyboardFunc(keyboard); // called when received keyboard interaction
	glutMouseFunc(mouse);	    // called when received mouse interaction
	glutTimerFunc(100,timer,nFPS); // a periodic timer. Usually used for updating animation
	
	startClock=clock();

	glutMainLoop(); // start the main message-callback loop

	return 0;
}
