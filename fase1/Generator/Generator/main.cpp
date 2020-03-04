#ifdef __GENERATOR__

#endif

#define _USE_MATH_DEFINES
#include <math.h>
#include <stdio.h>
#include <string.h>

#include "tinyxml.h"


/**
 * Desenho do plano e escrita dos respetivos pontos no ficheiro
*/
void drawPlane(FILE* fp, float size) {

	/* Definição do número de vértices */
	fprintf(fp, "6\n");

	fprintf(fp, "%f %f %f\n", 0.0f, -size/2, size/2);
	fprintf(fp, "%f %f %f\n", 0.0f, -size/2, -size/2);
	fprintf(fp, "%f %f %f\n", 0.0f, size/2, -size/2);

	fprintf(fp, "%f %f %f\n", 0.0f, -size/2, size/2);
	fprintf(fp, "%f %f %f\n", 0.0f, size/2, -size/2);
	fprintf(fp, "%f %f %f\n", 0.0f, size/2, size/2);

}

/**
 * Desenho da caixa e escrita dos respetivos pontos no ficheiro
*/
void drawBox(FILE* fp, float x, float y, float z) {

	/* Definição do número de vértices */
	fprintf(fp, "36\n");

	//Base da caixa

	fprintf(fp, "%f %f %f\n", -x / 2, -y / 2, -z / 2);
	fprintf(fp, "%f %f %f\n", x / 2, -y / 2, -z / 2);
	fprintf(fp, "%f %f %f\n", -x / 2, -y / 2, z / 2);

	fprintf(fp, "%f %f %f\n", x / 2, -y / 2, -z / 2);
	fprintf(fp, "%f %f %f\n", x / 2, -y / 2, z / 2);
	fprintf(fp, "%f %f %f\n", -x / 2, -y / 2, z / 2);


	//Tampa da caixa

	fprintf(fp, "%f %f %f\n", -x / 2, y / 2, -z / 2);
	fprintf(fp, "%f %f %f\n", -x / 2, y / 2, z / 2);
	fprintf(fp, "%f %f %f\n", x / 2, y / 2, -z / 2);

	fprintf(fp, "%f %f %f\n", x / 2, y / 2, -z / 2);
	fprintf(fp, "%f %f %f\n", -x / 2, y / 2, z / 2);
	fprintf(fp, "%f %f %f\n", x / 2, y / 2, z / 2);


	//Face ao longo do eixo x no z positivo

	fprintf(fp, "%f %f %f\n", x / 2, y / 2, z / 2);
	fprintf(fp, "%f %f %f\n", -x / 2, y / 2, z / 2);
	fprintf(fp, "%f %f %f\n", -x / 2, -y / 2, z / 2);

	fprintf(fp, "%f %f %f\n", x / 2, y / 2, z / 2);
	fprintf(fp, "%f %f %f\n", -x / 2, -y / 2, z / 2);
	fprintf(fp, "%f %f %f\n", x / 2, -y / 2, z / 2);


	//Face ao longo do eixo x no z negativo

	fprintf(fp, "%f %f %f\n", x / 2, y / 2, -z / 2);
	fprintf(fp, "%f %f %f\n", -x / 2, -y / 2, -z / 2);
	fprintf(fp, "%f %f %f\n", -x / 2, y / 2, -z / 2);

	fprintf(fp, "%f %f %f\n", x / 2, y / 2, -z / 2);
	fprintf(fp, "%f %f %f\n", x / 2, -y / 2, -z / 2);
	fprintf(fp, "%f %f %f\n", -x / 2, -y / 2, -z / 2);


	//Face ao longo do eixo z no x negativo

	fprintf(fp, "%f %f %f\n", -x / 2, y / 2, -z / 2);
	fprintf(fp, "%f %f %f\n", -x / 2, -y / 2, z / 2);
	fprintf(fp, "%f %f %f\n", -x / 2, y / 2, z / 2);

	fprintf(fp, "%f %f %f\n", -x / 2, y / 2, -z / 2);
	fprintf(fp, "%f %f %f\n", -x / 2, -y / 2, -z / 2);
	fprintf(fp, "%f %f %f\n", -x / 2, -y / 2, z / 2);


	//Face ao longo do eixo z no x positivo

	fprintf(fp, "%f %f %f\n", x / 2, y / 2, -z / 2);
	fprintf(fp, "%f %f %f\n", x / 2, y / 2, z / 2);
	fprintf(fp, "%f %f %f\n", x / 2, -y / 2, z / 2);

	fprintf(fp, "%f %f %f\n", x / 2, y / 2, -z / 2);
	fprintf(fp, "%f %f %f\n", x / 2, -y / 2, z / 2);
	fprintf(fp, "%f %f %f\n", x / 2, -y / 2, -z / 2);

}

/**
 * Desenho da esfera e escrita dos respetivos pontos no ficheiro
*/
void drawSphere(FILE* fp, int radius, int slices, int stacks) {

	// Usado para a construcao de cada uma das circunferencias
	float alpha = 2 * M_PI / slices;

	// Angulo usado para a construcao das circunferencias
	float beta = M_PI / stacks;

	float x, y, z;

	/* Definição do número de vértices */
	fprintf(fp, "%d\n", 3 * slices * stacks);

	for (int i = 0; i<stacks; i++) {

		float angleB = beta * i - M_PI_2;

		// Desenhar os circulos de cima e de baixo
		for (int j = 0; j < slices; j++) {

			float angleA = alpha * j;

			float nextAngle = angleA + alpha;

			x = 0.0f;
			y = radius * sin(angleB);
			z = 0.0f;

			fprintf(fp, "%f %f %f\n", x, y, z);

			x = (radius * cos(angleB)) * sin(angleA);
			y = radius * sin(angleB);
			z = (radius * cos(angleB)) * cos(angleA);

			fprintf(fp, "%f %f %f\n", x, y, z);

			x = (radius * cos(angleB)) * sin(nextAngle);
			y = radius * sin(angleB);
			z = (radius * cos(angleB)) * cos(nextAngle);

			fprintf(fp, "%f %f %f\n", x, y, z);
		}
	}
}

/**
 * Desenho do cone e escrita dos respetivos pontos no ficheiro
*/
void drawCone(FILE* fp, int bottomRadius, int height, int slices, int stacks) {

	float reason = (float)height / stacks;

	float alpha = 2 * M_PI / slices;

	float currentRadius = bottomRadius;

	float x, y, z;

	/* Definição do número de vértices */
	fprintf(fp, "%d\n", 3 * slices * stacks);

	for (int i =0; i<stacks; i++) {

		currentRadius = (float) ((stacks-i) * bottomRadius) / stacks;

		y = reason*i;

		for (int j = 0; j < slices; j++) {

			float angle = (float) alpha * j;

			float nextAngle = angle + alpha;

			x = 0.0f;
			z = 0.0f;

			fprintf(fp, "%f %f %f\n", x, y, z);

			x = currentRadius * sin(angle);
			z = currentRadius * cos(angle);

			fprintf(fp, "%f %f %f\n", x, y, z);

			x = currentRadius * sin(nextAngle);
			z = currentRadius * cos(nextAngle);

			fprintf(fp, "%f %f %f\n", x, y, z);
		}
	}
}

/**
Atualização do ficheiro XML com o novo nome da ultima atualizacao
*/
void writeXML(char type, const char* filename) {

	const char *plane, *box, *sphere, *cone;

	TiXmlDocument doc;
	doc.LoadFile("infoXML.xml");

	if (doc.Error() && doc.ErrorId() == TiXmlBase::TIXML_ERROR_OPENING_FILE) {
		printf("WARNING: File 'infoXML.xml' not found.\n");

		plane = "";
		box = "";
		sphere = "";
		cone = "";
	} else {

		TiXmlHandle docH(&doc);

		TiXmlElement* element = docH.FirstChildElement("scene").ChildElement(0).Element();
		plane = strdup(element->Attribute("file"));
		TiXmlElement* element1 = docH.FirstChildElement("scene").ChildElement(1).Element();
		box = strdup(element1->Attribute("file"));
		TiXmlElement* element2 = docH.FirstChildElement("scene").ChildElement(2).Element();
		sphere = strdup(element2->Attribute("file"));
		TiXmlElement* element3 = docH.FirstChildElement("scene").ChildElement(3).Element();
		cone = strdup(element3->Attribute("file"));
	}

	switch (type) {
		case 'p':
			plane = filename;
			break;
		case 'b':
			box = filename;
			break;
		case 's':
			sphere = filename;
			break;
		case 'c':
			cone = filename;
			break;
		default:
			break;
	}

	FILE* textfile = fopen("infoXML.xml", "w");
	if (textfile)
	{
		fputs("<?xml version='1.0'?>", textfile);
		fputs("<scene>", textfile);
		char linha[100];
		sprintf(linha, "<model file='%s' />", plane);
		fputs(linha, textfile);
		char linha1[100];
		sprintf(linha1, "<model file='%s' />", box);
		fputs(linha1, textfile);
		char linha2[100];
		sprintf(linha, "<model file='%s' />", sphere);
		fputs(linha, textfile);
		char linha3[100];
		sprintf(linha, "<model file='%s' />", cone);
		fputs(linha, textfile);
		fputs("</scene>", textfile);
		fclose(textfile);

		doc.LoadFile("infoXML.xml");

		doc.Print(stdout);
	}
}



/**
 * Funcao principal do Generator
*/

int main(int argc, const char* argv[]) {

	if (argc < 2) printf("INSIRA O NUMERO CORRETO DE PARAMETROS!");
	else {
		// IMPORTANTE: VER AQUELA QUESTAO DO NUMERO DE VERTICES!!!!!!!!!!
		// COMPLETAR
		
		FILE* fp;

		switch ( argv[1][0] ) {

			case 'p':
				writeXML(argv[1][0], argv[3]);
				fp = fopen(argv[3], "w");
				drawPlane(fp,atof(argv[2]));
				fclose(fp);
				break;
			case 'b':
				writeXML(argv[1][0], argv[5]);
				fp = fopen(argv[5], "w");
				drawBox(fp, atof(argv[2]), atof(argv[3]), atof(argv[4]));
				fclose(fp);
				break;
			case 's':
				writeXML(argv[1][0], argv[5]);
				fp = fopen(argv[5], "w");
				drawSphere(fp, atof(argv[2]), atof(argv[3]), atof(argv[4]));
				fclose(fp);
				break;
			case 'c':
				writeXML(argv[1][0], argv[6]);
				fp = fopen(argv[6], "w");
				drawCone(fp, atof(argv[2]), atof(argv[3]), atof(argv[4]), atof(argv[5]));
				fclose(fp);
				break;
			default:
				printf("INSIRA OS PARAMETROS CORRETOS!");
		}
	}

	return 0;
}