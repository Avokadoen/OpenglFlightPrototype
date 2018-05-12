
#include "sun.hpp"

//int Sun::sunCount = 0;

Sun::Sun(int pointLightIndex, char* path) : LoadedModel(path) {
	orbitPosition = glm::vec3(0);
}

void Sun::translate(glm::vec3 offset) {
	position += offset;
}

glm::vec3 Sun::getPosition() {
	return position;
}

void Sun::update(float deltaTime) {
	//glm::vec3 selfVector = position - orbitPosition;

	float rotAmount = velocity * deltaTime;
	glm::mat4 newTransform(1.0f);
	transform = glm::rotate(newTransform, rotAmount, glm::vec3(0, 0, 1));

}

void Sun::setOrbitTarget(glm::vec3 target) {
	orbitPosition = target;
}


/*

void Sun::update(double deltaTime) {
	position += glm::vec3(100 * deltaTime, 0, 0);
	translate(position);
	rotate(360 / dayAsSeconds * deltaTime, glm::vec3(0, 0, 1));
}

void Sun::updateShader(Shader shader) {
	shader.use();
	std::string pointName = "pointLights[" + std::to_string(pointLightIndex) + "].";
	shader.setVec3(pointName.append(".position"), position);
	shader.setVec3(pointName.append(".ambient"), ambient);
	shader.setVec3(pointName.append(".diffuse"), diffuse);
	shader.setVec3(pointName.append(".diffuse"), specular);

	// range 3250
	shader.setFloat(pointName.append(".constant"), 1);
	shader.setFloat(pointName.append(".linear"), 0.0014);
	shader.setFloat(pointName.append(".quadratic"), 0.000007);



}*/