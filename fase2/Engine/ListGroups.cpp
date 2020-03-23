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

