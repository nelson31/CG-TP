#include <stdio.h>
#include <stdlib.h>
#include "groupImpl.h"
#include "ListVertices.h"

/* Numero de opera��es */
#define _MIN_OPS 10

/**
Defini��o dos tres tipos de opera��o
*/
#define TRANSLATE 0
#define ROTATE 1
#define SCALE 2
#define COLOR 3

/**
Defini��o de ums estrutura de dados
que guarda uma opera��o de translate
*/
typedef struct translate {

	/* Array que guarda os par�metros
	da transla��o */
	float parametros[_MAX_PARAM_TRANSLATE];
	/* Vari�vel que guarda os pontos que
	formam a curva de cattmol-rom */
	ListVertices points;
} *Translate;

/**
Defini��o de uma estrutura de dados
que guarda uma opera��o de rotate
*/
typedef struct rotate {

	/* Array que guarda os par�metros
	da rota��o */
	float parametros[_MAX_PARAM_ROTATE];
} *Rotate;

/**
Defini��o de uma estrutura de dados 
que guarda uma opera��o de color
*/
typedef struct color {

	/* Array que guarda os par�metros
	da rota��o */
	float parametros[3];
} *Color;

/**
Defini��o de uma estrutura de dados que 
guarda uma opera��o de scale
*/
typedef struct scale {

	/* Array que guarda os par�metros
	da escala */
	float parametros[3];
} *Scale;

///////////////////////////////////////////////////////////////////////////////////////////////

/**
Defini��o do tipo opera��o
*/
typedef union operacao {

	Translate t;
	Rotate r;
	Color c;
	Scale s;
} *OperacaoParam;

/**
Estrutura de dados que 
representa uma opera��o
*/
typedef struct operation {

	OperacaoParam operacao;
	int type;
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
Fun��o que cria uma nova estrutura 
de dados do tipo Translate
*/
Translate newTranslate(float* params, int numParams) {

	Translate t = (Translate)malloc(sizeof(struct translate));
	/* Se apenas existirem 3 par�metros estamos 
	perante uma transla��o est�tica -> o parametro 
	time fica inv�lido (negativo)*/
	if (numParams <= 3) {
		/* N�o existem points */
		t->parametros[3] = -1;
	}
	for (int i = 0; i < numParams; i++)
		t->parametros[i] = params[i];
	return t;
}

/**
Fun��o que cria uma nova estrutura 
de dados do tipo rotate, tendo em conta 
o tipo de transla��o que � fornecido 
como par�metro da fun��o
*/
Rotate newRotate(float* params) {

	Rotate r = (Rotate)malloc(sizeof(struct rotate));
	for (int i = 0; i < _MAX_PARAM_ROTATE; i++)
		r->parametros[i] = params[i];
	return r;
}

/**
Fun��o que cria uma nova estrutura de 
dados do tipo color
*/
Color newColor(float* params) {

	Color c = (Color)malloc(sizeof(struct color));
	for (int i = 0; i < 3; i++)
		c->parametros[i] = params[i];
	return c;
}

/**
Fun��o que permite criar uma nova estrutura 
de dados do tipo scale
*/
Scale newScale(float* params) {

	Scale s = (Scale)malloc(sizeof(struct scale));
	for (int i = 0; i < _PARAM_SCALE; i++)
		s->parametros[i] = params[i];
	return s;
}

/**
Fun��o que permite adicionar um ponto para 
defini��o da curva de cattmol-rom
*/
void addPointTranslate(Translate t, float x, float y, float z) {

	addVertice(t->points, x, y, z);
}

/**
Fun��o que retorna o conjunto dos 
pontos que fazem parte do percurso que 
constitui a curva de catmoll-rom. Retorna 
NULL caso tenhamos uma transla��o est�tica
*/
ListVertices getPointsTranslate(Translate t) {

	if (t->points == NULL)
		return NULL;
	return t->points;
}

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
M�todo que printa uma opera��o
para o ecr�
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
Fun��o que retorna uma nova estrutura 
de dados do tipo opera��o 
*/
Operacao newOperacao(char* name, float* param) {

	int numParam = 0;
	/* Alocamos espa�o para a opera��o */
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
			/* Se estamos perante uma rota��o
			temos 5 par�metros */
			numParam = 5;
			break;

		case 'c':
			ret->type = COLOR;
			ret->operacao->c = newColor(param);
			/* Se estamos perante uma color
			temos 3 par�metros */
			numParam = 3;
			break;

		case 's':
			ret->type = SCALE;
			ret->operacao->s = newScale(param);
			/* Se estamos perante um scale 
			temos 3 par�metros */
			numParam = _PARAM_SCALE;
			break;

		default:
			break;
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
	//printOp(g->op[g->numOps - 1]);
	/* Se estamos perante uma transla��o est�tica colocamos os points a null */
	if (g->op[g->numOps-1]->type == TRANSLATE) {
		g->op[g->numOps-1]->operacao->t->points = NULL;
	}
}

/**
Fun��o que permite adicionar uma nova opera��o 
do tipo transla��o din�mica. � necess�rio um 
m�todo diferente para este tipo de opera��0, visto 
que existem os pontos que formam a curva de catmoll-rom
*/
void addDynamicTranslation(Group g, char* operacao, float* param, ListVertices catmull_rom_points) {

	/* Se o tamanho m�ximo tiver sido
	atingido realocamos espa�o */
	if (g->numOps == g->sizeOp) {
		/* Disponibilizamos apenas
		espa�o para esta opera��o */
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
		printOp(g->op[i]);
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

void printGroupOps(Group g) {

	printf("\n<group>{\n");
	for (int i = 0; i < g->numOps; i++)
		printOp(g->op[i]);
	printf("}</group>\n\n");
}

/**
M�todo que permite obter os par�metros de 
uma opera��o dada a pr�pria opera��o. name 
� uma vari�vel de output
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
M�todo que retorna a lista de v�rtices que comp�em 
a curva de catmoll-rom associada a uma opera��o 
de translate din�mica. � de notar que caso a 
fun��o seja aplicada a qualquer outra opera��o retorna NULL
*/
ListVertices getPoints(Operacao op) {

	if (op->type == TRANSLATE && (op->operacao->t->parametros[3] != -1))
		return getPointsTranslate(op->operacao->t);
	return NULL;
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
Fun��o que retorna um array com o conjuntos 
dos v�rtices correspondente � opera��o com o 
mesmo �ndice no group
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

