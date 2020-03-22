#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <vector>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#define _USE_MATH_DEFINES
#include <math.h>
using std::vector;

float alfa = 0.0f, beta = 0.5f, radius = 100.0f;
float camX, camY, camZ;

// Guardar a ultimas posicoes do rato na tela
int mouseXPos, mouseYPos;
int isPressed = -1;

// Número de arvores no ambiente
const int N = 300;
// Numero de bools de fora e de dentro, respetivamente
const int TF = 16, TD = 8;
// Tamanho circulo centro
const int tamCentro = 50;

// Usado para o bull de cha de fora e de dentro
float radiusF = 35.0f, radiusD = 15.0f;

// Array com as coordenadas das arvores
vector<float> coords;

// Variaveis usadas para rodar os teapots
float rodar = 0;
int timebase=0;

// Usado para mostrar os FPS
int frame = 0, timefps = 0;
float fps=0;

/*
 * Converter coordenadas polares em cartesianas
 */
void spherical2Cartesian() {

	camX = radius * cos(beta) * sin(alfa);
	camY = radius * sin(beta);
	camZ = radius * cos(beta) * cos(alfa);
}

/*
 * Obter os FPS, mostrando-os no titulo da janela
 */
void getFPS() {
	int time;
	frame++;
	time = glutGet(GLUT_ELAPSED_TIME);
	if (time - timefps > 1000) {
		fps = (float) frame * 1000.0 / (time - timefps);
		timefps = time;
		frame = 0;
	}
	char string[80];
	sprintf(string, "FPS COUNTER: %f", fps);
	glutSetWindowTitle(string);
}

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

/*
 * Usado para guardar as coordenadas das arvores 
 * A geracao dos numeros aleatorios é feita com numeros reais, e nao inteiros
*/
void guardaCoordenadas() {
	int num = 0;
	srand(time(NULL));

	while (num < N*2)
	{
		float x = -100 + (rand()) / (static_cast <float> (RAND_MAX / 200));
		float z = -100 + (rand()) / (static_cast <float> (RAND_MAX / 200));
		
		if ((x * x + z * z) > (tamCentro* tamCentro) && abs(x)<=100 && abs(z)<=100) {
			printf("Coordenada da Arvore %d -> x: %f, z: %f\n", num/2, x, z);
			coords.push_back(x);
			coords.push_back(z);
			num += 2;
		}
	}
}

/*
 * Desenha as arvores a partir das coordenadas feitas previamente aleatorias
 * Mudando a variavel N, mudamos o numero de arvores no ambiente
*/
void desenhaArvores() {
	int num = 0; int tam = N * 2;

	while (num < tam)
	{
		glPushMatrix();

		glTranslatef(coords.at(num), 0, coords.at(num+1));

		// Tronco
		glPushMatrix();
		glRotatef(-90, 1.0f, 0.0f, 0.0f);
		glColor3f(0.59f, 0.29f, 0.0f);
		glutSolidCone(0.5f, 3.0f, 10, 100);
		glPopMatrix();

		// Copa
		glPushMatrix();
		glTranslatef(0.0f, 2.0f, 0.0f);
		glRotatef(-90, 1.0f, 0.0f, 0.0f);
		glColor3f(0.13f, 0.55f, 0.13f);
		glutSolidCone(2.0f, 6.0f, 10, 100);
		glPopMatrix();

		glPopMatrix();

		num += 2;
	}
}

/*
 * Desenho dos teapots de fora
*/
void desenhaTeapotsFora() {

	float x, y = 1.5f, z;
	float deg, angl;
	float razao = (2 * M_PI) / TF;

	for (int i = 0; i < TF; i++) {
		glPushMatrix();
		angl = (float) i * razao + rodar;
		float x = radiusF * sin(angl);
		float z = radiusF * cos(angl);
		deg = (float) ((180 * angl) / M_PI);
		glTranslatef(x, y, z);
		glRotatef(deg, 0.0f, 1.0f, 0.0f);
		glColor3f(1.0f, 0.0f, 0.0f);
		glutSolidTeapot(2.0f);
		glPopMatrix();
	}
}

/*
 * Desenho dos teapots de dentro
*/
void desenhaTeapotsDentro() {

	float x, y = 1.5f, z;
	float deg, angl;
	float razao = (2 * M_PI) / TD;

	for (int i = 0; i < TF; i++) {
		glPushMatrix();
		angl = (float) (i * razao) - rodar;
		x = (float) radiusD * sin(angl);
		z = (float) radiusD * cos(angl);
		deg = (float) ((180 * angl) / M_PI) - 90;
		glTranslatef(x, y, z);
		glRotatef(deg, 0.0f, 1.0f, 0.0f);
		glColor3f(0.0f, 0.0f, 1.0f);
		glutSolidTeapot(2.0f);
		glPopMatrix();
	}
}

void renderScene(void) {

	// clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// set the camera
	glLoadIdentity();
	gluLookAt(camX, camY, camZ,
		0.0, 0.0, 0.0,
		0.0f, 1.0f, 0.0f);

	// Get de FPS
	getFPS();

	// Desenho do plano
	glColor3f(0.2f, 0.8f, 0.2f);
	glBegin(GL_TRIANGLES);
		glVertex3f(100.0f, 0, -100.0f);
		glVertex3f(-100.0f, 0, -100.0f);
		glVertex3f(-100.0f, 0, 100.0f);

		glVertex3f(100.0f, 0, -100.0f);
		glVertex3f(-100.0f, 0, 100.0f);
		glVertex3f(100.0f, 0, 100.0f);
	glEnd();

	// Desenhar as arvores 
	desenhaArvores();

	// Desenhar os objetos que estao no centro do plano
	desenhaTeapotsDentro();
	desenhaTeapotsFora();

	// Torus
	glPushMatrix();
	glTranslatef(0.0f, 1.0f, 0.0f);
	glColor3f(0.73f, 0.33f, 0.83f);
	glutSolidTorus(1.5f, 4.0f, 20, 20);
	glPopMatrix();

	// Usado para rodar os teapots
	int time = glutGet(GLUT_ELAPSED_TIME);
	if (time - timebase > 50) {
		rodar += 0.05f;
		timebase = time;
	}
	// End of frame
	glutSwapBuffers();
}


void processKeys(unsigned char c, int xx, int yy) {

// put code to process regular keys in here

}


void processSpecialKeys(int key, int xx, int yy) {

	switch (key) {

	case GLUT_KEY_RIGHT:
		alfa -= 0.1; break;

	case GLUT_KEY_LEFT:
		alfa += 0.1; break;

	case GLUT_KEY_UP:
		beta += 0.1f;
		if (beta > 1.5f)
			beta = 1.5f;
		break;

	case GLUT_KEY_DOWN:
		beta -= 0.1f;
		if (beta < -1.5f)
			beta = -1.5f;
		break;

	case GLUT_KEY_PAGE_DOWN: radius -= 1.0f;
		if (radius < 1.0f)
			radius = 1.0f;
		break;

	case GLUT_KEY_PAGE_UP: radius += 1.0f; break;
	}
	spherical2Cartesian();
	glutPostRedisplay();
}

void OnMouseMove(int x, int y) {

	if (isPressed == 1) {
		alfa += (x - mouseXPos) * 0.01f;
		beta += (y - mouseYPos) * 0.01f;
		if (beta > 1.5f)
			beta = 1.5f;
		if (beta < -1.5f)
			beta = -1.5f;
		mouseXPos = x;
		mouseYPos = y;
		spherical2Cartesian();
	}
	else if (isPressed == 2) {
		radius += (y - mouseYPos);
		if (radius < 5.0f)
			radius = 5.0f;
		mouseXPos = x;
		mouseYPos = y;
		spherical2Cartesian();
	}
	glutPostRedisplay();
}

void mouse_pressed(int button, int state, int x, int y) {

	if ((button == GLUT_LEFT_BUTTON) && (state == GLUT_DOWN)) {
		mouseXPos = x;
		mouseYPos = y;
		isPressed = 1;
	}
	else if ((button == GLUT_LEFT_BUTTON) && (state == GLUT_UP)) {
		isPressed = 0;
	}
	else if ((button == GLUT_RIGHT_BUTTON) && (state == GLUT_DOWN)) {
		mouseYPos = y;
		isPressed = 2;
	}
	else if ((button == GLUT_RIGHT_BUTTON) && (state == GLUT_UP)) {
		isPressed = 0;
	}
	glutPostRedisplay();
}

void printInfo() {

	printf("Vendor: %s\n", glGetString(GL_VENDOR));
	printf("Renderer: %s\n", glGetString(GL_RENDERER));
	printf("Version: %s\n", glGetString(GL_VERSION));

	printf("\nUse Arrows to move the camera up/down and left/right\n");
	printf("Home and End control the distance from the camera to the origin");
}

int main(int argc, char **argv) {

// init GLUT and the window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH|GLUT_DOUBLE|GLUT_RGBA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(800,800);
	glutCreateWindow("CG@DI-UM");
		
// Required callback registry 
	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);
	glutIdleFunc(renderScene);
	glutPostRedisplay();
	
// Callback registration for keyboard processing
	glutKeyboardFunc(processKeys);
	glutSpecialFunc(processSpecialKeys);
	glutMouseFunc(mouse_pressed);
	glutMotionFunc(OnMouseMove);

//  OpenGL settings
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);

	spherical2Cartesian();

	// Guardar as coordenadas das arvores
	guardaCoordenadas();

	printInfo();

// enter GLUT's main cycle
	glutMainLoop();
	
	return 1;
}
