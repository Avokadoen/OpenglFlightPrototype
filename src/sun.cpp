
#include "sun.hpp"

int Sun::sunCount = 0;

Sun::Sun(int pointLightIndex, char* path) : LoadedModel(path) {
	pointLightIndex = sunCount;
	sunCount++;

	ambient = glm::vec3(0.6f, 0.6f, 0.17f);
	diffuse = glm::vec3(1.0f, 1.0f, 0.35f);
	specular = glm::vec3(1.0f, 1.0f, 0.35f);
}

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



}