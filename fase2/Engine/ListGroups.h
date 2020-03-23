#ifndef __LISTGROUPS_H__
#define __LISTGROUPS_H__

#include "groupImpl.h"

/**
Defini��o de uma estrutura de dados 
que guarda grupos por aloca��o 
din�mica de mem�ria
*/
typedef struct listgroup* ListGroups;

/**
Fun��o que retorna uma nova estrutura
de dados do tipo ListGroups
*/
ListGroups newListGroups();

/**
M�todo que permite adicionar um novo grupo
a uma estrutura de dados do tipo ListGroups
*/
void addGroup(ListGroups lg, Group g);

#endif
