#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <stdlib.h>
#include <GL/glut.h>
#endif

#define _USE_MATH_DEFINES
#include <math.h>
#include <stdio.h>
#include <string.h>

#include "groupImpl.h"
#include "ListGroups.h"
#include "vertice.h"
#include "tinyxml.h"

using std::vector;

/*Nome do Ficheiro XML*/
const char* FILE_XML_NAME = "SistemaSolar.xml";

// VARIAVEIS GLOBAIS
float varx = -1000, vary = 250, varz = 0;

// Camera 
float raio = 1000, alfa = -90, beta = 20;

// Ultimo x e y lidos do ecra
int lastX, lastY;

/**
Conjuntos de grupos a serem 
desenhados
*/
ListGroups lg;

/**
Variável que nos diz o número de groups 
existente na variável ListGroups
*/
int size;

/**
Array que guarda o número de operações
de cada group indexado pelo índice 
tendo em conta o vbo indicado
*/
int* numOps;

/**
Variáveis usadas para desenhar o 
conteudo da ListGroups
*/
GLuint* vertices, *numVerticess;

/**
Variáveis para ir buscar as
transformações de todos os groups
na ListGroups
*/
float*** params;
char*** names;


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
	gluPerspective(45.0f ,ratio, 1.0f ,2000.0f);

	// return to the model view matrix mode
	glMatrixMode(GL_MODELVIEW);
}


void sphericalToCartesian() {
	varx = raio * sin(alfa * 3.14 / 180.0) * cos(beta * 3.14 / 180.0);
	varz = raio * cos(alfa * 3.14 / 180.0) * cos(beta * 3.14 / 180.0);
	vary = raio * sin(beta * 3.14 / 180.0);
}

/**
Função que adiciona um translate 
ao grupo g
*/
void processaTranslate(Group g, char* tagName, float x, float y, float z) {

	float param[3];
	param[0] = x;
	param[1] = y;
	param[2] = z;
	/* Adicionamos a operação ao grupo */
	addOperacao(g, tagName, (float*)param);
}

/**
Função que adiciona um rotate 
ao grupo g
*/
void processaRotate(Group g, char* tagName, float angle, float axisX, 
	float axisY, float axisZ) {

	float param[4];
	param[0] = angle;
	param[1] = axisX;
	param[2] = axisY;
	param[3] = axisZ;
	/* Adicionamos a operação ao grupo */
	addOperacao(g, tagName, (float*)param);
}

/**
Método que adiciona um modelo ao grupo 
ao qual é passado por parâmetro para 
ser desenhado
*/
void processaModels(TiXmlElement* element, Group g) {
	
	char* filename;

	int vertices;

	float x, y, z;
	
	TiXmlNode* ele = NULL;

	TiXmlElement* subelement;
	/* Antes de mais vamos iterar */
	for (int j = 0; ((ele = element->IterateChildren(ele))); j++) {
		subelement = ele->ToElement();
		char* tagName = (char*)subelement->Value();
		if (!strcmp(tagName, "model")) {
			/* Vamos buscar o nome do ficheiro */
			filename = (char*)subelement->Attribute("file");
			/* File pointer que será utilzado
			para ler dos ficheiros gerados
			com as respetivas figuras */
			FILE* fp = fopen(filename, "r");
			/* Lemos o tamanho inicial */
			fscanf(fp, "%d\n", &vertices);
			/* Lemos os valores do ficheiro em questão e
			adicionamos ao group em questão */
			while (fscanf(fp, "%f %f %f\n", &x, &y, &z) != -1) {
				/* Adicionamos o vértice ao
				respetivo grupo */
				addVertice(g, x, y, z);
			}
			fclose(fp);
			printf("File %s charged successfully!\n", filename);
		}
	}
}

/**
Método que permite adicionar as operações 
especificadas nos arrays ao grupo 
em questão
*/
void addOperacoes(Group g, char** opNames, float** params, int size) {

	/* Para cada uma das operações, adicionámo-la ao group */
	for (int i = 0; i < size; i++)
		addOperacao(g, opNames[i], params[i]);
}

/**
Método que tendo em conta 
a tag toma uma ação
*/
void processaGroup(TiXmlElement* element, char** opNames, float** params, int numOperacoes) {

	float** localParams = (float**)malloc(sizeof(float*) * numOperacoes);
	char** localOpNames = (char**)malloc(sizeof(char*) * numOperacoes);
	for (int i = 0; i < numOperacoes; i++) {
		localOpNames[i] = strdup(opNames[i]);
		int numParams = 0;
		switch (opNames[i][0]) {

			case 't':
				numParams = 3;
				break;
			case 'c':
				numParams = 3;
				break;
			case 'r':
				numParams = 4;
				break;
			default:
				break;
		}
		localParams[i] = (float*)malloc(sizeof(float)*numParams);
		for (int j = 0; j < numParams; j++) {
			localParams[i] = params[i];
		}
	}
	char** auxc;
	float** auxf;
	/* Vamos buscar o numero de operações 
	nos arrays opNames e params para o podermos 
	atualizar no contexto de cada chamada recursiva */
	int atualNumOps = numOperacoes;
	float angle=0, x=0, y=0, z=0;
	/* Criamos um novo grupo */
	Group g = newGroup();
	/* Se tivermos operações provenientes
	de um group pai adicionamos essas mesmas
	operações ao grupo filho */
	if (numOperacoes > 0) {
		addOperacoes(g, localOpNames, localParams, numOperacoes);
	}
	char* tagName = (char*)element->Value(), *tagNameSubElem;
	TiXmlNode* ele = NULL;
	//printf("<%s>\n", element->Value());
	/* Retirar os nomes do XML e carregar os
	ficheiros respetivos para as devidas
	estruturas de dados */
	for (int j = 0; ((ele = element->IterateChildren(ele))); j++) {
		/* Elemento sobre o qual estamos a iterar */
		TiXmlElement* subelement = ele->ToElement();
		//printf("<%s>\n", subelement->Value());
		/* Vamos buscar o nome da tag desse element */
		tagNameSubElem = (char*)subelement->Value();
		/* Se o filho for group chamamos 
		a função recursivamente */
		if (!strcmp(tagNameSubElem, "group")) {
			/* Processamos o subgrupo */
			processaGroup(subelement, localOpNames, localParams, atualNumOps);
		}
		/* Se não for group acrescentamos 
		informação à lista de groups */
		else {
			switch (tagNameSubElem[0]) {

				/* Estamos perante uma tag 
				de translação */
				case 't':
					auxc = (char**)realloc(localOpNames, sizeof(char*) * (++atualNumOps));
					localOpNames = auxc;
					localOpNames[atualNumOps - 1] = strdup("translate");
					auxf = (float**)realloc(localParams, sizeof(float*) * (atualNumOps));
					localParams = auxf;
					(subelement->Attribute("X") == NULL) ? x = 0 : x = atof(subelement->Attribute("X"));
					(subelement->Attribute("Y") == NULL) ? y = 0 : y = atof(subelement->Attribute("Y"));
					(subelement->Attribute("Z") == NULL) ? z = 0 : z = atof(subelement->Attribute("Z"));
					localParams[atualNumOps - 1] = (float*)malloc(sizeof(float) * 3);
					localParams[atualNumOps - 1][0] = x;
					localParams[atualNumOps - 1][1] = y;
					localParams[atualNumOps - 1][2] = z;
					processaTranslate(g, tagNameSubElem, x, y, z);
					break;

				/* Estamos perante uma tag 
				de rotação */
				case 'r':
					auxc = (char**)realloc(localOpNames, sizeof(char*) * (++atualNumOps));
					localOpNames = auxc;
					localOpNames[atualNumOps - 1] = strdup("rotate");
					auxf = (float**)realloc(localParams, sizeof(float*) * (atualNumOps));
					localParams = auxf;
					(subelement->Attribute("angle") == NULL) ? angle = 0 : angle = atof(subelement->Attribute("angle"));
					(subelement->Attribute("axisX") == NULL) ? x = 0 : x = atof(subelement->Attribute("axisX"));
					(subelement->Attribute("axisY") == NULL) ? y = 0 : y = atof(subelement->Attribute("axisY"));
					(subelement->Attribute("axisZ") == NULL) ? z = 0 : z = atof(subelement->Attribute("axisZ"));
					localParams[atualNumOps - 1] = (float*)malloc(sizeof(float) * 4);
					localParams[atualNumOps - 1][0] = angle;
					localParams[atualNumOps - 1][1] = x;
					localParams[atualNumOps - 1][2] = y;
					localParams[atualNumOps - 1][3] = z;
					processaRotate(g, tagNameSubElem, angle, x, y, z);
					break;

				/* Estamos perante uma tag
				de color */
				case 'c':
					auxc = (char**)realloc(localOpNames, sizeof(char*) * (++atualNumOps));
					localOpNames = auxc;
					localOpNames[atualNumOps - 1] = strdup("color");
					auxf = (float**)realloc(localParams, sizeof(float*) * (atualNumOps));
					localParams = auxf;
					(subelement->Attribute("R") == NULL) ? x = 0 : x = atof(subelement->Attribute("R"));
					(subelement->Attribute("G") == NULL) ? y = 0 : y = atof(subelement->Attribute("G"));
					(subelement->Attribute("B") == NULL) ? z = 0 : z = atof(subelement->Attribute("B"));
					localParams[atualNumOps - 1] = (float*)malloc(sizeof(float) * 3);
					localParams[atualNumOps - 1][0] = x;
					localParams[atualNumOps - 1][1] = y;
					localParams[atualNumOps - 1][2] = z;
					processaTranslate(g, tagNameSubElem, x, y, z);
					break;

				/* Estamos perante uma tag 
				models */
				case 'm':
					processaModels(subelement, g);
					break;

				default:
					break;

			}
		}
	}
	/* Adicionamos o grupo atual à
	lista para ser desenhado */
	addGroup(lg, g);
	//printf("\t>> Group added successfully!\n");
	free(localParams);
}


/**
 * Funcao que carrega o nome do ficheiro que se pretende reproduzir a partir do ficheiro XML
 * SistemaSolar.xml. Para isso recorreu-se ao uso da biblioteca de funcoes C++ 'TinyXML' de modo a
 * se efetuar o parse do ficheiro xml para se retirar o nome do ficheiro que contem as coordenadas
 * dos vários vértices que compoe a figura.
*/
void loadFile() {

	/* Inicializamos a 
	lista de groups */
	lg = newListGroups();

	TiXmlDocument doc;
	/* Carregamos o ficheiro xml 
	para a variável acima */
	doc.LoadFile(FILE_XML_NAME);

	TiXmlHandle docH(&doc);
	/* Variável para iterar as 
	tags do elemento */
	TiXmlNode* ele = NULL;

	/* Lemos o elemento scene */
	TiXmlElement* element = doc.FirstChildElement("scene");
	ele = NULL;
	/*
	Retirar os nomes do XML e carregar os 
	ficheiros respetivos para as devidas 
	estruturas de dados
	*/
	for (int j = 0; ((ele = element->IterateChildren(ele))); j++) {
		processaGroup(ele->ToElement(),NULL,NULL,0);
	}
}

/**
Método que desenha a cena tendo em conta 
a ListGroups em vigor. É de notar que para 
cada grupo é preparado um vbo diferente
*/
void prepareData() {

	int groups = numGroups(lg);
	vertices = (GLuint*)malloc(sizeof(GLuint)*groups);
	numVerticess = (GLuint*)malloc(sizeof(GLuint) * groups);
	int* sizes=(int*)malloc(sizeof(int)*groups);
	/* Sizes é parâmetro de saida */
	vector<float>** vec = getVectors(lg, &sizes);
	/* Criamos os vbo's */
	glGenBuffers(groups, vertices);
	/* Passamos os vetores para 
	a memória gráfica */
	for (int i = 0; i < groups; i++) {
		//printf("Nº de vértices do group %d\n", i);
		numVerticess[i] = sizes[i];
		//printf("N vertices do modelo %d: %d\n", i, numVerticess[i]);
		glBindBuffer(GL_ARRAY_BUFFER, vertices[i]);
		glBufferData(
			GL_ARRAY_BUFFER, // tipo do buffer, só é relevante na altura do desenho
			sizeof(float)*(*(vec[i])).size(), // tamanho do vector em bytes
			(*(vec[i])).data(), // os dados do array associado ao vector
			GL_STATIC_DRAW); // indicativo da utilização (estático e para desenho)
	}
}

/**
Método que desenha o conteudo pedido, 
inicializando previamente os vbo's
*/
void prepareScene(){

	/* Preparamos os vbo's 
	para serem desenhados */
	prepareData();
	names = (char***)malloc(sizeof(char**) * size);
	params = (float***)malloc(sizeof(float**) * size);
	/* Vamos buscar as transformações 
	de cada group */
	numOps = getOpsParams(lg, &names, &params);
	/*for (int i = 0; i < size; i++) {
		for (int j = 0; j < numOps[i]; j++) {
			printf("[Grupo %d e operacao numero %d]: %s -> %f ; %f ; %f \n", i, j, names[i][j], params[i][j][0], params[i][j][1], params[i][j][2]);
		}
	}*/
	printf("Preparation Done\n");
}

/**
Método que desenha a scene
*/
void drawScene() {

	/* Percorremos todos 
	os groups */
	for (int i = 0; i < size; i++) {
		/* Fazemos push da matriz 
		de transformações */
		if(numOps[i]>0) glPushMatrix();
		/* Percorremos as operações 
		dentro de cada group */
		for (int j = 0; j < numOps[i]; j++) {
			switch (names[i][j][0]) {
				/* Estamos perante um translate */
				case 't':
					glTranslatef(params[i][j][0], params[i][j][1], params[i][j][2]);
					break;

				case 'r':
					glRotatef(params[i][j][0], params[i][j][1], params[i][j][2], params[i][j][3]);
					break;

				case 'c':
					glColor3f(params[i][j][0], params[i][j][1], params[i][j][2]);
					break;

				default :
					break;
			}
		}
		/* Desenhamos os respetivos vértices */
		glBindBuffer(GL_ARRAY_BUFFER, vertices[i]);
		glVertexPointer(3, GL_FLOAT, 0, 0);
		glDrawArrays(GL_TRIANGLES, 0, numVerticess[i]);

		/* Fazemos pop da matrix */
		if (numOps[i] > 0) glPopMatrix();
	}
}

void renderScene(void) {

	// clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// set the camera
	glLoadIdentity();
	gluLookAt(varx, vary, varz, // camera position
		0.0, 0.0, 0.0,  // look at point
		0.0f, 1.0f, 0.0f);  // “up vector” (0.0f, 1.0f, 0.0f)

	// Scene Design
	drawScene();

	// End of frame
	glutSwapBuffers();
}


void processKeys(unsigned char c, int xx, int yy) {

}


void processSpecialKeys(int key, int xx, int yy) {

	// Process special keys

	switch (key)
	{
	case GLUT_KEY_UP:
		beta += (float)1.0;
		if (beta > 85.0)
			beta = 85.0;
		break;
	case GLUT_KEY_DOWN:
		beta -= (float)1.0;
		if (beta < -85.0)
			beta = -85.0;
		break;
	case GLUT_KEY_LEFT:
		alfa -= (float)1.0;
		break;
	case GLUT_KEY_RIGHT:
		alfa += (float)1.0;
		break;
	case GLUT_KEY_PAGE_UP:
		raio -= 10;
		if (raio < 150)
			raio = 150;
		break;
	case GLUT_KEY_PAGE_DOWN:
		raio += 10;
		break;
	default:
		break;
	}

	sphericalToCartesian();

	glutPostRedisplay();
}


/* Cuidado que deve-se fazer o build novamente com o CMake 
 * Deve-se buscar os ficheiros gerados ao programa "Generator"
 * Deve-se atualizar o XML com o nome dos ficheiros acabados de gerar
*/
int main(int argc, char **argv) {

	loadFile();

	printOpsLG(lg);

	size = numGroups(lg);

// init GLUT and the window
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DEPTH|GLUT_DOUBLE|GLUT_RGBA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(800,800);
	glutCreateWindow("CG@TP@Fase1");

#ifndef __APPLE__
	glewInit();
#endif
	
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
	glEnableClientState(GL_VERTEX_ARRAY);

// Prepare the scene to draw
	prepareScene();

// enter GLUT's main cycle
	glutMainLoop();

	printf("I'll return\n");
	return 0;
}
