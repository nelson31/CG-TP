

#include<stdio.h>
#include<stdlib.h>

#define _USE_MATH_DEFINES
#include <math.h>
#include <vector>

#include <IL/il.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif
using namespace std;

/**
Constante que define o número de 
árvores a serem desenhadas
*/
#define _TREES 200

/**
Variável que guarda a escala da 
altura do relevo do terreno
*/
#define __SCALE 0.5

/**
Definição de constantes que simbolizam 
os 2 tipos de camaras para inspecionar 
o ambiente
*/
#define _FPS 1000
#define _PANORAMICA 1001

/**
Variável que guarda o tipo de camara 
em vigor a cada momento
*/
int camera = _PANORAMICA;


/**
Variáveis responsáveis pela câmara panorâmica
*/
float camX = 00, camY = 30, camZ = 40;
int startX, startY, tracking = 0;

int alpha = 0, beta = 45, r = 50;

/**
Variáveis responsáveis pela câmara fps
*/
float eyeHeight = 3.0f;
float px = 0.0f, py = 0.0f, pz = 20.0f;
float lookx = 0.0f, looky = py, lookz = 0.0f;
float alphaa = 180.0f;
float deltaPos = 0.0f, deltaLeftRight = 0.0f, deltaAlphaa = 0.0f;


/**
Variável global que representa os vbo's
*/
GLuint* vertices;

/**
Variável que guarda as posições das 
árvores geradas aleatoriamente
*/
float positions[_TREES*2];

/**
Variáveis globais utilziadas para 
ler o conteúdo da imagem 
*/
unsigned int numStrips, imageWidth, imageHeight;
unsigned char* imageData;

/**
Variáveis que guardam parâmetros
de desenho dos teapots
*/
float raio = 50, ri = 35, rc = 15;
int nTeapotsIn = 8, nTeapotsOut = 16;
float ang = 0.0f;

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
Método que lê e processa a imagem
*/
void readImage(unsigned char** imageData, unsigned int* t, unsigned int* tw, unsigned int* th) {

	ilGenImages(1, t);
	ilBindImage(*t);
	// terreno.jpg is the image containing our height map
	ilLoadImage((ILstring)"terreno.jpg");
	// convert the image to single channel per pixel
	// with values ranging between 0 and 255
	ilConvertImage(IL_LUMINANCE, IL_UNSIGNED_BYTE);
	// important: check tw and th values
	// both should be equal to 256
	// if not there was an error loading the image
	// most likely the image could not be found
	*tw = ilGetInteger(IL_IMAGE_WIDTH);
	*th = ilGetInteger(IL_IMAGE_HEIGHT);
	// imageData is a LINEAR array with the pixel values
	*imageData = ilGetData();
}

/**
Função que dado dois inteiros que representam
um vértice da imagem retornam a sua altura
*/
float h(int x, int z) {

	/* Reposicionamos as coordenadas x e z como
	se o canto superior esquerdo da imagem
	estivesse no indice (0,0) */
	int arrayX = (int)((float)x + (float)imageHeight / 2 - 0.5);
	int arrayZ = (int)((float)z + (float)imageHeight / 2 - 0.5);
	/* Retornamos o valor da altura nesse vértice */
	return (float)imageData[arrayZ * imageWidth + arrayX]*__SCALE;
}

/**
Função que retorna a altura (valor da coordenada y)
de qualquer ponto na grelha desenhada dadas as
coordenadas x e z desse mesmo ponto
*/
float hf(float x, float z) {

	/* Definição dos vértices
	que compõem o quadrado */
	int x1 = floor(x), x2 = x1 + 1;
	int z1 = floor(z), z2 = z1 + 1;

	float fz = z - (float)z1;

	float h_x1_z = h(x1, z1) * (1 - fz) + h(x1, z2) * fz;
	float h_x2_z = h(x2, z1) * (1 - fz) + h(x2, z2) * fz;

	float fx = x - (float)x1;

	/* Retornamos o valor da altura no ponto */
	return  h_x1_z * (1 - fx) + h_x2_z * fx;
}

/**
Método que desenha uma árvore num
dado ponto do referencial. o y será
sempre 0
*/
void drawTree(float x, float y, float z) {

	glPushMatrix();
	glTranslatef(x, y, z);
	glRotatef(-90.0, 1.0f, 0.0f, 0.0f);
	glColor3f(0.5f, 0.35f, 0.05f);
	glutSolidCone(0.3f, 4.0f, 10, 200);


	glTranslatef(0.0f, 0.0f, 1.2f);
	glColor3f(0.2f, 0.5f, 0.2f);
	glutSolidCone(1.0f, 4.0f, 10, 200);
	glPopMatrix();
}

/**
Método que desenha xx arvores
de forma aleatória
*/
void gerarPositionTrees() {

	int i = 0, x, z;
	while (i < _TREES * 2) {

		x = (float)(rand() % 200 - 100);
		z = (float)(rand() % 200 - 100);

		if (sqrt(x * x + z * z) > 50) {
			positions[i++] = x;
			positions[i++] = z;
		}
	}
}

/**
Função que desenha o terreno
*/
void drawTerrain() {

	// colocar aqui o código de desnho do terreno usando VBOs com TRIANGLE_STRIPS
	glPushMatrix();
	glColor3f(0.2f, 0.4f, 0.2f);
	for (int i = 0; i < numStrips; i++) {
		glBindBuffer(GL_ARRAY_BUFFER, vertices[i]);
		glVertexPointer(3, GL_FLOAT, 0, 0);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, imageWidth * 2);
	}
	glPopMatrix();
}

/**
Função que desenha as árvores nas posições
geradas aleatoriamente anteriormente
*/
void drawAllTrees() {

	int i = 0, x, z;
	while (i < _TREES * 2) {
		x = positions[i++];
		z = positions[i++];
		drawTree(x, hf(x, z), z);
	}
}

/**
Função que atualiza o posicionamento dos teapots 
para estes apresentarem movimento
*/
void atualizaPos(int value) {

	ang += 0.013f;
}

/**
Função que desenha os teapots dispostos
em circulos com os raios previamente definidos
*/
void drawTeapots() {

	int helptimeF = 0, helptimeI = 0;
	float alpha = 2 * M_PI / (float)nTeapotsIn, curAngle, x, z, deg;
	int i;
	for (i = 0; i < nTeapotsIn; i++) {
		curAngle = i * alpha;
		/* Converter o ângulo
		para graus */
		deg = (180 * (curAngle - ang) / M_PI) - 90;
		x = sin(curAngle - ang) * rc;
		z = cos(curAngle - ang) * rc;
		glPushMatrix();
		glTranslatef(x, hf(x,z)+0.7f, z);
		glRotatef(deg, 0.0f, 1.0f, 0.0f);
		glColor3f(0.0f, 0.0f, 1.0f);
		glutSolidTeapot(1.0f);
		glPopMatrix();
	}

	/* Desenhamos os teapots mais afastados */
	alpha = 2 * M_PI / (float)nTeapotsOut;
	for (i = 0; i < nTeapotsOut; i++) {
		curAngle = i * alpha;
		/* Converter o ângulo
		para graus */
		deg = (180 * (curAngle + ang) / M_PI);
		x = sin(curAngle + ang) * ri;
		z = cos(curAngle + ang) * ri;
		glPushMatrix();
		glTranslatef(x, hf(x,z)+0.7f, z);
		glRotatef(deg, 0.0f, 1.0f, 0.0f);
		glColor3f(1.0f, 0.3f, 0.0f);
		glutSolidTeapot(1.0f);
		glPopMatrix();
	}

	glutTimerFunc(10, atualizaPos, 0);
}

/**
Função que desenha 
o torus
*/
void drawTorus() {

	glColor3f(0.68f, 0.85f, 0.9f);
	glutSolidTorus(1.5f, 3.0f, 10, 100);
}

/**
Função que processa o evento de pressionar 
uma tecla normal do teclado
*/
void pressKeys(unsigned char key, int xx, int yy) {

	switch (key) {
		case 'w': if (camera == _FPS) deltaPos = 1.0f; break;
		case 's': if (camera == _FPS) deltaPos = -1.0f; break;
		case 'a': if (camera == _FPS) deltaLeftRight = 1.0f; break;
		case 'd': if (camera == _FPS) deltaLeftRight = -1.0f; break;
		case 'x': 
			if (camera == _FPS) 
				camera = _PANORAMICA;  
			else {
				camera = _FPS; py = eyeHeight + hf(px, pz);
				looky = py;
			}
	}
}

/**
Função que trata do processamento do 
largar de uma tecla normal do teclado
*/
void releaseKeys(unsigned char key, int xx, int yy) {

	if ((key == 'w' || key == 's') && camera == _FPS)
		deltaPos = 0.0f;
	if ((key == 'a' || key == 'd') && camera == _FPS)
		deltaLeftRight = 0.0f;
}

/**
Função que processa o evento de 
pressionar numa tecla especial
*/
void pressSpecialKeys(int key, int xx, int yy) {

	switch (key) {
		case GLUT_KEY_RIGHT: if (camera == _FPS) deltaAlphaa = -5.0f; break;
		case GLUT_KEY_LEFT: if (camera == _FPS) deltaAlphaa = 5.0f; break;
	}
}

/**
Função que processa o evento de larga 
uma tecla especial
*/
void releaseSpecialKeys(int key, int xx, int yy) {

	if (key == GLUT_KEY_RIGHT || key == GLUT_KEY_LEFT)
		deltaAlphaa = 0.0f;
}

/**
Função que trata do movimento da camara fps 
para a frente e para trás
*/
void computeMove() {

	px += deltaPos * sin(alphaa * M_PI / 180.0f);
	pz += deltaPos * cos(alphaa * M_PI / 180.0f);
	py = hf(px, pz) + eyeHeight;
	looky = py;
}

/**
Função que trata do movimento da camara fps
para a esquerda e para a direita
*/
void computeMoveLeftRight() {

	px += deltaLeftRight * sin((alphaa + 90) * M_PI / 180.0f);
	pz += deltaLeftRight * cos((alphaa + 90) * M_PI / 180.0f);
	py = hf(px, pz) + eyeHeight;
	looky = py;
}

/**
Função que trata da alteração do ângulo 
de direção do olhar da câmara
*/
void computeAlphaa() {

	alphaa += deltaAlphaa;
}

/**
Função que é invocada em cada loop
*/
void renderScene(void) {

	float pos[4] = {-1.0, 1.0, 1.0, 0.0};

	glClearColor(0.0f,0.0f,0.0f,0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();

	switch (camera) {

		case _PANORAMICA:
			gluLookAt(camX, camY, camZ,
				0.0, 0.0, 0.0,
				0.0f, 1.0f, 0.0f);
			break;

		case _FPS :
			gluLookAt(px, py, pz,
				px+sin(alphaa*M_PI/180.0f), looky, pz+cos(alphaa*M_PI/180.0f),
				0.0f, 1.0f, 0.0f);
			break;
	}
	glPushMatrix();

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	drawTerrain();

	glPopMatrix();

	drawAllTrees();

	drawTeapots();

	drawTorus();

	/* 
	Se a tecla 'w' ou 's' estiverem
	premidas computamos o movimento
	*/
	if (deltaPos)
		computeMove();

	/*
	Se a tecla 'a' ous 'd' estiverem 
	premidas computamos o movimento
	*/
	if (deltaLeftRight)
		computeMoveLeftRight();

	/* Processamento das teclas left
	e right do teclado */
	if (deltaAlphaa)
		computeAlphaa();

	// just so that it renders something before the terrain is built
	// to erase when the terrain is ready
	//glutWireTeapot(2.0);

	// End of frame
	glutSwapBuffers();
}


void processMouseButtons(int button, int state, int xx, int yy) {
	
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


void processMouseMotion(int xx, int yy) {

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
	camY = rAux * 							     sin(betaAux * 3.14 / 180.0);
}

/**
Função que inicializa as estruturas de dados 
para desenhar os objetos
*/
void init() {

	float x, y = 0.0f, z;

// 	Load the height map "terreno.jpg"
	unsigned int t, tw, th;
	readImage(&imageData, &t, &tw, &th);

	numStrips = th - 1;
	imageWidth = tw;
	imageHeight = th;

	/* Inicializamos as variáveis
	globais para os vbo's */
	vertices = (GLuint*)malloc(sizeof(GLuint) * numStrips);

// 	Build the vertex arrays

	/* Para cada strip de triângulos temos um vbo específico */
	vector<float>** v = (vector<float>**)malloc(sizeof(vector<float>*)*numStrips);

	/* Incializamos cada um dos vectors */
	for (int i = 0; i < numStrips; i++)
		v[i] = new vector<float>();

	/* Percorremos cada uma das strips */
	for (int i = 0; i < numStrips; i++) {
		/* Variável z dos vértices mais atras da strip */
		z = -(float)th / 2 + (float)i + 0.5f;
		/* Adicionamos cada triângulo que constituem as strips */
		for (int j = 0; j < tw; j++) {

			x = -(float)tw / 2 + (float)j + 0.5f;
			y = (float)imageData[i * imageWidth + j] * __SCALE;
			/* Primeiro vértice */

			v[i]->push_back(x);
			v[i]->push_back(y);
			v[i]->push_back(z);

			y = (float)imageData[(i + 1) * imageWidth + j] * 0.5;
			/* Segundo vértice */
			v[i]->push_back(x);
			v[i]->push_back(y);
			v[i]->push_back(z + 1);
		}
	}

	/* Criamos o vbo */
	glGenBuffers(numStrips, vertices);

	/* copiar os vectores para a memória gráfica */

	for (int i = 0; i < numStrips; i++) {
		glBindBuffer(GL_ARRAY_BUFFER, vertices[i]);
		glBufferData(
			GL_ARRAY_BUFFER, // tipo do buffer, só é relevante na altura do desenho
			sizeof(float) * (*v[i]).size(), // tamanho do vector em bytes
			(*v[i]).data(), // os dados do array associado ao vector
			GL_STATIC_DRAW); // indicativo da utilização (estático e para desenho)
	}


// 	OpenGL settings
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);

	/* Geramos as posições aleatórias do 
	mapa para desenhar as árvores */
	gerarPositionTrees();
}


int main(int argc, char **argv) {

// init GLUT and the window
	glutInit(&argc, argv);
	ilInit();
	glutInitDisplayMode(GLUT_DEPTH|GLUT_DOUBLE|GLUT_RGBA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(320,320);
	glutCreateWindow("CG@DI-UM");
		
#ifndef __APPLE__
	glewInit();
#endif

// Required callback registry 
	glutDisplayFunc(renderScene);
	glutIdleFunc(renderScene);
	glutReshapeFunc(changeSize);

// Callback registration for keyboard processing
	glutKeyboardFunc(pressKeys);
	glutSpecialFunc(pressSpecialKeys);

	glutIgnoreKeyRepeat(1);
	glutKeyboardUpFunc(releaseKeys);
	glutSpecialUpFunc(releaseSpecialKeys);

	glutMouseFunc(processMouseButtons);
	glutMotionFunc(processMouseMotion);
	glEnableClientState(GL_VERTEX_ARRAY);

	init();

// enter GLUT's main cycle
	glutMainLoop();
	
	return 0;
}

