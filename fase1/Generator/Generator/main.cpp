#ifdef __GENERATOR__

#endif

#define _USE_MATH_DEFINES
#include <math.h>
#include <stdio.h>
#include <string.h>


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
 * Funcao principal do Generator
*/

int main(int argc, const char* argv[]) {

	if (argc < 2) printf("INSIRA O NUMERO CORRETO DE PARAMETROS!");
	else {
		
		FILE* fp;

		switch ( argv[1][0] ) {

			case 'p':
				fp = fopen(argv[3], "w");
				drawPlane(fp,atof(argv[2]));
				fclose(fp);
				break;
			case 'b':
				fp = fopen(argv[5], "w");
				drawBox(fp, atof(argv[2]), atof(argv[3]), atof(argv[4]));
				fclose(fp);
				break;
			case 's':
				fp = fopen(argv[5], "w");
				drawSphere(fp, atof(argv[2]), atof(argv[3]), atof(argv[4]));
				fclose(fp);
				break;
			case 'c':
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
