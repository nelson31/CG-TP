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
void addOperacao(Group g, char* operacao, float* param);

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
M�todo que retorna o n�mero de v�rtices
incluidos num Group
*/
int numVertices(Group g);

/**
M�todo que desenha um group
*/
void desenhaGroup(Group g);

/**
M�todo que permite obter um array com os
par�metros de cada uma das opera��es.
Conseguimos distingir as opera��es
associadas ao vetor, visto que possuem
tamanhos diferentes. � de notar que 
opNames � um par�metro de output de 
onde saem os nomes das opera��es associados 
aos parametros atrav�s dos INDICES.
*/
int getParams(Group g, char*** opNames, float*** params);

/**
M�todo que liberta uma estrutura
de dados do tipo Group
*/
void freeGroup(Group g);

/**
M�todo que retorna o n�mero de
opera��es de um grupo
*/
int getNumOps(Group g);

void printGroupOps(Group g);

#endif