#pragma once
#include "mesh.hpp"
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"

class Model {
public:
	Model();
	virtual void Draw(Shader shader);
	glm::mat4 getTransform();
	virtual void translate(glm::vec3 offset);
	virtual void scale(glm::vec3 scale);
	virtual void scale(float scale);
	void rotate(float degrees, glm::vec3 rotation);
	void correctDrawOrder();
protected:
	std::vector<Mesh> meshes;
	glm::mat4 transform;
};