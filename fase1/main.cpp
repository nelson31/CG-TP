#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#define _USE_MATH_DEFINES
#include <math.h>
#include <stdio.h>
#include <string.h>

#include "vertice.h"
#include "ListVertices.h"


// VARIAVEIS
float varx = 0, vary = 0, varz = 0;
float angle = 0;

int numVertices;

/**
Estrutura de dados que guarda os 
vertices a serem desenhados
*/
ListVertices lv;


void changeSize(int w, int h) {

	// Prevent a divide by zero, when window is too short
	// (you cant make a window with zero width).
	if(h == 0)
		h = 1;

	// compute window's aspect ratio 
	float ratio = w * 1.0 / h;

	// Set the projection matrix as current
	glMatrixMode(GL_PROJECTION);
	// Load Identity Matrix
	glLoadIdentity();
	
	// Set the viewport to be the entire window
    glViewport(0, 0, w, h);

	// Set perspective
	gluPerspective(45.0f ,ratio, 1.0f ,1000.0f);

	// return to the model view matrix mode
	glMatrixMode(GL_MODELVIEW);
}

/**
Função usada para desenhar os eixos
*/
void drawAxis() {

	glBegin(GL_LINES);
	// X axis in red
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(100.0f, 0.0f, 0.0f);
	// Y Axis in Green
	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 100.0f, 0.0f);
	// Z Axis in Blue
	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 100.0f);
	glEnd();
}

/**
Função que desenha os triangulos das formas a partir da lista de vertices
*/
void drawScene() {

	Vertice v,v2,v3;
	while ((v = nextV(lv)) != NULL) {
		v2 = nextV(lv);
		v3 = nextV(lv);
		glBegin(GL_TRIANGLES);
		glColor3f(0.0f, 0.0f, 1.0f);
		glVertex3f(getX(v),getY(v),getZ(v));
		glVertex3f(getX(v2),getY(v2),getZ(v2));
		glVertex3f(getX(v3),getY(v3),getZ(v3));
		glEnd();
	}

	// Colocar o pointer novamente a 0
	atualizaPointer(lv);
}

void renderScene(void) {

	// clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// set the camera
	glLoadIdentity();
	gluLookAt(10.0,10.0,10.0, 
		      0.0,0.0,0.0,
			  0.0f,1.0f,0.0f);


	// put the geometric transformations here

	glTranslatef(varx, vary, varz);

	// put drawing instructions here

	//AXIS
	drawAxis();

	//glRotatef(angle, 0.0f, 1.0f, 0.0f);

	drawScene();

	// End of frame
	glutSwapBuffers();
}


void processKeys(unsigned char c, int xx, int yy) {

// put code to process regular keys in here

	
}


void processSpecialKeys(int key, int xx, int yy) {

// put code to process special keys in here

	switch (key) {
	case GLUT_KEY_UP:
		vary += 0.1;
		break;
	case GLUT_KEY_DOWN:
		vary -= 0.1;
		break;
	case GLUT_KEY_LEFT:
		varx -= 0.1;
		break;
	case GLUT_KEY_RIGHT:
		varx += 0.1;
		break;
	default:
		break;
	}
}


void processMouse(int button, int state, int x, int y) {

	switch (button) {
	case GLUT_LEFT_BUTTON:
		vary += 0.1;
		break;
	case GLUT_RIGHT_BUTTON:
		vary -= 0.1;
		break;
	default:
		break;
	}
}



int main(int argc, char **argv) {

	lv = carregaFile("box.3D");
// init GLUT and the window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH|GLUT_DOUBLE|GLUT_RGBA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(800,800);
	glutCreateWindow("CG@TP@Fase1");
	
// Required callback registry 
	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);
	glutIdleFunc(renderScene);
	glutPostRedisplay();
	
// Callback registration for keyboard processing
	glutKeyboardFunc(processKeys);
	glutSpecialFunc(processSpecialKeys);
	glutMouseFunc(processMouse);

//  OpenGL settings
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	
// enter GLUT's main cycle
	glutMainLoop();

	return 1;
}
