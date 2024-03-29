#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "vertice.h"

/**
Defini��o da estrutura de dados 
que define um vertice
*/
struct vertice {

	float x, y, z;
};

/**
Fun��o que permite criar uma nova 
estrutura do tipo v�rtice 
*/
Vertice newVertice(float nx, float ny, float nz) {

	Vertice v = (Vertice)malloc(sizeof(struct vertice));
	v->x = nx;
	v->y = ny;
	v->z = nz;
	return v;
}

/**
Fun��o que retorna a coordenada 
x da estrutura de dados v�rtice 
que � passada por par�metro
*/
float getX(Vertice v) {

	float x = v->x;
	return x;
}

/**
Fun��o que retorna a coordenada
y da estrutura de dados v�rtice
que � passada por par�metro
*/
float getY(Vertice v) {

	float y = v->y;
	return y;
}

/**
Fun��o que retorna a coordenada
z da estrutura de dados v�rtice
que � passada por par�metro
*/
float getZ(Vertice v) {

	float z = v->z;
	return z;
}

/**
Fun��o que apresenta para o ecr� uma 
estrutura de dados do tipo v�rtice
*/
void apresentaVertice(Vertice v) {

	printf("Vertice: x=%f, y=%f, z=%f", v->x, v->y, v->z);
}