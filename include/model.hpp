#pragma once
#include "mesh.hpp"
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"


class Model {
public:
	Model();
	void Draw(Shader shader);
	glm::mat4 getTransform();
	void translate(glm::vec3 offset);
	void rotate(float degrees, glm::vec3 rotation);
	void scale(glm::vec3 scale);
	void scale(float scale);
protected:
	std::vector<Mesh> meshes;
	glm::mat4 transform;
};