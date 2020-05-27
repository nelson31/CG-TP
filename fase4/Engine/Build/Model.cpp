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
	/* Estrutura que guarda os vértices 
	do model a ser desenhado */
	vector<float>* vertices;
	/* Estrutura que guarda as normais em cada 
	ponto do model a ser desenhado */
	vector<float>* normals;
};


/**
Função que permite obter uma estrutura de 
dados do tipo material
*/
Material newMaterial(float difuse[], float specular[], float ambient[], float emission[], int shineness) {

	Material ret = (Material)malloc(sizeof(struct material));
	for (int i = 0; i < 4; i++) {
		ret->diffuse[i] = difuse[i];
		ret->specular[i] = specular[i];
		ret->emission[i] = emission[i];
	}
	ret->shineness = shineness;
	return ret;
}

/**
Função que permite obter uma estrutura de 
dados do tipo Texture
*/
Texture newTexture(int id, vector<float>* textureCoord) {

	Texture t = (Texture)malloc(sizeof(struct texture));
	t->id = id;
	t->textureCoord = textureCoord;
	return t;
}

/**
Função que permite criar uma nova estrutura de dados 
do tipo model cuja composição é material
*/
Model newMaterialModel(vector<float>* vertices, vector<float>* normals, float difuse[], float specular[], 
	float ambient[], float emission[], int shineness) {

	Model m = (Model)malloc(sizeof(struct model));
	m->c.mat = newMaterial(difuse, specular, ambient, emission, shineness);
	m->type = MATERIAL;
	m->vertices = vertices;
	m->normals = normals;
	return m;
}

/**
Função que permite criar uma nova estrutura de dados do 
tipo model cuja composição é texture
*/
Model newTextureModel(vector<float>* vertices, vector<float>* normals, int id, vector<float>* texCoord) {

	Model m = (Model)malloc(sizeof(struct model));
	m->c.tex = newTexture(id, texCoord);
	m->type = TEXTURE;
	m->vertices = vertices;
	m->normals = normals;
	return m;
}

/**
Função que nos diz se um dado model é 
definido por uma textura
*/
bool hasTexture(Model m) {

	if (m->type == TEXTURE)
		return true;
	else
		return false;
}

/**
Função que permite obter informação acerca do 
material de um dado model
*/
Material getMaterialInfo(Model m) {

	return m->c.mat;
}

/**
Função que permite obter informação acerca da 
textura de um dado model
*/
Texture getTextureInfo(Model m) {

	return m->c.tex;
}