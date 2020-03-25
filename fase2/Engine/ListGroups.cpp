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

/**
M�todo que permite obter os diferentes vetores 
de todos os grupos includos na ListGroups 
enviada por par�metro. Os respetivos tamanhos 
s�o passados pelo par�metro de saida designado 
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
M�todo que retorna o n�mero de groups existentes 
na estrutura de dados ListGroups que � passada 
por par�metro
*/
int numGroups(ListGroups lg) {

	return lg->numGroups;
}

/**
M�todo que desenha uma ListGroups no ecr�
*/
void desenhaListGroups(ListGroups lg) {

	for (int i = 0; i < lg->numGroups; i++) {
		printf("<Group n�%d>\n");
		desenhaGroup(lg->groups[i]);
		printf("</Group n�%d>\n");
	}
}

/**
M�todo que permite obter as diferentes 
transforma��es para cada group indexado 
pelo �ndice. � retornado um array os n�meros 
de opera��es por cada group
*/
int* getOpsParams(ListGroups lg, char*** namesByGroup, float*** paramsByGroup) {

	int* sizes = (int*)malloc(sizeof(int) * lg->numGroups);
	int numOps;
	char** opNames;
	float** params;
	for (int i = 0; i < lg->numGroups; i++) {
		/* Acedemos ao n�mero de opera��es de cada group */
		numOps = getNumOps(lg->groups[i]);
		if (numOps > 0) {
			opNames = (char**)malloc(sizeof(char*) * numOps);
			params = (float**)malloc(sizeof(float*) * numOps);
			/* Vamos buscar os par�metros de todas
			as opera��es de cada group */
			sizes[i] = getParams(lg->groups[i], &opNames, &params);
			/* Adicionamos os par�metros e os
			nomes aos arrays a retornar */
			paramsByGroup[i] = params;
			namesByGroup[i] = opNames;
		}
	}
	return sizes;
}

/**
 funcao que devolve o grupo posicionado na posicao i
*/
Group getGroup(ListGroups lg, int i) {
	Group g = lg->groups[i];
	return g;
}
