#ifndef __VERTICE_H__
#define __VERTICE_H__

typedef struct vertice* Vertice;

/**
Fun��o que permite criar uma nova
estrutura do tipo v�rtice
*/
Vertice newVertice(float nx, float ny, float nz);

/**
Fun��o que apresenta para o ecr� uma
estrutura de dados do tipo v�rtice
*/
void apresentaVertice(Vertice v);

#endif