#pragma once
#include "loadedModel.hpp"

class Tree : LoadedModel {
public:
	Tree(char* path, std::vector<glm::mat4> treeTransformAtions);
	virtual void Draw(Shader shader);
private:
	std::vector<glm::mat4> treeTransformAtions;
};