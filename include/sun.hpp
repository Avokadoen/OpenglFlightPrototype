#pragma once
#include "loadedModel.hpp"

#include <string>
class Sun : public LoadedModel{
public:
	Sun(int pointLightIndex, char* path) ;
	void update(double deltaTime);
	void updateShader(Shader shader);

private:
	glm::vec3 position;
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	int pointLightIndex;
	float dayAsSeconds;

	static int sunCount;
};