#pragma once

#include "shader.hpp"
#include "model.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "assimp/Importer.hpp"

#include <vector>

class LoadedModel : public Model
{
public:
	/*  Functions   */
	LoadedModel(char *path);
	LoadedModel();
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