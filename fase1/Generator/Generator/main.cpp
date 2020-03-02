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


	fprintf(fp, "%f %f %f\n", 0.0f, -size, size);
	fprintf(fp, "%f %f %f\n", 0.0f, -size, -size);
	fprintf(fp, "%f %f %f\n", 0.0f, size, -size);

	fprintf(fp, "%f %f %f\n", 0.0f, -size, size);
	fprintf(fp, "%f %f %f\n", 0.0f, size, -size);
	fprintf(fp, "%f %f %f\n", 0.0f, size, size);

}

/**
 * Desenho da caixa e escrita dos respetivos pontos no ficheiro
*/
void drawBox(FILE* fp, float x, float y, float z) {

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

	for (float angleB = -M_PI / 2; angleB < (2 * M_PI / 2); angleB += beta) {

		// Desenhar os circulos de cima e de baixo
		for (float angleA = 0; angleA < (2 * M_PI); angleA += alpha) {

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

	for (float currentHeight = 0.0f; currentHeight <= height; currentHeight += reason) {

		currentRadius = ((height - currentHeight) * bottomRadius) / height;

		for (float angle = 0; angle < (2 * M_PI); angle += alpha) {

			float nextAngle = angle + alpha;

			x = 0.0f;
			z = 0.0f;

			fprintf(fp, "%f %f %f\n", x, currentHeight, z);

			x = currentRadius * sin(angle);
			z = currentRadius * cos(angle);

			fprintf(fp, "%f %f %f\n", x, currentHeight, z);

			x = currentRadius * sin(nextAngle);
			z = currentRadius * cos(nextAngle);

			fprintf(fp, "%f %f %f\n", x, currentHeight, z);
		}
	}
}


void draw() {

	FILE* fp = fopen("sphere.3D", "w");

	drawSphere(fp, 2, 10, 10);

	FILE* fp1 = fopen("sphere.3D", "r");

	char str[50];

	int count = 0;

	while (fgets(str, sizeof(str), fp1)) {
		printf("%s", str);
		count++;
	}

	printf("Numero de vertices: %d",count);

}


/**
 * Funcao principal do Generator
*/

int main(int argc, char* argv[]) {

	if (argc < 2) printf("INSIRA O NUMERO CORRETO DE PARAMETROS!");
	else {

		// IMPORTANTE: VER AQUELA QUESTAO DO NUMERO DE VERTICES!!!!!!!!!!
		// COMPLETAR

		FILE* fp = fopen("box.3D", "w");

		drawBox(fp, 5, 5, 5);

		fclose(fp);

	}

	return 0;
}