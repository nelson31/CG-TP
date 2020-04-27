#ifndef __GROUPIMPL_H__
#define __GROUPIMPL_H__

#include <stdio.h>
#include "ListVertices.h"
#include <vector>
using std::vector;

/**
Definição de um tipo que guarda 
a informação acerca de um grupo
*/
typedef struct group *Group;

/**
Função que permite obter uma nova
estrutura de dados do tipo group
*/
Group newGroup();

/**
Método que dado o nome da operação
e os seus parâmetros adiciona essa
operação ao grupo
*/
void addOperacao(Group g, char* operacao, float* param);

/**
Função que permite adicionar um vértices
ao group para também ser desenhado
*/
void addVertice(Group g, float x, float y, float z);

/**
Método que retorna o vetor no qual se encontram
representados os vértices a serem desenhados
pela aplicação
*/
vector<float>* getVectorV(Group g);

/**
Método que retorna o número de vértices
incluidos num Group
*/
int numVertices(Group g);

/**
Método que desenha um group
*/
void desenhaGroup(Group g);

/**
Método que permite obter um array com os
parâmetros de cada uma das operações.
Conseguimos distingir as operações
associadas ao vetor, visto que possuem
tamanhos diferentes. É de notar que 
opNames é um parâmetro de output de 
onde saem os nomes das operações associados 
aos parametros através dos INDICES.
*/
int getParams(Group g, char*** opNames, float*** params);

/**
Método que liberta uma estrutura
de dados do tipo Group
*/
void freeGroup(Group g);

/**
Método que retorna o número de
operações de um grupo
*/
int getNumOps(Group g);

void printGroupOps(Group g);

#endif