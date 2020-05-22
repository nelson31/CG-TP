#ifndef __LISTGROUPS_H__
#define __LISTGROUPS_H__

#include "groupImpl.h"
using std::vector;

/**
Defini��o de uma estrutura de dados 
que guarda grupos por aloca��o 
din�mica de mem�ria
*/
typedef struct listgroup* ListGroups;

/**
Fun��o que retorna uma nova estrutura
de dados do tipo ListGroups
*/
ListGroups newListGroups();

/**
M�todo que permite adicionar um novo grupo
a uma estrutura de dados do tipo ListGroups
*/
void addGroup(ListGroups lg, Group g);

/**
M�todo que permite obter os diferentes vetores
de todos os grupos includos na ListGroups
enviada por par�metro. Os respetivos tamanhos
s�o passados pelo par�metro de saida designado
por sizes
*/
vector<float>** getVectors(ListGroups lg, int** sizes);

/**
M�todo que retorna o n�mero de groups existentes
na estrutura de dados ListGroups que � passada
por par�metro
*/
int numGroups(ListGroups lg);

/**
M�todo que desenha uma ListGroups no ecr�
*/
void desenhaListGroups(ListGroups lg);

/**
M�todo que permite obter as diferentes
transforma��es para cada group indexado
pelo �ndice. Notar que namesByGroup � um 
par�metro de saida, no qual constam os 
nomes das opera��es de cada group dentro 
da lista
*/
int* getOpsParams(ListGroups lg, char**** namesByGroup, float**** paramsByGroup);

/**
M�todo que retorna o conjunto de todos os
points das opera��es transla��o dinamica
dentro de cada group
*/
ListVertices** getCatmollPoints(ListGroups lg);

void printOpsLG(ListGroups lg);

#endif
