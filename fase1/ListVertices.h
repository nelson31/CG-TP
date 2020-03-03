#ifndef __LISTVERTICES_H__
#define __LISTVERTICES_H__

#include "vertice.h"

/**
Defini��o de uma estrutura de dados 
que implementa uma lista de v�rtices
*/
typedef struct vertices* ListVertices;

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

#endif
