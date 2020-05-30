#ifndef __MODEL_H__
#define __MODEL_H__

using std::vector;

/**
Defini��o de um tipo de dados que guarda 
todas as informa��es acerca de um model
*/
typedef struct model* Model;

/**
Fun��o que permite criar uma nova estrutura de
dados do tipo Model
*/
Model newModel(vector<float>* vertices, vector<float>* normals, int id, vector<float>* texCoord, float difuse[], float specular[],
	float ambient[], float emission[], int shineness);

/**
Fun��o que nos diz se um dado model �
definido por uma textura
*/
bool hasTexture(Model m);

/**
Fun��o que permite obter informa��o acerca do
material de um dado model
*/
void getMaterialInfo(Model m, float difuse[], float specular[], float ambient[], float emission[], int* shineness);

/**
Fun��o que permite obter informa��o acerca da
textura de um dado model
*/
vector<float>* getTextureInfo(Model m, int* idTexture);

/**
Fun��o que retorna o id de texture de um
determinado model
*/
int getTextureId(Model m);

/**
Fun��o que retorna os v�rtices de um dado model
*/
vector<float>* getVertices(Model m);

/**
Fun��o que retorna as normais de um dado model
*/
vector<float>* getNormals(Model m);

/**
Fun��o que permite atribuir um id de buffer a um model
*/
void setBufferId(Model m, int bufferId);

/**
Fun��o que permite aceder ao id de buffer do model
de texture passado como par�metro da fun��o
*/
int getBufferId(Model m);

#endif __MODEL_H__