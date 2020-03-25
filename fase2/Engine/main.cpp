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
const char* FILE_XML_NAME = "infoXML.xml";

// VARIAVEIS GLOBAIS
float px = 5, py = 5, pz = 5;
float varx = 0, vary = 0, varz = 0;

// Camera Explorer
float raio = 10, alfa = M_PI_4, beta = M_PI_4;

// FPS CAMERA
float dx, dy, dz;

// F1 para a camera Explorer(1), F2 para a camera FPS(2)
int camera_mode = 1;

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


/*Alterar os objetos a visualizar*/
int pol = 0;

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
Função que desenha os triangulos das formas a partir das listas de vertices
*/
/*
void drawScene() {

	Vertice v,v2,v3;
	while ((v = nextV(lv[pol])) != NULL) {
		v2 = nextV(lv[pol]);
		v3 = nextV(lv[pol]);
		glBegin(GL_TRIANGLES);
		glColor3f(0.0f, 0.0f, 1.0f);
		glVertex3f(getX(v), getY(v), getZ(v));
		glVertex3f(getX(v2), getY(v2), getZ(v2));
		glVertex3f(getX(v3), getY(v3), getZ(v3));
		glEnd();
	}
	// Colocar o pointer novamente a 0
	atualizaPointer(lv[pol]);
}
*/
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
	addOperacao(g, tagName, param);
	printf("%s added successfully to group\n", tagName);
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
	addOperacao(g, tagName, param);
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
			printf("%s\n", filename);
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
				addVertice(g, x, y, x);
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

	/* Vamos buscar o numero de operações 
	nos arrays opNames e params para o podermos 
	atualizar~no contexto de cada chamada recursiva */
	int atualNumOps = numOperacoes;
	float angle=0, x=0, y=0, z=0;
	/* Criamos um novo grupo */
	Group g = newGroup();
	/* Se tivermos operações provenientes
	de um group pai adicionamos essas mesmas
	operações ao grupo filho */
	if (numOperacoes != 0) {
		addOperacoes(g, opNames, params, numOperacoes);
	}
	char* tagName = (char*)element->Value(), *tagNameSubElem;
	TiXmlNode* ele = NULL;
	printf("<%s>\n", element->Value());
	/* Retirar os nomes do XML e carregar os
	ficheiros respetivos para as devidas
	estruturas de dados */
	for (int j = 0; ((ele = element->IterateChildren(ele))); j++) {
		/* Elemento sobre o qual estamos a iterar */
		TiXmlElement* subelement = ele->ToElement();
		printf("<%s>\n", subelement->Value());
		/* Vamos buscar o nome da tag desse element */
		tagNameSubElem = (char*)subelement->Value();
		/* Se o filho for group chamamos 
		a função recursivamente */
		if (!strcmp(tagNameSubElem, "group")) {
			/* Processamos o subgrupo */
			processaGroup(subelement, opNames, params, atualNumOps);
		}
		/* Se não for group acrescentamos 
		informação à lista de groups */
		else {
			switch (tagNameSubElem[0]) {

				/* Estamos perante uma tag 
				de translação */
				case 't':
					opNames = (char**)realloc(opNames, sizeof(char*) * (++atualNumOps));
					opNames[atualNumOps - 1] = strdup("translate");
					params = (float**)realloc(params, sizeof(float*) * (atualNumOps));
					(subelement->Attribute("X") == NULL) ? x = 0 : x = atof(subelement->Attribute("X"));
					(subelement->Attribute("Y") == NULL) ? y = 0 : y = atof(subelement->Attribute("Y"));
					(subelement->Attribute("Z") == NULL) ? z = 0 : z = atof(subelement->Attribute("Z"));
					params[atualNumOps - 1] = (float*)malloc(sizeof(float) * 3);
					params[atualNumOps - 1][0] = x;
					params[atualNumOps - 1][1] = y;
					params[atualNumOps - 1][2] = z;
					processaTranslate(g, tagNameSubElem, x, y, z);
					break;

				/* Estamos perante uma tag 
				de rotação */
				case 'r':
					opNames = (char**)realloc(opNames, sizeof(char*) * (++atualNumOps));
					opNames[atualNumOps - 1] = strdup("rotate");
					params = (float**)realloc(params, sizeof(float*) * (atualNumOps));
					(subelement->Attribute("angle") == NULL) ? angle = 0 : angle = atof(subelement->Attribute("angle"));
					(subelement->Attribute("axisX") == NULL) ? x = 0 : x = atof(subelement->Attribute("axisX"));
					(subelement->Attribute("axisY") == NULL) ? y = 0 : y = atof(subelement->Attribute("axisY"));
					(subelement->Attribute("axisZ") == NULL) ? z = 0 : z = atof(subelement->Attribute("axisZ"));
					params[atualNumOps - 1] = (float*)malloc(sizeof(float) * 4);
					params[atualNumOps - 1][0] = angle;
					params[atualNumOps - 1][1] = x;
					params[atualNumOps - 1][2] = y;
					params[atualNumOps - 1][3] = z;
					processaRotate(g, tagNameSubElem, angle, x, y, z);
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
	printf("\t>> Group added successfully!\n");
}


/**
 * Funcao que carrega o nome do ficheiro que se pretende reproduzir a partir do ficheiro XML
 * infoXML.xml. Para isso recorreu-se ao uso da biblioteca de funcoes C++ 'TinyXML' de modo a
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
		char** opNames = (char**)malloc(1);
		float** param = (float**)malloc(1);
		processaGroup(ele->ToElement(),opNames,param,0);
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
	int* sizes=NULL;
	/* Sizes é parâmetro de saida */
	vector<float>** vec = getVectors(lg, sizes);
	/* Criamos os vbo's */
	printf("Vou preparar os vbo's\n");
	glGenBuffers(groups, vertices);
	printf("Vbo's preparados\n");
	/* Passamos os vetores para 
	a memória gráfica */
	for (int i = 0; i < groups; i++) {
		glBindBuffer(GL_ARRAY_BUFFER, vertices[i]);
		glBufferData(
			GL_ARRAY_BUFFER, // tipo do buffer, só é relevante na altura do desenho
			sizeof(float)*(*vec[i]).size(), // tamanho do vector em bytes
			(*vec[i]).data(), // os dados do array associado ao vector
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
	/* Vamos buscar as transformações 
	de cada group */
	numOps = getOpsParams(lg, names, params);
}

/**
Método que desenha a scene
*/
void drawScene() {

	/* Percorremos as transformações 
	de cada group */
	for (int i = 0; i < size; i++) {

	}
}

void renderScene(void) {

	// clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// set the camera
	glLoadIdentity();
	if (camera_mode == 1) {
		// Explorer Mode Camera
		gluLookAt(raio * cos(beta) * sin(alfa), raio * sin(beta), raio * cos(beta) * cos(alfa), // camera position
			0.0, 0.0, 0.0,  // look at point
			0.0f, 1.0f, 0.0f);  // “up vector” (0.0f, 1.0f, 0.0f)
	} else {
		// FPS Camera
		dx = raio * cos(beta) * sin(alfa);
		dy = raio * sin(beta);
		dz = raio * cos(beta) * cos(alfa);
		gluLookAt(px, py, pz,  // camera position
			px + dx, py + dy, pz + dz,   // look at point
			0.0f, 1.0f, 0.0f);  // “up vector” (0.0f, 1.0f, 0.0f)
	}

	//AXIS
	drawAxis();

	// Transformations
	glTranslatef(varx, vary, varz);

	// Scene Design
	//drawScene();

	// :)
	glutWireTeapot(0.5);

	// End of frame
	glutSwapBuffers();
}


void processKeys(unsigned char c, int xx, int yy) {

	// Process regular keys

	float speed = 0.3f;

	float norma, crossX, crossY, crossZ;

	/*Somente válido na camera modo FPS*/
	if (camera_mode == 2) {

		switch (c)
		{
		case 'w':
			norma = sqrt(dx * dx + dy * dy + dz * dz);
			px += speed * (dx / norma);
			py += speed * (dy / norma);
			pz += speed * (dz / norma);
			break;
		case 's':
			norma = sqrt(dx * dx + dy * dy + dz * dz);
			px -= speed * (dx / norma);
			py -= speed * (dy / norma);
			pz -= speed * (dz / norma);
			break;
		case 'a':
			// Produto vetorial
			crossX = dy * 0.0f - dz * 1.0f;
			crossY = dz * 0.0f - dx * 0.0f;
			crossZ = dx * 1.0f - dy * 0.0f;
			norma = sqrt(crossX * crossX + crossY * crossY + crossZ * crossZ);
			px -= speed * (crossX / norma);
			py -= speed * (crossY / norma);
			pz -= speed * (crossZ / norma);
			break;
		case 'd':
			// Produto vetorial
			crossX = dy * 0.0f - dz * 1.0f;
			crossY = dz * 0.0f - dx * 0.0f;
			crossZ = dx * 1.0f - dy * 0.0f;
			norma = sqrt(crossX * crossX + crossY * crossY + crossZ * crossZ);
			px += speed * (crossX / norma);
			py += speed * (crossY / norma);
			pz += speed * (crossZ / norma);
			break;
		default:
			break;
		}
	}

	/*Válido para todas as cameras*/
	switch (c) {
	case 'r':
		// Translaçao do objeto desenhado para a direita ao longo do eixo do x
		varx += 1.0f;
		break;
	case 'l':
		// Translaçao do objeto desenhado para a esquerda ao longo do eixo do x
		varx -= 1.0f;
		break;
	case 'c':
		// Translaçao do objeto desenhado para cima ao longo do eixo do y
		vary += 1.0f;
		break;
	case 'b':
		// Translaçao do objeto desenhado para baixo ao longo do eixo do y
		vary -= 1.0f;
		break;
	case 'm':
		// Translaçao do objeto desenhado para fora ao longo do eixo do z
		varz += 1.0f;
		break;
	case 'n':
		// Translaçao do objeto desenhado para dentro ao longo do eixo do z
		varz -= 1.0f;
		break;
	default:
		break;
	}
	glutPostRedisplay();
}


void processSpecialKeys(int key, int xx, int yy) {

	// Process special keys

	switch (key)
	{
	case GLUT_KEY_UP:
		beta += (float)0.1;
		if (beta > 1.5f)
			beta = 1.5f;
		break;
	case GLUT_KEY_DOWN:
		beta -= (float)0.1;
		if (beta < -1.5f)
			beta = -1.5f;
		break;
	case GLUT_KEY_LEFT:
		if (camera_mode == 1) {
			alfa -= (float)0.1;
		} else {
			alfa += (float)0.1;
		}
		break;
	case GLUT_KEY_RIGHT:
		if (camera_mode == 1) {
			alfa += (float)0.1;
		} else {
			alfa -= (float)0.1;
		}
		break;
	case GLUT_KEY_PAGE_UP:
		// Fazer zoom in no modo Explorer Camera
		raio -= 1;
		if (raio < 0.1f)
			raio = 0.1f;
		break;
	case GLUT_KEY_PAGE_DOWN:
		// Fazer zoom out no modo Explorer Camera
		raio += 1;
		break;
	case GLUT_KEY_F1:
		// Mudar para o modo Explorer Camera
		camera_mode = 1;
		alfa = -alfa;
		beta = -beta;
		px = 5;
		py = 5;
		pz = 5;
		break;
	case GLUT_KEY_F2:
		// Mudar para o modo FPS Camera
		camera_mode = 2;
		alfa = -alfa;
		beta = -beta;
		break;
	default:
		break;
	}

	glutPostRedisplay();
}

/* Cuidado que deve-se fazer o build novamente com o CMake 
 * Deve-se buscar os ficheiros gerados ao programa "Generator"
 * Deve-se atualizar o XML com o nome dos ficheiros acabados de gerar
*/
int main(int argc, char **argv) {

	lg = newListGroups();
	// Carregar os ficheiros das primitivas a partir do xml 
	//addVertice(g, 0, 0, 0);
	//ListVertices lv = newListVertices();

	//for (int i = 0; i < 2000; i++)
	//	addVertice(lv, 0, 0, 0);

	loadFile();

	size = numGroups(lg);

	//desenhaListGroups(lg);

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

#ifndef __APPLE__
	glewInit();
#endif

//  OpenGL settings
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnableClientState(GL_VERTEX_ARRAY);

	prepareScene();

	printf("Scene prepared\n");

	for (int i = 0; i < size; i++)
		printf("Número de operações do group n%d: %d\n", i + 1, numOps[i]);

	drawScene();

// enter GLUT's main cycle
	glutMainLoop();
	return 0;
}
