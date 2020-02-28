#ifndef __LISTVERTICES_H__
#define __LISTVERTICES_H__

typedef struct vertices* ListVertices;

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

#endif
