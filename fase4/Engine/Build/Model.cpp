#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <vector>

#include "Model.h"

using std::vector;

/* Defini��o das macros que identificam 
um estrutura e um material */
#define TEXTURE 0
#define MATERIAL 1

/**
Defini��o de uma estrutura que guarda informa��o 
de textura para um objeto
*/
typedef struct texture {

	/* Inteiro que identifica o id da imagem
	aquando do seu load para mem�ria gr�fica */
	int id;
	/* Estrutura de dados que guarda as coordenadas
	de textura para o model em quest�o */
	vector<float>* textureCoord;
} *Texture;


/**
Defini��o de uma estrutura de dados que guarda 
informa��o do material para um objeto
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
Defini��o de uma estrutura de dados que 
define a composi��o do model
*/
typedef union composicao {

	Material mat;
	Texture tex;
};

/**
Defini��o de uma estrutura de dados que 
representa um model a ser desenhado
*/
struct model {

	composicao c;
	int type;
	/* Estrutura que guarda os v�rtices 
	do model a ser desenhado */
	vector<float>* vertices;
	/* Estrutura que guarda as normais em cada 
	ponto do model a ser desenhado */
	vector<float>* normals;
};


/**
Fun��o que permite obter uma estrutura de 
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
Fun��o que permite obter uma estrutura de 
dados do tipo Texture
*/
Texture newTexture(int id, vector<float>* textureCoord) {

	Texture t = (Texture)malloc(sizeof(struct texture));
	t->id = id;
	t->textureCoord = textureCoord;
	return t;
}

/**
Fun��o que permite criar uma nova estrutura de dados 
do tipo model cuja composi��o � material
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
Fun��o que permite criar uma nova estrutura de dados do 
tipo model cuja composi��o � texture
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
Fun��o que nos diz se um dado model � 
definido por uma textura
*/
bool hasTexture(Model m) {

	if (m->type == TEXTURE)
		return true;
	else
		return false;
}

/**
Fun��o que permite obter informa��o acerca do 
material de um dado model
*/
Material getMaterialInfo(Model m) {

	return m->c.mat;
}

/**
Fun��o que permite obter informa��o acerca da 
textura de um dado model
*/
Texture getTextureInfo(Model m) {

	return m->c.tex;
}