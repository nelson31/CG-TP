#include "ListGroups.h"
#include <stdio.h>

#define _INITIAL_SIZE 10

struct listgroup {

	/**
	Variável que guarda o número de 
	grupos inseridos na lista
	*/
	int numGroups;
	/**
	Variável que guarda o tamanho atual 
	do array de groups na lista
	*/
	int size;
	/**
	Variável que guarda os grupos
	*/
	Group* groups;
};

/**
Função que retorna uma nova estrutura 
de dados do tipo ListGroups
*/
ListGroups newListGroups() {

	ListGroups ret = (ListGroups)malloc(sizeof(struct listgroup));

	ret->numGroups = 0;
	ret->size = 0;
	ret->groups = (Group*)malloc(ret->size=_INITIAL_SIZE);

	return ret;
}

/**
Método que permite adicionar um novo grupo 
a uma estrutura de dados do tipo ListGroups
*/
void addGroup(ListGroups lg, Group g) {

	/* Se o tamanho máximo tiver sido 
	atingido realocamos espaço */
	if (lg->size == lg->numGroups) {
		/* Realocamos para o dobro do espaço */
		lg->groups = (Group*)realloc(lg->groups, lg->size * 2);
		lg->size *= 2;
	}
	/* Adicionamos o novo group ao array */
	lg->groups[lg->numGroups++] = g;
}

/**
Método que permite obter os diferentes vetores 
de todos os grupos includos na ListGroups 
enviada por parâmetro. Os respetivos tamanhos 
são passados pelo parâmetro de saida designado 
por sizes
*/
vector<float>** getVectors(ListGroups lg, int* sizes) {

	vector<float>** ret = (vector<float>**)malloc(sizeof(vector<float>*) * lg->numGroups);
	sizes = (int*)malloc(sizeof(int) * lg->numGroups);
	/* Para cada group vamos buscar o respetivo vector */
	for (int i = 0; i < lg->numGroups; i++) {
		ret[i] = getVectorV(lg->groups[i]);
		sizes[i] = numVertices(lg->groups[i]);
	}
	return ret;
}

/**
Método que retorna o número de groups existentes 
na estrutura de dados ListGroups que é passada 
por parâmetro
*/
int numGroups(ListGroups lg) {

	return lg->numGroups;
}

/**
Método que desenha uma ListGroups no ecrã
*/
void desenhaListGroups(ListGroups lg) {

	for (int i = 0; i < lg->numGroups; i++) {
		printf("<Group nº%d>\n");
		desenhaGroup(lg->groups[i]);
		printf("</Group nº%d>\n");
	}
}