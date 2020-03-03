#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "ListVertices.h"
#include "vertice.h"

/**
Defini��o da estrutura de dados que 
representar� uma lista de v�rtices
*/
struct vertices {

	int pointer;
	int size;
	int nVertices;
	Vertice* lista;
};

/**
Fun��o que permite criar uma nova estrutura 
de dados do tipo lista de v�rtices
*/
ListVertices newListVertices(int n) {

	ListVertices novo = (ListVertices)malloc(sizeof(struct vertices));
	novo->nVertices = 0;
	novo->pointer = 0;
	novo->size = n;
	novo->lista = (Vertice*) malloc(sizeof(Vertice)*n);
	return novo;
}

/**
Fun��o que permite adicionar um 
novo v�rtice � lista
*/
void addVertice(ListVertices lv, float x, float y, float z) {

	int i = 0;
	Vertice v = newVertice(x, y, z);
	lv->lista[lv->nVertices++] = v;
}

/**
Fun��o que l� dum ficheiro um n�mero 
predefinido de v�rtices 
*/
ListVertices carregaFile(const char filename[]) {

	char* next;
	char str[50];
	errno_t err;
	FILE* fp = fopen(filename, "r");
	//int size = atoi(fgets(str, 50, fp));
	fgets(str, 50, fp);
	str[strlen(str) - 1] = '\0';
	int size = atoi(str);

	printf("Tamanho lido: %d\n", size);
	ListVertices lv = newListVertices(size);
	while (fgets(str, 50, fp)) {
		str[strlen(str) - 1] = '\0';
		float x = atof(strtok_s(str, " ", &next));
		float y = atof(strtok_s(NULL, " ", &next));
		float z = atof(strtok_s(NULL, " ", &next));
		addVertice(lv, x, y, z);
	}
	fclose(fp);
	return lv;
}

/**
Fun��o que apresenta para o utilizador 
uma estrutura de dados do tipo 
ListVertices
*/
void printList(ListVertices lv) {

	int i;
	for (i = 0; i < lv->nVertices; i++) {
		apresentaVertice(lv->lista[i]);
		printf(";\n");
	}
}

/**
Fun��o que retorna o pr�ximo v�rtice, 
retornando NULL caso se teha chegado ao 
fim da lista de v�rtices
*/
Vertice nextV(ListVertices lv) {

	if (lv->pointer < lv->nVertices)
		return lv->lista[lv->pointer++];
	else 
		return NULL;
}