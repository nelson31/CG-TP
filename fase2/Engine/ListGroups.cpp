#include "ListGroups.h"
#include <stdio.h>

#define _INITIAL_SIZE 10

struct listgroup {

	/**
	Vari�vel que guarda o n�mero de 
	grupos inseridos na lista
	*/
	int numGroups;
	/**
	Vari�vel que guarda o tamanho atual 
	do array de groups na lista
	*/
	int size;
	/**
	Vari�vel que guarda os grupos
	*/
	Group* groups;
};

/**
Fun��o que retorna uma nova estrutura 
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
M�todo que permite adicionar um novo grupo 
a uma estrutura de dados do tipo ListGroups
*/
void addGroup(ListGroups lg, Group g) {

	/* Se o tamanho m�ximo tiver sido 
	atingido realocamos espa�o */
	if (lg->size == lg->numGroups) {
		/* Realocamos para o dobro do espa�o */
		lg->groups = (Group*)realloc(lg->groups, lg->size * 2);
		lg->size *= 2;
	}
	/* Adicionamos o novo group ao array */
	lg->groups[lg->numGroups++] = g;
}

