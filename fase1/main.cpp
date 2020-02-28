#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#define _USE_MATH_DEFINES
#include <math.h>

// VARIAVEIS
float varx = 0, vary = 0, varz = 0;


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

void drawPlane() {

// put code to draw cylinder in here

	glBegin(GL_TRIANGLES);
	glColor3f(0.0f, 0.2f, 1.0f);
	glVertex3f(0.0f, -5.0f, 5.0f);
	glVertex3f(0.0f, -5.0f, -5.0f);
	glVertex3f(0.0f, 5.0f, -5.0f);
	glEnd();

	glBegin(GL_TRIANGLES);
	glColor3f(0.0f, 0.2f, 1.0f);
	glVertex3f(0.0f, -5.0f, 5.0f);
	glVertex3f(0.0f, 5.0f, -5.0f);
	glVertex3f(0.0f, 5.0f, 5.0f);
	glEnd();

}

/**
	Desenho da esfera (INCOMPLETO)
*/
void drawSphere(int radius, int slices, int stacks) {

	GLfloat alpha = 2 * M_PI / slices;

	// Desenhar os circulos de cima e de baixo
	for (GLfloat angle = 0; angle < (2 * M_PI); angle += alpha) {

		GLfloat nextAngle = angle + alpha;

		glBegin(GL_TRIANGLES);
		glColor3f(0.0f, 0.0f, 1.0f);
		glVertex3f(0.0f, 0.0f, 0.0f);
		glVertex3f(radius * sin(angle), 0.0f, radius * cos(angle));
		glVertex3f(radius * sin(nextAngle), 0.0f, radius * cos(nextAngle));
		glEnd();

	}

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

	//Eixos
	drawAxis();
	//Plano
	drawPlane();
	//Cubo
	glColor3f(1.0f, 1.0f, 0.0f);
	glTranslatef(6, 1, 0);
	glutSolidCube(5);
	// Esfera
	//drawSphere(1, 10, 10);

	// Cone
	// ..

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


int main(int argc, char **argv) {

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

//  OpenGL settings
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	
// enter GLUT's main cycle
	glutMainLoop();
	
	return 1;
}
