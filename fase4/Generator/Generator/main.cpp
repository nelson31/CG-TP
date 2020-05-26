#ifdef __GENERATOR__

#endif

#define _USE_MATH_DEFINES
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUM_POINTS_PATCH 16
#define NUM_COORDENADAS 3

// Variáveis globais só necessária para os Bezier Patches
int numP = -1; // numero de patches
int numCP = -1; // numero de control points
// Matriz que vai guardar os indices dos vários patches respetivos aos control points
int** indicesP;
/* Ler todos os control points */
float** controlPoints;
/* Variável usada para guardar todos os vértices do objeto que se pretende desenhar 
 *com os patches de Bezier
 */
float **objectVertices, **objectDeriv_u, **objectDeriv_v;


/*
 * Usado para mulltiplicar uma matriz por um vetor
*/
void multMatrixVector(float* m, float* v, float* res) {

	for (int j = 0; j < 4; ++j) {
		res[j] = 0;
		for (int k = 0; k < 4; ++k) {
			res[j] += v[k] * m[j * 4 + k];
		}
	}
}

/**
 * Calcula o produto externo entre dois vetores
*/
void cross(float* a, float* b, float* res) {

	res[0] = a[1] * b[2] - a[2] * b[1];
	res[1] = a[2] * b[0] - a[0] * b[2];
	res[2] = a[0] * b[1] - a[1] * b[0];
}

/**
 * Normaliza um vetores
*/
void normalize(float* a) {

	float l = sqrt(a[0] * a[0] + a[1] * a[1] + a[2] * a[2]);
	a[0] = a[0] / l;
	a[1] = a[1] / l;
	a[2] = a[2] / l;
}


/**
 * Desenho do plano e escrita dos respetivos pontos no ficheiro
*/
void drawPlane(FILE* fp, float size) {

	/* Definição do número de vértices */
	fprintf(fp, "12\n");

	// Posicao
	fprintf(fp, "%f %f %f\n", 0.0f, -size/2, size/2);
	// Normal
	fprintf(fp, "%f %f %f\n", 0.0f, 1.0f, 0.0f);
	// Posicao
	fprintf(fp, "%f %f %f\n", 0.0f, -size/2, -size/2);
	// Normal
	fprintf(fp, "%f %f %f\n", 0.0f, 1.0f, 0.0f);
	// Posicao
	fprintf(fp, "%f %f %f\n", 0.0f, size/2, -size/2);
	// Normal
	fprintf(fp, "%f %f %f\n", 0.0f, 1.0f, 0.0f);
	// Posicao
	fprintf(fp, "%f %f %f\n", 0.0f, -size/2, size/2);
	// Normal
	fprintf(fp, "%f %f %f\n", 0.0f, 1.0f, 0.0f);
	// Posicao
	fprintf(fp, "%f %f %f\n", 0.0f, size/2, -size/2);
	// Normal
	fprintf(fp, "%f %f %f\n", 0.0f, 1.0f, 0.0f);
	// Posicao
	fprintf(fp, "%f %f %f\n", 0.0f, size/2, size/2);
	// Normal
	fprintf(fp, "%f %f %f\n", 0.0f, 1.0f, 0.0f);
}

/**
 * Desenho da caixa e escrita dos respetivos pontos no ficheiro
*/
void drawBox(FILE* fp, float x, float y, float z) {

	/* Definição do número de vértices */
	fprintf(fp, "72\n");

	//Base da caixa

	// Posicao
	fprintf(fp, "%f %f %f\n", -x / 2, -y / 2, -z / 2);
	// Normal
	fprintf(fp, "%f %f %f\n", 0.0f, -1.0f, 0.0f);
	// Posicao
	fprintf(fp, "%f %f %f\n", x / 2, -y / 2, -z / 2);
	// Normal
	fprintf(fp, "%f %f %f\n", 0.0f, -1.0f, 0.0f);
	// Posicao
	fprintf(fp, "%f %f %f\n", -x / 2, -y / 2, z / 2);
	// Normal
	fprintf(fp, "%f %f %f\n", 0.0f, -1.0f, 0.0f);
	// Posicao
	fprintf(fp, "%f %f %f\n", x / 2, -y / 2, -z / 2);
	// Normal
	fprintf(fp, "%f %f %f\n", 0.0f, -1.0f, 0.0f);
	// Posicao
	fprintf(fp, "%f %f %f\n", x / 2, -y / 2, z / 2);
	// Normal
	fprintf(fp, "%f %f %f\n", 0.0f, -1.0f, 0.0f);
	// Posicao
	fprintf(fp, "%f %f %f\n", -x / 2, -y / 2, z / 2);
	// Normal
	fprintf(fp, "%f %f %f\n", 0.0f, -1.0f, 0.0f);

	//Tampa da caixa

	// Posicao
	fprintf(fp, "%f %f %f\n", -x / 2, y / 2, -z / 2);
	// Normal
	fprintf(fp, "%f %f %f\n", 0.0f, 1.0f, 0.0f);
	// Posicao
	fprintf(fp, "%f %f %f\n", -x / 2, y / 2, z / 2);
	// Normal
	fprintf(fp, "%f %f %f\n", 0.0f, 1.0f, 0.0f);
	// Posicao
	fprintf(fp, "%f %f %f\n", x / 2, y / 2, -z / 2);
	// Normal
	fprintf(fp, "%f %f %f\n", 0.0f, 1.0f, 0.0f);
	// Posicao
	fprintf(fp, "%f %f %f\n", x / 2, y / 2, -z / 2);
	// Normal
	fprintf(fp, "%f %f %f\n", 0.0f, 1.0f, 0.0f);
	// Posicao
	fprintf(fp, "%f %f %f\n", -x / 2, y / 2, z / 2);
	// Normal
	fprintf(fp, "%f %f %f\n", 0.0f, 1.0f, 0.0f);
	// Posicao
	fprintf(fp, "%f %f %f\n", x / 2, y / 2, z / 2);
	// Normal
	fprintf(fp, "%f %f %f\n", 0.0f, 1.0f, 0.0f);


	//Face ao longo do eixo x no z positivo

	// Posicao
	fprintf(fp, "%f %f %f\n", x / 2, y / 2, z / 2);
	// Normal
	fprintf(fp, "%f %f %f\n", 0.0f, 0.0f, 1.0f);
	// Posicao
	fprintf(fp, "%f %f %f\n", -x / 2, y / 2, z / 2);
	// Normal
	fprintf(fp, "%f %f %f\n", 0.0f, 0.0f, 1.0f);
	// Posicao
	fprintf(fp, "%f %f %f\n", -x / 2, -y / 2, z / 2);
	// Normal
	fprintf(fp, "%f %f %f\n", 0.0f, 0.0f, 1.0f);
	// Posicao
	fprintf(fp, "%f %f %f\n", x / 2, y / 2, z / 2);
	// Normal
	fprintf(fp, "%f %f %f\n", 0.0f, 0.0f, 1.0f);
	// Posicao
	fprintf(fp, "%f %f %f\n", -x / 2, -y / 2, z / 2);
	// Normal
	fprintf(fp, "%f %f %f\n", 0.0f, 0.0f, 1.0f);
	// Posicao
	fprintf(fp, "%f %f %f\n", x / 2, -y / 2, z / 2);
	// Normal
	fprintf(fp, "%f %f %f\n", 0.0f, 0.0f, 1.0f);

	//Face ao longo do eixo x no z negativo

	// Posicao
	fprintf(fp, "%f %f %f\n", x / 2, y / 2, -z / 2);
	// Normal
	fprintf(fp, "%f %f %f\n", 0.0f, 0.0f, -1.0f);
	// Posicao
	fprintf(fp, "%f %f %f\n", -x / 2, -y / 2, -z / 2);
	// Normal
	fprintf(fp, "%f %f %f\n", 0.0f, 0.0f, -1.0f);
	// Posicao
	fprintf(fp, "%f %f %f\n", -x / 2, y / 2, -z / 2);
	// Normal
	fprintf(fp, "%f %f %f\n", 0.0f, 0.0f, -1.0f);
	// Posicao
	fprintf(fp, "%f %f %f\n", x / 2, y / 2, -z / 2);
	// Normal
	fprintf(fp, "%f %f %f\n", 0.0f, 0.0f, -1.0f);
	// Posicao
	fprintf(fp, "%f %f %f\n", x / 2, -y / 2, -z / 2);
	// Normal
	fprintf(fp, "%f %f %f\n", 0.0f, 0.0f, -1.0f);
	// Posicao
	fprintf(fp, "%f %f %f\n", -x / 2, -y / 2, -z / 2);
	// Normal
	fprintf(fp, "%f %f %f\n", 0.0f, 0.0f, -1.0f);

	//Face ao longo do eixo z no x negativo

	// Posicao
	fprintf(fp, "%f %f %f\n", -x / 2, y / 2, -z / 2);
	// Normal
	fprintf(fp, "%f %f %f\n", -1.0f, 0.0f, 0.0f);
	// Posicao
	fprintf(fp, "%f %f %f\n", -x / 2, -y / 2, z / 2);
	// Normal
	fprintf(fp, "%f %f %f\n", -1.0f, 0.0f, 0.0f);
	// Posicao
	fprintf(fp, "%f %f %f\n", -x / 2, y / 2, z / 2);
	// Normal
	fprintf(fp, "%f %f %f\n", -1.0f, 0.0f, 0.0f);
	// Posicao
	fprintf(fp, "%f %f %f\n", -x / 2, y / 2, -z / 2);
	// Normal
	fprintf(fp, "%f %f %f\n", -1.0f, 0.0f, 0.0f);
	// Posicao
	fprintf(fp, "%f %f %f\n", -x / 2, -y / 2, -z / 2);
	// Normal
	fprintf(fp, "%f %f %f\n", -1.0f, 0.0f, 0.0f);
	// Posicao
	fprintf(fp, "%f %f %f\n", -x / 2, -y / 2, z / 2);
	// Normal
	fprintf(fp, "%f %f %f\n", -1.0f, 0.0f, 0.0f);

	//Face ao longo do eixo z no x positivo

	// Posicao
	fprintf(fp, "%f %f %f\n", x / 2, y / 2, -z / 2);
	// Normal
	fprintf(fp, "%f %f %f\n", 1.0f, 0.0f, 0.0f);
	// Posicao
	fprintf(fp, "%f %f %f\n", x / 2, y / 2, z / 2);
	// Normal
	fprintf(fp, "%f %f %f\n", 1.0f, 0.0f, 0.0f);
	// Posicao
	fprintf(fp, "%f %f %f\n", x / 2, -y / 2, z / 2);
	// Normal
	fprintf(fp, "%f %f %f\n", 1.0f, 0.0f, 0.0f);
	// Posicao
	fprintf(fp, "%f %f %f\n", x / 2, y / 2, -z / 2);
	// Normal
	fprintf(fp, "%f %f %f\n", 1.0f, 0.0f, 0.0f);
	// Posicao
	fprintf(fp, "%f %f %f\n", x / 2, -y / 2, z / 2);
	// Normal
	fprintf(fp, "%f %f %f\n", 1.0f, 0.0f, 0.0f);
	// Posicao
	fprintf(fp, "%f %f %f\n", x / 2, -y / 2, -z / 2);
	// Normal
	fprintf(fp, "%f %f %f\n", 1.0f, 0.0f, 0.0f);
}

/**
 * Desenho da esfera e escrita dos respetivos pontos no ficheiro
*/
void drawSphere(FILE* fp, int radius, int slices, int stacks) {

	// Usado para a construcao de cada uma das circunferencias
	float alpha = 2 * M_PI / slices;
	// Vetor auxiliar
	float* aux = (float*)malloc(sizeof(float) * NUM_COORDENADAS);

	// Angulo usado para a construcao das circunferencias
	float beta = M_PI / stacks;

	float x, y, z;

	/* Definição do número de vértices/linhas presentes no ficheiro*/
	fprintf(fp, "%d\n", 3*(6 * slices * stacks));

	for (int i = 0; i<stacks; i++) {

		float angleB = beta * i - M_PI_2;
		float nexAngleB = angleB + beta;

		// Desenhar os circulos de cima e de baixo
		for (int j = 0; j < slices; j++) {

			float angleA = alpha * j;
			float nextAngleA = angleA + alpha;

			// Posicao
			x = (radius * cos(angleB)) * sin(angleA);
			y = radius * sin(angleB);
			z = (radius * cos(angleB)) * cos(angleA);

			fprintf(fp, "%f %f %f\n", x, y, z);

			// Normal
			aux[0] = (radius * cos(angleB)) * sin(angleA);
			aux[1] = radius * sin(angleB);
			aux[2] = (radius * cos(angleB)) * cos(angleA);

			normalize(aux);

			fprintf(fp, "%f %f %f\n", aux[0], aux[1], aux[2]);

			// Coordenadas Texturas
			x = (float)j/slices;
			y = (float)i/stacks;

			fprintf(fp, "%f %f\n", x, y);

			// Posicao
			x = (radius * cos(nexAngleB)) * sin(nextAngleA);
			y = radius * sin(nexAngleB);
			z = (radius * cos(nexAngleB)) * cos(nextAngleA);

			fprintf(fp, "%f %f %f\n", x, y, z);

			// Normal
			aux[0] = (radius * cos(nexAngleB)) * sin(nextAngleA);
			aux[1] = radius * sin(nexAngleB);
			aux[2] = (radius * cos(nexAngleB)) * cos(nextAngleA);

			normalize(aux);

			fprintf(fp, "%f %f %f\n", aux[0], aux[1], aux[2]);

			// Coordenadas Texturas
			x = (float)(j + 1) / slices;
			y = (float)(i + 1) / stacks;

			fprintf(fp, "%f %f\n", x, y);

			// Posicao
			x = (radius * cos(nexAngleB)) * sin(angleA);
			y = radius * sin(nexAngleB);
			z = (radius * cos(nexAngleB)) * cos(angleA);

			fprintf(fp, "%f %f %f\n", x, y, z);

			// Normal
			aux[0] = (radius * cos(nexAngleB)) * sin(angleA);
			aux[1] = radius * sin(nexAngleB);
			aux[2] = (radius * cos(nexAngleB)) * cos(angleA);

			normalize(aux);

			fprintf(fp, "%f %f %f\n", aux[0], aux[1], aux[2]);

			// Coordenadas Texturas
			x = (float)j / slices;
			y = (float)(i + 1) / stacks;

			fprintf(fp, "%f %f\n", x, y);

			// Posicao
			x = (radius * cos(nexAngleB)) * sin(nextAngleA);
			y = radius * sin(nexAngleB);
			z = (radius * cos(nexAngleB)) * cos(nextAngleA);

			fprintf(fp, "%f %f %f\n", x, y, z);

			// Normal
			aux[0] = (radius * cos(nexAngleB)) * sin(nextAngleA);
			aux[1] = radius * sin(nexAngleB);
			aux[2] = (radius * cos(nexAngleB)) * cos(nextAngleA);

			normalize(aux);

			fprintf(fp, "%f %f %f\n", aux[0], aux[1], aux[2]);

			// Coordenadas Texturas
			x = (float)(j + 1) / slices;
			y = (float)(i + 1) / stacks;

			fprintf(fp, "%f %f\n", x, y);

			// Posicao
			x = (radius * cos(angleB)) * sin(angleA);
			y = radius * sin(angleB);
			z = (radius * cos(angleB)) * cos(angleA);

			fprintf(fp, "%f %f %f\n", x, y, z);

			// Normal
			aux[0] = (radius * cos(angleB)) * sin(angleA);
			aux[1] = radius * sin(angleB);
			aux[2] = (radius * cos(angleB)) * cos(angleA);

			normalize(aux);

			fprintf(fp, "%f %f %f\n", aux[0], aux[1], aux[2]);

			// Coordenadas Texturas
			x = (float)j / slices;
			y = (float)i / stacks;

			fprintf(fp, "%f %f\n", x, y);

			// Posicao
			x = (radius * cos(angleB)) * sin(nextAngleA);
			y = radius * sin(angleB);
			z = (radius * cos(angleB)) * cos(nextAngleA);

			fprintf(fp, "%f %f %f\n", x, y, z);

			// Normal
			aux[0] = (radius * cos(angleB)) * sin(nextAngleA);
			aux[1] = radius * sin(angleB);
			aux[2] = (radius * cos(angleB)) * cos(nextAngleA);

			normalize(aux);

			fprintf(fp, "%f %f %f\n", aux[0], aux[1], aux[2]);

			// Coordenadas Texturas
			x = (float)(j + 1) / slices;
			y = (float)i / stacks;

			fprintf(fp, "%f %f\n", x, y);
		}
	}
}

/*
 * radius1- raio interior / radius2 - raio exterior
*/
void drawCrown(FILE* fp, int radius1, int radius2, int slices) {

	// Usado para a construcao de cada uma das circunferencias
	float alpha = 2 * M_PI / slices;

	float x, y, z;

	/* Definição do número de vértices */
	fprintf(fp, "%d\n", 2 * 3 * slices );

	// Desenhar os circulos de cima e de baixo
	for (int j = 0; j < slices; j++) {

		float angleA = alpha * j;

		float nextAngle = angleA + alpha;

		// Primeiro triangulo

		x = (radius1 * sin(angleA));
		y = 0.0f;
		z = (radius1 * cos(angleA));

		fprintf(fp, "%f %f %f\n", x, y, z);

		x = (radius2 * sin(angleA));
		y = 0.0f;
		z = (radius2 * cos(angleA));

		fprintf(fp, "%f %f %f\n", x, y, z);

		x = (radius1 * sin(nextAngle));
		y = 0.0f;
		z = (radius1 * cos(nextAngle));

		fprintf(fp, "%f %f %f\n", x, y, z);

		// Segundo triangulo

		x = (radius2 * sin(angleA));
		y = 0.0f;
		z = (radius2 * cos(angleA));

		fprintf(fp, "%f %f %f\n", x, y, z);

		x = (radius2 * sin(nextAngle));
		y = 0.0f;
		z = (radius2 * cos(nextAngle));

		fprintf(fp, "%f %f %f\n", x, y, z);

		x = (radius1 * sin(nextAngle));
		y = 0.0f;
		z = (radius1 * cos(nextAngle));

		fprintf(fp, "%f %f %f\n", x, y, z);
	}
}


/**
 * Desenho do cone e escrita dos respetivos pontos no ficheiro
*/
void drawCone(FILE* fp, int bottomRadius, int height, int slices, int stacks) {

	// Razao entre a altura e o numero de stacks 
	float reason = (float)height / stacks;

	float currentRadius = bottomRadius;

	// Usado para a construcao de cada uma das circunferencias
	float alpha = 2 * M_PI / slices;
	// Vetor auxiliar
	float* aux = (float*)malloc(sizeof(float) * NUM_COORDENADAS);

	float x = 0.0f, y = 0.0f, z = 0.0f;

	/* Definição do número de vértices */
	fprintf(fp, "%d\n", 3*slices + 6 * slices * stacks);

	// Base
	for (int j = 0; j < slices; j++) {

		float angle = alpha * j;
		float nextAngle = angle + alpha;

		x = 0.0f;
		z = 0.0f;

		fprintf(fp, "%f %f %f\n", x, y, z);

		x = bottomRadius * sin(nextAngle);
		z = bottomRadius * cos(nextAngle);

		fprintf(fp, "%f %f %f\n", x, y, z);

		x = bottomRadius * sin(angle);
		z = bottomRadius * cos(angle);

		fprintf(fp, "%f %f %f\n", x, y, z);
	}

	// Corpo
	for (int i =0; i<stacks; i++) {

		// Raios a serem usados no momento
		currentRadius = (float) ((stacks-i) * bottomRadius) / stacks;
		float nextRadius = (float)((stacks - (i+1)) * bottomRadius) / stacks;

		// Proxima altura a que o circulo se encontra da base
		y = reason*i;
		float nexty = y + reason;

		for (int j = 0; j < slices; j++) {

			float angle = alpha * j;
			float nextAngle = angle + alpha;

			x = nextRadius * sin(angle);
			z = nextRadius * cos(angle);

			fprintf(fp, "%f %f %f\n", x, nexty, z);

			x = currentRadius * sin(angle);
			z = currentRadius * cos(angle);

			fprintf(fp, "%f %f %f\n", x, y, z);

			x = currentRadius * sin(nextAngle);
			z = currentRadius * cos(nextAngle);

			fprintf(fp, "%f %f %f\n", x, y, z);

			x = currentRadius * sin(nextAngle);
			z = currentRadius * cos(nextAngle);

			fprintf(fp, "%f %f %f\n", x, y, z);

			x = nextRadius * sin(nextAngle);
			z = nextRadius * cos(nextAngle);

			fprintf(fp, "%f %f %f\n", x, nexty, z);

			x = nextRadius * sin(angle);
			z = nextRadius * cos(angle);

			fprintf(fp, "%f %f %f\n", x, nexty, z);
		}
	}
}

/*
 * Desenho do objeto que estava presente no ficheiro de patch
 * e escrita dos respetivos pontos no ficheiro de saída passado por parâmetro
*/
void drawObjectBP(FILE* fp, int tesselevel) {

	// Numero de Vertices = 2*(numPatches * tesselationlevel^2 * 3Vertices * 2triangulos)
	// 2x por causa das normais
	int numVertices = 2*(numP * tesselevel * tesselevel * 3 * 2);
	// Variavel usada para a normal a um ponto
	float* normal = (float*)malloc(sizeof(float) * NUM_COORDENADAS);
	float x, y, z;
	/* Definição do número de vértices */
	fprintf(fp, "%d\n", numVertices);

	/* Desenhamos os triângulos que constituem um quadrado a partir dos vertíces do objectVertices*/
	for (int i = 0; i < numP; i++) {
		int numoldV = i * tesselevel * tesselevel;
		// Variacao do u
		for (int j = 0; j < tesselevel - 1; j++) {
			// Variacao do v
			for (int k = 0; k < tesselevel - 1; k++) {
				// Vértices do primeiro triangulo do quadrado respetivo
				//Posicao
				x = objectVertices[numoldV + j * tesselevel + k][0];
				y = objectVertices[numoldV + j * tesselevel + k][1];
				z = objectVertices[numoldV + j * tesselevel + k][2];
				fprintf(fp, "%f %f %f\n", x, y, z);

				// Normal
				cross(objectDeriv_v[numoldV + j * tesselevel + k], objectDeriv_u[numoldV + j * tesselevel + k], normal);
				normalize(normal);

				fprintf(fp, "%f %f %f\n", normal[0], normal[1], normal[2]);

				//Posicao
				x = objectVertices[numoldV + (j + 1) * tesselevel + (k + 1)][0];
				y = objectVertices[numoldV + (j + 1) * tesselevel + (k + 1)][1];
				z = objectVertices[numoldV + (j + 1) * tesselevel + (k + 1)][2];
				fprintf(fp, "%f %f %f\n", x, y, z);

				// Normal
				cross(objectDeriv_v[numoldV + (j + 1) * tesselevel + (k + 1)], objectDeriv_u[numoldV + (j + 1) * tesselevel + (k + 1)], normal);
				normalize(normal);

				fprintf(fp, "%f %f %f\n", normal[0], normal[1], normal[2]);

				// Posicao
				x = objectVertices[numoldV + j * tesselevel + (k + 1)][0];
				y = objectVertices[numoldV + j * tesselevel + (k + 1)][1];
				z = objectVertices[numoldV + j * tesselevel + (k + 1)][2];
				fprintf(fp, "%f %f %f\n", x, y, z);

				// Normal
				cross(objectDeriv_v[numoldV + j * tesselevel + (k + 1)], objectDeriv_u[numoldV + j * tesselevel + (k + 1)], normal);
				normalize(normal);

				fprintf(fp, "%f %f %f\n", normal[0], normal[1], normal[2]);

				// Vértices do segundo triangulo do quadrado respetivo
				//Posicao
				x = objectVertices[numoldV + (j + 1) * tesselevel + (k + 1)][0];
				y = objectVertices[numoldV + (j + 1) * tesselevel + (k + 1)][1];
				z = objectVertices[numoldV + (j + 1) * tesselevel + (k + 1)][2];
				fprintf(fp, "%f %f %f\n", x, y, z);

				// Normal
				cross(objectDeriv_v[numoldV + (j + 1) * tesselevel + (k + 1)], objectDeriv_u[numoldV + (j + 1) * tesselevel + (k + 1)], normal);
				normalize(normal);

				fprintf(fp, "%f %f %f\n", normal[0], normal[1], normal[2]);

				// Posicao
				x = objectVertices[numoldV + j * tesselevel + k][0];
				y = objectVertices[numoldV + j * tesselevel + k][1];
				z = objectVertices[numoldV + j * tesselevel + k][2];
				fprintf(fp, "%f %f %f\n", x, y, z);

				// Normal
				cross(objectDeriv_v[numoldV + j * tesselevel + k], objectDeriv_u[numoldV + j * tesselevel + k], normal);
				normalize(normal);

				fprintf(fp, "%f %f %f\n", normal[0], normal[1], normal[2]);

				// Posicao
				x = objectVertices[numoldV + (j + 1) * tesselevel + k][0];
				y = objectVertices[numoldV + (j + 1) * tesselevel + k][1];
				z = objectVertices[numoldV + (j + 1) * tesselevel + k][2];
				fprintf(fp, "%f %f %f\n", x, y, z);

				// Normal
				cross(objectDeriv_v[numoldV + (j + 1) * tesselevel + k], objectDeriv_u[numoldV + (j + 1) * tesselevel + k], normal);
				normalize(normal);

				fprintf(fp, "%f %f %f\n", normal[0], normal[1], normal[2]);
			}
		}
	}
}


/*
 * Funcao usada para a partir do numero de patch, devolver os seus pontos de controlo
*/
float** getCPsbyPatch(int o) {
	
	float** patchCPs = (float**)malloc(NUM_POINTS_PATCH * sizeof(float*));
	for (int i = 0; i < NUM_POINTS_PATCH; i++) {
		patchCPs[i] = (float*)malloc(NUM_COORDENADAS * sizeof(float));
		patchCPs[i][0] = controlPoints[indicesP[o][i]][0];
		patchCPs[i][1] = controlPoints[indicesP[o][i]][1];
		patchCPs[i][2] = controlPoints[indicesP[o][i]][2];
	}

	return patchCPs;
}

/*
 * Funcao que determina a posicao de um vértice dado um conjunto de control points de um patch,
 * o parâmetro u e o v
 * Usou-se a fórmula de cálculo matricial para os Patches de Bezier(p(u,v) = u x M x P x MT x v)
*/
void getPosition(int indice, float** patchCPs, float u, float v) {

	// Matriz de Bezier (nao esquecer que M = Mtransposto)
	float m[4][4] = { {-1.0f,  3.0f, -3.0f,  1.0f},
					  {3.0f, -6.0f,  3.0f, 0.0f },
					  {-3.0f,  3.0f,  0.0f,  0.0f},
	                  {1.0f,  0.0f,  0.0f,  0.0f }
    };
	// Matriz usada para guardar as coordenadas x, y ou z dos control points
	float p[4][4];
	// Usados para irem guardando os vetores resultantes dos cálculos 
	float vetor1[4];
	float vetor2[4];

	/* Para cada coordenada x,y e z do ponto */
	for (int i = 0; i < NUM_COORDENADAS; i++) {

		/* Construimos a matriz p dependendo da coordenada ser x,y ou z */
		for (int j = 0; j < 4; j++) {
			for (int k = 0; k < 4; k++) {
				p[j][k] = patchCPs[j*4+k][i];
			}
		}

		// Cálculo das componentes do vetor v
		vetor1[0] = pow((double)v, (double)3); vetor1[1] = pow((double)v, (double)2); vetor1[2] = v; vetor1[3] = 1.0f;

		/* Calculamos o resultado da multiplicação de Mtrans por vetor v */
		multMatrixVector((float*)m, vetor1, vetor2);

		/* Calculamos o resultado da multiplicação da matriz p pela multiplicacao anterior */
		multMatrixVector((float*)p, vetor2, vetor1);

		/* Calculamos o resultado da multiplicação da matriz M pela multiplicacao anterior */
		multMatrixVector((float*)m, vetor1, vetor2);

		/* Calculamos o resultado da multiplicaçao do vetor u pelo vetor da mult anterior*/
		/* Calcular agora p(u,v) = u * (vetor resultado anterior) */
		objectVertices[indice][i] = 0;

		for (int j = 0; j < 4; j++) {
			objectVertices[indice][i] += (float)pow((double)u, (double)3 - j) * vetor2[j];
		}
	}
}

/*
 * Funcao que determina a derivada parcial em ordem a u de um ponto num patch de Bezier
 * Usou-se a fórmula de cálculo matricial (dp/du(u,v) = u' x M x P x MT x v)
*/
void getDeriv_u(int indice, float** patchCPs, float u, float v) {

	// Matriz de Bezier (nao esquecer que M = Mtransposto)
	float m[4][4] = { {-1.0f,  3.0f, -3.0f,  1.0f},
					  {3.0f, -6.0f,  3.0f, 0.0f },
					  {-3.0f,  3.0f,  0.0f,  0.0f},
					  {1.0f,  0.0f,  0.0f,  0.0f }
	};
	// Matriz usada para guardar as coordenadas x, y ou z dos control points
	float p[4][4];
	// Usados para irem guardando os vetores resultantes dos cálculos 
	float vetor1[4];
	float vetor2[4];

	/* Para cada coordenada x,y e z do ponto */
	for (int i = 0; i < NUM_COORDENADAS; i++) {

		/* Construimos a matriz p dependendo da coordenada ser x,y ou z */
		for (int j = 0; j < 4; j++) {
			for (int k = 0; k < 4; k++) {
				p[j][k] = patchCPs[j * 4 + k][i];
			}
		}

		// Cálculo das componentes do vetor v
		vetor1[0] = pow((double)v, (double)3); vetor1[1] = pow((double)v, (double)2); vetor1[2] = v; vetor1[3] = 1.0f;

		/* Calculamos o resultado da multiplicação de Mtrans por vetor v */
		multMatrixVector((float*)m, vetor1, vetor2);

		/* Calculamos o resultado da multiplicação da matriz p pela multiplicacao anterior */
		multMatrixVector((float*)p, vetor2, vetor1);

		/* Calculamos o resultado da multiplicação da matriz M pela multiplicacao anterior */
		multMatrixVector((float*)m, vetor1, vetor2);

		/* Calculamos o resultado da multiplicaçao do vetor u pelo vetor da mult anterior*/
		/* Calcular agora dp/du(u,v) = u' * (vetor resultado anterior) */
		objectDeriv_u[indice][i] = 0;

		for (int j = 0; j < 4; j++) {
			objectDeriv_u[indice][i] += (3-j)*(float)pow((double)u, (double)2 - j) * vetor2[j];
		}
	}
}

/*
 * Funcao que determina a derivada parcial em ordem a v de um ponto num patch de Bezier
 * Usou-se a fórmula de cálculo matricial (dp/dv(u,v) = u x M x P x MT x v')
*/
void getDeriv_v(int indice, float** patchCPs, float u, float v) {

	// Matriz de Bezier (nao esquecer que M = Mtransposto)
	float m[4][4] = { {-1.0f,  3.0f, -3.0f,  1.0f},
					  {3.0f, -6.0f,  3.0f, 0.0f },
					  {-3.0f,  3.0f,  0.0f,  0.0f},
					  {1.0f,  0.0f,  0.0f,  0.0f }
	};
	// Matriz usada para guardar as coordenadas x, y ou z dos control points
	float p[4][4];
	// Usados para irem guardando os vetores resultantes dos cálculos 
	float vetor1[4];
	float vetor2[4];

	/* Para cada coordenada x,y e z do ponto */
	for (int i = 0; i < NUM_COORDENADAS; i++) {

		/* Construimos a matriz p dependendo da coordenada ser x,y ou z */
		for (int j = 0; j < 4; j++) {
			for (int k = 0; k < 4; k++) {
				p[j][k] = patchCPs[j * 4 + k][i];
			}
		}

		// Cálculo das componentes do vetor v'
		vetor1[0] = 3*(float)pow((double)v, (double)2); vetor1[1] = 2*v; vetor1[2] = 1.0f; vetor1[3] = 0.0f;

		/* Calculamos o resultado da multiplicação de Mtrans por vetor v */
		multMatrixVector((float*)m, vetor1, vetor2);

		/* Calculamos o resultado da multiplicação da matriz p pela multiplicacao anterior */
		multMatrixVector((float*)p, vetor2, vetor1);

		/* Calculamos o resultado da multiplicação da matriz M pela multiplicacao anterior */
		multMatrixVector((float*)m, vetor1, vetor2);

		/* Calculamos o resultado da multiplicaçao do vetor u pelo vetor da mult anterior*/
		/* Calcular agora dp/dv(u,v) = u * (vetor resultado anterior) */
		objectDeriv_v[indice][i] = 0;

		for (int j = 0; j < 4; j++) {
			objectDeriv_v[indice][i] += (float)pow((double)u, (double)3 - j) * vetor2[j];
		}
	}
}

/*
 * Funcao usada para calcular os vértices dado o tesselation level
 * a partir dos control points que foram lidos do ficheiro dos patches
*/
void calculateObjVertexs(int tesselevel) {

	objectVertices = (float**) malloc(sizeof(float*) * numP * tesselevel * tesselevel);
	objectDeriv_u = (float**) malloc(sizeof(float*) * numP * tesselevel * tesselevel);
	objectDeriv_v = (float**) malloc(sizeof(float*) * numP * tesselevel * tesselevel);

	for (int i = 0; i < (numP * tesselevel * tesselevel); i++) {
		objectVertices[i] = (float*) malloc(sizeof(float) * NUM_COORDENADAS);
		objectDeriv_u[i] = (float*)malloc(sizeof(float) * NUM_COORDENADAS);
		objectDeriv_v[i] = (float*)malloc(sizeof(float) * NUM_COORDENADAS);
	}

	// Descobrir todos os vértices necessários para se desenhar os triângulos
	for (int i = 0; i < numP; i++) {
		float** patchCPs = getCPsbyPatch(i);

		for (int j = 0; j < tesselevel; j++) {
			float u = (float)((float) 1.0 * j) / (tesselevel - 1);
			for (int k = 0; k < tesselevel; k++) {
				float v = (float)((float)1.0 * k) / (tesselevel - 1);
				getPosition(i*tesselevel*tesselevel+j*tesselevel+k, patchCPs, u, v);
				getDeriv_u(i * tesselevel * tesselevel + j * tesselevel + k, patchCPs, u, v);
				getDeriv_v(i * tesselevel * tesselevel + j * tesselevel + k, patchCPs, u, v);
			}
		}
	}
}

/*
 * Funcao usada para ler do ficheiro recebido como parâmetro que contém 
 * os patches de Bezier
*/
int readPatches(const char* filename) {

	int valor;
	float x, y, z;

	// Abrir o ficheiro com os patches
	FILE* fp = fopen(filename, "r");

	/* Lemos o numero de patches existentes no ficheiro */
	fscanf(fp, "%d", &numP);

	if (numP < 1) return -1;

	// Matriz que vai guardar os indices dos vários patches respetivos aos control points
	indicesP = (int**) malloc(numP*sizeof(int*));

	/* Lemos os valores dos indices dos respetivos patches de Bezier */
	for (int i = 0; (i<numP && (fscanf(fp, "\n%d", &valor) > 0));i++) {
		indicesP[i] = (int*)malloc(NUM_POINTS_PATCH * sizeof(int));
		indicesP[i][0] = valor;
		for (int j = 1; (fscanf(fp, ", %d", &valor) > 0); j++){
			indicesP[i][j] = valor;
		}
	}

	/* Lemos o numero de control points existentes no ficheiro */
	fscanf(fp, "\n%d\n", &numCP);

	/* Ler todos os control points */
	controlPoints = (float**)malloc(numCP * sizeof(float*));

	for (int i = 0; (i < numCP && (fscanf(fp, " %f, %f, %f\n", &x,&y,&z) > 0)); i++) {
		controlPoints[i] = (float*)malloc(NUM_COORDENADAS*sizeof(float));
		controlPoints[i][0] = x;
		controlPoints[i][1] = y;
		controlPoints[i][2] = z;
	}

	fclose(fp);
	printf("File %s charged successfully with %d patches!\n", filename, numP);

	return 0;
}

/*
 * Devolve um inteiro que distingue o tipo de ação a tomar, mediante o parametro 
 * passado como argumento
*/
int drawtype(const char* arg) {

	int type = -1;
	if (!strcmp(arg, strdup("plane"))) {
		type = 1;
	} else if (!strcmp(arg, strdup("box"))) {
		type = 2;
	} else if (!strcmp(arg, strdup("sphere"))) {
		type = 3;
	} else if (!strcmp(arg, strdup("cone"))) {
		type = 4;
	} else if (!strcmp(arg, strdup("crown"))) {
		type = 5;
	} else if (!strcmp(arg, strdup("bezier"))) {
		type = 6;
	}
	return type;
}

/**
 * Funcao principal do Generator
*/
int main(int argc, const char* argv[]) {

	if (argc < 3) printf("INSIRA O NUMERO CORRETO DE PARAMETROS!");
	else {
		// Apontador para o ficheiro de output a escrever 
		FILE* fp;

		// Obter o tipo de ação a tomar
		int type = drawtype(argv[1]);

		/*Tipo de primitiva a gerar*/
		switch ( type ) {

			case 1:
				// plane <size> <output.3d>
				fp = fopen(argv[3], "w");
				drawPlane(fp,atof(argv[2]));
				fclose(fp);
				break;
			case 2:
				// box <x> <y> <z> <output.3d>
				fp = fopen(argv[5], "w");
				drawBox(fp, atof(argv[2]), atof(argv[3]), atof(argv[4]));
				fclose(fp);
				break;
			case 3:
				// sphere <Radius> <slices> <stacks> <output.3d>
				fp = fopen(argv[5], "w");
				drawSphere(fp, atof(argv[2]), atof(argv[3]), atof(argv[4]));
				fclose(fp);
				break;
			case 4:
				// cone <bottomRadius> <height> <slices> <stacks> <output.3d>
				fp = fopen(argv[6], "w");
				drawCone(fp, atof(argv[2]), atof(argv[3]), atof(argv[4]), atof(argv[5]));
				fclose(fp);
				break;
			case 5:
				// crown <raioI> <raioS> <slices> <output.3d>
				fp = fopen(argv[5], "w");
				drawCrown(fp, atof(argv[2]), atof(argv[3]), atof(argv[4]));
				fclose(fp);
				break;
			case 6:
				// bezier <name.patch> <tesselationlevel> <output.3d>
				fp = fopen(argv[4],"w");
				readPatches(argv[2]);
				calculateObjVertexs(atoi(argv[3]));
				drawObjectBP(fp, atoi(argv[3]));
				fclose(fp);
				break;
			default:
				printf("INSIRA OS PARAMETROS CORRETOS!");
		}
	}
	return 0;
}

