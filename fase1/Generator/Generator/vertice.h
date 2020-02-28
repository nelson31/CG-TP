#ifndef __VERTICE_H__
#define __VERTICE_H__

typedef struct vertice* Vertice;

/**
Função que permite criar uma nova
estrutura do tipo vértice
*/
Vertice newVertice(float nx, float ny, float nz);

/**
Função que apresenta para o ecrã uma
estrutura de dados do tipo vértice
*/
void apresentaVertice(Vertice v);

#endif