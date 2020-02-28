#ifndef __LISTVERTICES_H__
#define __LISTVERTICES_H__

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

#endif
