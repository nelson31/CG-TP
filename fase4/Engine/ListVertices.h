#ifndef __LISTVERTICES_H__
#define __LISTVERTICES_H__

#include "vertice.h"

/**
Defini��o de uma estrutura de dados 
que implementa uma lista de v�rtices
*/
typedef struct vertices* ListVertices;

/**
Fun��o que permite criar uma nova estrutura
de dados do tipo lista de v�rtices
*/
ListVertices newListVertices();

/**
Fun��o que permite adicionar um
novo v�rtice � lista
*/
void addVertice(ListVertices lv, float x, float y, float z);

/**
Fun��o que l� dum ficheiro um n�mero
predefinido de v�rtices
*/
ListVertices carregaFile(const char filename[]);

/**
Fun��o que apresenta para o utilizador
uma estrutura de dados do tipo
ListVertices
*/
void printList(ListVertices lv);

/**
Fun��o que retorna o pr�ximo v�rtice,
retornando NULL caso se teha chegado ao
fim da lista de v�rtices
*/
Vertice nextV(ListVertices lv);

/**
Fun��o que atualiza o valor do pointer para zero
*/
int atualizaPointer(ListVertices lv);

/**
Fun��o que retorna o n�mero de v�rtices que
existe na estrutura de dados do tipo ListVertices
que � passada como par�metro da fun��o
*/
int numVertices(ListVertices lv);

#endif
