#include "tree.hpp"

Tree::Tree(char* path, std::vector<glm::mat4> treeTransformAtions) : LoadedModel(path){
	scale(1.5f);

	this->treeTransformAtions = treeTransformAtions;
	
}

void Tree::Draw(Shader shader) {
	for (auto transform : treeTransformAtions) {
		shader.setMat4("model", transform);
		for (auto&& mesh : meshes)
			mesh.Draw(shader);
	}
	
}