#ifndef __GROUPIMPL_H__
#define __GROUPIMPL_H__

#include <stdio.h>
#include "ListVertices.h"
#include <vector>
#include "Model.h"
using std::vector;

/* M�ximo do tamanho do array
par�metros de cada opera��o */
#define _MAX_PARAM_TRANSLATE 4
#define _MAX_PARAM_ROTATE 5
#define _PARAM_COLOR 3
#define _PARAM_SCALE 3

/* Defini��o dos 2 tipos de
rota��o (est�tica/din�mica)*/
#define _STATIC 100
#define _DYNAMIC 101

/**
Defini��o de um tipo que guarda 
a informa��o acerca de um grupo
*/
typedef struct group *Group;

/**
Fun��o que permite obter uma nova
estrutura de dados do tipo group
*/
Group newGroup();

/**
M�todo que dado o nome da opera��o
e os seus par�metros adiciona essa
opera��o ao grupo
*/
void addOperacao(Group g, char* operacao, float* param);

/**
Fun��o que permite adicionar uma nova opera��o
do tipo transla��o din�mica. � necess�rio um
m�todo diferente para este tipo de opera��0, visto
que existem os pontos que formam a curva de catmoll-rom
*/
void addDynamicTranslation(Group g, char* operacao, float* param, ListVertices catmull_rom_points);

/**
Fun��o que permite adicionar um novo model a desenhar
para o group especificado por par�metro na fun��o
*/
void addModel(Group g, Model m);

/**
Fun��o que permite obter a estrutura com todos os
models pertencentes a um group
*/
vector<Model>* getModelsGroup(Group g);

/**
M�todo que desenha um group
*/
//void desenhaGroup(Group g);

/**
M�todo que permite obter um array com os
par�metros de cada uma das opera��es.
Conseguimos distingir as opera��es
associadas ao vetor, visto que possuem
tamanhos diferentes. � de notar que 
opNames � um par�metro de output de 
onde saem os nomes das opera��es associados 
aos parametros atrav�s dos INDICES.
*/
int getParams(Group g, char*** opNames, float*** params);

/**
Fun��o que retorna um array com o conjuntos
dos v�rtices correspondente � opera��o com o
mesmo �ndice no group
*/
ListVertices* getGroupPoints(Group g);

/**
Fun��o que retorna o n�mero de models que
existem numa estrutura de dados do tipo group
*/
int getNumModels(Group g);

/**
M�todo que liberta uma estrutura
de dados do tipo Group
*/
void freeGroup(Group g);

/**
M�todo que retorna o n�mero de
opera��es de um grupo
*/
int getNumOps(Group g);

void printGroupOps(Group g);

#endif