#ifndef __MODEL_H__
#define __MODEL_H__

using std::vector;

/**
Defini��o de um tipo de dados que guarda 
todas as informa��es acerca de um model
*/
typedef struct model* Model;

/**
Fun��o que permite criar uma nova estrutura de dados
do tipo model cuja composi��o � material
*/
Model newMaterialModel(vector<float>* vertices, vector<float>* normals, float difuse[], float specular[],
	float ambient[], float emission[], int shineness);

/**
Fun��o que permite criar uma nova estrutura de dados do
tipo model cuja composi��o � texture
*/
Model newTextureModel(vector<float>* vertices, vector<float>* normals, int id, vector<float>* texCoord);

/**
Fun��o que nos diz se um dado model �
definido por uma textura
*/
bool hasTexture(Model m);

/**
Fun��o que permite obter informa��o acerca do
material de um dado model
*/
Material getMaterialInfo(Model m);

/**
Fun��o que permite obter informa��o acerca da
textura de um dado model
*/
Texture getTextureInfo(Model m);

#endif __MODEL_H__