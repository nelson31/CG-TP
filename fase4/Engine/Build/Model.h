#ifndef __MODEL_H__
#define __MODEL_H__

using std::vector;

/**
Definição de um tipo de dados que guarda 
todas as informações acerca de um model
*/
typedef struct model* Model;

/**
Função que permite criar uma nova estrutura de dados
do tipo model cuja composição é material
*/
Model newMaterialModel(vector<float>* vertices, vector<float>* normals, float difuse[], float specular[],
	float ambient[], float emission[], int shineness);

/**
Função que permite criar uma nova estrutura de dados do
tipo model cuja composição é texture
*/
Model newTextureModel(vector<float>* vertices, vector<float>* normals, int id, vector<float>* texCoord);

/**
Função que nos diz se um dado model é
definido por uma textura
*/
bool hasTexture(Model m);

/**
Função que permite obter informação acerca do
material de um dado model
*/
Material getMaterialInfo(Model m);

/**
Função que permite obter informação acerca da
textura de um dado model
*/
Texture getTextureInfo(Model m);

#endif __MODEL_H__