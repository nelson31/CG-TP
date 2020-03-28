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

using std::vector;

float camX = 00, camY = 30, camZ = 40;
int startX, startY, tracking = 0;

int alpha = 0, beta = 45, r = 50;

// Valor da altura e largura da grelha, respetivamente
unsigned imageWidth, imageHeight;

// Array linear que guarda os valores dos pixeis convertidos para os valores 0-255
unsigned char* imageData;

// Usado para referenciar o VBO 
GLuint buffers[1];

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
 * Funcao que é chamada na renderScene e que e responsavel por desenhar o terreno
 * com o uso de VBOs e ainda TRIANGLE_STRIPS
*/
void drawTerrain() {

    // colocar aqui o código de desnho do terreno usando VBOs com TRIANGLE_STRIPS

	glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
	glVertexPointer(3, GL_FLOAT, 0, 0);
	for (int i = 0; i < imageHeight - 1; i++) {
		glDrawArrays(GL_TRIANGLE_STRIP, (imageWidth) * 2 * i, (imageWidth) * 2);
	}
}



void renderScene(void) {

	float pos[4] = {-1.0, 1.0, 1.0, 0.0};

	glClearColor(0.0f,0.0f,0.0f,0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();
	gluLookAt(camX, camY, camZ, 
		      0.0,0.0,0.0,
			  0.0f,1.0f,0.0f);

	drawTerrain();

	// just so that it renders something before the terrain is built
	// to erase when the terrain is ready
	//glutWireTeapot(2.0);

// End of frame
	glutSwapBuffers();
}



void processKeys(unsigned char key, int xx, int yy) {

// put code to process regular keys in here
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
 * Funcao que converte o valor dos vértices que se encontra inicialmente entre 0-255
 * para a escala de 0-30
 * Os parametros x e z representam a posicao na matriz de valores dos pixeis, pelo que 
 * corresponde a um vértice no nosso modelo(deve ainda se converter estes valores matriciais
 * para valores correspondentes no array linear)
*/
float scaleVertexValues(int x, int z) {

	/*Posicao do vértice pretendido no buffer imageData*/
	int posicao=imageHeight*z+x;

	/*Converter a altura do vértice para a escala 0-30*/
	float y = (float) ((int) imageData[posicao]) * 30 / 255;

	//printf("ImageData[%d] : %u\n",posicao,imageData[posicao]);
	//printf("Altura: %f\n", y);

	return y;
}

/**
 * Funcao responsavel por gerar as coordenadas de todos os vértices da grelha e
 * carrega-los para o VBO para depois se poder desenhar
 * Aqui tem se em atencao a altura e largura da imagem previamente carregada e ainda
 * o valor das alturas dos vertices mediante o valor dos pixeis, que foi sujeito a um 
 * fator de escala através da chamada da funcao scaleVertexValues
*/
void buildTerrain() {

	// array for vertices
	vector<float> vertexB;

	// Coordenadas iniciais 
	float initialValue = -((float)(imageHeight - 1) / 2);
	//printf("InitialValue: %f\n", initialValue);
	float x = initialValue, z = -((float)(imageWidth - 1) / 2);
	/*Coordenada y(ou altura) do vértice*/
	float y = 0;

	for (int i = 0; i < imageWidth-1; i++) {
		for (int j = 0; j < imageHeight; j++) {
			y = scaleVertexValues(i,j);
			// Vertice de cima
			vertexB.push_back(x);
			vertexB.push_back(y);
			vertexB.push_back(z);
			y = scaleVertexValues(i+1, j);
			// Vertice de baixo
			vertexB.push_back(x);
			vertexB.push_back(y);
			vertexB.push_back(z+1);
			x++;
		}
		z++;
		x = initialValue;
	}

	//int sizeA = vertexB.size();
	//// Imprimir as coordenadas
	//for (int i = 0; i < sizeA; i++) {
	//	printf("[Posicao %d] -> %f\n", i+1, vertexB.at(i));
	//}

	// criar o VBO

	glGenBuffers(1, buffers);

	// copiar o vector para a memória gráfica

	glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
	glBufferData(
		GL_ARRAY_BUFFER, // tipo do buffer, só é relevante na altura do desenho
		sizeof(float) * vertexB.size(), // tamanho do vector em bytes
		vertexB.data(), // os dados do array associado ao vector
		GL_STATIC_DRAW); // indicativo da utilização (estático e para desenho)

}

/**
 * Inicializacao dos componentes necessarios 
*/
void init() {

    ilInit();

// 	Load the height map "terreno.jpg"

	unsigned int t;
	ilGenImages(1, &t);
	ilBindImage(t);
	// terreno.jpg is the image containing our height map
	ilLoadImage((ILstring)"terreno.jpg");
	// convert the image to single channel per pixel
	// with values ranging between 0 and 255
	ilConvertImage(IL_LUMINANCE, IL_UNSIGNED_BYTE);
	// important: check tw and th values
	// both should be equal to 256
	// if not there was an error loading the image
	// most likely the image could not be found
	imageWidth = ilGetInteger(IL_IMAGE_WIDTH);
	printf("[ImageWidth] -> %d", imageWidth);
	imageHeight = ilGetInteger(IL_IMAGE_HEIGHT);
	// imageData is a LINEAR array with the pixel values
	imageData = ilGetData();

// 	Build the vertex arrays

	buildTerrain();

// 	OpenGL settings
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);
	glEnableClientState(GL_VERTEX_ARRAY);
}


int main(int argc, char **argv) {

// init GLUT and the window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH|GLUT_DOUBLE|GLUT_RGBA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(320,320);
	glutCreateWindow("CG@DI-UM");
		
// Required callback registry 
	glutDisplayFunc(renderScene);
	glutIdleFunc(renderScene);
	glutReshapeFunc(changeSize);

// Callback registration for keyboard processing
	glutKeyboardFunc(processKeys);
	glutMouseFunc(processMouseButtons);
	glutMotionFunc(processMouseMotion);

#ifndef __APPLE__
	glewInit();
#endif

	init();	

// enter GLUT's main cycle
	glutMainLoop();
	
	return 0;
}

