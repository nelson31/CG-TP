#include <stdio.h>
#include <stdlib.h>
#include "groupImpl.h"
#include "ListVertices.h"

/* Numero de operações */
#define _MIN_OPS 10

/**
Definição dos tres tipos de operação
*/
#define TRANSLATE 0
#define ROTATE 1
#define SCALE 2
#define COLOR 3

/**
Definição de ums estrutura de dados
que guarda uma operação de translate
*/
typedef struct translate {

	/* Array que guarda os parâmetros
	da translação */
	float parametros[_MAX_PARAM_TRANSLATE];
	/* Variável que guarda os pontos que
	formam a curva de cattmol-rom */
	ListVertices points;
} *Translate;

/**
Definição de uma estrutura de dados
que guarda uma operação de rotate
*/
typedef struct rotate {

	/* Array que guarda os parâmetros
	da rotação */
	float parametros[_MAX_PARAM_ROTATE];
} *Rotate;

/**
Definição de uma estrutura de dados 
que guarda uma operação de color
*/
typedef struct color {

	/* Array que guarda os parâmetros
	da rotação */
	float parametros[3];
} *Color;

/**
Definição de uma estrutura de dados que 
guarda uma operação de scale
*/
typedef struct scale {

	/* Array que guarda os parâmetros
	da escala */
	float parametros[3];
} *Scale;

///////////////////////////////////////////////////////////////////////////////////////////////

/**
Definição do tipo operação
*/
typedef union operacao {

	Translate t;
	Rotate r;
	Color c;
	Scale s;
} *OperacaoParam;

/**
Estrutura de dados que 
representa uma operação
*/
typedef struct operation {

	OperacaoParam operacao;
	int type;
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
Função que cria uma nova estrutura 
de dados do tipo Translate
*/
Translate newTranslate(float* params, int numParams) {

	Translate t = (Translate)malloc(sizeof(struct translate));
	/* Se apenas existirem 3 parâmetros estamos 
	perante uma translação estática -> o parametro 
	time fica inválido (negativo)*/
	if (numParams <= 3) {
		/* Não existem points */
		t->parametros[3] = -1;
	}
	for (int i = 0; i < numParams; i++)
		t->parametros[i] = params[i];
	return t;
}

/**
Função que cria uma nova estrutura 
de dados do tipo rotate, tendo em conta 
o tipo de translação que é fornecido 
como parâmetro da função
*/
Rotate newRotate(float* params) {

	Rotate r = (Rotate)malloc(sizeof(struct rotate));
	for (int i = 0; i < _MAX_PARAM_ROTATE; i++)
		r->parametros[i] = params[i];
	return r;
}

/**
Função que cria uma nova estrutura de 
dados do tipo color
*/
Color newColor(float* params) {

	Color c = (Color)malloc(sizeof(struct color));
	for (int i = 0; i < 3; i++)
		c->parametros[i] = params[i];
	return c;
}

/**
Função que permite criar uma nova estrutura 
de dados do tipo scale
*/
Scale newScale(float* params) {

	Scale s = (Scale)malloc(sizeof(struct scale));
	for (int i = 0; i < _PARAM_SCALE; i++)
		s->parametros[i] = params[i];
	return s;
}

/**
Função que permite adicionar um ponto para 
definição da curva de cattmol-rom
*/
void addPointTranslate(Translate t, float x, float y, float z) {

	addVertice(t->points, x, y, z);
}

/**
Função que retorna o conjunto dos 
pontos que fazem parte do percurso que 
constitui a curva de catmoll-rom. Retorna 
NULL caso tenhamos uma translação estática
*/
ListVertices getPointsTranslate(Translate t) {

	if (t->points == NULL)
		return NULL;
	return t->points;
}

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
Método que printa uma operação
para o ecrã
*/
void printOp(Operacao op) {

	float type;
	switch (op->type) {

	case TRANSLATE:
		printf("[Operacao] translate; ");
		printf("Params:(%f, %f, %f, time=%f)\n", op->operacao->t->parametros[0], op->operacao->t->parametros[1], op->operacao->t->parametros[2], op->operacao->t->parametros[3]);
		if (op->operacao->t->parametros[3] > 0) {
			printf("\tCatmoll-Room points: {\n");
			printList(op->operacao->t->points);
			printf("}\n");
		}
		break;

	case ROTATE:
		printf("[Operacao] rotate; ");
		type = op->operacao->r->parametros[4];
		printf("%f\n", type);
		type == _DYNAMIC ?
			printf("Params:(%f, %f, %f, %f, %s)\n", op->operacao->r->parametros[0], op->operacao->r->parametros[1], op->operacao->r->parametros[2], op->operacao->r->parametros[3], "Dynamic") :
			printf("Params:(%f, %f, %f, %f, %s)\n", op->operacao->r->parametros[0], op->operacao->r->parametros[1], op->operacao->r->parametros[2], op->operacao->r->parametros[3], "Static");
		break;

	case COLOR:
		printf("[Operacao] color; ");
		printf("Params:(%f, %f, %f)\n", op->operacao->c->parametros[0], op->operacao->c->parametros[1], op->operacao->c->parametros[2]);
		break;
	}
}

/**
Função que retorna uma nova estrutura 
de dados do tipo operação 
*/
Operacao newOperacao(char* name, float* param) {

	int numParam = 0;
	/* Alocamos espaço para a operação */
	Operacao ret = (Operacao)malloc(sizeof(struct operation));
	ret->operacao = (OperacaoParam)malloc(sizeof(union operacao));
	switch (name[0]) {
		
		case 't':
			ret->type = TRANSLATE;
			ret->operacao->t = newTranslate(param, _MAX_PARAM_TRANSLATE);
			numParam = 4;
			break;

		case 'r':
			ret->type = ROTATE;
			ret->operacao->r = newRotate(param);
			/* Se estamos perante uma rotação
			temos 5 parâmetros */
			numParam = 5;
			break;

		case 'c':
			ret->type = COLOR;
			ret->operacao->c = newColor(param);
			/* Se estamos perante uma color
			temos 3 parâmetros */
			numParam = 3;
			break;

		case 's':
			ret->type = SCALE;
			ret->operacao->s = newScale(param);
			/* Se estamos perante um scale 
			temos 3 parâmetros */
			numParam = _PARAM_SCALE;
			break;

		default:
			break;
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
	//printOp(g->op[g->numOps - 1]);
	/* Se estamos perante uma translação estática colocamos os points a null */
	if (g->op[g->numOps-1]->type == TRANSLATE) {
		g->op[g->numOps-1]->operacao->t->points = NULL;
	}
}

/**
Função que permite adicionar uma nova operação 
do tipo translação dinâmica. É necessário um 
método diferente para este tipo de operaçã0, visto 
que existem os pontos que formam a curva de catmoll-rom
*/
void addDynamicTranslation(Group g, char* operacao, float* param, ListVertices catmull_rom_points) {

	/* Se o tamanho máximo tiver sido
	atingido realocamos espaço */
	if (g->numOps == g->sizeOp) {
		/* Disponibilizamos apenas
		espaço para esta operação */
		Operacao* aux = (Operacao*)realloc(g->op, (g->sizeOp + 1) * sizeof(Operacao));
		g->op = aux;
		/* Atualizamos o tamanho
		do array para mais uma unidade */
		g->sizeOp++;
	}
	g->op[g->numOps] = newOperacao(operacao, param);
	g->op[g->numOps]->operacao->t->points = catmull_rom_points;
	g->numOps++;
	//printOp(g->op[g->numOps - 1]);
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
		printOp(g->op[i]);
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

void printGroupOps(Group g) {

	printf("\n<group>{\n");
	for (int i = 0; i < g->numOps; i++)
		printOp(g->op[i]);
	printf("}</group>\n\n");
}

/**
Método que permite obter os parâmetros de 
uma operação dada a própria operação. name 
é uma variável de output
*/
float* getParamsOp(Operacao op, char** name) {

	int nParam;
	float* param;
	switch (op->type) {
		case TRANSLATE:
			param = (float*)malloc(sizeof(float) * 4);
			nParam = _MAX_PARAM_TRANSLATE;
			strcpy(*name,"translate");
			break;

		case ROTATE:
			param = (float*)malloc(sizeof(float) * 5);
			nParam = _MAX_PARAM_ROTATE;
			strcpy(*name,"rotate");
			break;

		case COLOR:
			param = (float*)malloc(sizeof(float) * 3);
			nParam = _PARAM_COLOR;
			strcpy(*name,"color");
			break;

		default:
			break;
	}
	for (int i = 0; i < nParam; i++) {
		switch (op->type) {
			case TRANSLATE: param[i] = op->operacao->t->parametros[i]; break;
			case ROTATE: param[i] = op->operacao->r->parametros[i]; break;
			case COLOR: param[i] = op->operacao->c->parametros[i]; break;
		}
	}
	return param;
}

/**
Método que retorna a lista de vértices que compõem 
a curva de catmoll-rom associada a uma operação 
de translate dinâmica. É de notar que caso a 
função seja aplicada a qualquer outra operação retorna NULL
*/
ListVertices getPoints(Operacao op) {

	if (op->type == TRANSLATE && (op->operacao->t->parametros[3] != -1))
		return getPointsTranslate(op->operacao->t);
	return NULL;
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
Função que retorna um array com o conjuntos 
dos vértices correspondente à operação com o 
mesmo índice no group
*/
ListVertices* getGroupPoints(Group g) {

	ListVertices* lv = (ListVertices*)malloc(sizeof(ListVertices)*g->numOps);
	ListVertices list;
	for (int i = 0; i < g->numOps; i++) {
		list = getPoints(g->op[i]);
		list == NULL ? lv[i] = NULL : lv[i] = list;
	}
	return lv;
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

