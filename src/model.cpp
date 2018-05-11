#include "model.hpp"

Model::Model()
{
}

void Model::Draw(Shader shader)
{
	shader.setMat4("model", transform);
	for (unsigned int i = 0; i < meshes.size(); i++)
		meshes[i].Draw(shader);
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