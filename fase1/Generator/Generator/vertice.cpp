#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "vertice.h"

struct vertice {

	float x, y, z;
};

/**
Função que permite criar uma nova 
estrutura do tipo vértice 
*/
Vertice newVertice(float nx, float ny, float nz) {

	Vertice v = (Vertice)malloc(sizeof(struct vertice));
	v->x = nx;
	v->y = ny;
	v->z = nz;
	return v;
}

/**
Função que apresenta para o ecrã uma 
estrutura de dados do tipo vértice
*/
void apresentaVertice(Vertice v) {

	printf("Vertice: x=%f, y=%f, z=%f", v->x, v->y, v->z);
}