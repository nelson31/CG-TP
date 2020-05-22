#ifndef __LISTVERTICES_H__
#define __LISTVERTICES_H__

#include "vertice.h"

/**
Definição de uma estrutura de dados 
que implementa uma lista de vértices
*/
typedef struct vertices* ListVertices;

/**
Função que permite criar uma nova estrutura
de dados do tipo lista de vértices
*/
ListVertices newListVertices();

/**
Função que permite adicionar um
novo vértice à lista
*/
void addVertice(ListVertices lv, float x, float y, float z);

/**
Função que lê dum ficheiro um número
predefinido de vértices
*/
ListVertices carregaFile(const char filename[]);

/**
Função que apresenta para o utilizador
uma estrutura de dados do tipo
ListVertices
*/
void printList(ListVertices lv);

/**
Função que retorna o próximo vértice,
retornando NULL caso se teha chegado ao
fim da lista de vértices
*/
Vertice nextV(ListVertices lv);

/**
Função que atualiza o valor do pointer para zero
*/
int atualizaPointer(ListVertices lv);

/**
Função que retorna o número de vértices que
existe na estrutura de dados do tipo ListVertices
que é passada como parâmetro da função
*/
int numVertices(ListVertices lv);

#endif
