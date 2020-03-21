#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "vertice.h"

/**
Definição da estrutura de dados 
que define um vertice
*/
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
Função que retorna a coordenada 
x da estrutura de dados vértice 
que é passada por parâmetro
*/
float getX(Vertice v) {

	float x = v->x;
	return x;
}

/**
Função que retorna a coordenada
y da estrutura de dados vértice
que é passada por parâmetro
*/
float getY(Vertice v) {

	float y = v->y;
	return y;
}

/**
Função que retorna a coordenada
z da estrutura de dados vértice
que é passada por parâmetro
*/
float getZ(Vertice v) {

	float z = v->z;
	return z;
}

/**
Função que apresenta para o ecrã uma 
estrutura de dados do tipo vértice
*/
void apresentaVertice(Vertice v) {

	printf("Vertice: x=%f, y=%f, z=%f", v->x, v->y, v->z);
}