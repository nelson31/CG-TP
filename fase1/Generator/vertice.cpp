#include <string.h>
#include <stdio.h>

struct vertice {

	float x, y, z;
} Vertice;

/** 
Função que permite criar uma nova 
estrutura do tipo vértice 
*/
Vertice newVertice(int nx, int ny, int nz) {

	Vertice v = malloc(sizeof(struct vertice));
	v.x = nx;
	v.y = ny;
	v.z = nz;
}