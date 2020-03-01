#ifndef __VERTICE_H__
#define __VERTICE_H__

/**
Definição da estrutura de dados 
que representa um vértice 
*/
typedef struct vertice* Vertice;

/**
Função que permite criar uma nova
estrutura do tipo vértice
*/
Vertice newVertice(float nx, float ny, float nz);

/**
Função que retorna a coordenada
x da estrutura de dados vértice
que é passada por parâmetro
*/
int getX(Vertice v);

/**
Função que retorna a coordenada
y da estrutura de dados vértice
que é passada por parâmetro
*/
int getY(Vertice v);

/**
Função que retorna a coordenada
z da estrutura de dados vértice
que é passada por parâmetro
*/
int getZ(Vertice v);

/**
Função que apresenta para o ecrã uma
estrutura de dados do tipo vértice
*/
void apresentaVertice(Vertice v);

#endif