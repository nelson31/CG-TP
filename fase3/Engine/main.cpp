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
const char* FILE_XML_NAME = "testeXML.xml";

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

/**
Variável que guarda o conjunto de pontos 
de cada operação caso ela se trata de 
uma translação dinâmica
*/
float**** points;

/**
Variável que guarda o número de catmoll-rom 
points em cada operação de cada group
*/
float** numPoints;

/**
Variáveis que possibilitam a adição do efeito 
do tempo nas trajetorias dos objetos que apresentam 
translações dinâmicas
*/
float oldTimeSinceStart = 0;
float timeSinceStart;
float deltaTime;

void buildRotMatrix(float* x, float* y, float* z, float* m) {

	m[0] = x[0]; m[1] = x[1]; m[2] = x[2]; m[3] = 0;
	m[4] = y[0]; m[5] = y[1]; m[6] = y[2]; m[7] = 0;
	m[8] = z[0]; m[9] = z[1]; m[10] = z[2]; m[11] = 0;
	m[12] = 0; m[13] = 0; m[14] = 0; m[15] = 1;
}


void cross(float* a, float* b, float* res) {

	res[0] = a[1] * b[2] - a[2] * b[1];
	res[1] = a[2] * b[0] - a[0] * b[2];
	res[2] = a[0] * b[1] - a[1] * b[0];
}


void normalize(float* a) {

	float l = sqrt(a[0] * a[0] + a[1] * a[1] + a[2] * a[2]);
	a[0] = a[0] / l;
	a[1] = a[1] / l;
	a[2] = a[2] / l;
}


float length(float* v) {

	float res = sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
	return res;

}

void multMatrixVector(float* m, float* v, float* res) {

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


void getCatmullRomPoint(float t, float* p0, float* p1, float* p2, float* p3, float* pos, float* deriv) {

	// catmull-rom matrix
	float m[4][4] = { {-0.5f,  1.5f, -1.5f,  0.5f},
						{ 1.0f, -2.5f,  2.0f, -0.5f},
						{-0.5f,  0.0f,  0.5f,  0.0f},
						{ 0.0f,  1.0f,  0.0f,  0.0f} };
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
			pos[i] += (float)pow((double)t, (double)3 - j) * a[j];
		}

		/* compute deriv = T' * A */
		deriv[i] = computeDeriv(t, a);
	}
}


// given  global t, returns the point in the curve
void getGlobalCatmullRomPoint(int group, int opIndice, float gt, float* pos, float* deriv) {

	gt /= params[group][opIndice][3]/10.0f;
	int point_count = numPoints[group][opIndice];
	float t = gt * (float)point_count; // this is the real global t
	int index = floor(t);  // which segment
	t = t - index; // where within  the segment

	// indices store the points
	int indices[4];
	indices[0] = (index + point_count - 1) % point_count;
	indices[1] = (indices[0] + 1) % point_count;
	indices[2] = (indices[1] + 1) % point_count;
	indices[3] = (indices[2] + 1) % point_count;

	getCatmullRomPoint(t, points[group][opIndice][indices[0]], points[group][opIndice][indices[1]], points[group][opIndice][indices[2]], points[group][opIndice][indices[3]], pos, deriv);
}

/**
Método que calcula o resto da divisão inteira
*/
float root(float x, float value) {

	while (x > value)
		x -= value;
	return x;
}

/**
Função que dado um tempo global calcula o ângulo de 
rotação para um group que apresente rotação dinâmica
*/
float getCurrentAngle(float gt, int group, int opIndice) {
	
	gt /= params[group][opIndice][0] / 10.0f;
	return root(gt*360.0f,360.0f);
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
Função que dado um elemento de um ficheiro xml
do tipo dynamic translate retorna uma lista vértices
com todos os pontos associados à catmoll-rom curve
associados a esse pontos
*/
ListVertices getPoints(TiXmlElement* element) {

	ListVertices lv = newListVertices();
	TiXmlNode* ele = NULL;
	TiXmlElement* subelement;
	for (int j = 0; ((ele = element->IterateChildren(ele))); j++) {
;		subelement = ele->ToElement();
		addVertice(lv, atof(subelement->Attribute("X")), atof(subelement->Attribute("Y")), atof(subelement->Attribute("Z")));
	}
	return lv;
}

/**
Função que adiciona um translate 
ao grupo g. Retorna a lista de vértices 
correspondente ao translate se for dinamica.
*/
ListVertices processaTranslate(Group g, char* tagName, float x, float y, float z, float time, TiXmlElement* element) {

	ListVertices lv;
	float param[_MAX_PARAM_TRANSLATE];
	param[0] = x;
	param[1] = y;
	param[2] = z;
	param[3] = time;
	if (time > 0) {
		addDynamicTranslation(g, tagName, param, (lv = getPoints(element)));
	}
	/* Adicionamos a operação ao grupo */
	else {
		addOperacao(g, tagName, (float*)param);
		lv = NULL;
	}
	return lv;
}

/**
Função que adiciona um rotate 
ao grupo g
*/
void processaRotate(Group g, char* tagName, float atrib, float axisX, 
	float axisY, float axisZ, float type) {

	float param[5];
	param[0] = atrib;
	param[1] = axisX;
	param[2] = axisY;
	param[3] = axisZ;
	param[4] = type;
	/* Adicionamos a operação ao grupo */
	addOperacao(g, tagName, (float*)param);
}

/**
Função que adiciona um color 
ao grupo g
*/
void processaColor(Group g, char* tagName, float r, float gr, float b) {

	float param[3];
	param[0] = r;
	param[1] = gr;
	param[2] = b;
	/* Adicionamos a operação ao group */
	addOperacao(g, tagName, (float*)param);
}

/**
Função que adiciona um scale 
ao group g
*/
void processaScale(Group g, char* tagName, float x, float y, float z) {

	float param[3];
	param[0] = x;
	param[1] = y;
	param[2] = z;
	/* Adicionamos a operação ao group */
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
void addOperacoes(Group g, char** opNames, float** params, ListVertices* catmollpoints, int size) {

	/* Para cada uma das operações, adicionámo-la ao group */
	for (int i = 0; i < size; i++) {
		//printf("Operacao: %s\n",opNames[i]);
		if (catmollpoints[i] == NULL)
			addOperacao(g, opNames[i], params[i]);
		else
			addDynamicTranslation(g, opNames[i], params[i], catmollpoints[i]);
		//printf("Parametros: (%f,%f,%f,%f,%f)\n", params[i][0], params[i][1], params[i][2], params[i][3], params[i][4]);
	}
}

/**
Método que tendo em conta 
a tag toma uma ação
*/
void processaGroup(TiXmlElement* element, char** opNames, float** params, ListVertices* catmollpoints, int numOperacoes) {

	float** localParams = (float**)malloc(sizeof(float*) * numOperacoes);
	char** localOpNames = (char**)malloc(sizeof(char*) * numOperacoes);
	ListVertices* localCatmollPoints = (ListVertices*)malloc(sizeof(ListVertices) * numOperacoes);
	for (int i = 0; i < numOperacoes; i++) {
		localOpNames[i] = strdup(opNames[i]);
		int numParams = 0;
		switch (opNames[i][0]) {

			case 't':
				numParams = 4;
				break;
			case 'c':
				numParams = 3;
				break;
			case 'r':
				numParams = 5;
				break;
			default:
				break;
		}
		localParams[i] = (float*)malloc(sizeof(float)*numParams);
		localCatmollPoints[i] = catmollpoints[i];
		for (int j = 0; j < numParams; j++) {
			localParams[i][j] = params[i][j];
		}
		//printf("Novo array criado: (%f,%f,%f,%f,%f)\n", localParams[i][0], localParams[i][1], localParams[i][2], localParams[i][3], localParams[i][4]);
	}
	char** auxc;
	float** auxf;
	/* Vamos buscar o numero de operações 
	nos arrays opNames e params para o podermos 
	atualizar no contexto de cada chamada recursiva */
	int atualNumOps = numOperacoes;
	float angle=0, x=0, y=0, z=0, time = 0;
	/* Criamos um novo grupo */
	Group g = newGroup();
	/* Se tivermos operações provenientes
	de um group pai adicionamos essas mesmas
	operações ao grupo filho */
	if (numOperacoes > 0) {
		addOperacoes(g, localOpNames, localParams, localCatmollPoints, numOperacoes);
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
			processaGroup(subelement, localOpNames, localParams, localCatmollPoints, atualNumOps);
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
					(subelement->Attribute("time") == NULL) ? time = -1 : time = atof(subelement->Attribute("time"));
					localParams[atualNumOps - 1] = (float*)malloc(sizeof(float) * 4);
					localParams[atualNumOps - 1][0] = x;
					localParams[atualNumOps - 1][1] = y;
					localParams[atualNumOps - 1][2] = z;
					localParams[atualNumOps - 1][3] = time;
					localCatmollPoints = (ListVertices*)realloc(localCatmollPoints, sizeof(ListVertices) * atualNumOps);
					localCatmollPoints[atualNumOps - 1] = processaTranslate(g, tagNameSubElem, x, y, z, time, subelement);
					break;

				/* Estamos perante uma tag 
				de rotação */
				case 'r':
					auxc = (char**)realloc(localOpNames, sizeof(char*) * (++atualNumOps));
					localOpNames = auxc;
					localOpNames[atualNumOps - 1] = strdup("rotate");
					auxf = (float**)realloc(localParams, sizeof(float*) * (atualNumOps));
					localParams = auxf;
					(subelement->Attribute("angle") == NULL) ? angle = -1 : angle = atof(subelement->Attribute("angle"));
					(subelement->Attribute("axisX") == NULL) ? x = 0 : x = atof(subelement->Attribute("axisX"));
					(subelement->Attribute("axisY") == NULL) ? y = 0 : y = atof(subelement->Attribute("axisY"));
					(subelement->Attribute("axisZ") == NULL) ? z = 0 : z = atof(subelement->Attribute("axisZ"));
					(subelement->Attribute("time") == NULL) ? time = -1 : time = atof(subelement->Attribute("time"));
					localParams[atualNumOps - 1] = (float*)malloc(sizeof(float) * 5);
					if (angle > 0) {
						localParams[atualNumOps - 1][0] = angle;
						localParams[atualNumOps - 1][4] = _STATIC;
					}
					else{
						localParams[atualNumOps - 1][0] = time;
						localParams[atualNumOps - 1][4] = _DYNAMIC;
					}
					localParams[atualNumOps - 1][1] = x;
					localParams[atualNumOps - 1][2] = y;
					localParams[atualNumOps - 1][3] = z;
					/* Esta operação não possui catmoll-points logo colocamos a null */
					localCatmollPoints = (ListVertices*)realloc(localCatmollPoints, sizeof(ListVertices) * atualNumOps);
					localCatmollPoints[atualNumOps - 1] = NULL;
					//printf("Reconheci (%f,%f,%f,%f,%f)\n", localParams[atualNumOps - 1][0], localParams[atualNumOps - 1][1], localParams[atualNumOps - 1][2], localParams[atualNumOps - 1][3], localParams[atualNumOps - 1][4]);
					processaRotate(g, tagNameSubElem, localParams[atualNumOps-1][0], x, y, z, localParams[atualNumOps-1][4]);
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
					localCatmollPoints = (ListVertices*)realloc(localCatmollPoints, sizeof(ListVertices) * atualNumOps);
					localCatmollPoints[atualNumOps - 1] = NULL;
					processaColor(g, tagNameSubElem, x, y, z);
					break;

				case 's':
					auxc = (char**)realloc(localOpNames, sizeof(char*) * (++atualNumOps));
					localOpNames = auxc;
					localOpNames[atualNumOps - 1] = strdup("scale");
					auxf = (float**)realloc(localParams, sizeof(float*) * (atualNumOps));
					localParams = auxf;
					(subelement->Attribute("X") == NULL) ? x = 0 : x = atof(subelement->Attribute("X"));
					(subelement->Attribute("Y") == NULL) ? y = 0 : y = atof(subelement->Attribute("Y"));
					(subelement->Attribute("Z") == NULL) ? z = 0 : z = atof(subelement->Attribute("Z"));
					localParams[atualNumOps - 1] = (float*)malloc(sizeof(float) * 3);
					localParams[atualNumOps - 1][0] = x;
					localParams[atualNumOps - 1][1] = y;
					localParams[atualNumOps - 1][2] = z;
					localCatmollPoints = (ListVertices*)realloc(localCatmollPoints, sizeof(ListVertices) * atualNumOps);
					localCatmollPoints[atualNumOps - 1] = NULL;
					processaScale(g, tagNameSubElem, x, y, z);
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
		processaGroup(ele->ToElement(),NULL,NULL,NULL,0);
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

void printArrays() {

	for (int i = 0; i < size; i++) {
		for (int j = 0; j < numOps[i]; j++) {
			printf("[Grupo %d e operacao numero %d]: %s -> %f ; %f ; %f ; %f ; %f \n", i, j, names[i][j], params[i][j][0], params[i][j][1], params[i][j][2], params[i][j][3], params[i][j][4]);
			printf("\tCatmoll-Rom Points: \n");
			if (points[i][j] != NULL) {
				for (int k = 0; k < numPoints[i][j]; k++) {
					printf("\t\t(%f, %f, %f)\n", points[i][j][k][0], points[i][j][k][1], points[i][j][k][2]);
				}
			}
		}
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
	/* Vamos buscar os catmoll-points */
	ListVertices** cat = getCatmollPoints(lg);
	/* Inicializamos ambos os array responsáveis 
	por lidar pelo catmoll-rom points */
	points = (float****)malloc(sizeof(float***) * size);
	numPoints = (float**)malloc(sizeof(float*) * size);
	int numOp, numCatPoints; Vertice v;
	/* Vamos preencher o array points */
	for (int i = 0; i < size; i++) {
		numOp = numOps[i];
		points[i] = (float***)malloc(sizeof(float**) * numOp);
		numPoints[i] = (float*)malloc(sizeof(float) * numOp);
		/* Percorremos cada operação dentro de cada group */
		for (int j = 0; j < numOp; j++) {
			if (cat[i][j] != NULL) {
				numCatPoints = numVertices(cat[i][j]);
				points[i][j] = (float**)malloc(sizeof(float*) * (numPoints[i][j] = numCatPoints));
				for (int k = 0; k<numCatPoints; k++) {
					v = nextV(cat[i][j]);
					points[i][j][k] = (float*)malloc(sizeof(float) * 3);
					points[i][j][k][0] = getX(v);
					points[i][j][k][1] = getY(v);
					points[i][j][k][2] = getZ(v);
				}
				atualizaPointer(cat[i][j]);
			}
			else {
				points[i][j] = NULL;
				numPoints[i][j] = 0;
			}
		}
	}
	//printArrays();
	//printf("Preparation Done\n");
}

/**
Função que trata das transformações
geométricas para um determinado group
tendo em conta o instante. Para além disso 
esta função desenha o respetivo group
t
*/
void reposicionaModels(float gt) {

	float position[3], deriv[3];
	/* Percorremos cada um dos groups */
	for (int i = 0; i < size; i++) {
		glPushMatrix();
		/* Percorremos cada uma das operações de cada group */
		for (int j = 0; j < numOps[i]; j++) {
			switch (names[i][j][0]) {

			case 't':
				/* Caso estejamos perante uma translação estática fazemos um simples translate */
				if (params[i][j][3] == -1)
					glTranslatef(params[i][j][0], params[i][j][1], params[i][j][2]);
				else {
					getGlobalCatmullRomPoint(i, j, gt, position, deriv);
					glTranslatef(position[0], position[1], position[2]);
				}
				break;

			case 'r':
				if (params[i][j][4] == _STATIC)
					glRotatef(params[i][j][0], params[i][j][1], params[i][j][2], params[i][j][3]);
				else
					glRotatef(getCurrentAngle(gt,i,j), params[i][j][1], params[i][j][2], params[i][j][3]);
				break;

			case 'c':
				glColor3f(params[i][j][0], params[i][j][1], params[i][j][2]);
				break;

			default:
				break;
			}
		}
		/* Desenhamos os respetivos vértices */
		glBindBuffer(GL_ARRAY_BUFFER, vertices[i]);
		glVertexPointer(3, GL_FLOAT, 0, 0);
		glDrawArrays(GL_TRIANGLES, 0, numVerticess[i]);

		/* Fazemos pop da matrix */
		glPopMatrix();
	}
}

/**
Função que será posta a correr dentro 
do ciclo do glut
*/
void renderScene(void) {

	timeSinceStart = glutGet(GLUT_ELAPSED_TIME);
	deltaTime = timeSinceStart - oldTimeSinceStart;
	oldTimeSinceStart = timeSinceStart;
	static float a = 0;
	// clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// set the camera
	glLoadIdentity();
	gluLookAt(varx, vary, varz, // camera position
		0.0, 0.0, 0.0,  // look at point
		0.0f, 1.0f, 0.0f);  // “up vector” (0.0f, 1.0f, 0.0f)

	// Scene Design
	reposicionaModels(a);

	//printf("%f\n", a);

	// End of frame
	glutSwapBuffers();

	a += 0.0001*deltaTime;
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

// init GLUT and the window
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DEPTH|GLUT_DOUBLE|GLUT_RGBA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(800,800);
	glutCreateWindow("CG@TP@Fase3");

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
	loadFile();

	//printOpsLG(lg);

	size = numGroups(lg);

	prepareScene();

// enter GLUT's main cycle
	glutMainLoop();

	//printf("I'll return\n");
	return 0;
}
