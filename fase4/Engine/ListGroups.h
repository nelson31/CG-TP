#ifndef __LISTGROUPS_H__
#define __LISTGROUPS_H__

#include "groupImpl.h"
using std::vector;

/**
Definição de uma estrutura de dados 
que guarda grupos por alocação 
dinâmica de memória
*/
typedef struct listgroup* ListGroups;

/**
Função que retorna uma nova estrutura
de dados do tipo ListGroups
*/
ListGroups newListGroups();

/**
Método que permite adicionar um novo grupo
a uma estrutura de dados do tipo ListGroups
*/
void addGroup(ListGroups lg, Group g);

/**
Método que permite obter os diferentes vetores
de todos os grupos includos na ListGroups
enviada por parâmetro. Os respetivos tamanhos
são passados pelo parâmetro de saida designado
por sizes
*/
vector<float>** getVectors(ListGroups lg, int** sizes);

/**
Método que retorna o número de groups existentes
na estrutura de dados ListGroups que é passada
por parâmetro
*/
int numGroups(ListGroups lg);

/**
Método que desenha uma ListGroups no ecrã
*/
void desenhaListGroups(ListGroups lg);

/**
Método que permite obter as diferentes
transformações para cada group indexado
pelo índice. Notar que namesByGroup é um 
parâmetro de saida, no qual constam os 
nomes das operações de cada group dentro 
da lista
*/
int* getOpsParams(ListGroups lg, char**** namesByGroup, float**** paramsByGroup);

/**
Método que retorna o conjunto de todos os
points das operações translação dinamica
dentro de cada group
*/
ListVertices** getCatmollPoints(ListGroups lg);

void printOpsLG(ListGroups lg);

#endif
