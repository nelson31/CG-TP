#include <stdio.h>
#include <stdlib.h>
#define _USE_MATH_DEFINES
#include <math.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif


float camX = 0, camY, camZ = 5;
int startX, startY, tracking = 0;

int alpha = 0, beta = 0, r = 5;

#define POINT_COUNT 5
// Points that make up the loop for catmull-rom interpolation
float p[POINT_COUNT][3] = {{-1,-1,0},{-1,1,0},{1,1,0},{0,0,0},{1,-1,0}};

/**
Variável que guarda os 
pontos do percurso
*/
float percurso[100][3];

/**
Variáveis que guardam os vetores que 
representam os eixos do teapot
*/
float X[3], Y[3] = { 0.0f, 1.0f, 0.0f }, Z[3];


void buildRotMatrix(float *x, float *y, float *z, float *m) {

	m[0] = x[0]; m[1] = x[1]; m[2] = x[2]; m[3] = 0;
	m[4] = y[0]; m[5] = y[1]; m[6] = y[2]; m[7] = 0;
	m[8] = z[0]; m[9] = z[1]; m[10] = z[2]; m[11] = 0;
	m[12] = 0; m[13] = 0; m[14] = 0; m[15] = 1;
}


void cross(float *a, float *b, float *res) {

	res[0] = a[1]*b[2] - a[2]*b[1];
	res[1] = a[2]*b[0] - a[0]*b[2];
	res[2] = a[0]*b[1] - a[1]*b[0];
}


void normalize(float *a) {

	float l = sqrt(a[0]*a[0] + a[1] * a[1] + a[2] * a[2]);
	a[0] = a[0]/l;
	a[1] = a[1]/l;
	a[2] = a[2]/l;
}


float length(float *v) {

	float res = sqrt(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]);
	return res;

}

void multMatrixVector(float *m, float *v, float *res) {

	for (int j = 0; j < 4; ++j) {
		res[j] = 0;
		for (int k = 0; k < 4; ++k) {
			res[j] += v[k] * m[j * 4 + k];
		}
	}

}

/**
Função que trata da computação do cálculo de uma 
componente de derivada dada a respetiva linha da 
matriz resultante da multiplicação da matriz 
m pelo vetor p
*/
float computeDeriv(float t, float* a) {

	float ret = 0;
	for (int i = 0; i < 4; i++) {
		switch (i) {
			case 0: ret += 3 * (float)pow((double)t, 2) * a[i]; break;
			case 1: ret += 2 * t * a[i]; break;
			case 2: ret += a[i]; break;
			default: break;
		}
	}
	return ret;
}


void getCatmullRomPoint(float t, float *p0, float *p1, float *p2, float *p3, float *pos, float *deriv) {

	// catmull-rom matrix
	float m[4][4] = {	{-0.5f,  1.5f, -1.5f,  0.5f},
						{ 1.0f, -2.5f,  2.0f, -0.5f},
						{-0.5f,  0.0f,  0.5f,  0.0f},
						{ 0.0f,  1.0f,  0.0f,  0.0f}};
	float v[4];
	float a[4];
	/* Para cada coordenada x,y e z */
	for (int i = 0; i < 3; i++) {
		
		/* Construimos o array v para cada coordenada */
		v[0] = p0[i]; v[1] = p1[i]; v[2] = p2[i]; v[3] = p3[i];

		/* Calculamos o vertor a para cada coordenada */
		multMatrixVector((float*)m, v, a);

		/* Compute pos = T * A */
		pos[i] = 0;

		for (int j = 0; j < 4; j++) {
			pos[i] += (float)pow((double)t,(double)3-j) * a[j];
		}

		/* compute deriv = T' * A */
		deriv[i] = computeDeriv(t,a);
	}
}


// given  global t, returns the point in the curve
void getGlobalCatmullRomPoint(float gt, float *pos, float *deriv) {

	float t = gt * POINT_COUNT; // this is the real global t
	int index = floor(t);  // which segment
	t = t - index; // where within  the segment

	// indices store the points
	int indices[4]; 
	indices[0] = (index + POINT_COUNT-1)%POINT_COUNT;	
	indices[1] = (indices[0]+1)%POINT_COUNT;
	indices[2] = (indices[1]+1)%POINT_COUNT; 
	indices[3] = (indices[2]+1)%POINT_COUNT;

	getCatmullRomPoint(t, p[indices[0]], p[indices[1]], p[indices[2]], p[indices[3]], pos, deriv);
}


void changeSize(int w, int h) {

	// Prevent a divide by zero, when window is too short
	// (you cant make a window with zero width).
	if(h == 0)
		h = 1;

	// compute window's aspect ratio 
	float ratio = w * 1.0 / h;

	// Reset the coordinate system before modifying
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	// Set the viewport to be the entire window
    glViewport(0, 0, w, h);

	// Set the correct perspective
	gluPerspective(45,ratio,1,1000);

	// return to the model view matrix mode
	glMatrixMode(GL_MODELVIEW);
}

/**
Função que inicializa o vetor a partir do qual 
será desenhado o percurso a ser percorrido pelo 
teapot
*/
void initCurve() {

	float deriv[3];
	float gt;
	/* Construimos o array */
	for (int i = 0; i < 100; i++) {
		gt = (i + 1) / (float)100;
		getGlobalCatmullRomPoint(gt, percurso[i], deriv);
	}
}

/**
Função que desenha a curva a ser
percorrida pelo teapot
*/
void renderCatmullRomCurve() {

	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < 100; i++) {
		/* Desenhamos o vértice */
		glVertex3f(percurso[i][0], percurso[i][1], percurso[i][2]);
		//printf("(%f, %f, %f)\n", p[i][0], p[i][1], p[i][2]);
	}
	glEnd();
}

/**
Função que trata das transformações 
geométricas tendo em conta o instante 
t
*/
void reposicionaTeapot(float gt) {

	/* Matriz para a qual será copia 
	a matriz de rotação */
	float matrix[4][4];

	float position[3];
	float deriv[3];
	/* Obtemos os valores da posições e da derivada */
	getGlobalCatmullRomPoint(gt, position, deriv);
	/* Efetuamos a translação */
	glTranslatef(position[0], position[1], position[2]);
	/* Calculamos os valores dos vetores para os eixos */
	for (int i = 0; i < 3; i++)
		X[i] = deriv[i];
	normalize(X);
	cross(X, Y, Z);
	normalize(Z);
	cross(Z, X, Y);
	normalize(Y);
	/* Construimos a matriz de rotação */
	buildRotMatrix(X, Y, Z, (float*)matrix);
	glMultMatrixf((float*)matrix);
}


void renderScene(void) {

	static float t = 0;
	glClearColor(0.0f,0.0f,0.0f,0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();
	gluLookAt(camX, camY, camZ, 
		      0.0,0.0,0.0,
			  0.0f,1.0f,0.0f);

	renderCatmullRomCurve();

	/* Obtemos a posição e a derivada 
	para o instante atual */

	// apply transformations here
	// ...

	reposicionaTeapot(t);

	glutWireTeapot(0.1);


	glutSwapBuffers();
	t+=0.0005;
}


void processMouseButtons(int button, int state, int xx, int yy) 
{
	if (state == GLUT_DOWN)  {
		startX = xx;
		startY = yy;
		if (button == GLUT_LEFT_BUTTON)
			tracking = 1;
		else if (button == GLUT_RIGHT_BUTTON)
			tracking = 2;
		else
			tracking = 0;
	}
	else if (state == GLUT_UP) {
		if (tracking == 1) {
			alpha += (xx - startX);
			beta += (yy - startY);
		}
		else if (tracking == 2) {
			
			r -= yy - startY;
			if (r < 3)
				r = 3.0;
		}
		tracking = 0;
	}
}


void processMouseMotion(int xx, int yy)
{
	int deltaX, deltaY;
	int alphaAux, betaAux;
	int rAux;

	if (!tracking)
		return;

	deltaX = xx - startX;
	deltaY = yy - startY;

	if (tracking == 1) {

		alphaAux = alpha + deltaX;
		betaAux = beta + deltaY;

		if (betaAux > 85.0)
			betaAux = 85.0;
		else if (betaAux < -85.0)
			betaAux = -85.0;

		rAux = r;
	}
	else if (tracking == 2) {

		alphaAux = alpha;
		betaAux = beta;
		rAux = r - deltaY;
		if (rAux < 3)
			rAux = 3;
	}
	camX = rAux * sin(alphaAux * 3.14 / 180.0) * cos(betaAux * 3.14 / 180.0);
	camZ = rAux * cos(alphaAux * 3.14 / 180.0) * cos(betaAux * 3.14 / 180.0);
	camY = rAux *							     sin(betaAux * 3.14 / 180.0);
}


int main(int argc, char **argv) {

// inicialization
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH|GLUT_DOUBLE|GLUT_RGBA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(320,320);
	glutCreateWindow("CG@DI-UM");
		
// callback registration 
	glutDisplayFunc(renderScene);
	glutIdleFunc(renderScene);
	glutReshapeFunc(changeSize);

// mouse callbacks
	glutMouseFunc(processMouseButtons);
	glutMotionFunc(processMouseMotion);

// OpenGL settings
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	initCurve();

// enter GLUT's main cycle 
	glutMainLoop();
	
	return 1;
}

