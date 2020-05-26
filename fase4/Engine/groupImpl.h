#ifndef __GROUPIMPL_H__
#define __GROUPIMPL_H__

#include <stdio.h>
#include "ListVertices.h"
#include <vector>
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
Fun��o que permite adicionar um v�rtices
ao group para tamb�m ser desenhado
*/
void addVertice(Group g, float x, float y, float z);

/**
Fun��o que permite adicionar um v�rtice de normal ao
group especificado como par�metro da fun��o
*/
void addNormalVertice(Group g, float x, float y, float z);

/**
Fun��o que permite adicionar uma coordenada de textura
ao group especificado como par�metro da fun��o
*/
void addTextureVertice(Group g, float x, float y);

/**
M�todo que retorna o vetor no qual se encontram
representados os v�rtices a serem desenhados
pela aplica��o
*/
vector<float>* getVectorV(Group g);

/**
M�todo que retorna o n�mero de v�rtices
incluidos num Group
*/
int numVertices(Group g);

/**
M�todo que desenha um group
*/
void desenhaGroup(Group g);

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