#ifdef __APPLE__

#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <stdlib.h>
#include <GL/glut.h>
#endif

#include <IL/il.h>

#define _USE_MATH_DEFINES
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <string>

#include "groupImpl.h"
#include "ListGroups.h"
#include "vertice.h"
#include "Model.h"
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
Vari�vel que nos diz o n�mero de groups 
existente na vari�vel ListGroups
*/
int size;

/**
Array que guarda o n�mero de opera��es
de cada group indexado pelo �ndice 
tendo em conta o vbo indicado
*/
int* numOps;

/**
Vari�veis usadas para desenhar o 
conteudo da ListGroups
*/
GLuint *vertices, *normals, *texCoord, *numVerticess, *vertPercursos;

/**
Vari�veis para ir buscar as
transforma��es de todos os groups
na ListGroups
*/
float*** params;
char*** names;

/**
Vari�vel que guarda o conjunto de pontos 
de cada opera��o caso ela se trata de 
uma transla��o din�mica
*/
float**** points;

/**
Vari�vel que guarda o n�mero de catmoll-rom 
points em cada opera��o de cada group
*/
float** numPoints;

/**
Vari�veis que possibilitam a adi��o do efeito 
do tempo nas trajetorias dos objetos que apresentam 
transla��es din�micas
*/
float oldTimeSinceStart = 0;
float timeSinceStart;
float deltaTime;

/**
Vari�vel que guarda os v�rtices de modo a 
desenhar os percursos de todos os objetos 
que apresentam transla��es din�micas
*/
vector<float>** percursos;

/**
Vari�vel que associa os percursos aos respetivos 
groups e opera��es, tendo em conta os indices
*/
float** groupOpToPercurso;

/**
Vari�vel que guarda o n�mero de transla��es 
din�micas em vigor na aplica��o
*/
int nPercursos = 0;

/**
Vari�vel que serve para ativar ou desativar as 
rotas dos modelos que possuem transla��es din�micas
*/
int showRotas = 1;

/**
Vari�veis que guardam os vetores que
representam os eixos do teapot
*/
float** X, **Y, **Z;

/**
Vari�vel que nos informa acerca do n�mero 
de models que possuem textura
*/
int textureModels = 0;

/**
Vari�vel que guarda as posi��es das luzes 
a serem colocadas pelo nosso engine
*/
vector<float>* lights;

/* Vari�vel que guarda o n�mero de 
luzes em vigor */
int numLights = 0;


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
Fun��o que trata da computa��o do c�lculo de uma
componente de derivada dada a respetiva linha da
matriz resultante da multiplica��o da matriz
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

	getCatmullRomPoint(t, points[group][opIndice][indices[0]], points[group][opIndice][indices[1]], 
		points[group][opIndice][indices[2]], points[group][opIndice][indices[3]], pos, deriv);
}

/**
Fun��o que retorna um ponto de um determinada 
rota n�o tendo em conta o tempo
*/
void getGlobalCatmullRomPointNoTime(int group, int opIndice, float gt, float* pos, float* deriv) {

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

	getCatmullRomPoint(t, points[group][opIndice][indices[0]], points[group][opIndice][indices[1]], 
		points[group][opIndice][indices[2]], points[group][opIndice][indices[3]], pos, deriv);
}

/**
M�todo que calcula o resto da divis�o inteira
*/
float root(float x, float value) {

	while (x > value)
		x -= value;
	return x;
}

/**
Fun��o que dado um tempo global calcula o �ngulo de 
rota��o para um group que apresente rota��o din�mica
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
Fun��o que dado um elemento de um ficheiro xml
do tipo dynamic translate retorna uma lista v�rtices
com todos os pontos associados � catmoll-rom curve
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
Fun��o que adiciona um translate 
ao grupo g. Retorna a lista de v�rtices 
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
	/* Adicionamos a opera��o ao grupo */
	else {
		addOperacao(g, tagName, (float*)param);
		lv = NULL;
	}
	return lv;
}

/**
Fun��o que adiciona um rotate 
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
	/* Adicionamos a opera��o ao grupo */
	addOperacao(g, tagName, (float*)param);
}

/**
Fun��o que adiciona um color 
ao grupo g
*/
void processaColor(Group g, char* tagName, float r, float gr, float b) {

	float param[3];
	param[0] = r;
	param[1] = gr;
	param[2] = b;
	/* Adicionamos a opera��o ao group */
	addOperacao(g, tagName, (float*)param);
}

/**
Fun��o que adiciona um scale 
ao group g
*/
void processaScale(Group g, char* tagName, float x, float y, float z) {

	float param[3];
	param[0] = x;
	param[1] = y;
	param[2] = z;
	/* Adicionamos a opera��o ao group */
	addOperacao(g, tagName, (float*)param);
}

/**
M�todo que permite carregar uma
textura para mem�ria gr�fica
*/
int loadTexture(const char* s) {

	unsigned int t, tw, th;
	unsigned char* texData;
	unsigned int texID;

	// Iniciar o DevIL
	ilInit();
	// Colocar a origem da textura no canto inferior esquerdo
	ilEnable(IL_ORIGIN_SET);
	ilOriginFunc(IL_ORIGIN_LOWER_LEFT);
	// Carregar a textura para a memoria
	ilGenImages(1, &t);
	ilBindImage(t);
	ilLoadImage((ILstring)s);
	tw = ilGetInteger(IL_IMAGE_WIDTH);
	th = ilGetInteger(IL_IMAGE_HEIGHT);
	// Assegurar que a textura se encontra em RGBA(Red,Green,Blue,Alpha)
	// com um byte (0-255) por componente
	ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
	texData = ilGetData();

	// Gerar a textura para a placa gr�fica
	glGenTextures(1, &texID);

	glBindTexture(GL_TEXTURE_2D, texID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	// Upload dos dados da imagem
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tw, th, 0, GL_RGBA, GL_UNSIGNED_BYTE, texData);
	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);

	return texID;
}

/**
M�todo que adiciona um modelo ao grupo 
ao qual � passado por par�metro para 
ser desenhado
*/
void processaModels(TiXmlElement* element, Group g) {
	
	char* filename;

	int vertices;

	/* Vari�veis auxiliares */
	float posx, posy, posz;
	float normx, normy, normz;
	float textx, texty;
	float diffuse[4], specular[4], emission[4], ambient[4];
	int shineness;
	
	TiXmlNode* ele = NULL;

	TiXmlElement* subelement;

	Model m;
	vector<float>* points;
	vector<float>* normals;
	vector<float>* textures;
	int id;
	/* Antes de mais vamos iterar */
	for (int j = 0; ((ele = element->IterateChildren(ele))); j++) {
		subelement = ele->ToElement();
		char* tagName = (char*)subelement->Value();
		if (!strcmp(tagName, "model")) {
			points = new vector<float>();
			textures = new vector<float>();
			normals = new vector<float>();
			/* Vamos buscar o nome do ficheiro */
			filename = (char*)subelement->Attribute("file");
			/* File pointer que ser� utilzado
			para ler dos ficheiros gerados
			com as respetivas figuras */
			FILE* fp = fopen(filename, "r");
			/* Lemos o tamanho inicial */
			fscanf(fp, "%d\n", &vertices);
			/* Lemos os valores do ficheiro em quest�o e
			adicionamos ao group em quest�o */
			while (fscanf(fp, "%f %f %f\n%f %f %f\n%f %f\n", &posx, &posy, &posz, &normx, &normy, &normz, &textx, &texty) != -1) {
				/* Adicionamos o v�rtice ao
				respetivo grupo */
				points->push_back(posx);
				points->push_back(posy);
				points->push_back(posz);
				/* Adicionamos o v�rtice que 
				corresponde � normal do respetivo ponto */
				normals->push_back(normx);
				normals->push_back(normy);
				normals->push_back(normz);
				/* Adicionamos o v�rtices que corresponde 
				� textura no respetivo ponto */
				textures->push_back(textx);
				textures->push_back(texty);
			}
			/* Verificamos se existe algum 
			ficheiro de textura. Caso exista carregamos a respetiva 
			imagem para mem�ria gr�fica e adicionamos ao group em 
			quest�o */
			if (subelement->Attribute("texture") != NULL) {
				id = loadTexture(subelement->Attribute("texture"));
				printf("Nova imagem de textura carregada: %s\n", subelement->Attribute("texture"));
				/* Carregamos a imagem da texture */
				m = newTextureModel(points, normals, id, textures);
				/* Incrementamos ao n�mero de models que 
				possuem textura */
				textureModels++;
			}
			else {
				(subelement->Attribute("diffR") == NULL) ? diffuse[0] = 0.0f : diffuse[0] = atof(subelement->Attribute("diffR"));
				(subelement->Attribute("diffG") == NULL) ? diffuse[1] = 0.0f : diffuse[1] = atof(subelement->Attribute("diffG"));
				(subelement->Attribute("diffB") == NULL) ? diffuse[2] = 0.0f : diffuse[2] = atof(subelement->Attribute("diffB"));
				diffuse[3] = 1.0f;

				(subelement->Attribute("specR") == NULL) ? specular[0] = 0.0f : specular[0] = atof(subelement->Attribute("specR"));
				(subelement->Attribute("specG") == NULL) ? specular[1] = 0.0f : specular[1] = atof(subelement->Attribute("specG"));
				(subelement->Attribute("specB") == NULL) ? specular[2] = 0.0f : specular[2] = atof(subelement->Attribute("specB"));
				specular[3] = 1.0f;

				(subelement->Attribute("ambR") == NULL) ? ambient[0] = 0.0f : ambient[0] = atof(subelement->Attribute("ambR"));
				(subelement->Attribute("ambG") == NULL) ? ambient[1] = 0.0f : ambient[1] = atof(subelement->Attribute("ambG"));
				(subelement->Attribute("ambB") == NULL) ? ambient[2] = 0.0f : ambient[2] = atof(subelement->Attribute("ambB"));
				ambient[3] = 1.0f;

				(subelement->Attribute("emisR") == NULL) ? emission[0] = 0.0f : emission[0] = atof(subelement->Attribute("emisR"));
				(subelement->Attribute("emisG") == NULL) ? emission[1] = 0.0f : emission[1] = atof(subelement->Attribute("emisG"));
				(subelement->Attribute("emisB") == NULL) ? emission[2] = 0.0f : emission[2] = atof(subelement->Attribute("emisB"));
				emission[3] = 1.0f;

				(subelement->Attribute("shineness") == NULL) ? shineness = 128 : shineness = atof(subelement->Attribute("shineness"));

				m = newMaterialModel(points, normals, diffuse, specular, ambient, emission, shineness);
			}
			/* Adicionamos o model ao group */
			addModel(g, m);

			fclose(fp);
			printf("File %s loaded successfully!\n", filename);
		}
	}
}

/**
M�todo que permite adicionar as opera��es 
especificadas nos arrays ao grupo 
em quest�o
*/
void addOperacoes(Group g, char** opNames, float** params, ListVertices* catmollpoints, int size) {

	/* Para cada uma das opera��es, adicion�mo-la ao group */
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
M�todo que tendo em conta 
a tag toma uma a��o
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
	/* Vamos buscar o numero de opera��es 
	nos arrays opNames e params para o podermos 
	atualizar no contexto de cada chamada recursiva */
	int atualNumOps = numOperacoes;
	float angle=0, x=0, y=0, z=0, time = 0;
	/* Criamos um novo grupo */
	Group g = newGroup();
	/* Se tivermos opera��es provenientes
	de um group pai adicionamos essas mesmas
	opera��es ao grupo filho */
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
		a fun��o recursivamente */
		if (!strcmp(tagNameSubElem, "group")) {
			/* Processamos o subgrupo */
			processaGroup(subelement, localOpNames, localParams, localCatmollPoints, atualNumOps);
		}
		/* Se n�o for group acrescentamos 
		informa��o � lista de groups */
		else {
			switch (tagNameSubElem[0]) {

				/* Estamos perante uma tag 
				de transla��o */
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
				de rota��o */
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
					/* Esta opera��o n�o possui catmoll-points logo colocamos a null */
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
	/* Adicionamos o grupo atual �
	lista para ser desenhado */
	addGroup(lg, g);
	//printf("\t>> Group added successfully!\n");
	free(localParams);
}

/**
Fun��o que processa a leitura das luzes
*/
void processaLights(TiXmlElement* element) {
	
	int count = 0;
	float posx, posy, posz;
	TiXmlElement* subelement;
	TiXmlNode* ele = NULL;
	/* Percorremos cada um dos subelementos 
	para ler as luzes */
	while ((ele = element->IterateChildren(ele)) && count<8) {
		subelement = ele->ToElement();
		(subelement->Attribute("posx") == NULL ? posx = 0 : posx = atof(subelement->Attribute("posx")));
		(subelement->Attribute("posy") == NULL ? posy = 0 : posy = atof(subelement->Attribute("posy")));
		(subelement->Attribute("posz") == NULL ? posz = 0 : posz = atof(subelement->Attribute("posz")));
		lights->push_back(posx);
		lights->push_back(posy);
		lights->push_back(posz);
	}
	numLights = count;
}


/**
 * Funcao que carrega o nome do ficheiro que se pretende reproduzir a partir do ficheiro XML
 * SistemaSolar.xml. Para isso recorreu-se ao uso da biblioteca de funcoes C++ 'TinyXML' de modo a
 * se efetuar o parse do ficheiro xml para se retirar o nome do ficheiro que contem as coordenadas
 * dos v�rios v�rtices que compoe a figura.
*/
void loadFile() {

	lights = new vector<float>();
	/* Inicializamos a 
	lista de groups */
	lg = newListGroups();

	TiXmlDocument doc;
	/* Carregamos o ficheiro xml 
	para a vari�vel acima */
	doc.LoadFile(FILE_XML_NAME);

	TiXmlHandle docH(&doc);
	/* Vari�vel para iterar as 
	tags do elemento */
	TiXmlNode* ele = NULL;

	/* Lemos o elemento scene */
	TiXmlElement* element = doc.FirstChildElement("scene");
	TiXmlElement* lights;
	ele = NULL;
	/*
	Retirar os nomes do XML e carregar os 
	ficheiros respetivos para as devidas 
	estruturas de dados
	*/
	for (int j = 0; ((ele = element->IterateChildren(ele))); j++) {
		/* Lemos o elemento "lights" */
		if(!strcmp(ele->ToElement()->Value(),"lights"))
			processaLights(ele->ToElement());
		processaGroup(ele->ToElement(),NULL,NULL,NULL,0);
	}
}

/**
Fun��o que define a cor para uma 
luz em espec�fico
*/
void defineColor(int num) {

	GLfloat dark[4] = { 0.2, 0.2, 0.2, 1.0 };
	GLfloat white[4] = { 1.0, 1.0, 1.0, 1.0 };
	switch (num) {
		case 0: glLightfv(GL_LIGHT0, GL_AMBIENT, dark);
				glLightfv(GL_LIGHT0, GL_DIFFUSE, white);
				glLightfv(GL_LIGHT0, GL_SPECULAR, white); break;
		case 1: glLightfv(GL_LIGHT1, GL_AMBIENT, dark);
				glLightfv(GL_LIGHT1, GL_DIFFUSE, white);
				glLightfv(GL_LIGHT1, GL_SPECULAR, white); break;
		case 2: glLightfv(GL_LIGHT2, GL_AMBIENT, dark);
				glLightfv(GL_LIGHT2, GL_DIFFUSE, white);
				glLightfv(GL_LIGHT2, GL_SPECULAR, white); break;
		case 3: glLightfv(GL_LIGHT3, GL_AMBIENT, dark);
				glLightfv(GL_LIGHT3, GL_DIFFUSE, white);
				glLightfv(GL_LIGHT3, GL_SPECULAR, white); break;
		case 4: glLightfv(GL_LIGHT4, GL_AMBIENT, dark);
				glLightfv(GL_LIGHT4, GL_DIFFUSE, white);
				glLightfv(GL_LIGHT4, GL_SPECULAR, white); break;
		case 5: glLightfv(GL_LIGHT5, GL_AMBIENT, dark);
				glLightfv(GL_LIGHT5, GL_DIFFUSE, white);
				glLightfv(GL_LIGHT5, GL_SPECULAR, white); break;
		case 6: glLightfv(GL_LIGHT6, GL_AMBIENT, dark);
				glLightfv(GL_LIGHT6, GL_DIFFUSE, white);
				glLightfv(GL_LIGHT6, GL_SPECULAR, white); break;
		case 7: glLightfv(GL_LIGHT7, GL_AMBIENT, dark);
				glLightfv(GL_LIGHT7, GL_DIFFUSE, white);
				glLightfv(GL_LIGHT7, GL_SPECULAR, white); break;
	}
}

/**
Fun��o que faz enable para suportar as luzes
*/
void enableLights() {

	for (int i = 0; i < numLights; i++) {
		switch (i) {
			case 0: glEnable(GL_LIGHT0); defineColor(i); break;
			case 1: glEnable(GL_LIGHT1); defineColor(i); break;
			case 2: glEnable(GL_LIGHT2); defineColor(i); break;
			case 3: glEnable(GL_LIGHT3); defineColor(i); break;
			case 4: glEnable(GL_LIGHT4); defineColor(i); break;
			case 5: glEnable(GL_LIGHT5); defineColor(i); break;
			case 6: glEnable(GL_LIGHT6); defineColor(i); break;
			case 7: glEnable(GL_LIGHT7); defineColor(i); break;
		}
	}
}

/**
M�todo que desenha a cena tendo em conta 
a ListGroups em vigor. � de notar que para 
cada grupo � preparado um vbo diferente
*/
void prepareData() {

	int size = 0, vboIndex = 0, texture_models_id = 0, id_texture;
	int groups = numGroups(lg), n_models = getNumModelsTotal(lg);
	vertices = (GLuint*)malloc(sizeof(GLuint)*n_models);
	normals = (GLuint*)malloc(sizeof(GLuint) * n_models);
	numVerticess = (GLuint*)malloc(sizeof(GLuint) * n_models);
	texCoord = (GLuint*)malloc(sizeof(GLuint) * textureModels);
	/* Sizes � par�metro de saida */
	vector<Model>** models = (vector<Model>**) getModels(lg);

	vector<float>* vec;
	vector<float>* norm;
	vector<float>* texture;
	/* Ativamos as luzes */
	enableLights();

	/* Criamos os vbo's para os v�rtices e 
	para as normais */
	glGenBuffers(n_models, vertices);
	glGenBuffers(n_models, normals);
	glGenBuffers(textureModels, texCoord);
	/* Passamos os vetores para 
	a mem�ria gr�fica */
	for (int i = 0; i < groups; i++, vboIndex++) {
		/* Percorremos os models de cada group */
		size = models[i]->size();
		numVerticess[vboIndex] = size;
		/* Percorremos cada um dos models
		dentro do respetivo group */
		for (int j = 0; j < size; j++) {
			vec = getVertices(models[i]->at(j));
			norm = getNormals(models[i]->at(j));
			//printf("N vertices do modelo %d: %d\n", i, numVerticess[i]);
			glBindBuffer(GL_ARRAY_BUFFER, vertices[i]);
			glBufferData(
				GL_ARRAY_BUFFER, // tipo do buffer, s� � relevante na altura do desenho
				sizeof(float) * vec->size(), // tamanho do vector em bytes
				vec->data(), // os dados do array associado ao vector
				GL_STATIC_DRAW); // indicativo da utiliza��o (est�tico e para desenho)
			glBindBuffer(GL_ARRAY_BUFFER, normals[i]);
			glBufferData(
				GL_ARRAY_BUFFER,
				sizeof(float) * norm->size(),
				norm->data(),
				GL_STATIC_DRAW);
			/* Verificamos se a sua composi��o � 
			do tipo texture */
			if (hasTexture(models[i]->at(j))) {
				texture = getTextureInfo(models[i]->at(j), &id_texture);
				glBindBuffer(GL_ARRAY_BUFFER, texCoord[texture_models_id]);
				glBufferData(GL_ARRAY_BUFFER, 
					texture->size(),
					texture->data(), 
					GL_STATIC_DRAW);
				/* Associamos um valor ao id do buffer do respetivo
				model com composi��o texture */
				setBufferId(models[i]->at(j), texCoord[texture_models_id]);
				texture_models_id++;
			}
		}
	}
}

/**
Fun��o usada para preparar as estruturas de 
dados para desenhar as rotas de cada group 
que possua transla��es din�micas
*/
void prepareRotaData() {

	vertPercursos = (GLuint*)malloc(sizeof(GLuint) * nPercursos);
	/* Criamos os vbo's para desenhar as rotas */
	glGenBuffers(nPercursos, vertPercursos);
	for (int i = 0; i < nPercursos; i++) {
		glBindBuffer(GL_ARRAY_BUFFER, vertPercursos[i]);
		glBufferData(
			GL_ARRAY_BUFFER,
			sizeof(float) * 303,
			percursos[i]->data(),
			GL_STATIC_DRAW
		);
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
Fun��o que prepara as estruturas de dados para
*/
void preparePercurso(int group, int indiceOP) {

	float gt;
	float rota[3], deriv[3];
	/**
	Se estivermos perante uma rota de translate
	din�mica adicionamos valores ao percurso
	*/
	if (!strcmp(names[group][indiceOP], "translate") && params[group][indiceOP][3] > 0) {
		percursos = (vector<float>**)realloc(percursos, sizeof(vector<float>*) * nPercursos);
		percursos[nPercursos - 1] = new vector<float>();
		groupOpToPercurso = (float**)realloc(groupOpToPercurso, sizeof(float*) * nPercursos);
		groupOpToPercurso[nPercursos - 1] = (float*)malloc(sizeof(float) * 2);
		groupOpToPercurso[nPercursos - 1][0] = group;
		groupOpToPercurso[nPercursos - 1][1] = indiceOP;
		/* Construimos o array */
		for (int i = 0; i < 100; i++) {
			gt = (float)i / (float)100;
			getGlobalCatmullRomPointNoTime(group, indiceOP, gt, rota, deriv);
			percursos[nPercursos - 1]->push_back(rota[0]);
			percursos[nPercursos - 1]->push_back(rota[1]);
			percursos[nPercursos - 1]->push_back(rota[2]);
		}
	}
}

/**
Fun��o que inicializa as estruturas de dados 
que guardam os eixos de cada group
*/
void prepareAxis(int group) {

	X[group] = (float*)malloc(sizeof(float) * 3);
	Y[group] = (float*)malloc(sizeof(float) * 3);
	Z[group] = (float*)malloc(sizeof(float) * 3);
	Y[group][0] = 0.0f;
	Y[group][1] = 1.0f;
	Y[group][2] = 0.0f;
}

/**
M�todo que desenha o conteudo pedido, 
inicializando previamente os vbo's
*/
void prepareScene(){

	X = (float**)malloc(sizeof(float*) * size);
	Y = (float**)malloc(sizeof(float*) * size);
	Z = (float**)malloc(sizeof(float*) * size);
	/* Alocamos espa�o para o array percursos */
	percursos = (vector<float>**)malloc(sizeof(vector<float>*));
	/* Alocamos espa�o para o array que referencia 
	os percursos para o group */
	groupOpToPercurso = (float**)malloc(sizeof(float*));
	/* Preparamos os vbo's 
	para serem desenhados */
	prepareData();
	names = (char***)malloc(sizeof(char**) * size);
	params = (float***)malloc(sizeof(float**) * size);
	/* Vamos buscar as transforma��es 
	de cada group */
	numOps = getOpsParams(lg, &names, &params);
	/* Vamos buscar os catmoll-points */
	ListVertices** cat = getCatmollPoints(lg);
	/* Inicializamos ambos os array respons�veis 
	por lidar pelo catmoll-rom points */
	points = (float****)malloc(sizeof(float***) * size);
	numPoints = (float**)malloc(sizeof(float*) * size);
	int numOp, numCatPoints; Vertice v;
	/* Vamos preencher o array points */
	for (int i = 0; i < size; i++) {
		prepareAxis(i);
		numOp = numOps[i];
		points[i] = (float***)malloc(sizeof(float**) * numOp);
		numPoints[i] = (float*)malloc(sizeof(float) * numOp);
		/* Percorremos cada opera��o dentro de cada group */
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
				/* Visto que estamos perante uma transla��o 
				din�mica aumentamos em uma unidade o n�mero 
				de rotas a desenhar */
				nPercursos++;
				preparePercurso(i, j);
			}
			else {
				points[i][j] = NULL;
				numPoints[i][j] = 0;
			}
		}
	}
	prepareRotaData();
	//printArrays();
	//printf("Preparation Done\n");
}

/**
Fun��o que desenha a rota a ser percorrida 
pelos modelos incluidos no group e opera��o 
especificados nos par�metros d� fun��o
*/
void drawRotas(int group, int indiceOP) {

	/* Percorremos cada um dos percursos */
	for (int i = 0; i < nPercursos; i++) {
		/* verificamos se existe alguma rota 
		referenciada para a opera��o respetiva */
		if (groupOpToPercurso[i][0] == group && groupOpToPercurso[i][1] == indiceOP) {

			glColor3f(1.0f, 1.0f, 1.0f);
			/* Desenhamos os respetivos v�rtices */
			glBindBuffer(GL_ARRAY_BUFFER, vertPercursos[i]);
			glVertexPointer(3, GL_FLOAT, 0, 0);
			glDrawArrays(GL_LINE_LOOP, 0, 100);
		}
	}
}

/**
M�todo que dado um n�mero de light nos devolve 
as 3 coordenadas para a posi��o da mesma
*/
float* getLightPosition(int num) {

	int index = 3 * num;
	float* coord = (float*)malloc(sizeof(float) * 3);
	coord[0] = lights->at(index);
	coord[1] = lights->at(index + 1);
	coord[2] = lights->at(index + 2);
	return coord;
}

/**
Fun��o que permite posicionar cada 
uma das luzes
*/
void putLight(int num, float* pos) {

	switch (num) {
		case 0: glLightfv(GL_LIGHT0, GL_POSITION, pos); break;
		case 1: glLightfv(GL_LIGHT1, GL_POSITION, pos); break;
		case 2: glLightfv(GL_LIGHT2, GL_POSITION, pos); break;
		case 3: glLightfv(GL_LIGHT3, GL_POSITION, pos); break;
		case 4: glLightfv(GL_LIGHT4, GL_POSITION, pos); break;
		case 5: glLightfv(GL_LIGHT5, GL_POSITION, pos); break;
		case 6: glLightfv(GL_LIGHT6, GL_POSITION, pos); break;
		case 7: glLightfv(GL_LIGHT7, GL_POSITION, pos); break;
	}
}

/**
Fun��o que trata das transforma��es
geom�tricas para um determinado group
tendo em conta o instante. Para al�m disso 
esta fun��o desenha o respetivo group
t
*/
void reposicionaModels(float gt) {

	float* lightPosition;
	int vboIndex = 0, nModels;
	float matrix[4][4];
	float position[3], deriv[3];
	float diffuse[4], specular[4], emission[4], ambient[4]; int shineness;
	vector<Model>* group_models;
	/* Colocamos cada uma das luzes */
	for (int i = 0; i < numLights; i++) {
		lightPosition = getLightPosition(i);
		putLight(i, lightPosition);
		free(lightPosition);
	}
	/* Percorremos cada um dos groups */
	for (int i = 0; i < size; i++) {
		glPushMatrix();
		/* Percorremos cada uma das opera��es de cada group */
		for (int j = 0; j < numOps[i]; j++) {
			switch (names[i][j][0]) {

			case 't':
				/* Caso estejamos perante uma transla��o est�tica fazemos um simples translate */
				if (params[i][j][3] == -1)
					glTranslatef(params[i][j][0], params[i][j][1], params[i][j][2]);
				else {
					/* Desenhamos o percurso dos modelos do respetivo group */
					if(showRotas)
						drawRotas(i, j);
					getGlobalCatmullRomPoint(i, j, gt, position, deriv);
					glTranslatef(position[0], position[1], position[2]);
					/* Calculamos os valores dos vetores para os eixos */
					for (int k = 0; k < 3; k++)
						X[i][k] = deriv[k];
					normalize(X[i]);
					cross(X[i], Y[i], Z[i]);
					normalize(Z[i]);
					cross(Z[i], X[i], Y[i]);
					normalize(Y[i]);
					/* Construimos a matriz de rota��o */
					buildRotMatrix(X[i], Y[i], Z[i], (float*)matrix);
					glMultMatrixf((float*)matrix);
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

			case 's':
				glScalef(params[i][j][0], params[i][j][1], params[i][j][2]);
				break;

			default:
				break;
			}
		}
		/* Vamos buscar o n�mero de models do respetivo group */
		nModels = getNumModels(getGroup(lg, i));
		group_models = (vector<Model>*)getModelsGroup(getGroup(lg, i));
		/* Desenhamos cada model pertencente 
		a esse group */
		for (int j = 0; j < nModels; j++, vboIndex++) {
			/* Estamos perante uma compisi��o 
			material */
			if (!hasTexture(group_models->at(j))) {
				getMaterialInfo(group_models->at(j), diffuse, specular, ambient, emission, &shineness);
				/* Definimos o material para o modelo */
				glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
				glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
				glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
				glMaterialfv(GL_FRONT, GL_EMISSION, emission);
				glMaterialf(GL_FRONT, GL_SHININESS, shineness);
			}
			/* Estamos perante uma composi��o do 
			tipo Texture */
			else {
				/* Associamos a respetiva imagem ao model */
				glBindTexture(GL_TEXTURE_2D, getTextureId(group_models->at(j)));

				glBindBuffer(GL_ARRAY_BUFFER, getBufferId(group_models->at(j)));
				glTexCoordPointer(2, GL_FLOAT, 0, 0);
			}
			/* Desenhamos os respetivos v�rtices */
			glBindBuffer(GL_ARRAY_BUFFER, vertices[vboIndex]);
			glVertexPointer(3, GL_FLOAT, 0, 0);

			glBindBuffer(GL_ARRAY_BUFFER, normals[vboIndex]);
			glNormalPointer(GL_FLOAT, 0, 0);
			glDrawArrays(GL_TRIANGLES, 0, numVerticess[vboIndex]);
		}

		/* Fazemos pop da matrix */
		glPopMatrix();
	}
}

/**
Fun��o que ser� posta a correr dentro 
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
		0.0f, 1.0f, 0.0f);  // �up vector� (0.0f, 1.0f, 0.0f)

	// Scene Design
	reposicionaModels(a);

	printf("Coloquei modelos\n");

	// End of frame
	glutSwapBuffers();

	a += 0.0001*deltaTime;
}

/**
Fun��o que reaje � intera��o do utilizador com o teclado
*/
void processKeys(unsigned char c, int xx, int yy) {

	switch (c) {

		case 'r':
			showRotas = !showRotas;
			break;

		default :
			break;
	}
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

/**
Fun��o que permite imprimir 
informa��es no ecr�
*/
void printInfo() {

	printf("Press 'r' to switch the orbits' lines off...");
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
	glEnable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

// Prepare the scene to draw
	loadFile();

	//printOpsLG(lg);

	size = numGroups(lg);

	prepareScene();

	printInfo();

// enter GLUT's main cycle
	glutMainLoop();

	return 0;
}
