#include "model.hpp"

Model::Model()
{
	transform = glm::mat4(1.0f);
}

void Model::Draw(Shader shader)
{
	shader.setMat4("model", transform);
	for(auto&& mesh : meshes)
		mesh.Draw(shader);
}

glm::mat4 Model::getTransform() {
	return transform;
}
void Model::translate(glm::vec3 offset) {
	transform = glm::translate(transform, offset);
}
void Model::rotate(float degrees, glm::vec3 rotation) {
	transform = glm::rotate(transform, glm::radians(degrees), rotation);
}
void Model::scale(glm::vec3 scale) {
	transform = glm::scale(transform, scale);
}
void Model::scale(float scale) {
	transform = glm::scale(transform, glm::vec3(scale, scale, scale));
}

/*	
	"Hacked" function for correcting blending. only partially works
	Ment to correct vector so that transparent object are drawn
	last. Does not account for distance from view.
*/
void Model::correctDrawOrder() {
	int index = 0;
	int rotateCount = 0;
	for(auto mesh : meshes){
		if (mesh.material.opacity < 1.0f) {
			auto it = meshes.begin() + index;
			std::rotate(it, it + 1, meshes.end() - rotateCount);
			rotateCount++;
		}
		index++;
		if (index + rotateCount >= meshes.size()) break;
	}

}