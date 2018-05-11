#pragma once

#include "shader.hpp"
#include "model.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "assimp/Importer.hpp"

#include <vector>



// TODO: RIP APART!!! (modularity.)



class loadedModel : public Model
{
public:
	/*  Functions   */
	loadedModel(char *path);

private:
	std::vector<Texture> textures_loaded;

	/*  Model Data  */
	std::string directory;

	/*  Functions   */
	void loadModel(std::string path);
	void processNode(aiNode *node, const aiScene *scene);
	Mesh processMesh(aiMesh *mesh, const aiScene *scene);
	std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type,
		std::string typeName);
};