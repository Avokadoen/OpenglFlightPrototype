#pragma once
#include "loadedModel.hpp"

#include <string>
class Sun : public LoadedModel{
public:
	Sun(int pointLightIndex, char* path) ;
	virtual void translate(glm::vec3 offset);
	void update(float deltaTime);
	glm::vec3 getPosition();
	void setOrbitTarget(glm::vec3 target);
	void setVelocity(float velocity);
private:
	float velocity;
	glm::vec3 position;
	glm::vec3 orbitPosition;
	//glm::vec3 ambient;
	//glm::vec3 diffuse;
	//glm::vec3 specular;
	//int pointLightIndex;
	//float dayAsSeconds;

	//static int sunCount;
};