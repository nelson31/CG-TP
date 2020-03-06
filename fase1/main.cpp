#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <stdlib.h>
#include <GL/glut.h>
#endif

#define _USE_MATH_DEFINES
#include <math.h>
#include <stdio.h>
#include <string.h>

#include "vertice.h"
#include "ListVertices.h"
#include "tinyxml.h"

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
Estrutura de dados que guarda os 
vertices a serem desenhados
*/
ListVertices* lv;
/*Tamanho do array de ListVertices*/
int size;

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

/**
 * Funcao que carrega o nome do ficheiro que se pretende reproduzir a partir do ficheiro XML 
 * infoXML.xml. Para isso recorreu-se ao uso da biblioteca de funcoes C++ 'TinyXML' de modo a 
 * se efetuar o parse do ficheiro xml para se retirar o nome do ficheiro que contem as coordenadas 
 * dos vários vértices que compoe a figura.
*/
void loadFile() {

	TiXmlDocument doc;
	char* filename = "";
	doc.LoadFile(FILE_XML_NAME);

	TiXmlHandle docH(&doc);

	TiXmlNode* ele=NULL;
	
	// Descobrir o numero de entradas no ficheiro XML com nomes de ficheiros
	int i = 0;
	while ((ele = doc.FirstChildElement("scene")->IterateChildren(ele)) != 0) {
		i++;
	}

	size = i;

	lv = (ListVertices*) malloc(i * sizeof(ListVertices));

	TiXmlElement* element;

	// Retirar os nomes do XML e carregar os ficheiros respetivos para as devidas estruturas de dados
	for (int j = 0; j < i; j++) {
		element = docH.FirstChildElement("scene").ChildElement(j).Element();

		filename = strdup(element->Attribute("file"));

		lv[j] = carregaFile(filename);
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
	drawScene();

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
	case 'x':
		// Mudar de objeto a desenhar
		pol = ((pol+1) % size);
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


int main(int argc, char **argv) {

	// Carregar os ficheiros das primitivas a partir do xml 
	loadFile();
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
