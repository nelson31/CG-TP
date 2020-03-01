#ifndef __VERTICE_H__
#define __VERTICE_H__

/**
Defini��o da estrutura de dados 
que representa um v�rtice 
*/
typedef struct vertice* Vertice;

/**
Fun��o que permite criar uma nova
estrutura do tipo v�rtice
*/
Vertice newVertice(float nx, float ny, float nz);

/**
Fun��o que retorna a coordenada
x da estrutura de dados v�rtice
que � passada por par�metro
*/
int getX(Vertice v);

/**
Fun��o que retorna a coordenada
y da estrutura de dados v�rtice
que � passada por par�metro
*/
int getY(Vertice v);

/**
Fun��o que retorna a coordenada
z da estrutura de dados v�rtice
que � passada por par�metro
*/
int getZ(Vertice v);

/**
Fun��o que apresenta para o ecr� uma
estrutura de dados do tipo v�rtice
*/
void apresentaVertice(Vertice v);

#endif