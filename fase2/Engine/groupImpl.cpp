#include <stdio.h>
#include <stdlib.h>
#include "groupImpl.h"
#include "ListVertices.h"

/* M�ximo do tamanho do array
par�metros de cada opera��o */
#define _MAX_PARAM 4

/* Numero de opera��es */
#define _MIN_OPS 10

/**
Defini��o do tipo opera��o
*/
typedef struct operacao {

	/* Nome da opera��o */
	enum nome {
		TRANSLATE = 0,
		ROTATE = 1
	} nome;
	/* Par�metros associados 
	� opera��o */
	float parametros[_MAX_PARAM];
} *Operacao;

/**
Defini��o do tipo Group
*/
struct group {

	/* Conjunto de opera��es 
	a serem realizadas */
	Operacao* op;
	/* Tamanho atual do array
	de opera��es */
	int sizeOp;
	/* Vari�vel que guarda o n�mero de 
	opera��es do group em quest�o */
	int numOps;
	/* Lista de v�rtices */
	vector<float>* lv;
	/* Vari�vel que guarda o n�mero de 
	v�rtices inseridos no grupo */
	int numVertices;
};

/**
Fun��o que permite obter uma nova 
estrutura de dados do tipo group
*/
Group newGroup() {

	Group novo = (Group)malloc(sizeof(struct group));

	novo->op = (Operacao*)malloc(sizeof(Operacao)*_MIN_OPS);
	novo->sizeOp = _MIN_OPS;
	novo->numOps = 0;
	novo->lv = new vector<float>();
	novo->numVertices = 0;

	return novo;
}

/**
Fun��o que retorna uma nova estrutura 
de dados do tipo opera��o 
*/
Operacao newOperacao(char* name, float param[]) {

	int numParam = 0;
	/* Alocamos espa�o para a opera��o */
	Operacao ret = (Operacao) malloc(sizeof(struct operacao));
	switch (name[0]) {
		
		case 't':
			ret->nome = operacao::nome::TRANSLATE;
			/* Se estamos perante uma transla��o 
			temos 3 par�metros */
			numParam = 3;
			break;

		case 'r':
			ret->nome = operacao::nome::ROTATE;
			/* Se estamos perante uma transla��o
			temos 4 par�metros */
			numParam = 4;
			break;

		default:
			break;
	}
	for (int i = 0; i < numParam; i++) {
		ret->parametros[i] = param[i];
	}
	return ret;
}

/**
M�todo que dado o nome da opera��o 
e os seus par�metros adiciona essa 
opera��o ao grupo
*/
void addOperacao(Group g, char* operacao, float* param) {

	/* Se o tamanho m�ximo tiver sido 
	atingido realocamos espa�o */
	if (g->numOps == g->sizeOp) {
		/* Disponibilizamos apenas 
		espa�o para esta opera��o */
		Operacao *aux = (Operacao*)realloc(g->op, (g->sizeOp + 1) * sizeof(Operacao));
		g->op = aux;
		/* Atualizamos o tamanho 
		do array para mais uma unidade */
		g->sizeOp++;
	}
	/* Colocamos a nova opera��o */
	g->op[g->numOps++] = newOperacao(operacao, param);
}

/**
Fun��o que permite adicionar um v�rtices 
ao group para tamb�m ser desenhado
*/
void addVertice(Group g, float x, float y, float z) {

	g->lv->push_back(x);
	g->lv->push_back(y);
	g->lv->push_back(z);
	g->numVertices++;
}

/**
M�todo que retorna o vetor no qual se encontram 
representados os v�rtices a serem desenhados 
pela aplica��o
*/
vector<float>* getVectorV(Group g) {

	return g->lv;
}

/**
M�todo que retorna o n�mero de v�rtices 
incluidos num Group
*/
int numVertices(Group g) {

	return g->numVertices;
}

/**
M�todo que desenha um group
*/
void desenhaGroup(Group g) {

	printf("Opera��es: {\n");
	for (int i = 0; i < g->numOps; i++) {
		switch (g->op[i]->nome) {

		case operacao::nome::TRANSLATE:
			printf("translate ");
			printf("(%f, %f, %f)\n", g->op[i]->parametros[0], g->op[i]->parametros[1],
				g->op[i]->parametros[2]);
			break;

		case operacao::nome::ROTATE:
			printf("rotate");
			printf("(%f, %f, %f, %f)\n", g->op[i]->parametros[0], g->op[i]->parametros[1],
				g->op[i]->parametros[2], g->op[i]->parametros[3]);
			break;
		}
	}
	printf("}\n");
	int j = 0;
	int size = (int)g->lv->size()/3;
	printf("V�rtices: {\n");
	/* Imprimimos cada um dos v�rtices */
	for (int i = 0; i < size; i++) {
		printf("(%f, %f, %f)\n", g->lv->at(j++), g->lv->at(j++), g->lv->at(j++));
	}
	printf("}\n");
}

/**
M�todo que printa uma opera��o 
para o ecr�
*/
void printOp(Operacao op) {

	switch (op->nome) {

		case operacao::nome::TRANSLATE:
			printf("[Operacao] translate; ");
			printf("Params:(%f, %f, %f)\n", op->parametros[0], op->parametros[1], op->parametros[2]);
			break;

		case operacao::nome::ROTATE:
			printf("[Operacao] rotate; ");
			printf("Params:(%f, %f, %f, %f)\n", op->parametros[0], op->parametros[1], op->parametros[2],op->parametros[3]);
			break;
	}
}

void printGroupOps(Group g) {

	printf("<group>{\n");
	for (int i = 0; i < g->numOps; i++)
		printOp(g->op[i]);
	printf("}</group>\n");
}

/**
M�todo que permite obter os par�metros de 
uma opera��o dada a pr�pria opera��o. name 
� uma vari�vel de output
*/
float* getParamsOp(Operacao op, char** name) {

	int nParam;
	float* param;
	switch (op->nome) {
		case operacao::nome::TRANSLATE:
			param = (float*)malloc(sizeof(float) * 3);
			nParam = 3;
			strcpy(*name,"translate");
			break;

		case operacao::nome::ROTATE:
			param = (float*)malloc(sizeof(float) * 4);
			nParam = 4;
			strcpy(*name,"rotate");
			break;

		default:
			break;
	}
	for (int i = 0; i < nParam; i++)
		param[i] = op->parametros[i];
	return param;
}

/**
M�todo que permite obter um array com os 
par�metros de cada uma das opera��es. 
Conseguimos distingir as opera��es 
associadas ao vetor, visto que possuem 
tamanhos diferentes.
*/
int getParams(Group g, char*** opNames, float*** params) {

	char* nome;
	/* Vamos buscar os par�metros de cada 
	uma das opera��es */
	int i;
	for (i = 0; i < g->numOps; i++) {
		nome = (char*)malloc(sizeof(char) * 20);
		(*params)[i] = getParamsOp(g->op[i],&nome);
		/* Adicionamos o nome ao array 
		opNames de output */
		(*opNames)[i] = nome;
	}
	return g->numOps;
}

/**
M�todo que liberta uma estrutura 
de dados do tipo Group
*/
void freeGroup(Group g) {

	/* Libertamos o espa�o 
	alocado */
	free(g->op);
	/* Libertamos o pr�prio 
	grupo */
	free(g);
}

/**
M�todo que retorna o n�mero de 
opera��es de um grupo
*/
int getNumOps(Group g) {

	return g->numOps;
}

