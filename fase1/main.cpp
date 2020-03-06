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


const char* FILE_XML_NAME = "..\\Generator\\Generator\\infoXML.xml";

// VARIAVEIS GLOBAIS
float px = 5, py = 5, pz = 5;
float varx = 0, vary = 0, varz = 0;

// Camera Explorer
float raio = 10, alfa = M_PI_4, beta = M_PI_4;

// FPS CAMERA
float dx, dy, dz;

int camera_mode = 1;

/**
Estrutura de dados que guarda os 
vertices a serem desenhados
*/
ListVertices lv;


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
Fun��o usada para desenhar os eixos
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
Fun��o que desenha os triangulos das formas a partir da lista de vertices
*/
void drawScene() {

	Vertice v,v2,v3;
	while ((v = nextV(lv)) != NULL) {
		v2 = nextV(lv);
		v3 = nextV(lv);
		glBegin(GL_TRIANGLE_STRIP);
		glColor3f(0.0f, 0.0f, 1.0f);
		glVertex3f(getX(v),getY(v),getZ(v));
		glVertex3f(getX(v2),getY(v2),getZ(v2));
		glVertex3f(getX(v3),getY(v3),getZ(v3));
		glEnd();
	}

	// Colocar o pointer novamente a 0
	atualizaPointer(lv);
}

/**
Funcao que carrega o nome do ficheiro que se pretende reproduzir!
*/
void loadFile(int type) {

	TiXmlDocument doc;
	char* filename = "";
	doc.LoadFile(FILE_XML_NAME);

	TiXmlHandle docH(&doc);

	TiXmlElement* element;

	switch (type) {
		case 1:
			element = docH.FirstChildElement("scene").ChildElement(0).Element();
			filename = strdup(element->Attribute("file"));
			break;
		case 2:
			element = docH.FirstChildElement("scene").ChildElement(1).Element();
			filename = strdup(element->Attribute("file"));
			break;
		case 3:
			element = docH.FirstChildElement("scene").ChildElement(2).Element();
			filename = strdup(element->Attribute("file"));
			break;
		case 4:
			element = docH.FirstChildElement("scene").ChildElement(3).Element();
			filename = strdup(element->Attribute("file")); 
			break;
		default:
			break;
	}
	lv = carregaFile(filename);
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
			0.0f, 1.0f, 0.0f);  // �up vector� (0.0f, 1.0f, 0.0f)
	} else {
		// FPS Camera
		dx = raio * cos(beta) * sin(alfa);
		dy = raio * sin(beta);
		dz = raio * cos(beta) * cos(alfa);
		gluLookAt(px, py, pz,  // camera position
			px + dx, py + dy, pz + dz,   // look at point
			0.0f, 1.0f, 0.0f);  // �up vector� (0.0f, 1.0f, 0.0f)
	}

	//glTranslatef(varx, vary, varz);

	//AXIS
	drawAxis();

	glTranslatef(varx, vary, varz);

	//glRotatef(angle, 0.0f, 1.0f, 0.0f);

	drawScene();

	// End of frame
	glutSwapBuffers();
}


void processKeys(unsigned char c, int xx, int yy) {

	// put code to process regular keys in here

	float speed = 0.3f;

	float norma, crossX, crossY, crossZ;

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
		case 'r':
			// Transla�ao do objeto desenhado para a direita ao longo do eixo do x
			varx += 1.0f;
			break;
		case 'l':
			// Transla�ao do objeto desenhado para a esquerda ao longo do eixo do x
			varx -= 1.0f;
			break;
		case 'c':
			// Transla�ao do objeto desenhado para cima ao longo do eixo do y
			vary += 1.0f;
			break;
		case 'b':
			// Transla�ao do objeto desenhado para baixo ao longo do eixo do y
			vary -= 1.0f;
			break;
		case 'm':
			// Transla�ao do objeto desenhado para fora ao longo do eixo do z
			varz += 1.0f;
			break;
		case 'n':
			// Transla�ao do objeto desenhado para dentro ao longo do eixo do z
			varz -= 1.0f;
			break;
		default:
			break;
		}
	}
}


void processSpecialKeys(int key, int xx, int yy) {

	// put code to process special keys in here

	switch (key)
	{
	case GLUT_KEY_UP:
		beta += (float)0.1;
		break;
	case GLUT_KEY_DOWN:
		beta -= (float)0.1;
		break;
	case GLUT_KEY_LEFT:
		if (camera_mode == 1) {
			alfa -= (float)0.1;
		}
		else {
			alfa += (float)0.1;
		}
		break;
	case GLUT_KEY_RIGHT:
		if (camera_mode == 1) {
			alfa += (float)0.1;
		}
		else {
			alfa -= (float)0.1;
		}
		break;
	case GLUT_KEY_PAGE_UP:
		raio -= 1;
		break;
	case GLUT_KEY_PAGE_DOWN:
		raio += 1;
		break;
	case GLUT_KEY_F1:
		camera_mode = 1;
		alfa = -alfa;
		beta = -beta;
		px = 5;
		py = 5;
		pz = 5;
		break;
	case GLUT_KEY_F2:
		camera_mode = 2;
		alfa = -alfa;
		beta = -beta;
		break;
	default:
		break;
	}
}



int main(int argc, char **argv) {

	// type : 1 -> plane
	//        2 -> box
	//        3 -> sphere
	//        4 -> cone
	loadFile(2);
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
