#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <vector>

#include "Model.h"

using std::vector;

/* Definição das macros que identificam 
um estrutura e um material */
#define TEXTURE 0
#define MATERIAL 1

/**
Definição de uma estrutura que guarda informação 
de textura para um objeto
*/
typedef struct texture {

	/* Inteiro que identifica o id da imagem
	aquando do seu load para memória gráfica */
	int id;
	/* Estrutura de dados que guarda as coordenadas
	de textura para o model em questão */
	vector<float>* textureCoord;
} *Texture;


/**
Definição de uma estrutura de dados que guarda 
informação do material para um objeto
*/
typedef struct material {

	float diffuse[4];
	float specular[4];
	float ambient[4];
	float emission[4];
	/* Parametro para a componente 
	especular */
	int shineness;
} *Material;

/**
Definição de uma estrutura de dados que 
define a composição do model
*/
typedef union composicao {

	Material mat;
	Texture tex;
};

/**
Definição de uma estrutura de dados que 
representa um model a ser desenhado
*/
struct model {

	composicao c;
	int type;
	vector<float>* vertices;
};