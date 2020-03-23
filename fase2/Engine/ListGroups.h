#ifndef __LISTGROUPS_H__
#define __LISTGROUPS_H__

#include "groupImpl.h"

/**
Definição de uma estrutura de dados 
que guarda grupos por alocação 
dinâmica de memória
*/
typedef struct listgroup* ListGroups;

/**
Função que retorna uma nova estrutura
de dados do tipo ListGroups
*/
ListGroups newListGroups();

/**
Método que permite adicionar um novo grupo
a uma estrutura de dados do tipo ListGroups
*/
void addGroup(ListGroups lg, Group g);

#endif
