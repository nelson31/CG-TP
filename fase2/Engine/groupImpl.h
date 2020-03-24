#ifndef __GROUPIMPL_H__
#define __GROUPIMPL_H__

#include <stdio.h>
#include "ListVertices.h"
#include <vector>
using std::vector;

/**
Defini��o de um tipo que guarda 
a informa��o acerca de um grupo
*/
typedef struct group *Group;

/**
Fun��o que permite obter uma nova
estrutura de dados do tipo group
*/
Group newGroup();

/**
M�todo que dado o nome da opera��o
e os seus par�metros adiciona essa
opera��o ao grupo
*/
void addOperacao(Group g, char* operacao, float param[]);

/**
Fun��o que permite adicionar um v�rtices
ao group para tamb�m ser desenhado
*/
void addVertice(Group g, float x, float y, float z);

/**
M�todo que retorna o vetor no qual se encontram
representados os v�rtices a serem desenhados
pela aplica��o
*/
vector<float>* getVectorV(Group g);

/**
M�todo que desenha um group
*/
void desenhaGroup(Group g);

/**
M�todo que liberta uma estrutura
de dados do tipo Group
*/
void freeGroup(Group g);

#endif