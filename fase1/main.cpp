#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#define _USE_MATH_DEFINES
#include <math.h>

// VARIAVEIS
float varx = 0, vary = 0, varz = 0;

float angle = 0;

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

void drawPlane(float size) {

// put code to draw cylinder in here

	glBegin(GL_TRIANGLES);
	glColor3f(0.0f, 0.2f, 1.0f);
	glVertex3f(0.0f, -size, size);
	glVertex3f(0.0f, -size, -size);
	glVertex3f(0.0f, size, -size);
	glEnd();

	glBegin(GL_TRIANGLES);
	glColor3f(0.0f, 0.2f, 1.0f);
	glVertex3f(0.0f, -size, size);
	glVertex3f(0.0f, size, -size);
	glVertex3f(0.0f, size, size);
	glEnd();

}

void drawBox(float x, float y, float z) {

	// put code to draw cylinder in here

	//Base da caixa
	glBegin(GL_TRIANGLES);
	glColor3f(0.0f, 0.2f, 1.0f);
	glVertex3f(-x / 2, -y / 2, -z / 2);
	glVertex3f(x / 2, -y / 2, -z / 2);
	glVertex3f(-x / 2, -y / 2, z / 2);
	glEnd();

	glBegin(GL_TRIANGLES);
	glColor3f(0.0f, 0.2f, 1.0f);
	glVertex3f(x / 2, -y / 2, -z / 2);
	glVertex3f(x / 2, -y / 2, z / 2);
	glVertex3f(-x / 2, -y / 2, z / 2);
	glEnd();

	//Tampa da caixa
	glBegin(GL_TRIANGLES);
	glColor3f(0.0f, 0.2f, 1.0f);
	glVertex3f(-x / 2, y / 2, -z / 2);
	glVertex3f(-x / 2, y / 2, z / 2);
	glVertex3f(x / 2, y / 2, -z / 2);
	glEnd();

	glBegin(GL_TRIANGLES);
	glColor3f(0.0f, 0.2f, 1.0f);
	glVertex3f(x / 2, y / 2, -z / 2);
	glVertex3f(-x / 2, y / 2, z / 2);
	glVertex3f(x / 2, y / 2, z / 2);
	glEnd();

	//Face ao longo do eixo x no z positivo
	glBegin(GL_TRIANGLES);
	glColor3f(1.0f, 0.0f, 1.0f);
	glVertex3f(x / 2, y / 2, z / 2);
	glVertex3f(-x / 2, y / 2, z / 2);
	glVertex3f(-x / 2, -y / 2, z / 2);
	glEnd();

	glBegin(GL_TRIANGLES);
	glColor3f(1.0f, 0.0f, 1.0f);
	glVertex3f(x / 2, y / 2, z / 2);
	glVertex3f(-x / 2, -y / 2, z / 2);
	glVertex3f(x / 2, -y / 2, z / 2);
	glEnd();

	//Face ao longo do eixo x no z negativo
	glBegin(GL_TRIANGLES);
	glColor3f(1.0f, 1.0f, 0.0f);
	glVertex3f(x / 2, y / 2, -z / 2);
	glVertex3f(-x / 2, -y / 2, -z / 2);
	glVertex3f(-x / 2, y / 2, -z / 2);
	glEnd();

	glBegin(GL_TRIANGLES);
	glColor3f(1.0f, 1.0f, 0.0f);
	glVertex3f(x / 2, y / 2, -z / 2);
	glVertex3f(x / 2, -y / 2, -z / 2);
	glVertex3f(-x / 2, -y / 2, -z / 2);
	glEnd();

	//Face ao longo do eixo z no x negativo
	glBegin(GL_TRIANGLES);
	glColor3f(2.0f, 1.0f, 1.0f);
	glVertex3f(-x / 2, y / 2, -z / 2);
	glVertex3f(-x / 2, -y / 2, z / 2);
	glVertex3f(-x / 2, y / 2, z / 2);
	glEnd();

	glBegin(GL_TRIANGLES);
	glColor3f(2.0f, 1.0f, 0.0f);
	glVertex3f(-x / 2, y / 2, -z / 2);
	glVertex3f(-x / 2, -y / 2, -z / 2);
	glVertex3f(-x / 2, -y / 2, z / 2);
	glEnd();

	//Face ao longo do eixo z no x positivo
	glBegin(GL_TRIANGLES);
	glColor3f(2.0f, 1.0f, 1.0f);
	glVertex3f(x / 2, y / 2, -z / 2);
	glVertex3f(x / 2, y / 2, z / 2);
	glVertex3f(x / 2, -y / 2, z / 2);
	glEnd();

	glBegin(GL_TRIANGLES);
	glColor3f(2.0f, 1.0f, 0.0f);
	glVertex3f(x / 2, y / 2, -z / 2);
	glVertex3f(x / 2, -y / 2, z / 2);
	glVertex3f(x / 2, -y / 2, -z / 2);
	glEnd();
}

/**
	Desenho da esfera (INCOMPLETO)
*/
void drawSphere(int radius, int slices, int stacks) {

	// Usado para a construcao de cada uma das circunferencias
	GLfloat alpha = 2 * M_PI / slices;

	// Angulo usado para a construcao das circunferencias
	GLfloat beta = M_PI / stacks;

	// Usado para dar uma cor diferente
	GLfloat cy = 0.0f;

	for (GLfloat angleB = -M_PI/2; angleB < (2 * M_PI / 2); angleB += beta) {

		cy += 0.0185f;

		// Desenhar os circulos de cima e de baixo
		for (GLfloat angleA = 0; angleA < (2 * M_PI); angleA += alpha) {

			GLfloat nextAngle = angleA + alpha;

			glBegin(GL_TRIANGLES);
			glColor3f(1.0f, cy, 0.0f);
			glVertex3f(0.0f, radius*sin(angleB), 0.0f);
			glVertex3f((radius * cos(angleB)) * sin(angleA), radius * sin(angleB), (radius * cos(angleB)) * cos(angleA));
			glVertex3f((radius * cos(angleB)) * sin(nextAngle), radius * sin(angleB), (radius * cos(angleB)) * cos(nextAngle));
			glEnd();
		}
	}
}

/*
	Desenho de um cone
*/
void drawCone(int bottomRadius, int height, int slices, int stacks) {

	GLfloat reason = (GLfloat) height / stacks;

	GLfloat alpha = 2 * M_PI / slices;

	GLfloat currentRadius=bottomRadius;

	for (GLfloat currentHeight = 0.0f; currentHeight <= height; currentHeight += reason) {

		currentRadius = ((height-currentHeight) * bottomRadius) / height;

		for (GLfloat angle = 0; angle < (2 * M_PI); angle += alpha) {

			GLfloat nextAngle = angle + alpha;

			glBegin(GL_TRIANGLES);
			glColor3f(1.0f, 0.0f, 0.0f);
			glVertex3f(0.0f, currentHeight, 0.0f);
			glVertex3f(currentRadius * sin(angle), currentHeight, currentRadius * cos(angle));
			glVertex3f(currentRadius * sin(nextAngle), currentHeight, currentRadius * cos(nextAngle));
			glEnd();
		}
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
	glRotatef(angle, 0.0f, 1.0f, 0.0f);
	//drawPlane(1.0f);

	//Cubo
	
	// ....

	// Esfera
	//glRotatef(angle, 0.0f, 1.0f, 0.0f);
	//drawSphere(2, 100, 100);
	

	// Cone
	drawCone(1,5,100,500);

	// End of frame
	glutSwapBuffers();
}


void processKeys(unsigned char c, int xx, int yy) {

// put code to process regular keys in here

	switch (c)
	{
	case 'r':
		angle += 1.0f;
		break;
	default:
		break;
	}
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
