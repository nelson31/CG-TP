#include <stdio.h>
#include <stdlib.h>
#include "groupImpl.h"
#include "ListVertices.h"

/* Máximo do tamanho do array
parâmetros de cada operação */
#define _MAX_PARAM 4

/* Numero de operações */
#define _MIN_OPS 10

/**
Definição do tipo operação
*/
typedef struct operacao {

	/* Nome da operação */
	enum nome {
		TRANSLATE = 0,
		ROTATE = 1
	} nome;
	/* Parâmetros associados 
	à operação */
	float parametros[_MAX_PARAM];
} *Operacao;

/**
Definição do tipo Group
*/
struct group {

	/* Conjunto de operações 
	a serem realizadas */
	Operacao* op;
	/* Tamanho atual do array
	de operações */
	int sizeOp;
	/* Variável que guarda o número de 
	operações do group em questão */
	int numOps;
	/* Lista de vértices */
	vector<float>* lv;
	/* Variável que guarda o número de 
	vértices inseridos no grupo */
	int numVertices;
};

/**
Função que permite obter uma nova 
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
Função que retorna uma nova estrutura 
de dados do tipo operação 
*/
Operacao newOperacao(char* name, float param[]) {

	int numParam = 0;
	/* Alocamos espaço para a operação */
	Operacao ret = (Operacao) malloc(sizeof(struct operacao));
	switch (name[0]) {
		
		case 't':
			ret->nome = operacao::nome::TRANSLATE;
			/* Se estamos perante uma translação 
			temos 3 parâmetros */
			numParam = 3;
			break;

		case 'r':
			ret->nome = operacao::nome::ROTATE;
			/* Se estamos perante uma translação
			temos 4 parâmetros */
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
Método que dado o nome da operação 
e os seus parâmetros adiciona essa 
operação ao grupo
*/
void addOperacao(Group g, char* operacao, float* param) {

	/* Se o tamanho máximo tiver sido 
	atingido realocamos espaço */
	if (g->numOps == g->sizeOp) {
		/* Disponibilizamos apenas 
		espaço para esta operação */
		Operacao *aux = (Operacao*)realloc(g->op, (g->sizeOp + 1) * sizeof(Operacao));
		g->op = aux;
		/* Atualizamos o tamanho 
		do array para mais uma unidade */
		g->sizeOp++;
	}
	/* Colocamos a nova operação */
	g->op[g->numOps++] = newOperacao(operacao, param);
}

/**
Função que permite adicionar um vértices 
ao group para também ser desenhado
*/
void addVertice(Group g, float x, float y, float z) {

	g->lv->push_back(x);
	g->lv->push_back(y);
	g->lv->push_back(z);
	g->numVertices++;
}

/**
Método que retorna o vetor no qual se encontram 
representados os vértices a serem desenhados 
pela aplicação
*/
vector<float>* getVectorV(Group g) {

	return g->lv;
}

/**
Método que retorna o número de vértices 
incluidos num Group
*/
int numVertices(Group g) {

	return g->numVertices;
}

/**
Método que desenha um group
*/
void desenhaGroup(Group g) {

	printf("Operações: {\n");
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
	printf("Vértices: {\n");
	/* Imprimimos cada um dos vértices */
	for (int i = 0; i < size; i++) {
		printf("(%f, %f, %f)\n", g->lv->at(j++), g->lv->at(j++), g->lv->at(j++));
	}
	printf("}\n");
}

/**
Método que printa uma operação 
para o ecrã
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
Método que permite obter os parâmetros de 
uma operação dada a própria operação. name 
é uma variável de output
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
Método que permite obter um array com os 
parâmetros de cada uma das operações. 
Conseguimos distingir as operações 
associadas ao vetor, visto que possuem 
tamanhos diferentes.
*/
int getParams(Group g, char*** opNames, float*** params) {

	char* nome;
	/* Vamos buscar os parâmetros de cada 
	uma das operações */
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
Método que liberta uma estrutura 
de dados do tipo Group
*/
void freeGroup(Group g) {

	/* Libertamos o espaço 
	alocado */
	free(g->op);
	/* Libertamos o próprio 
	grupo */
	free(g);
}

/**
Método que retorna o número de 
operações de um grupo
*/
int getNumOps(Group g) {

	return g->numOps;
}

