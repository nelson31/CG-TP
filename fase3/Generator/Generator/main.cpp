#ifdef __GENERATOR__

#endif

#define _USE_MATH_DEFINES
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
float** objectVertices;


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

	// Angulo alfa entre cada slice
	float alpha = 2 * M_PI / slices;

	float currentRadius = bottomRadius;

	float x, y, z;

	/* Definição do número de vértices */
	fprintf(fp, "%d\n", 3 * slices * stacks);

	for (int i =0; i<stacks; i++) {

		// Raio do proximo circulo a ser feito
		currentRadius = (float) ((stacks-i) * bottomRadius) / stacks;

		// Proxima altura a que o circulo se encontra da base
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

/*
 * Funcao usada para a partir do numero de patch, devolver os seus pontos de controlo
*/
float** getCPsbyPatch(int o) {

	int num;
	
	for (num = 0; indicesP[o][num] != -1; num++);
	
	float** patchCPs = (float**)malloc(num * sizeof(float*));
	for (int i = 0; i < num; i++) {
		patchCPs[i] = (float*)malloc(3 * sizeof(float));
		patchCPs[i][0] = controlPoints[indicesP[o][i]][0];
		patchCPs[i][1] = controlPoints[indicesP[o][i]][1];
		patchCPs[i][2] = controlPoints[indicesP[o][i]][2];
	}

	return patchCPs;
}

/*
 * Funcao que determina a posicao de um vértice dado um conjunto de control points de um patch,
 * o parâmetro u e o v
 * Usou-se a fórmula de cálculo matricial para os Patches de Bezier
*/
float* getPosition(float** patchCPs, float u, float v) {
	
	float* pos = (float*)malloc(sizeof(float) * 3);

	// Matriz de Bezier (nao esquecer que M = Mtransposto)
	float m[4][4] = { {-1.0f,  3.0f, -3.0f,  1.0f},
					  {3.0f, -6.0f,  3.0f, 0.0f },
					  {-3.0f,  3.0f,  0.0f,  0.0f},
	                  {1.0f,  0.0f,  0.0f,  0.0f }
    };
	// Matriz usada para guardar as coordenadas x, y ou z dos control points
	float p[4][4];
	// Usados para irem guardandos os respetivoos vetores
	float vetor1[4];
	float vetor2[4];

	/* Para cada coordenada x,y e z */
	for (int i = 0; i < 3; i++) {

		/* Construimos a matriz p com as respetivas coordenadas x,y ou z */
		for (int j = 0; j < 4; j++) {
			for (int k = 0; k < 4; k++) {
				p[j][k] = patchCPs[j*4+k][i];
			}
		}

		vetor1[0] = powf(v, 3); vetor1[0] = powf(v, 2); vetor1[0] = v; vetor1[0] = 1.0f;

		/* Calculamos o resultado da multiplicação de Mtrans por vetor v */
		multMatrixVector((float*)m, vetor1, vetor2);

		/* Calculamos o resultado da multiplicação da matriz p pela multiplicacao anterior */
		multMatrixVector((float*)p, vetor2, vetor1);

		/* Calculamos o resultado da multiplicação da matriz M pela multiplicacao anterior */
		multMatrixVector((float*)m, vetor1, vetor2);

		/* Calculamos o resultado da multiplicaçao do vetor u pelo vetor da mult anterior*/

		/* Calcular agora p(u,v) = u * (vetor resultado anterior) */
		pos[i] = 0;

		for (int j = 0; j < 4; j++) {
			pos[i] += (float)powf(u, 3 - j) * vetor2[j];
		}

	}

	return pos;
}

/*
 * Funcao usada para construir a grelha a partir dos patches de Bezier
*/
void drawBezierPatches(int tesselevel) {

	objectVertices = (float**) malloc(sizeof(float*)*numP * tesselevel * tesselevel);
	for (int i = 0; i < (numP * tesselevel * tesselevel); i++) {
		objectVertices[i] = (float*) malloc(sizeof(float) * 3);
	}

	// Descobrir todos os vértices necessários para se desenhar os triângulos
	for (int i = 0; i < numP; i++) {
		float** patchCPs = getCPsbyPatch(i);
		for (int j = 0; j < tesselevel; j++) {
			float u = 1.0 * j / (tesselevel - 1);
			for (int k = 0; k < tesselevel; k++) {
				float v = 1.0 * k / (tesselevel - 1);
				objectVertices[i * tesselevel * tesselevel + j * tesselevel + k] = getPosition(patchCPs, u, v);
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
	for (int i = 0; i < numP; i++) {
		indicesP[i] = (int*)malloc(500 * sizeof(int));
		for (int j = 0; j < 500; j++) {
			indicesP[i][j] = -1;
		}
	}

	/* Lemos os valores dos indices dos respetivos patches de Bezier */
	for (int i = 0; (i<numP && (fscanf(fp, "\n%d", &valor) > 0));i++) {
		indicesP[i][0] = valor;
		for (int j = 1; (fscanf(fp, ", %d", &valor) > 0); j++){
			indicesP[i][j] = valor;
		}
		//fscanf(fp, "\n");
	}

	/* Lemos o numero de control points existentes no ficheiro */
	fscanf(fp, "\n%d\n", &numCP);

	/* Ler todos os control points */
	controlPoints = (float**)malloc(numCP * sizeof(float*));

	for (int i = 0; (i < numCP && (fscanf(fp, "%f, %f, %f\n", &x,&y,&z) > 0)); i++) {
		controlPoints[i] = (float*)malloc(3*sizeof(float));
		controlPoints[i][0] = x;
		controlPoints[i][1] = y;
		controlPoints[i][2] = z;
	}

	fclose(fp);
	printf("File %s charged successfully with %d patches!\n", filename, numP);

	/*for (int i = 0; i < numP; i++) {
		printf("Novo Patch\n");
		for (int j = 0; indicesP[i][j] > -1; j++) {
			printf("%d\n",indicesP[i][j]);
		}
	}*/

	/*printf("Numero de CP: %d\n",numCP);
	for (int j = 0; j<numCP; j++) {
		printf("%f, %f, %f\n", controlPoints[j][0], controlPoints[j][1], controlPoints[j][2]);
	}*/

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

	if (argc < 2) printf("INSIRA O NUMERO CORRETO DE PARAMETROS!");
	else {
		
		FILE* fp;

		int type = drawtype(argv[1]);

		/*Tipo de primitiva a gerar*/
		switch ( type ) {

			case 1:
				fp = fopen(argv[3], "w");
				drawPlane(fp,atof(argv[2]));
				fclose(fp);
				break;
			case 2:
				fp = fopen(argv[5], "w");
				drawBox(fp, atof(argv[2]), atof(argv[3]), atof(argv[4]));
				fclose(fp);
				break;
			case 3:
				fp = fopen(argv[5], "w");
				drawSphere(fp, atof(argv[2]), atof(argv[3]), atof(argv[4]));
				fclose(fp);
				break;
			case 4:
				fp = fopen(argv[6], "w");
				drawCone(fp, atof(argv[2]), atof(argv[3]), atof(argv[4]), atof(argv[5]));
				fclose(fp);
				break;
			case 5:
				fp = fopen(argv[5], "w");
				drawCrown(fp, atof(argv[2]), atof(argv[3]), atof(argv[4]));
				fclose(fp);
				break;
			case 6:
				readPatches(argv[2]);
				drawBezierPatches(atoi(argv[3]));
				break;
			default:
				printf("INSIRA OS PARAMETROS CORRETOS!");
		}
	}
	return 0;
}
