#ifndef __MODEL_H__
#define __MODEL_H__

using std::vector;

/**
Definição de um tipo de dados que guarda 
todas as informações acerca de um model
*/
typedef struct model* Model;

/**
Função que permite criar uma nova estrutura de
dados do tipo Model
*/
Model newModel(vector<float>* vertices, vector<float>* normals, int id, vector<float>* texCoord, float difuse[], float specular[],
	float ambient[], float emission[], int shineness);

/**
Função que nos diz se um dado model é
definido por uma textura
*/
bool hasTexture(Model m);

/**
Função que permite obter informação acerca do
material de um dado model
*/
void getMaterialInfo(Model m, float difuse[], float specular[], float ambient[], float emission[], int* shineness);

/**
Função que permite obter informação acerca da
textura de um dado model
*/
vector<float>* getTextureInfo(Model m, int* idTexture);

/**
Função que retorna o id de texture de um
determinado model
*/
int getTextureId(Model m);

/**
Função que retorna os vértices de um dado model
*/
vector<float>* getVertices(Model m);

/**
Função que retorna as normais de um dado model
*/
vector<float>* getNormals(Model m);

/**
Função que permite atribuir um id de buffer a um model
*/
void setBufferId(Model m, int bufferId);

/**
Função que permite aceder ao id de buffer do model
de texture passado como parâmetro da função
*/
int getBufferId(Model m);

#endif __MODEL_H__